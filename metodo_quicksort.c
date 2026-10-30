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

void retiraMin(TipoArea *area, Aluno *retirado);
void retiraMax(TipoArea *area, Aluno *retirado);

void escreveMin(Aluno *escrito, FILE **arqEi, int *Ei);
void escreveMax(Aluno *escrito, FILE **arqEs, int *Es);

//Função chamada pela main para realização da ordenação aplicando o método do QuickSort
void realizaQuicksort(Analise *a, char *nomeArquivo, int quantidade){
    //Gera os ponteiros que irão se locomover através dos arquivos
    FILE *Li = fopen (nomeArquivo, "rb+");
    FILE *Ei = fopen (nomeArquivo, "rb+");
    FILE *LEs = fopen (nomeArquivo, "rb+");
    QuickSort(&Li, &Ei, &LEs, 1, quantidade);
    fclose(Li);
    fclose(Ei);
    fclose(LEs);
}

//Realiza a partição dos arquivos e faz a recursivade com os sub-arquivos acontecer
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

//Faz com que não tenha nenhum elemento na memória principal (area)
void esvaziaArea(TipoArea *area){
    area->numAlunosArea = 0;
}

void Particao(FILE **arqLi, FILE **arqEi, FILE ** arqLEs, TipoArea area,int esq, int dir, int *i, int *j){
    //Criação e inicialização de variáveis e ponteiros de arquivos
    int Li = esq, Ls = dir, Ei = esq, Es = dir;
    float Linf = -1, Lsup = 101;
    
    *i = esq-1;
    *j = dir+1;

    bool ondeLer = true;
    esvaziaArea(&area);
    Aluno ultLido, retirado;
    
    fseek(*arqLi, (Li-1) * sizeof(Aluno), SEEK_SET);
    fseek(*arqEi, (Ei-1) * sizeof(Aluno), SEEK_SET);


    while(Ls >= Li){ //Enquanto os ponteiros de leitura não se cruzarem o loop irá acontecer
        if (area.numAlunosArea < TAMAREA - 1){ //Irá preencher até o penúltimo elemento da memória principal (area)
            if (ondeLer)
                LeSup (arqLEs, &ultLido, &Ls, &ondeLer);
            else
                LeInf(arqLi, &ultLido, &Li, &ondeLer);

            inserirArea(&ultLido, &area);
            continue;
        }

        //Fazendo a leitura primeiramente onde Li,Ls == Ei,Es para que não haja perca de itens
        if (Ls == Es)
            LeSup (arqLEs, &ultLido, &Ls, &ondeLer);
        else if (Li == Ei)
            LeInf(arqLi, &ultLido, &Li, &ondeLer);
        else if (ondeLer)
            LeSup (arqLEs, &ultLido, &Ls, &ondeLer);
        else
            LeInf(arqLi, &ultLido, &Li, &ondeLer);
        
        if (ultLido.nota < Linf){ //Elemento abaixo do limite inferior
            escreveMin(&ultLido, arqEi, &Ei);
            *i = Ei;
        }
        else if (ultLido.nota > Lsup){ //Elemento acima do limite inferior
            escreveMax(&ultLido, arqLEs, &Es);
            *j = Es;
        }
        else{ //Elemento dentro do limite inferior e superior
            inserirArea(&ultLido, &area);
            if ((Ei-esq) < (dir-Es)){ //T1 < T2
                retiraMin(&area, &retirado);
                escreveMin(&retirado, arqEi, &Ei);
                Lsup = retirado.nota;
                *j = Es;
            }
            else{ //T2 <= T1
                retiraMax(&area, &retirado);
                escreveMax(&retirado, arqLEs, &Es);
                Linf = retirado.nota;
                *i = Ei;
            }
        }   
    }
    while (Ei <= Es){
        retiraMin(&area, &retirado);
        escreveMin(&retirado, arqEi, &Ei);
    }
}

//Le no ponteiro de leitura superior
void LeSup(FILE **arqLs, Aluno *ultLido, int *Ls, bool *ondeLer){
    fseek(*arqLs, (*Ls - 1) * sizeof(Aluno), SEEK_SET);
    fread(ultLido, sizeof(Aluno), 1, *arqLs);
    (*Ls)--;
    *ondeLer = false;
}
//Le no ponteiro de leitura inferior
void LeInf(FILE **arqLi, Aluno *ultLido, int *Li, bool *ondeLer){
    fread(ultLido, sizeof(Aluno), 1, *arqLi);
    (*Li)++;
    *ondeLer = true;
}

//Insere o item de forma ordenada na memória principal (area)
void inserirArea(Aluno *ultLido, TipoArea *area){
    int i = area->numAlunosArea;
    while (i > 0 && area->alunos[i-1].nota > ultLido->nota){
        area->alunos[i] = area->alunos[i-1];
        i--;
    }
    area->alunos[i] = *ultLido;
    area->numAlunosArea++;
}

//Retira o menor item da memória principal (area)
void retiraMin(TipoArea *area, Aluno *retirado){
    *retirado = area->alunos[0];
    for (int i = 0; i < area->numAlunosArea; i++)
    area->alunos[i] = area->alunos[i+1];
    area->numAlunosArea--;
}
//Retira o maior item da memória principal (area)
//
void retiraMax(TipoArea *area, Aluno *retirado){
    *retirado = area->alunos[area->numAlunosArea-1];
    area->numAlunosArea--;
}

//Escreve na parte dos menores elementos que os pivôs
void escreveMin(Aluno *escrito, FILE **arqEi, int *Ei){
    fwrite(escrito, sizeof(Aluno), 1, *arqEi);
    (*Ei)++;
}
//Escreve na parte dos maiores elementos que os pivôs
void escreveMax(Aluno *escrito, FILE **arqEs, int *Es){
    fseek(*arqEs, (*Es-1) * sizeof(Aluno), SEEK_SET);
    fwrite(escrito, sizeof(Aluno), 1, *arqEs);
    (*Es)--;

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