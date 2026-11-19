#include "metodo_int_balanceada_2f.h"
#include "metodo_int_balanceada_f1.h"
#include "metodo_quicksort.h"
#include "base.h"


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

void imprimeQuantidadeOrdenada(char *situacao, int quantidade, int opt);

int main(int argc, char *argv[]){
    double tempoDeExecucao;
    //Verificação de comandos no console
    char *check;

    if (argc < 4){printf("Comando no console incompleto\n");  return 1;}
    bool opt = false;

    int metodo =     strtol(argv[1], &check, 10); //Métodos: [1]Intercalação Balanceada 2f [2]Intercalação Balanceada F+1 [3]Quicksort
    if (*check != '\0') {printf("Metodo invalido\n"); return 1;}
    
    int quantidade = strtol(argv[2], &check, 10); //Quantidade de alunos (linhas) a serem ordenadas
    if (*check != '\0') {printf("Quantidade invalida\n"); return 1;}

    char situacao[30];
    if      (strtol(argv[3], &check, 10) == 1)  strcpy(situacao, "Provao_crescente.bin");  //Crescente
    else if (strtol(argv[3], &check, 10) == 2)  strcpy(situacao, "Provao_decrescente.bin");//Decrescente
    else                                        strcpy(situacao, "Provao_aleatorio.bin");  //Aleatorio
    if (*check != '\0') return 1;

    if (argc == 5) opt = true;

    FILE *arqProvao = fopen(situacao, "rb+");
    if (!arqProvao) {printf("Leitura do arquivo (%s) falhou\n", situacao); return 1;}

    Analise analisador;
    inicializaAnalise(&analisador);

    if (metodo == 1){
        realizaIntercalacaoBalanceada2f(&analisador, arqProvao, quantidade);
        printf("======== Intercalacao Balanceada 2f ========\n\n");
    }
    if (metodo == 2){
        realizaIntercalacaoBalanceadaf1(&analisador, arqProvao, quantidade, opt);
        printf("======== Intercalacao Balanceada f + 1 ========\n\n");
    }
    if (metodo == 3){
        realizaQuicksort(&analisador, situacao, quantidade);
        printf("======== QuickSort ========\n\n");
    }

    tempoDeExecucao = (analisador.fim.tv_sec - analisador.inicio.tv_sec) + (analisador.fim.tv_nsec - analisador.inicio.tv_nsec)/ 1e9; //Tempo total de execução do método escolhido
    
    imprimeQuantidadeOrdenada(situacao, quantidade, opt);

    //Analise dos métodos
    printf("- Tempo de execucao: %.9f\n", tempoDeExecucao);
    printf("- Numero de comparacoes: %ld\n", analisador.comparacoes);
    printf("- Numero de transferencias (Leitura): %ld\n", analisador.transferenciasLeituras);
    printf("- Numero de transferencias (Escritas): %ld\n", analisador.transferenciasEscritas);

    fclose(arqProvao);
    return 0;
}

void imprimeQuantidadeOrdenada(char *situacao, int quantidade, int opt){
    FILE *arqBin = fopen(situacao, "rb");
    FILE *arqTxt = fopen("ordenado.txt", "w");

    Aluno *aux = (Aluno*) malloc (quantidade * sizeof(Aluno));

    if (fread(aux, sizeof(Aluno), quantidade, arqBin) != (size_t)quantidade)
        printf("Erro na criação do arquivo ordenado.txt");
    
    for (int i=0;i<quantidade;i++){
        fprintf(arqTxt, "%8ld %5.1f %84s\n", aux[i].inscricao, aux[i].nota, aux[i].informacoes);
        if (opt)
            printf("%8ld %5.1f %84s\n", aux[i].inscricao, aux[i].nota, aux[i].informacoes);
    }
    fclose(arqBin);
    fclose(arqTxt);
}
