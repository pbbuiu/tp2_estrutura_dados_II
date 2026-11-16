#include "metodo_int_balanceada_f1.h"



#include <stdlib.h>
// heap e fitas 
#define F 19
#define TOTAL_FITAS 20
#define MARCADOR -1
#define SAIDA 19 //indice da fita de saida, a ultima fita é a de saida
#define LARGURA 101  

// Constantes para verificar se aluno lido é valido.
#define ALUNO      1    // leu um aluno de verdade
#define FIM_BLOCO  0    // leu o marcador -1
#define FIM_FITA  -1

typedef struct {
    Aluno alunos[F];
    bool marcado[F];
    int tamanho; 
}Heap;

static bool vemAntes(Heap *h, int x, int y, Analise *a) {
    //Regra de ordenação, o sem marca tem prioridade, sobre o marcado
    if (h->marcado[x] != h->marcado[y]) {   
        if (h->marcado[x] == false)
            return true;    // x é o não marcado → x vem antes
        else
            return false;   // x é o marcado → y vem antes
    }
    // contador de comparação
    a->comparacoes++;

    if (h->alunos[x].nota < h->alunos[y].nota)
        return true;        // x tem nota menor, x vem antes
    else
        return false;       // y tem nota menor ou igual, y vem antes
}

static void troca(Heap *h, int x, int y) {
    Aluno auxA = h->alunos[x];
    h->alunos[x] = h->alunos[y];
    h->alunos[y] = auxA;

    //troca a marca tambem

    bool auxM = h->marcado[x];
    h->marcado[x] = h->marcado[y];
    h->marcado[y] = auxM;
}

static void heapRefaz(Heap *h, int i, Analise *a){
    while(true){
        int menor =i;
        int esq = 2*i + 1;
        int dir = 2*i + 2;

        if (esq < h->tamanho){//filho da esquerda existe e é menor
            if(vemAntes(h,esq,menor,a))
                menor= esq;
        }

        if(dir < h->tamanho){//filho da direita existe e é menor
            if(vemAntes(h,dir,menor,a))
                menor= dir;
        }

        if(menor == i)//se o menor for o proprio i, quer dizer que ele ja esta na posicao correta, entao sai do loop
            return;
    
        troca(h,i, menor);   
        i= menor;
    }
}
static void heapConstroi(Heap *h, Analise *a){//constroi o heap inicial
    int ultimoPai = h->tamanho /2 - 1;
    for(int i= ultimoPai; i>=0; i--){
        heapRefaz(h,i,a);
    }
}

static void escreveRegistro(Aluno *aluno, FILE *arq, Analise *a){//escreve um registro no arquivo e atualiza a trasnferencia de escrita
    fwrite(aluno,sizeof(Aluno),1,arq);
    a->transferenciasEscritas++;
}
static void escreveFimDeBloco(FILE *arq){//escreve o marcador de fim de bloco no arquivo
    Aluno marcadorDeFim = {0};//cria um registro de aluno com todos os campos zerados
    marcadorDeFim.nota = MARCADOR;//marca o campo nota com o valor do marcador
    fwrite(&marcadorDeFim,sizeof(Aluno),1,arq);
}

static int geraBlocos (FILE *arqProvao, FILE *fitas[], int quantidade, Analise *a){//gera o primeiro bloco no heap da "memoria interna"
    Heap h;//gera o heap
    h.tamanho=0;    
    int lidos=0;  //quantos alunos ja foram lidos.

    rewind(arqProvao); //le a aprtir do inicio do arquivo

    while(h.tamanho < F && lidos <quantidade){//criacao do primeiro bloco 
        fread(&h.alunos[h.tamanho], sizeof(Aluno), 1, arqProvao);
        a->transferenciasLeituras++;
        h.marcado[h.tamanho]= false; //todso os primeiros começam como não marcados
        h.tamanho++;
        lidos++;
    }

    heapConstroi(&h, a);//constroi o heap inicial ja ordenado
    
    int fitaAtual=0; //indice da fita atual
    int numBlocos=0; //quantidade de blocos gerados

    while(h.tamanho > 0){

        if(h.marcado[0] ==true){//se o primeiro elemento do ehap estiver amrcado, quer dizer que todso outros estão marcados, então o bloco acabou
            escreveFimDeBloco(fitas[fitaAtual]);//escreve o marcador de fim de bloco na fita atual
            numBlocos++;//+1 na quantidade de blocos gerados
            fitaAtual++;//passa para a proxima fita

            if(fitaAtual == F)//se ja tiver usado todas as fitas, volta para a primeira
                fitaAtual=0;

            for(int i=0; i<h.tamanho; i++){//remove todas as marcas do heap, para gerar o proximo bloco
                h.marcado[i]= false;
            }
        }
        
        Aluno saiu =h.alunos[0];//pega o aluno que vai sair do heap
        escreveRegistro(&saiu, fitas[fitaAtual], a);//escreve o aluno na fita atual

        if(lidos < quantidade){//se ainda houver alunos para ler do arquivo, le a o proximo e joga no heap
            fread(&h.alunos[0], sizeof(Aluno), 1, arqProvao);//le o proximo aluno do arquivo
            a->transferenciasLeituras++;
            lidos++;
            
            a->comparacoes++;
            if(h.alunos[0].nota <saiu.nota)// se for menor que o aluno que saiu, marca ele como marcado
                h.marcado[0]=true;
            else{
                h.marcado[0]=false;
            }
        }

        else{//acabou as leituras do arquivo, então joga o ultimo elemento do heap pra primeira posicao e diminui o tamanho do heap
            h.tamanho--;
            h.alunos[0]=h.alunos[h.tamanho];
            h.marcado[0]=h.marcado[h.tamanho];
        }

        heapRefaz(&h,0,a);//refaz o heap
    }

    if (lidos > 0) {
        escreveFimDeBloco(fitas[fitaAtual]);
        numBlocos++;
    }

    return numBlocos;
}

