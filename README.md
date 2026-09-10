# Escalonamento de CPU

Programa em C que simula e compara dois escalonadores de tempo real para tarefas periódicas com deadline — Rate-Monotonic (RM) e Earliest-Deadline-First (EDF).

## Compilação

```bash
make
```

## Execução

```bash
./scheduler [algoritmo] [arquivo_de_entrada]
```

Exemplo:

```bash
./scheduler rate voo.txt
./scheduler edf voo.txt
```

## Limpeza

```bash
make clean
```

## Saída

Cada execução gera um arquivo de saída com o traço de execução unidade a unidade e a contagem de deadlines perdidos, execuções completas e tarefas mortas ao fim da simulação:

- **rate_vchlm.out** — resultado da simulação usando Rate-Monotonic
- **edf_vchlm.out** — resultado da simulação usando Earliest-Deadline-First

## Algoritmos de escalonamento

- **Rate-Monotonic (RM)**: escalonamento preemptivo com prioridade estática baseada no menor período da tarefa.
- **Earliest-Deadline-First (EDF)**: escalonamento preemptivo com prioridade dinâmica baseada no menor deadline absoluto.

## Estrutura do código

- **main.c** — ponto de entrada, orquestra a leitura da entrada e a chamada do escalonador escolhido
- **errors.c** — validação de argumentos de linha de comando e verificação de erros no arquivo de entrada
- **rate.c** — implementação do escalonador Rate-Monotonic
- **edf.c** — implementação do escalonador Earliest-Deadline-First