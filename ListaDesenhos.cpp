#include "ListaDesenhos.h"
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <ctype.h>
#include <math.h>

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
        ve->ant = NULL;
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
    if(pol->ant == NULL) //remove o primeiro desenho
    {
        if(pol->prox != NULL)
        {
            lista->head = pol->prox;
            pol->prox->ant = NULL;
        }
        else
        {
            lista->head = NULL;
        }
    }
    else
    {
        if(pol->prox != NULL)
        {
            pol->prox->ant = pol->ant;
            pol->ant->prox = pol->prox;
        }
        else
        {
            pol->ant->prox = NULL;
        }
    }
    free(pol);
}

void escala_desenho(Desenho* pol, double escala)
{
    if(pol->vertices)
    {
        Vertice* tmp_vert = pol->vertices;

        while(tmp_vert != NULL)
        {
            tmp_vert->x *= escala;
            tmp_vert->y *= escala;
            tmp_vert = tmp_vert->prox;
        }
    }
}

void espelhamento(Desenho* pol)
{
    int maxX;
    int flg_first = 1;

    Vertice* tmp_vertice = pol->vertices;

    while(tmp_vertice)
    {
        if(flg_first)
        {
            maxX = tmp_vertice->x;
            flg_first = 0;
        }
        else
        {
            if(tmp_vertice->x > maxX)
            {
                maxX = tmp_vertice->x;
            }
        }
        tmp_vertice = tmp_vertice->prox;
    }

    if(pol->vertices)
    {
        int i = 0;
        Vertice* new_vert = pol->vertices;
        Vertice* tmp_vert = pol->vertices;

        while(new_vert != NULL)
        {
            new_vert->x = maxX+(maxX - (tmp_vert->x));
            new_vert->y = tmp_vert->y;
            //printf("x[%d]': %d\n", i, new_vert->x);
            //printf("y[%d]': %d\n", i, new_vert->y);
            //printf("\n");
            new_vert = tmp_vert->prox;
            tmp_vert = tmp_vert->prox;
            i++;
        }
        //printf("---------------------\n");
    }
}

int salva_arquivo(char* nome,ListaD* desenhos)
{
    FILE* arq;
    arq = fopen(nome,"w+");
    if(arq == NULL)
    {
        printf("Erro ao abrir arquivo.\n");
        return -1;
    }
    Desenho* tmp = desenhos->head;
    while(tmp != NULL)
    {
        fprintf(arq,"D\n");
        fprintf(arq,"%f,%f,%f,%f\n",tmp->corPolig.r,tmp->corPolig.g,tmp->corPolig.b,tmp->corPolig.t);
        if(tmp->vertices != NULL)
        {
            Vertice* tmp_v = tmp->vertices;
            while(tmp_v != NULL)
            {
                fprintf(arq,"%d,%d ",tmp_v->x,tmp_v->y);
                tmp_v = tmp_v->prox;
            }
        }
        tmp=tmp->prox;
        fprintf(arq,"\n");
    }

    fclose(arq);
}

ListaD* carrega_arquivo(char* nome)
{
    FILE* arq;
    arq = fopen(nome,"r+");
    ListaD* lista = (ListaD*)malloc(sizeof(ListaD));
    lista->head = NULL;

    char ch;
    if(arq == NULL)
    {
        printf("Problema ao abrir arquivo.\n");
    }
    while((ch=fgetc(arq)) != EOF)
    {
        printf("%c",ch);
        if(ch == 'D')
        {
            Desenho* novo = inicia_desenho();
            fgetc(arq);
            int contador = 0;

            char arrayCor[4][8]= {NULL};
            int index = 0;

            //Ler cores
            while((ch=fgetc(arq)) != '\n')
            {
                if(ch != ',')
                {
                    arrayCor[index][contador]=ch;
                    contador++;
                }
                else if(ch == ',')
                {
                    index++;
                    contador = 0;
                }
            }
            novo->corPolig.r = atof(arrayCor[0]);
            novo->corPolig.g = atof(arrayCor[1]);
            novo->corPolig.b = atof(arrayCor[2]);
            novo->corPolig.t = atof(arrayCor[3]);

            char arrayVerticeX[100]= {NULL};
            char arrayVerticeY[100]= {NULL};
            index = 0;
            Vertice* n_vert = NULL;
            int flg_y=0;

            while((ch=fgetc(arq)) != '\n')
            {
                if((ch != ',' && !isspace(ch))&& flg_y == 0)
                {
                    arrayVerticeX[index]=ch;
                    index++;
                }
                else if((ch != ',' && !isspace(ch))&& flg_y ==1)
                {
                    arrayVerticeY[index]=ch;
                    index++;
                }
                else if(ch == ',')
                {
                    flg_y = 1;
                    index = 0;
                }
                else if(isspace(ch))
                {
                    flg_y = 0;
                    index = 0;
                    n_vert = cria_vertice(atoi(arrayVerticeX),atoi(arrayVerticeY));
                    adiciona_vertice_desenho(novo,n_vert);
                    n_vert = NULL;

                    int k;
                    for(k=0; k<100; k++)
                    {
                        arrayVerticeX[k]=' ';
                        arrayVerticeY[k]=' ';
                    }

                }
            }
            adiciona_novo_desenho(lista,novo);
        }
    }
    return lista;
}

double distancia_entre_2pt(int x1,int x2,int y1,int y2)
{
    return sqrt(pow(x2-x1,2)+pow(y2-y1,2));
}

point baricentro(Desenho* poligono)
{
    int minX,minY,maxX,maxY;
    point retorno;
    int flg_first = 1;
    if(poligono->vertices)
    {
        Vertice* tmp_vertice = poligono->vertices;
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
        retorno.x = minX + ((maxX - minX)/2);
        retorno.y = minY + ((maxY - minY)/2);
        return retorno;
    }else{
        printf("Erro: Poligono não possuí vertices.\n");
    }
    /*
    Exemplo para utilizar a função:
        point bar = baricentro(atual);
        printf("Baricentro do Envelope:\nX:%d\nY:%d\n",bar.x,bar.y);
    */
}
