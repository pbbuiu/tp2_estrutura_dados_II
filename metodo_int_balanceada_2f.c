#include "metodo_int_balanceada_2f.h"

void InicializadorFitas(ArqFitas *arq)
{
    for(int i = 0; i < num_arq; i++)
    {
        sprintf(arq->nomearq[i], "fita%d.bin", (i + 1));
        if(i < 10)
            arq->Fitas[i] = fopen(arq->nomearq[i], "wb+");
    }
}

void DestruirFitas(ArqFitas *arq, bool opc, bool fim)
{
    int i = 0;
    if(fim)
        i++;
    if(opc)
    {
        for(i = i; i < 10; i++)
        {
            fclose(arq->Fitas[i]);
            remove(arq->nomearq[i]);
        }  
    }
    else
    {
        for(i = i; i < 10; i++)
        {
            fclose(arq->Fitas[(i + 10)]);
            remove(arq->nomearq[(i + 10)]);
        }  
    }
}

void ReabrirFitas(ArqFitas *arq, bool opc)
{
    if(opc)
    {
        for(int i = 10; i < 20; i++)
            arq->Fitas[i] = fopen(arq->nomearq[i], "wb+");
    }
    else
    {
        for(int i = 0; i < 10; i++)
            arq->Fitas[i] = fopen(arq->nomearq[i], "wb+");
    }
}

