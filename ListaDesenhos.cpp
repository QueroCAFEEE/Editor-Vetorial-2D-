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

void espelhamento(Desenho* pol) // realiza o espelhamento do poligono selecionado
{
    int maxX; //variável para armazenar o ponto X mais a direita
    int flg_first = 1; //variável para identificar a primeira iteração

    Vertice* tmp_vertice = pol->vertices; //tmp_vertice recebe os vértices do polígono recebido pela função

    while(tmp_vertice) //enquanto existirem vértices a serem visitados:
    {
        if(flg_first) //1ª iteração:
        {
            maxX = tmp_vertice->x; //maxX recebe o valor de x do primeiro vértice percorrido
            flg_first = 0; //fim da primeira iteração
        }
        else
        {
            if(tmp_vertice->x > maxX) //caso o valor de x do vértice atual seja maior do que o valor armazenado anteriormente em maxX:
            {                           
                maxX = tmp_vertice->x; //maxX recebe o valor x do vértice atual
            }
        }
        tmp_vertice = tmp_vertice->prox; //passa-se para o próximo vértice do polígono
    }
    /*Fim do while: a variável maxX agora possui o valor do 'x' mais a direita do polígono (o maior valor que x assume 
    entre todos os vértices que compõem o polígono)*/

    if(pol->vertices)
    {
        Vertice* new_vert = pol->vertices; //new_vert diz respeito aos novos vértices que iram compor o polígono espelhado
        Vertice* tmp_vert = pol->vertices; 

        while(new_vert != NULL) //enquanto o polígono ainda possuir vértices para serem atualizados
        {
            /*
            O espelhamento em relação ao eixo x: dado cada valor de x de cada vértice do polígono, o novo valor de x é dado por
            (Valor do maxX) + (Valor do maxX - Valor do x do vértice atual). Como o espelhamento está sendo feito em relação ao eixo x, 
            o valor de y continua o mesmo.
            */
            new_vert->x = maxX + ( maxX - ( tmp_vert->x ) ); //valor de x atualizado de acordo com a fórmula
            new_vert = tmp_vert->prox; //segue para atualização do próximo vértice do polígono
            tmp_vert = tmp_vert->prox; //segue para o próximo vértice do polígono
        }
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

point ponto_medio(Desenho* poligono) //encontra o ponto médio do envelope do polígono selecionado
{
    int minX, minY, maxX, maxY; //variáveis para armazenar os pontos máximos e mínimos do polígono
    point retorno; //ponto médio a ser retornado
    int flg_first = 1; //variável para identificar a primeira iteração
    
    if(poligono->vertices)
    {
        Vertice* tmp_vertice = poligono->vertices; 
        
        while(tmp_vertice) //enquanto houverem vértices para serem visitados:
        {
            if(flg_first) //1ª iteração: valores de máximos de mínimos recebem os valores (x,y) do primeiro vértice percorrido
            {   
                minX = tmp_vertice->x;
                minY = tmp_vertice->y;
                maxX = tmp_vertice->x;
                maxY = tmp_vertice->y;
                flg_first = 0; //fim da primeira iteração
            }
            else
            {   // atualização dos valores máximos e mínimos
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
            tmp_vertice = tmp_vertice->prox; //segue para o próximo vértice do polígono 
        }
        /*Fim do while: as variáveis agora possuem os valores máximos e mínimos do polígono*/
        
        //Calculo do ponto médio: dados os pontos máximos e mínimos do envelope, temos que o valor do seu ponto médio é:
        retorno.x = minX + ((maxX - minX)/2); //calculo do ponto médio x do envelope
        retorno.y = minY + ((maxY - minY)/2); //calculo do ponto médio y do envelope
        return retorno; //retorna o ponto médio (x,y) encontrado

    }else{
        printf("Erro: Poligono não possuí vertices.\n");
    }
    /*
    Exemplo para utilizar a função:
        point bar = ponto_medio(atual);
        printf("Ponto médio do Envelope:\nX:%d\nY:%d\n",bar.x,bar.y);
    */
}
void rotacao_desenho(Desenho* pol, float angulo){
   float s = sin(angulo);
   float c = cos(angulo);
   point bar = ponto_medio(pol);;

   int difX;
   int difY;

    if(pol->vertices)
    {
        Vertice* tmp_vert = pol->vertices;

        while(tmp_vert != NULL)
        {
            difX = (tmp_vert->x - bar.x);
            difY = (tmp_vert ->y - bar.y);

            float newX = ((difX * c) - (difY * s)) ;
            float newY = ((difX * s) + (difY * c));

            tmp_vert->x = newX + bar.x;
            tmp_vert->y = newY + bar.y;
            tmp_vert = tmp_vert->prox;
        }

    }
}
