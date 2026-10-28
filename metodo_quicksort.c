#include "metodo_quicksort.h"

void quickSort(FILE **arqLi, FILE **arqEi, FILE ** arqLEs, int esq, int dir);

void realizaQuicksort(Analise *a, char *nomeArquivo, int quantidade){
    FILE *Li = fopen (nomeArquivo, "rb+");
    FILE *Ei = fopen (nomeArquivo, "rb+");
    FILE *LEs = fopen (nomeArquivo, "rb+");
    quickSort(&Li, &Ei, &LEs, 1, quantidade);
}

void quickSort(FILE **arqLi, FILE **arqEi, FILE ** arqLEs, int esq, int dir){
    // int Ls = dir, Es = dir;
    // long int linf = -999999999, lsup = 999999999;
    // int i, j;
}

/*
== Variaveis: ==
Li Ls leituras
Ei Es escritas
linf lsup limites
j e i delimitadores de subarquivos
Esq Dir
TamEsq TamDir tamanhos
*/