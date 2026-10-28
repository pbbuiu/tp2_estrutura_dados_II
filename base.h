#ifndef BASE_H
#define BASE_H

#define TAM_DADO2 5000

#include <time.h>

typedef struct{
    long int inscricao;
    float nota;
    char estado[3];
    char cidade[51];
    char curso[31];
} Aluno;

typedef struct analise{
    long int transferenciasLeituras;
    long int transferenciasEscritas;
    long int comparacoes;
    struct timespec inicio;
    struct timespec fim;
    double tempoTotal;
} Analise;

void inicializaAnalise(Analise *a);

#endif //BASE_H