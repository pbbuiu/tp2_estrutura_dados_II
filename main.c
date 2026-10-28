#include "metodo_int_balanceada_2f.h"
#include "metodo_int_balanceada_f1.h"
#include "metodo_quicksort.h"
#include "base.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

FILE * transformaBin(FILE *arqProvao, char *nomeArqProvaoBin);

int main(int argc, char *argv[]){
    //Verificação de comandos no console
    char *check;

    if (argc < 5){printf("Comando no console incompleto\n");  return 1;}
    bool opt;

    int metodo =     strtol(argv[1], &check, 10); //Métodos: [1]Intercalação Balanceada 2f [2]Intercalação Balanceada F+1 [3]Quicksort
    if (*check != '\0') {printf("Metodo invalido\n"); return 1;}
    
    int quantidade = strtol(argv[2], &check, 10); //Quantidade de alunos (linhas) a serem ordenadas
    if (*check != '\0') {printf("Quantidade invalida\n"); return 1;}

    char situacao[30];
    if      (strtol(argv[3], &check, 10) == 1)  strcpy(situacao, "Provao_crescente.txt");  //Crescente
    else if (strtol(argv[3], &check, 10) == 2)  strcpy(situacao, "Provao_decrescente.txt");//Decrescente
    else                                        strcpy(situacao, "Provao_aleatorio.txt");  //Aleatorio
    if (*check != '\0') return 1;

    if (argc == 6) opt = true;

    FILE *arqProvao = fopen(situacao, "r");
    if (!arqProvao) {printf("Leitura do arquivo (PROVAO.TXT) falhou\n"); return 1;}

    char *nomeArqProvaBin = NULL;
    FILE *arqProvaoBin = transformaBin(arqProvao, nomeArqProvaBin);
    if (!arqProvaoBin) {printf("Não foi possivel transformar o arquivo em bin\n"); return 1;}

    

    Analise a;
    inicializaAnalise(&a);

    if (metodo == 1){
        realizaIntercalacaoBalanceada2f(&a, arqProvaoBin, quantidade);
    }
    if (metodo == 2){
        realizaIntercalacaoBalanceadaf1(&a, arqProvaoBin, quantidade);
    }
    if (metodo == 3){
        realizaQuicksort(&a, nomeArqProvaBin, quantidade);
    }

    if (opt) printf("%d\n", quantidade); //Apenas para não acusar erro de 
    fclose(arqProvaoBin);
    return 0;
}

FILE * transformaBin(FILE *arqProvao, char *nomeArqProvaoBin){

    fclose(arqProvao);
    return NULL;
}