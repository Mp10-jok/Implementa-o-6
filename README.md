# Implementação 6
# Algoritmo do Banqueiro 

Implementação em C do algoritmo do banqueiro, com número de clientes e de recursos definidos pela entrada.

## Arquivos

| Arquivo | Descrição |
|---|---|
| `banker.c` | Código-fonte |
| `Makefile` | Compilação (`make`) e limpeza (`make clean`) |
| `run_tests.sh` | Executa os testes e gera `evidencias.log` |
| `evidencias.log` | Evidências das execuções (casos normais e de erro) |

## Compilação

```bash
make         
make clean    
```

## Execução

Cada argumento é o número de instâncias de um tipo de recurso:

```bash
./banker 10 5 7
```

Na mesma pasta precisam estar:

- `customer.txt`: uma linha por cliente, com a demanda máxima de cada recurso separada por vírgula (ex.: `7,5,3`). O número de linhas define o número de clientes.
- `commands.txt`: um comando por linha:
  - `RQ <cliente> <r0> <r1> ...`: requisição de recursos
  - `RL <cliente> <r0> <r1> ...`: liberação de recursos
  - `*`: imprime as matrizes MAXIMUM, ALLOCATION, NEED e o vetor AVAILABLE

A saída é gravada em `result.txt`.

## Regras de uma requisição (RQ)

1. Se exceder o NEED do cliente, é negada ("exceed its maximum allocation").
2. Se exceder o AVAILABLE, é negada ("not enough").
3. Os recursos são alocados provisoriamente e o algoritmo de segurança é executado; se o estado for inseguro, a alocação é desfeita ("unsafe state").
4. Caso contrário, a alocação é confirmada.

Uma liberação (RL) maior que o alocado ao cliente é negada.

## Tratamento de erros

O programa valida tudo antes de gerar o `result.txt`, mostra uma mensagem em `stderr` e termina com código 1 quando:

- não há argumentos, ou algum argumento não é inteiro não negativo;
- a quantidade de argumentos difere da quantidade de recursos em `customer.txt` ou `commands.txt`;
- `customer.txt` ou `commands.txt` não existe ou não pode ser aberto;
- algum dos arquivos está fora do formato (valor não numérico, campo vazio, comando desconhecido, cliente inexistente, `*` com argumentos, arquivo de clientes vazio).

Quebras de linha Windows (CRLF) e linhas em branco são aceitas.

## Testes

```bash
./run_tests.sh
```

O script compila o projeto, roda 21 cenários (incluindo o exemplo do enunciado, comparado com a saída esperada) e registra tudo em `evidencias.log`.
