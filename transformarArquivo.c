#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "base.h"
#include <string.h>

// usar q_sort , so pensei nisso.
int compararDecrescente(const void *a, const void *b){
  Aluno *a1 = (Aluno*)a;
  Aluno *a2 = (Aluno*)b;

  if(a2->nota > a1->nota) return 1;
  if(a2->nota < a1->nota) return -1;
  return 0;
}

int compararCrescente(const void *a, const void *b){
  Aluno *a1 = (Aluno*)a;
  Aluno *a2 = (Aluno*)b;

  if(a1->nota > a2->nota) return 1;
  if(a1->nota < a2->nota) return -1;
  return 0;
}

int main(void){
  //Criacao arquivos que vao sair.
  FILE* arqBin  = fopen("Provao_aleatorio.bin","wb"); // original ja e aleatorio
  FILE* arqBinD = fopen("Provao_decrescente.bin","wb");
  FILE* arqBinA = fopen("Provao_crescente.bin","wb");
  FILE* arqTxt  = fopen("PROVAO.TXT","r");
  if(arqTxt==NULL ) {
    printf("Erro ao abrir arquivo\n");
    return 1;
  }

  int total=0;
  char linha[200];
  long inscricao;
  double nota;

  while(fgets(linha,sizeof(linha),arqTxt)){
    if(sscanf(linha,"%ld %lf",&inscricao,&nota) != 2) continue;

    Aluno base = {0};
    base.inscricao = inscricao;
    base.nota = nota;
    strncpy(base.informacoes,linha+15,sizeof(base.informacoes)-1);
    base.informacoes[strcspn(base.informacoes,"\r\n")] = '\0';

    fwrite(&base,sizeof(Aluno),1,arqBin);
    total++;//total de alunos(usar fazer veto)
  }
  fclose(arqTxt);
  fclose(arqBin);

  Aluno* vet = malloc(total * sizeof(Aluno));
  if(!vet){
    printf("Erro de memoria\n");
    return 1;
  }
  arqBin = fopen("Provao_aleatorio.bin","rb");
  //colocar no vetor para fazer as operacoes ()
  fread(vet,sizeof(Aluno),total,arqBin);
  fclose(arqBin);

  // poderia ter criado outro vetor aq para nao precisar , ordenar ascendentemente o descrescente mas da trabalho demais.
  //criacao arquivo decrescende de uma vez
  qsort(vet,total,sizeof(Aluno),compararDecrescente);
  fwrite(vet,sizeof(Aluno),total,arqBinD);
  fclose(arqBinD);
  //criacao arquivo crescente de uma vez
  qsort(vet,total,sizeof(Aluno),compararCrescente);
  fwrite(vet,sizeof(Aluno),total,arqBinA);
  fclose(arqBinA);

  free(vet);
  printf("Total Alunos: %d alunos\n",total);
  return 0;
}