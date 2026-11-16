# Implementação 6 – Algoritmo do Banqueiro

Implementação em C do algoritmo do banqueiro, apresentado na página P-45 do livro
*Operating System Concepts* (Silberschatz et al., 10ª ed.). O número de clientes e
de recursos é definido pela entrada do programa.

**Aluno:** Márcio Rodriguez Perez de Farias

## Estrutura do repositório

| Arquivo | Descrição |
|---|---|
| `mrpf/banker.c` | Código-fonte |
| `mrpf/Makefile` | Compilação (`make`) e limpeza (`make clean`) |
| `evidencias.log` | Registro do terminal com a execução dos testes |
| `mrpf.pdf` | Relatório |

## Compilação

```bash
cd mrpf
make         
make clean   
```

## Execução

Cada argumento é a quantidade de instâncias de um tipo de recurso:

```bash
./banker 10 5 7
```

Os arquivos `customer.txt` e `commands.txt` precisam estar na mesma pasta do executável:

- **`customer.txt`**: uma linha por cliente, com a demanda máxima de cada recurso
  separada por vírgula (ex.: `7,5,3`). O número de linhas define o número de clientes.
- **`commands.txt`**: um comando por linha:
  - `RQ <cliente> <r0> <r1> ...`: pede recursos
  - `RL <cliente> <r0> <r1> ...`: libera recursos
  - `*`: mostra as matrizes MAXIMUM, ALLOCATION, NEED e o vetor AVAILABLE

O resultado é gravado no arquivo `result.txt`.

## Regras de uma requisição (RQ)

1. Se o pedido for maior que o NEED do cliente, é negado.
2. Se o pedido for maior que o AVAILABLE, é negado.
3. Os recursos são alocados provisoriamente e o algoritmo de segurança é executado.
   Se o estado for inseguro, a alocação é desfeita e o pedido é negado.
4. Se passar por todas as verificações, a alocação é confirmada.

Uma liberação (RL) maior que o que o cliente tem alocado é negada.

## Tratamento de erros

O programa mostra uma mensagem de erro e encerra quando:

- não recebe argumentos ou recebe um argumento que não é número inteiro não negativo;
- a quantidade de argumentos é diferente da quantidade de recursos no
  `customer.txt` ou no `commands.txt`;
- o `customer.txt` ou o `commands.txt` não existe ou não pode ser aberto;
- algum dos arquivos está fora do formato (valor não numérico, comando desconhecido
  ou cliente inexistente).

## Evidências

O arquivo `evidencias.log` foi gerado com o comando `script`, que grava tudo o que
acontece no terminal. Ele mostra a compilação, a execução com o exemplo do enunciado
e os testes de erro.