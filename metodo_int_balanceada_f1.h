#ifndef METODO_INT_BALANCEADA_F1_H
#define METODO_INT_BALANCEADA_F1_H

#include "base.h"
#include <stdio.h>

void realizaIntercalacaoBalanceadaf1(Analise *a, FILE *arqProvao, int quantidade);

#endif //INTERCALACAO_BALANCEADA_2F_H


/*
Para o método 2, deve ser considerada a existência de
(a) memória interna disponível para armazenar um vetor de, no máximo, 19 registros, e (b) 20 fitas de
armazenamento externo, sendo 19 fitas de entrada e 1 fita de saída a cada etapa de intercalação
*/