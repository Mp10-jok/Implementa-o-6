#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CUSTOMER_FILE "customer.txt"
#define COMMANDS_FILE "commands.txt"
#define RESULT_FILE "result.txt"
#define LINE_BUF 4096

static int n_resources = 0;
static int n_customers = 0;
static int **maximun = NULL;

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

static void ler_customers(void){
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
    if(n_customers){
        fatal("O arquivo %s não tem clientes", CUSTOMER_FILE);
    }
}

int main(int argc, char *argv[]){
    printf("Deu certo!! Recebi %d argumentos\n", argc - 1);
    return 0;
}