static int leProximo(FILE *fita, Aluno *aluno, Analise *a) {
    if(fread(aluno, sizeof(Aluno), 1, fita) == 0) {
        return FIM_FITA; // fim da fita
    }

    if(aluno->nota == MARCADOR){
        return FIM_BLOCO; // fim do bloco
    }

    a->transferenciasLeituras++;
    return ALUNO; // aluno válido
}

static bool intercalaBloco(FILE *fitas[], Analise *a){//se retornar true, ainda há blocos a serem intercalados, se retornar false, todas as fitas estão vazias
    Aluno atual[F]; // array para armazenar os alunos lidos de cada fita
    bool ativa[F];// array para indicar se a fita ainda tem alunos válidos
    int ativas=0; // contador de fitas ativas

    for(int i =0; i<F; i++){//carrega o primeiro aluno de cada fita e marca se a fita está ativa ou não
        if(leProximo(fitas[i],&atual[i],a)==ALUNO){
            ativa[i]=true;
            ativas++;
        }
        else{
            ativa[i]=false;
        }
    }

    if(ativas == 0) {
        return false; // todas as fitas estão vazias
    }

    while(ativas > 0){//enquanto ainda tiver fitas ativas;

        int menor=-1;
        for(int i=0; i<F;i++){//busca o menor aluno entre as fitas ativas
            if(ativa[i] == true){
                if(menor == -1){
                    menor = i;//primeiro aluno ativo encontrado, assume ele como o menor
                }
                else{
                    a->comparacoes++;
                    if (atual[i].nota < atual[menor].nota) {
                        menor = i; //a posicao i tem o menor aluno, consequentemente o aluno da fita i
                    }

                }
            }
        }
        escreveRegistro(&atual[menor], fitas[SAIDA], a);//escreve o menor aluno na fita de saida

        
        if(leProximo(fitas[menor], &atual[menor], a) != ALUNO){//se a fita do menor aluno acabou, marca ela como inativa
            ativa[menor]=false;//marca a fita como inativa
            ativas--;
        }
    }

    escreveFimDeBloco(fitas[SAIDA]);//escreve o marcador de fim de bloco na fita de saida

    return true; // ainda há blocos a serem intercalados
}

static bool reabreFita(FILE *fitas[], int i) {//esvazia a fita i e reabre ela para escrita e leitura.
    
    char nome[20];
    sprintf(nome, "fita%02d.bin", i);

    fclose(fitas[i]);
    fitas[i] = fopen(nome, "wb+");

    if (fitas[i] == NULL) {
        printf("Erro ao reabrir a fita %s\n", nome);
        return false;
    }
    return true;
}

static int Passada(FILE *fitas[], Analise *a) {

    for(int i=0; i<F; i++){
        rewind(fitas[i]); // volta para o início de cada fita
    }

    reabreFita(fitas, SAIDA); // reabre a fita de saída para escrita e leitura

    int blocosSaida = 0; // contador de blocos gerados na fita de saída
    while(intercalaBloco(fitas, a)==true) {//intercala os blocos das fitas de entrada e escreve na fita de saída    
        blocosSaida++;
    }

    return blocosSaida;//retorna quantos blocos foram pra saida;
}

static void redistribuiFitas(FILE *fitas[], Analise *a) {//redistribui os blocos da fita de saída para as fitas de entrada
    
    for(int i =0; i<F ; i++){
        reabreFita(fitas, i); // limpa mtodas as fitas de entrada e reabre para escrita e leitura
    }

    rewind(fitas[SAIDA]); // volta para o início da fita de saída

    int fitaAtual = 0;
    Aluno aluno;
    int resultado = leProximo(fitas[SAIDA], &aluno, a);

    while(resultado != FIM_FITA) {//enquanto não chegar no fim da fita de saída
        if(resultado == ALUNO){//enquanto for um aluno valido e não o marcador de fim de bloco, escreve na fita atual
            escreveRegistro(&aluno, fitas[fitaAtual], a);
        } 
        else{//quando chega no marcador de fim de bloco, escreve o marcador na fita atual e passa para a proxima fita
            escreveFimDeBloco(fitas[fitaAtual]);
            fitaAtual++;
            if(fitaAtual == F){//se ja tiver usado todas as fitas, volta para a primeira
                fitaAtual=0;
            }   
        }
        resultado= leProximo(fitas[SAIDA], &aluno, a);//le o proximo aluno da fita de saida
    }

}

