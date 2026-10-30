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
    double tempoDeExecucao;
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

    

    Analise analisador;
    inicializaAnalise(&analisador);

    if (metodo == 1){
        realizaIntercalacaoBalanceada2f(&analisador, arqProvaoBin, quantidade, opt);
        printf("======== Intercalacao Balanceada 2f ========\n\n");
    }
    if (metodo == 2){
        realizaIntercalacaoBalanceadaf1(&analisador, arqProvaoBin, quantidade, opt);
        printf("======== Intercalacao Balanceada f + 1 ========\n\n");
    }
    if (metodo == 3){
        realizaQuicksort(&analisador, nomeArqProvaBin, quantidade, opt);
        printf("======== QuickSort ========\n\n");
    }

    tempoDeExecucao = (analisador.fim.tv_sec - analisador.inicio.tv_sec) + (analisador.fim.tv_nsec - analisador.inicio.tv_nsec)/ 1e9; //Tempo total de execução do método escolhido
    
    //Analise dos métodos
    printf("- Tempo de execucao: %.9f\n", tempoDeExecucao);
    printf("- Numero de comparacoes: %ld\n", analisador.comparacoes);
    printf("- Numero de transferencias (Leitura): %ld\n", analisador.transferenciasLeituras);
    printf("- Numero de comparacoes (Escritas): %ld\n", analisador.transferenciasEscritas);

    fclose(arqProvaoBin);
    return 0;
}

FILE * transformaBin(FILE *arqProvao, char *nomeArqProvaoBin){

    fclose(arqProvao);
    return NULL;
}