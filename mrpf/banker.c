#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#define CUSTOMER_FILE "customer.txt"
#define COMMANDS_FILE "commands.txt"
#define RESULT_FILE "result.txt"
#define LINE_BUF 4096

typedef enum { CMD_RQ, CMD_RL, CMD_PRINT } CommandType; 

typedef struct {
    CommandType type;   
    int customer;      
    int *values;        
} Command;

static int n_resources = 0;
static int n_customers = 0;
static int *available = NULL;
static int **maximun = NULL;
static int **allocation = NULL;
static int **need = NULL;
static Command *commands = NULL;
static int n_commands = 0;
static FILE *result = NULL;

static int read_line(FILE *f, char *buf, const char *fname, int line_no){
    if(!fgets(buf, LINE_BUF, f)) return 0;
    size_t len = strlen(buf);
    if(len == LINE_BUF - 1 && buf[len - 1] != '\n' && !feof(f)) fatal("A linha %d do arquivo %s é muito longa\n", line_no, fname);
    return 1;
}

static void fprintf_vec(FILE *f, const int *v){
    for(int j = 0; j < n_resources; j++){
        fprintf(f, j ? "%d" : "%d", v[j]);
    }
}

static void load_customers(void){
    FILE *f = fopen(CUSTOMER_FILE, "r");
    if(!f) fatal("Não foi possível abrir o arquivo %s: %s\n", CUSTOMER_FILE, strerror(errno));

    char buf[LINE_BUF];
    int line_no = 0, cap = 0;

    while(read_line(f, buf, CUSTOMER_FILE, line_no + 1)){
        line_no++;
        char *line = trim(buf);
        if(*line == '\0') continue;

        int fields = 1;
        for(char *p = line; *p; p++){
            if(*p == ',') fields++;
        }
        if(fields != n_resources){
            fclose(f);
            fatal("A linha %d do %s tem %d recursos, mas foram passados %d argumentos\n", CUSTOMER_FILE, line_no, fields, n_resources);
        }
        if(n_customers == cap){
            cap = cap ? cap * 2 : 8;
            maximun = xrealloc(maximun, (size_t)cap * sizeof(int *));
        }
        int *row = xmalloc((size_t)n_resources * sizeof(int));
        maximun[n_customers] = row;
        n_customers++;

        char *tok = line;
        for(int j = 0; j < n_resources; j++){
            char *comma = strchr(tok, ',');
            if(comma) *comma = '\0';
            char *field = trim(tok);
            if(!parse_nonneg_int(field, &row[j])){
                fclose(f);
                fatal("Valor inválido %s em %s na linha %d\n", field, CUSTOMER_FILE, line_no);
            }
            tok = comma ? comma + 1 : tok + strlen(tok);
        }
    }
    fclose(f);
    if(n_customers == 0){
        fatal("O arquivo %s não tem clientes", CUSTOMER_FILE);
    }
}

static void load_commands(void){
    FILE *f = fopen(COMMANDS_FILE, "r");
    if(!f){
        fatal("Não foi possível abrir o arquivo %s: %s\n", COMMANDS_FILE, strerror(errno));
    }
    char buf[LINE_BUF];
    int line_no = 0, cap = 0;

    while(read_line(f, buf, COMMANDS_FILE, line_no + 1)){
        line_no++;
        char *line = trim(buf);
        if(*line == '\0') continue;

        char *tokens[LINE_BUF / 2] = {0};
        int nt = 0;
        for(char *t = strtok(line, "\t"); t; t = strtok(NULL, "\t")){
            tokens[nt++] = t;
        }

        Command cmd = {0};
        if(strcmp(tokens[0], "*") == 0){
            if(nt != 1){
                fclose(f);
                fatal("Linha %d do %s: o comando '*' não recebe argumentos\n", line_no, COMMANDS_FILE);
            }
            cmd.type = CMD_PRINT;
        }
        else if(strcmp(tokens[0], "RQ") == 0 || strcmp(tokens[0], "RL") == 0){
            cmd.type = (tokens[0][1] == 'Q') ? CMD_RQ : CMD_RL;
            if(nt - 2 != n_resources){
                fclose(f);
                fatal("A linha %d do %s tem %d recursos, mas foram passados %d argumentos\n", line_no, COMMANDS_FILE, nt - 2 < 0 ? 0 : nt - 2, n_resources);
            }

            if (!parse_nonneg_int(tokens[1], &cmd.customer)){
                fclose(f);
                fatal("Linha %d do %s: número de cliente inválido '%s'", line_no, COMMANDS_FILE, tokens[1]);
            }

            if (cmd.customer >= n_customers) {
                fclose(f);
                fatal("Linha %d do %s: o cliente %d não existe (validos: 0 a %d)", line_no, COMMANDS_FILE, cmd.customer, n_customers - 1);
            }
            cmd.values = xmalloc((size_t)n_resources * sizeof(int));
            for (int j = 0; j < n_resources; j++){
                if (!parse_nonneg_int(tokens[j + 2], &cmd.values[j])) {
                    free(cmd.values);
                    fclose(f);
                    fatal("Linha %d do %s: valor de recurso inválido '%s'",line_no, COMMANDS_FILE, tokens[j + 2]);
                }
            }
        } else {
            fclose(f);
            fatal("linha %d do %s: comando desconhecido '%s' (esperado: RQ, RL ou *)",line_no, COMMANDS_FILE, tokens[0]);
        }
        if (n_commands == cap) {
            cap = cap ? cap * 2 : 16;
            commands = xrealloc(commands, (size_t)cap * sizeof(Command));
        }
        commands[n_commands++] = cmd;
    }
    fclose(f);
}

static int is_safe(void) {
    int *work = xmalloc((size_t)n_resources * sizeof(int));
    int *finish = xmalloc((size_t)n_customers * sizeof(int));
    memcpy(work, available, (size_t)n_resources * sizeof(int));

    int progress = 1;
    while(progress){
        progress = 0;
        for(int i = 0; i < n_customers; i++){
            if(finish[i]) continue;
            
            int ok = 1;
            
            for(int j = 0; j < n_resources; j++){
                if(need[i][j] > work[j]){
                    ok = 0; 
                    break; 
                }
            }

            if(ok){
                for(int j = 0; j < n_resources; j++){
                    work[j] += allocation[i][j];
                } 
                finish[i] = 1;
                progress = 1;
            }
        }
    }

    int safe = 1;
    for (int i = 0; i < n_customers; i++){
        if (!finish[i]){
            safe = 0; 
            break;
        } 
    }
        
    free(work);
    free(finish);
    return safe;
}

int main(int argc, char *argv[]){
    printf("Deu certo!! Recebi %d argumentos\n", argc - 1);
    return 0;
}