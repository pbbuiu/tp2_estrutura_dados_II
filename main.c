#include "intercalacao_balanceada_TwoF.h"
#include "int_balanceada_FPlusOne.h"
#include "quicksort.h"
#include "base.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

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
    if      (strtol(argv[3], &check, 10) == 1)  strcpy(situacao, "Provao_crescente.bin");  //Crescente
    else if (strtol(argv[3], &check, 10) == 2)  strcpy(situacao, "Provao_decrescente.bin");//Decrescente
    else                                        strcpy(situacao, "Provao_aleatorio.bin");  //Aleatorio
    if (*check != '\0') return 1;

    if (argc == 6) opt = true;

    FILE *arqProvao = fopen("PROVAO.TXT", "r");
    if (!arqProvao) {printf("Leitura do arquivo (PROVAO.TXT) falhou\n"); return 1;}

    Analise a;
    inicializaAnalise(&a);

    if (metodo == 1){
        // realizaIntercalacaoBalanceadaTwoF(&a, arqProvao, quantidade);
    }
    if (metodo == 2){
        // realizaIntercalacaoBalanceadaFPlusOne(&a, arqProvao, quantidade);
    }
    if (metodo == 3){
        // realizaQuicksort(&a, arqProvao, quantidade);
    }

    if (opt) printf("%d\n", quantidade);
    return 0;
}