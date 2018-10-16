#include "ListaDesenhos.h"
#include <stdio.h>
#include <stdlib.h>

using namespace std;

int ordem = 0;

Desenho* inicia_desenho()
{
    Desenho *novo = (Desenho*)malloc(sizeof(Desenho));
    if(!novo)
    {
        printf("ERRO AO ALOCAR MEMORIA PARA O DESENHO");
    }
    novo->prox = NULL;
    novo->ant = NULL;
    novo->ordemDesenho = ordem;
    ordem++;
    novo->vertices = NULL;
    novo->qtdVertices = 0;
    ordem++;
    return novo;
}

Vertice* cria_vertice(int x,int y)
{
    Vertice *novo = (Vertice*)malloc(sizeof(Vertice));
    if(!novo)
    {
        printf("ERRO AO ALOCAR MEMORIA PARA O VERTICE");
    }
    novo->x = x;
    novo->y = y;
    novo->prox = NULL;
    return novo;
}

void imprime_cor(Cor c)
{
    printf("R: %f     G: %f     B: %f    T: %f",c.r,c.g,c.b,c.t);
}

void imprime_vertices(Vertice* ve)
{
    printf("X: %d   Y: %d\n",ve->x,ve->y);
    if(ve->prox)
        imprime_vertices(ve->prox);
}

void imprime_info_desenhos(ListaD* lista)
{
    if(lista->head == NULL)
    {
        printf("Lista vazia.");
    }
    else
    {
        Desenho* d = lista->head;
        while(d != NULL)
        {
            printf("########################################\n");
            printf("INFORMACOES DO DESENHO:\nOrdem:%d\n",d->ordemDesenho);
            printf("QTD_VERTICES: %d\nCOR:",d->qtdVertices);
            imprime_cor(d->corPolig);
            printf("\nVertices:\n");
            imprime_vertices(d->vertices);
            printf("########################################\n");
            d = d->prox;
        }
    }


}

void adiciona_vertice_desenho(Desenho* pol,Vertice* ve)
{
    if(!pol->vertices) //Logo não contem nenhum vertice.
    {
        pol->vertices = ve;
        pol->qtdVertices++;
    }
    else
    {
        Vertice* tmp = pol->vertices;
        while(tmp->prox != NULL)
        {
            tmp = tmp->prox;
        }
        tmp->prox = ve;
        ve->ant = tmp;
        pol->qtdVertices++;

    }
}

void adiciona_novo_desenho(ListaD* lista, Desenho* novo)
{
    if(!lista->head)
    {
        lista->head = novo;
        lista->head->ordemDesenho++;
    }
    else
    {
        Desenho* tmp = lista->head;
        while(tmp->prox != NULL)
        {
            tmp = tmp->prox;
        }
        tmp->prox = novo;
        novo->ant = tmp;
    }
}

Desenho* clica_desenho(ListaD* lista, int x, int y)
{
    if(lista->head)
    {

        Desenho* tmp_desenho = lista->head;
        Desenho* tmp_desenho_ret = NULL;
        while(tmp_desenho)
        {
            int minX,minY,maxX,maxY;
            int flg_first = 1;
            if(tmp_desenho->vertices)
            {
                Vertice* tmp_vertice = tmp_desenho->vertices;
                while(tmp_vertice)
                {
                    if(flg_first)
                    {
                        minX = tmp_vertice->x;
                        minY = tmp_vertice->y;
                        maxX = tmp_vertice->x;
                        maxY = tmp_vertice->y;
                        flg_first = 0;
                    }
                    else
                    {
                        if(tmp_vertice->x < minX)
                        {
                            minX = tmp_vertice->x;
                        }
                        else if(tmp_vertice->x > maxX)
                        {
                            maxX = tmp_vertice->x;
                        }
                        if(tmp_vertice->y < minY)
                        {
                            minY = tmp_vertice->y;
                        }
                        else if(tmp_vertice->y > maxY)
                        {
                            maxY = tmp_vertice->y;
                        }
                    }
                    tmp_vertice = tmp_vertice->prox;
                }
            }
            if(x <= maxX && x >= minX && y <= maxY && y >= minY)
            {
                tmp_desenho_ret = tmp_desenho;
            }
            tmp_desenho = tmp_desenho->prox;
        }
        return tmp_desenho_ret;
    }
    else
    {
        printf("LISTA DE DESENHOS VAZIO!\n");
    }

}

void move_desenho(Desenho* pol, int varx,int vary)
{
    if(pol->vertices)
    {
        Vertice* tmp_vert = pol->vertices;

        while(tmp_vert != NULL)
        {
            tmp_vert->x = tmp_vert->x + (varx);
            tmp_vert->y = tmp_vert->y + (vary);
            tmp_vert = tmp_vert->prox;
        }
    }
}

void apaga_desenho(ListaD* lista,Desenho* pol)
{
    if(pol->ant == NULL){//remove o primeiro desenho
        if(pol->prox != NULL){
                printf("TESTE\n");
            lista->head = pol->prox;
            pol->prox->ant = NULL;
        }else{
            lista->head = NULL;
        }
    }else{
        if(pol->prox != NULL){
            pol->prox->ant = pol->ant;
            pol->ant->prox = pol->prox;
        }else{
            pol->ant->prox = NULL;
        }
    }
    free(pol);
}