void realizaIntercalacaoBalanceada2f(Analise *a, FILE * arqProvao, int quantidade)
{
    if (!a || !arqProvao)
    {
        printf("Erro: parametro(s) invalido(s).\n");
        return;
    }

    if (quantidade <= 0)
    {
        printf("Erro: quantidade de itens invalido.\n");
        return;
    }

    ArqFitas arq;
    Pagina entrada[10], saida;
    int arqatual = 0;
    
    InicializadorFitas(&arq);
    entrada[0].ind = 0;
    saida.ind = 0;
    saida.qntlida = qnt_pagina;

    inicializaAnalise(a);

    clock_gettime(CLOCK_MONOTONIC, &a->inicio);

    entrada[0].qntlida = fread(entrada[0].v, sizeof(Aluno), qnt_pagina, arqProvao);
    Mem_Int memoria;
    IniciaHeap(&memoria, &entrada[0]);

    for (long int i = 10; i < quantidade; i++, entrada[0].ind++)
    {
        if(entrada[0].ind == entrada[0].qntlida)
        {
            entrada[0].ind = 0;
            entrada[0].qntlida = fread(entrada[0].v, sizeof(Aluno), qnt_pagina, arqProvao);
            a->transferencia++;
            if(!entrada[0].qntlida)
            {
                printf("Erro ao ler o registro %ld do arquivo-base.\n", i);
                //CalculaTempoArvoreBEstrela(a);
                DestruirFitas(&arq, true, false);
                return;
            }
        }
        saida.v[saida.ind] = memoria.cel[0].alu;
        saida.ind++;

        if(entrada[0].v[entrada[0].ind].nota < memoria.cel[0].alu.nota)
        {
            memoria.cel[0].mar = true;
        }
        memoria.cel[0].alu = entrada[0].v[entrada[0].ind];
        OrganizaHeap(&memoria, 0);

        if(saida.ind == saida.qntlida || memoria.cel[0].mar)
        {
            fwrite(saida.v, sizeof(Aluno), saida.ind, arq.Fitas[arqatual]);
            saida.ind = 0;
            if(memoria.cel[0].mar)
            {
                for(int j = 0; j < 10; j++)
                    memoria.cel[j].mar = false;
                arqatual = (arqatual + 1) % 10;
            }
        }
    }

    for(int i = 0; i <= 10; i++)
    {
        if(saida.ind == saida.qntlida || memoria.cel[i].mar || i == 10)
        {
            fwrite(saida.v, sizeof(Aluno), saida.ind, arq.Fitas[arqatual]);
            saida.ind = 0;
            if(i == 10)
                break;
            if(memoria.cel[i].mar)
            {
                for(int j = i; j < 10; j++)
                    memoria.cel[j].mar = false;
                arqatual = (arqatual + 1) % 10;
            }
        }

        saida.v[saida.ind] = memoria.cel[0].alu;
        saida.ind++;
        RemoverHeap(&memoria);
    }

    arqatual = 0;
    bool flag = true, flag2 = true, lado = true;

    while(flag && !flag2)
    {
        if(lado)
        {
            flag2 = true; 
            memoria.qnt = 0;
            for(int i = 0; i < 10; i++)
            {
                rewind(arq.Fitas[i]);
                entrada[i].ind = 1; //O primeiro item já vai ser colocado na memoria interna
                entrada[i].qntlida = fread(entrada[i].v, sizeof(Aluno), qnt_pagina, arq.Fitas[i]);
                a->transferencia++;
                if(!entrada[i].qntlida)
                {
                    flag = false;
                    break;
                }
                memoria.cel[i].alu = entrada[i].v[0];
                memoria.cel[i].arq = i;
            }
            IniciaHeap2(&memoria);
            ReabrirFitas(&arq, lado);
            arqatual = 0;
            for(long int i = 0; i < quantidade; i++)
            {
                if(saida.ind == saida.qntlida || memoria.cel[0].mar)
                {
                    fwrite(saida.v, sizeof(Aluno), saida.ind, arq.Fitas[(arqatual + 10)]);
                    saida.ind = 0;
                    if(memoria.cel[0].mar)
                    {
                        for(int j = 0; j < 10; j++)
                            memoria.cel[j].mar = false;
                        arqatual = (arqatual + 1) % 10;
                        flag2 = false;
                    }
                }
                saida.v[saida.ind] = memoria.cel[0].alu;
                saida.ind++;
                if(entrada[memoria.cel->arq].ind == entrada[memoria.cel->arq].qntlida)
                {
                    entrada[memoria.cel->arq].ind = 0;
                    entrada[memoria.cel->arq].qntlida = fread(entrada[memoria.cel->arq].v, sizeof(Aluno), qnt_pagina, arq.Fitas[memoria.cel->arq]);
                    a->transferencia++;
                    if(!entrada[memoria.cel->arq].qntlida)
                    {
                        RemoverHeap(&memoria);
                        continue;
                    }
                }
                if(entrada[memoria.cel->arq].v[entrada[memoria.cel->arq].ind].nota < memoria.cel[0].alu.nota)
                {
                    memoria.cel[0].mar = true;
                }
                memoria.cel[0].alu = entrada[memoria.cel->arq].v[entrada[memoria.cel->arq].ind];
                entrada[memoria.cel->arq].ind++;
                OrganizaHeap(&memoria, 0);
            }
            DestruirFitas(&arq, lado, false);
            lado = false;
        }
        else 
        {
            flag2 = true;
            memoria.qnt = 0;
            for(int i = 0; i < 10; i++)
            {
                rewind(arq.Fitas[(i + 10)]);
                entrada[i].ind = 1; 
                entrada[i].qntlida = fread(entrada[i].v, sizeof(Aluno), qnt_pagina, arq.Fitas[(i + 10)]);
                a->transferencia++;
                if(!entrada[i].qntlida)
                {
                    flag = false;
                    break;
                }
                memoria.cel[i].alu = entrada[i].v[0];
                memoria.cel[i].arq = i + 10;
                memoria.qnt++;
            }
            IniciaHeap2(&memoria);
            ReabrirFitas(&arq, lado);
            arqatual = 0;
            for(long int i = 0; i < quantidade; i++)
            {
                if(saida.ind == saida.qntlida || memoria.cel[0].mar)
                {
                    fwrite(saida.v, sizeof(Aluno), saida.ind, arq.Fitas[arqatual]);
                    saida.ind = 0;
                    if(memoria.cel[0].mar)
                    {
                        for(int j = 0; j < 10; j++)
                            memoria.cel[j].mar = false;
                        arqatual = (arqatual + 1) % 10;
                        flag2 = false;
                    }
                }
                saida.v[saida.ind] = memoria.cel[0].alu;
                saida.ind++;
                if(entrada[(memoria.cel->arq % 10)].ind == entrada[(memoria.cel->arq % 10)].qntlida)
                {
                    entrada[(memoria.cel->arq % 10)].ind = 0;
                    entrada[(memoria.cel->arq % 10)].qntlida = fread(entrada[(memoria.cel->arq % 10)].v, sizeof(Aluno), qnt_pagina, arq.Fitas[(memoria.cel->arq % 10)]);
                    a->transferencia++;
                    if(!entrada[(memoria.cel->arq % 10)].qntlida)
                    {
                        RemoverHeap(&memoria);
                        continue;
                    }
                }
                if(entrada[(memoria.cel->arq % 10)].v[entrada[(memoria.cel->arq % 10)].ind].nota < memoria.cel[0].alu.nota)
                {
                    memoria.cel[0].mar = true;
                }
                memoria.cel[0].alu = entrada[(memoria.cel->arq % 10)].v[entrada[(memoria.cel->arq % 10)].ind];
                entrada[(memoria.cel->arq % 10)].ind++;
                OrganizaHeap(&memoria, 0);
            }
            DestruirFitas(&arq, lado, false);
            lado = true;
        }
        if(saida.ind)
        {
            if(lado)
                fwrite(saida.v, sizeof(Aluno), saida.ind, arq.Fitas[arqatual]);
            else
                fwrite(saida.v, sizeof(Aluno), saida.ind, arq.Fitas[(arqatual + 10)]);
            saida.ind = 0;
        }
    }

    if(lado)
    {
        fclose(arq.Fitas[10]);
    }
    else
    {
        fclose(arq.Fitas[0]);
    }
    //INFORMAÇÕES 

}

