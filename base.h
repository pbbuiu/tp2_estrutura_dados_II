#ifndef BASE_H
#define BASE_H

#define TAM_DADO2 5000

#include <time.h>

typedef struct{
    long int inscricao;
    float nota;
    char informacoes[83];
} Aluno;

typedef struct analise{
    long int transferenciasLeituras;
    long int transferenciasEscritas;
    long int comparacoes;
    struct timespec inicio;
    struct timespec fim;
} Analise;

void inicializaAnalise(Analise *a);

#endif //BASE_H