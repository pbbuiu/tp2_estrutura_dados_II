#ifndef METODO_INT_BALANCEADA_F1_H
#define METODO_INT_BALANCEADA_F1_H

#include "base.h"
#include <stdio.h>
#include <stdbool.h>


#define F 19                // fitas de entrada = registros na memória interna
#define TOTAL_FITAS 20      // 19 de entrada + 1 de saída
#define SAIDA 19            // índice da fita de saída
#define MARCADOR -1         // nota do registro que indica fim de bloco
#define LARGURA 101         // largura interna da caixa do -P

#define ALUNO      1        // leProximo leu um aluno de verdade
#define FIM_BLOCO  0        // leProximo leu o marcador -1
#define FIM_FITA  -1        // leProximo não tinha mais nada pra ler


typedef struct {//HEAP STRUCT
    Aluno alunos[F];
    bool marcado[F];
    int tamanho;
} Heap;

//FUNCOES DO HEAP
bool vemAntesK(Heap *h, int x, int y, Analise *a);
void trocaK(Heap *h, int x, int y);
void heapRefazK(Heap *h, int i, Analise *a);
void heapConstroiK(Heap *h, Analise *a);

//FUNCOES DA FITA
void escreveRegistro(Aluno *aluno, FILE *arq, Analise *a);
void escreveFimDeBloco(FILE *arq);
int  leProximo(FILE *fita, Aluno *aluno, Analise *a);
bool reabreFita(FILE *fitas[], int i);

//MANIPULACAO DAS FITAS GERAR/INTERCALAR/REDISTRIBUIR
int  geraBlocos(FILE *arqProvao, FILE *fitas[], int quantidade, Analise *a);
bool intercalaBloco(FILE *fitas[], Analise *a);
int  Passada(FILE *fitas[], Analise *a);
void redistibuiFitas(FILE *fitas[], Analise *a);
void copiaFita(FILE *origem, FILE *arqProvao);

//IMPRESSAO 
void imprimeBorda(void);
void imprimeLinha(char *texto);
void imprimeAlunos(FILE *arq, int quantidade, char *titulo);

//FUNCAO PRINCIPAL
void realizaIntercalacaoBalanceadaf1(Analise *a, FILE *arqProvao, int quantidade, bool opt);

#endif