void IniciaHeap(Mem_Int *mem, Pagina *ent)
{
    if (ent->qntlida >= 10)
        mem->qnt = 10;
    else
        mem->qnt = ent->qntlida;
    for(int i = 0; i < mem->qnt; i++, ent->ind++)
    {
        mem->cel[i].alu = ent->v[ent->ind];
        mem->cel[i].mar = false;
    }
    int ult_pai = (mem->qnt / 2) - 1;
    for(int i = ult_pai; i >= 0; i--)
        OrganizaHeap(mem, i);
}

void IniciaHeap2(Mem_Int *mem)
{
    for(int i = 0; i < mem->qnt; i++)
        mem->cel[i].mar = false;
    int ult_pai = (mem->qnt / 2) - 1;
    for(int i = ult_pai; i >= 0; i--)
        OrganizaHeap(mem, i);
}

void OrganizaHeap(Mem_Int *mem, int ind)
{
    int esq = 2 * ind + 1;
    int dir = 2 * ind + 2;
    if(dir >= mem->qnt)
    {
        if(esq >= mem->qnt)
            return;
        if(mem->cel[ind].alu.nota > mem->cel[esq].alu.nota)
        {
            if((mem->cel[ind].mar == false) && (mem->cel[esq].mar == true))
                return;
            TrocaHeap(mem, ind, esq);
        }
        else if((mem->cel[ind].mar == true) && (mem->cel[esq].mar == false))
            TrocaHeap(mem, ind, esq);
        return;
    }
    int menor;
    if(mem->cel[esq].alu.nota > mem->cel[dir].alu.nota)
    {
        if((mem->cel[esq].mar == false) && (mem->cel[dir].mar == true))
            menor = esq;
        else
            menor = dir;
    }
    else
    {
        if((mem->cel[dir].mar == false) && (mem->cel[esq].mar == true))
            menor = dir;
        else
            menor = esq;
    }
    if(mem->cel[ind].alu.nota > mem->cel[menor].alu.nota)
    {
        if((mem->cel[ind].mar == false) && (mem->cel[menor].mar == true))
            return;
        else
        {
            TrocaHeap(mem, ind, menor);
            OrganizaHeap(mem, menor);
        }
    }
    else if((mem->cel[ind].mar == true) && (mem->cel[menor].mar == false))
    {
        TrocaHeap(mem, ind, menor);
        OrganizaHeap(mem, menor);
    }
}

void TrocaHeap(Mem_Int *mem, int ind, int eoud) //esq ou dir
{
    Heap aux;
    aux = mem->cel[ind];
    mem->cel[ind] = mem->cel[eoud];
    mem->cel[eoud] = aux;
}

void RemoverHeap(Mem_Int *mem)
{
    mem->cel[0] = mem->cel[(mem->qnt - 1)];
    mem->qnt--;
    OrganizaHeap(mem, 0);
}