static void copiaFita(FILE *origem, FILE *arqProvao) {//copia o conteudo da fita de origem para a fita de destino
    rewind(origem);
    rewind(arqProvao);

    Aluno aluno;
    while(fread(&aluno, sizeof(Aluno), 1, origem)==1){
        if(aluno.nota == MARCADOR)
            break; // encontrou o marcador de fim de bloco(-1), para a cópia
        fwrite(&aluno, sizeof(Aluno), 1, arqProvao);

    }
    fflush(arqProvao); //forcar a gravar no arquivo 
    }

static void imprimeBorda() {
    printf("+");
    for (int i = 0; i < LARGURA; i++)
        printf("-");
    printf("+\n");
}

static void imprimeLinha(char *texto) {
    printf("| %-99s |\n", texto);
}

static void imprimeAlunos(FILE *arq, int quantidade, char *titulo) {
    rewind(arq);
    Aluno aluno;

    imprimeBorda();
    imprimeLinha(titulo);
    imprimeBorda();

    for (int i = 0; i < quantidade; i++) {
        if (fread(&aluno, sizeof(Aluno), 1, arq) == 0)
            break;                       // arquivo acabou antes do esperado
        printf("| %08ld %5.1f %-84s |\n", aluno.inscricao, aluno.nota, aluno.informacoes);
    }

    imprimeBorda();
}

void realizaIntercalacaoBalanceadaf1(Analise *a, FILE *arqProvao, int quantidade, bool opt){

    if(opt == true){
        printf("\n\n");
        imprimeAlunos(arqProvao, quantidade, "ALUNOS ANTES DA ORDENACAO");
    }

    clock_gettime(CLOCK_MONOTONIC, &a->inicio);  //inicia a contagem do tempo de execução

    //Abre as 20 fitas: fita00.bin, fita01.bin, ..., fita19.bin
    FILE *fitas[TOTAL_FITAS];//
    char nomeFita[20];
    for(int i=0; i<TOTAL_FITAS; i++){//nomear as 20 fitas
        sprintf(nomeFita, "fita%02d.bin", i);
        fitas[i] = fopen(nomeFita, "wb+");//abrea  fita pra escrita e leitura
        if(fitas[i] == NULL) {
            printf("Erro ao abrir/criar a fita %s\n", nomeFita);
            return;
        }
    }

    int numBlocos = geraBlocos(arqProvao, fitas, quantidade, a);//gera os blocos iniciais e retorna a quantidade de blocos gerados
    int blocosGerados = numBlocos;

    int numPassadas = 0; // contador de passadas
    int numRedistribuicoes = 0; // contador de redistribuições

    int fitaResultado=0; // índice da fita que contém o resultado final

    while(numBlocos > 1){
        numBlocos = Passada(fitas, a);
        fitaResultado=SAIDA; // a fita de saída contém o resultado final
        numPassadas++;

        if(numBlocos > 1) {
            redistribuiFitas(fitas, a); // redistribui os blocos da fita de saída para as fitas de entrada
            numRedistribuicoes++;
        }
    }
    
    copiaFita(fitas[fitaResultado], arqProvao); // copia o conteúdo da fita de resultado para o arquivo de prova

    for(int i=0; i<TOTAL_FITAS; i++){//fecha as fitas
        fclose(fitas[i]);
    }

      clock_gettime(CLOCK_MONOTONIC, &a->fim);  //finaliza a contagem do tempo de execução

     
    for (int i = 0; i < TOTAL_FITAS; i++) {//Apaga as fitas do disco 
        sprintf(nomeFita, "fita%02d.bin", i);
        remove(nomeFita);
    }

    if(opt == true){
        printf("\n\n");
        imprimeAlunos(arqProvao, quantidade, "ALUNOS DEPOIS DA ORDENACAO");
        printf("\n\n");
    }

    char texto[100];
    imprimeBorda();
    imprimeLinha("RESUMO - INTERCALACAO BALANCEADA F+1");
    imprimeBorda();
    sprintf(texto, "Blocos iniciais gerados: %d", blocosGerados);
    imprimeLinha(texto);
    sprintf(texto, "Passadas de intercalacao: %d", numPassadas);
    imprimeLinha(texto);
    sprintf(texto, "Redistribuicoes: %d", numRedistribuicoes);
    imprimeLinha(texto);
    imprimeBorda();

}