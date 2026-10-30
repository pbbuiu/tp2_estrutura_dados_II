#ifndef METODO_INT_BALANCEADA_2F_H
#define METODO_INT_BALANCEADA_2F_H

#include "base.h"
#include <stdio.h>
#include <stdbool.h>

void realizaIntercalacaoBalanceada2f(Analise *a, FILE * arqProvao, int quantidade, bool opt);


#endif //INTERCALACAO_BALANCEADA_F+1_H

/*
Para o método 1, deve ser considerada a existência de (a) memória interna disponível para armazenar um
vetor de, no máximo, 10 registros, e (b) 20 fitas de armazenamento externo, sendo 10 fitas de entrada e
10 fitas de saída a cada etapa de intercalação
*/