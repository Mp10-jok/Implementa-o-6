#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CUSTOMER_FILE "customer.txt"
#define COMMANDS_FILE "commands.txt"
#define RESULT_FILE "result.txt"
#define LINE_BUF 4096

static int n_resources = 0;

static int read_line(FILE *f, char *buf, const char *fname, int line_no){
    if(!fgets(buf, LINE_BUF, f)) return 0;
    size_t len = strlen(buf);
    if(len == LINE_BUF - 1 && buf[len - 1] != '\n' && !feof(f)) fatal("A linha %d de %s é muito longa\n", line_no, fname);
    return 1;
}

static void fprintf_vec(FILE *f, const int *v){
    for(int j = 0; j < n_resources; j++){
        fprintf(f, j ? "%d" : "%d", v[j]);
    }
}

int main(int argc, char *argv[]){
    printf("Deu certo!! Recebi %d argumentos\n", argc - 1);
    return 0;
}