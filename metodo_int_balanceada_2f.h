#ifndef METODO_INT_BALANCEADA_2F_H
#define METODO_INT_BALANCEADA_2F_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "base.h"

#define num_arq 20
#define qnt_pagina 100

typedef struct{
    Aluno v[qnt_pagina];
    int qnt0[qnt_pagina];
    int qntlida;
    int ind;
} Pagina;

typedef struct{
    FILE *Fitas[num_arq];
    char nomearq[num_arq][12];
} ArqFitas;

typedef struct{
    Aluno alu;
    bool mar;
    int arq;
} Heap;

typedef struct{
    Heap cel[10];
    int qnt;
} Mem_Int; //Memoria interna

void realizaIntercalacaoBalanceada2f(Analise *a, FILE * arqProvao, int quantidade);
void InicializadorFitas(ArqFitas *arq);
void DestruirFitas(ArqFitas *arq, bool opc, bool fim);
void ReabrirFitas(ArqFitas *arq, bool opc);
void IniciaHeap(Mem_Int *mem, Pagina *ent);
void IniciaHeap2(Mem_Int *mem);
void OrganizaHeap(Mem_Int *mem, int ind, Analise *a);
void TrocaHeap(Mem_Int *mem, int ind, int eoud);
void RemoverHeap(Mem_Int *mem);

#endif //INTERCALACAO_BALANCEADA_2F_H

/*
Para o método 1, deve ser considerada a existência de (a) memória interna disponível para armazenar um
vetor de, no máximo, 10 registros, e (b) 20 fitas de armazenamento externo, sendo 10 fitas de entrada e
10 fitas de saída a cada etapa de intercalação
*/