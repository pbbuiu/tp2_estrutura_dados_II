#include "base.h"
#include <stdio.h>
#include <stdlib.h>

// Função de comparação para ordem CRESCENTE (menor nota -> maior nota)
int comparaCrescente(const void *a, const void *b) {
    Aluno *alunoA = (Aluno *)a;
    Aluno *alunoB = (Aluno *)b;

    if (alunoA->nota < alunoB->nota) return -1;
    if (alunoA->nota > alunoB->nota) return 1;
    return 0;
}

// Função de comparação para ordem DECRESCENTE (maior nota -> menor nota)
int comparaDecrescente(const void *a, const void *b) {
    Aluno *alunoA = (Aluno *)a;
    Aluno *alunoB = (Aluno *)b;

    if (alunoA->nota > alunoB->nota) return -1;
    if (alunoA->nota < alunoB->nota) return 1;
    return 0;
}

int main() {
    FILE *arqOrigem = fopen("Provao_aleatorio.bin", "rb");

    // Descobre a quantidade total de registros no arquivo binário
    fseek(arqOrigem, 0, SEEK_END);
    long tamanhoBytes = ftell(arqOrigem);
    int totalAlunos = tamanhoBytes / sizeof(Aluno);
    rewind(arqOrigem);

    // Aloca o vetor e lê todos os registros de uma vez
    Aluno *vetor = (Aluno *)malloc(totalAlunos * sizeof(Aluno));
    fread(vetor, sizeof(Aluno), totalAlunos, arqOrigem);
    fclose(arqOrigem);

    // 1. Ordena CRESCENTEMENTE e salva
    qsort(vetor, totalAlunos, sizeof(Aluno), comparaCrescente);
    FILE *arqCresc = fopen("Provao_crescente.bin", "wb");
    fwrite(vetor, sizeof(Aluno), totalAlunos, arqCresc);
    fclose(arqCresc);

    // 2. Ordena DECRESCENTEMENTE e salva
    qsort(vetor, totalAlunos, sizeof(Aluno), comparaDecrescente);
    FILE *arqDecresc = fopen("Provao_decrescente.bin", "wb");
    fwrite(vetor, sizeof(Aluno), totalAlunos, arqDecresc);
    fclose(arqDecresc);

    free(vetor);
    return 0;
}