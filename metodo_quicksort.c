#include "metodo_quicksort.h"
#include <stdbool.h>
#include <stdlib.h>

#define TAMAREA 10

typedef struct{
    Aluno alunos[TAMAREA];
    int numAlunosArea;
} TipoArea;

void esvaziaArea(TipoArea *area);

void QuickSort(FILE **arqLi, FILE **arqEi, FILE ** arqLEs, int esq, int dir);

void Particao(FILE **arqLi, FILE **arqEi, FILE ** arqLEs, TipoArea area,int esq, int dir, int *i, int *j);

void LeSup(FILE **arqLs, Aluno *ultLido, int *Ls, bool *ondeLer);
void LeInf(FILE **arqLi, Aluno *ultLido, int *Li, bool *ondeLer);

void inserirArea(Aluno *ultLido, TipoArea *area);



void realizaQuicksort(Analise *a, char *nomeArquivo, int quantidade){
    FILE *Li = fopen (nomeArquivo, "rb+");
    FILE *Ei = fopen (nomeArquivo, "rb+");
    FILE *LEs = fopen (nomeArquivo, "rb+");
    QuickSort(&Li, &Ei, &LEs, 1, quantidade);
    fclose(Li);
    fclose(Ei);
    fclose(LEs);
}

void QuickSort(FILE **arqLi, FILE **arqEi, FILE ** arqLEs, int esq, int dir){
    int i,j;
    TipoArea area;
    
    if (dir - esq < 1) return; //Há apenas 1 elemento no caso base, que siginifica que o subarquivo já está ordenado
    
    esvaziaArea(&area);
    Particao(arqLi, arqEi, arqLEs, area, esq, dir, &i, &j);

    if (i - esq < dir - j){
        QuickSort(arqLi, arqEi, arqLEs, esq, i);
        QuickSort(arqLi, arqEi, arqLEs, j, dir);
    }
    else{
        QuickSort(arqLi, arqEi, arqLEs, j, dir);
        QuickSort(arqLi, arqEi, arqLEs, esq, i);
    }
}

void esvaziaArea(TipoArea *area){
    area->numAlunosArea = 0;
}

void Particao(FILE **arqLi, FILE **arqEi, FILE ** arqLEs, TipoArea area,int esq, int dir, int *i, int *j){
    int Li = esq, Ls = dir, Ei = esq, Es = dir;
    bool ondeLer = true;
    esvaziaArea(&area);
    Aluno ultLido, var;
    
    fseek(*arqLi, (Li-1) * sizeof(Aluno), SEEK_SET);
    fseek(*arqEi, (Ei-1) * sizeof(Aluno), SEEK_SET);

    *i = esq-1;
    *j = dir+1;

    while(Ls >= Li){
        if (area.numAlunosArea < TAMAREA - 1){
            if (ondeLer)
                LeSup (arqLEs, &ultLido, &Ls, &ondeLer);
            else
                LeInf(arqLi, &ultLido, &Li, &ondeLer);

            inserirArea(&ultLido, &area);
            continue;
        }

        if (ondeLer)
            LeSup (arqLEs, &ultLido, &Ls, &ondeLer);
        else
            LeInf(arqLi, &ultLido, &Li, &ondeLer);
        if 
        
            
    }
}

//Le no ponteiro de leitura superior
void LeSup(FILE **arqLs, Aluno *ultLido, int *Ls, bool *ondeLer){
    fseek(*arqLs, (*Ls - 1) * sizeof(Aluno), SEEK_SET);
    fread(ultLido, sizeof(Aluno), 1, *arqLs);
    (*Ls)--;
    *ondeLer = false;
}

void LeInf(FILE **arqLi, Aluno *ultLido, int *Li, bool *ondeLer){
    fread(ultLido, sizeof(Aluno), 1, *arqLi);
    (*Li)++;
    *ondeLer = true;
}

void inserirArea(Aluno *ultLido, TipoArea *area){
    int i = area->numAlunosArea;
    while (i > 0 && area->alunos[i-1].nota > ultLido->nota){
        area->alunos[i] = area->alunos[i-1];
        i--;
    }
    area->alunos[i] = *ultLido;
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