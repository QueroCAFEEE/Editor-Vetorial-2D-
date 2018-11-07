#include "ListaDesenhos.h"
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <ctype.h>
#include <math.h>

using namespace std;

int ordem = 0;//sequencial de ordem dos desenhos.

//Aloca o espaço necessário na memória para a estrutura, e inicializa os ponteiros para não dar chance de erros. 
Desenho* inicia_desenho()
{
    Desenho *novo = (Desenho*)malloc(sizeof(Desenho));//aloca o espaço na memória
    if(!novo)   //caso novo não exista, então ocorreu algum erro.
    {
        printf("ERRO AO ALOCAR MEMORIA PARA O DESENHO");
    }
    //Inicializações
    novo->prox = NULL;
    novo->ant = NULL;
    novo->ordemDesenho = ordem;
    ordem++;
    novo->vertices = NULL;
    novo->qtdVertices = 0;
    return novo;
}

//Aloca espaço para a estrutura e inicializa os ponteiros.
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

//Função para debugar a cor.
void imprime_cor(Cor c)
{
    printf("R: %f     G: %f     B: %f    T: %f",c.r,c.g,c.b,c.t);
}

//Função recursiva para debugar os vértices do polígono.
void imprime_vertices(Vertice* ve)
{
    printf("X: %d   Y: %d\n",ve->x,ve->y);
    if(ve->prox)
        imprime_vertices(ve->prox);//chama a função novamente com o próximo vértice da lista.
}

//Função para debugar as informações do poligono contida em toda a estrutura, basta chamar esta função e ela imprimirá em conjunto os vértices e a cor.
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

//Função genérica de lista duplamente encadeada para adicionar um item à lista.
void adiciona_vertice_desenho(Desenho* pol,Vertice* ve)
{
    if(!pol->vertices) //Logo não contem nenhum vertice, então adiciona o endereço do primeiro vértice na estrutura Desenho.
    {
        pol->vertices = ve;
        ve->ant = NULL;
        pol->qtdVertices++; //Cada vértice adicionado, incrementa a qtd de vértices.
    }
    else    //já existe um vértice no desenho, então adiciona o vértice novo no final da lista.
    {
        Vertice* tmp = pol->vertices;
        while(tmp->prox != NULL)
        {
            tmp = tmp->prox;
        }
        tmp->prox = ve;
        ve->ant = tmp;
        pol->qtdVertices++; //Cada vértice adicionado, incrementa a qtd de vértices.

    }
}

//Função genérica de lista duplamente encadeada para adicionar um item à lista.
void adiciona_novo_desenho(ListaD* lista, Desenho* novo)
{
    if(!lista->head)    //Logo não contem nenhum desenho, então adiciona o endereço do primeiro desenho na estrutura ListaD.
    {
        lista->head = novo;
        lista->head->ordemDesenho++;
    }
    else    //já existe um desenho na lista, então adiciona o desenho novo no final da lista.
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

//Função que define o "envelope" do poligono"
Desenho* clica_desenho(ListaD* lista, int x, int y)
{
    if(lista->head)
    {

        Desenho* tmp_desenho = lista->head;
        Desenho* tmp_desenho_ret = NULL;
        while(tmp_desenho) //Varre toda a lista de polígonos
        {
            int minX,minY,maxX,maxY;
            int flg_first = 1;//Primeiro vértice
            if(tmp_desenho->vertices) //Varre toda a lista de vértices do polígono.
            {
                Vertice* tmp_vertice = tmp_desenho->vertices;
                while(tmp_vertice)
                {
                    if(flg_first)//Se for o primeiro poligono a ser verificado, seus valores serão todos os extremos do envelope.
                    {
                        minX = tmp_vertice->x;
                        minY = tmp_vertice->y;
                        maxX = tmp_vertice->x;
                        maxY = tmp_vertice->y;
                        flg_first = 0;
                    }
                    else
                    {
                        if(tmp_vertice->x < minX)       //Verifica se o vértice X atual é menor que o menor vértice X até o momento. 
                        {
                            minX = tmp_vertice->x;      //Se sim, então este é o novo mínimo X do polígono.
                        }
                        else if(tmp_vertice->x > maxX)  //Verifica se o vértice X atual é maior que o maior vértice X até o momento. 
                        {
                            maxX = tmp_vertice->x;      //Se sim, então este é o novo máximo X do polígono.
                        }
                        if(tmp_vertice->y < minY)       //Verifica se o vértice Y atual é menor que o menor vértice Y até o momento.
                        {
                            minY = tmp_vertice->y;      //Se sim, então este é o novo mínimo Y do polígono.
                        }
                        else if(tmp_vertice->y > maxY)  //Verifica se o vértice Y atual é maior que o maior vértice Y até o momento.
                        {
                            maxY = tmp_vertice->y;      //Se sim, então este é o novo máximo Y do polígono.
                        }
                    }
                    tmp_vertice = tmp_vertice->prox;    //Vai para o próximo vértice.
                }
            }
            if(x <= maxX && x >= minX && y <= maxY && y >= minY)    //verifica se as coordenadas do mouse se encontram dentro do intervalo de vértices da função
            {
                tmp_desenho_ret = tmp_desenho; //salva o endereço em uma variável temporária de retorno, assim o caso exita mais de um desenho na mesma posição, ele retorna o mais novo(ou mais a cima).
            }
            tmp_desenho = tmp_desenho->prox;
        }
        return tmp_desenho_ret;
    }
    else
    {
        //printf("LISTA DE DESENHOS VAZIO!\n");
    }

}

//Função para mover os vértices de um poligono dada as variações nos eixos X e Y.
void move_desenho(Desenho* pol, int varx,int vary)
{
    if(pol->vertices)
    {
        Vertice* tmp_vert = pol->vertices;

        //Varre os vértices do polígono, somando as coordenadas X com a variação X, e as coordenadas Y com a variação Y.
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
    else //remove um desenho dentro da lista
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

void escala_desenho(Desenho* pol, double escala) //realiza o escalonamento do polígono
{
    point pm = ponto_medio(pol); //pm: ponto médio do envelope do polígono

    if(pol->vertices)
    {
        Vertice* tmp_vert = pol->vertices;
        Vertice* new_vert = pol->vertices; //novos valores de vértices após o escalonamento

        while(tmp_vert != NULL)
        {
            /*calculo padrão para a realização do escalonamento: 
              ( (ponto atual - ponto médio) * fator de escala ) + ponto médio
            */
            new_vert->x = ((tmp_vert->x - pm.x)*escala) + pm.x;
            new_vert->y = ((tmp_vert->y - pm.y)*escala) + pm.y;
            
            new_vert = tmp_vert->prox;
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
    arq = fopen(nome,"w+"); //abre o arrquivo, ou cria caso não exita
    if(arq == NULL)
    {
        printf("Erro ao abrir arquivo.\n");
        return -1;
    }
    Desenho* tmp = desenhos->head;
    while(tmp != NULL)  //varre os desenhos armazenando as informações no arquivo
    {
        fprintf(arq,"D\n"); //Primeira linha - Indica que é um novo desenho
        fprintf(arq,"%f,%f,%f,%f\n",tmp->corPolig.r,tmp->corPolig.g,tmp->corPolig.b,tmp->corPolig.t); //Segunda linha - Indica a cor do polígono no formato R,G,B,A
        if(tmp->vertices != NULL) //Terceira linha indica os vértices no formato x1,y1 x2,y2 ... xn,yn na mesma linha.
        {
            Vertice* tmp_v = tmp->vertices;
            while(tmp_v != NULL)
            {
                fprintf(arq,"%d,%d ",tmp_v->x,tmp_v->y);
                tmp_v = tmp_v->prox;
            }
        }
        tmp=tmp->prox;
        fprintf(arq,"\n");//quebra de linha, representa o fim dos vértices
    }

    fclose(arq);//fecha o arquivo
}

ListaD* carrega_arquivo(char* nome)
{
    FILE* arq;
    arq = fopen(nome,"r+"); //abre o arquivo.
    ListaD* lista = (ListaD*)malloc(sizeof(ListaD)); //aloca espaço para a lista.
    lista->head = NULL; //inicializa.

    char ch;
    if(arq == NULL)
    {
        printf("Problema ao abrir arquivo.\n");
    }
    /*
        Começa a ler as informações dos poligonos no arquivo, que se encontram no seguinte formato:
        D
        R,G,B,A
        x1,y1 x2,y2 x3,y3 ... xn,yn
        D
        R,G,B,A
        x1,y1 x2,y2 x3,y3 ... xn,yn
    */
    while((ch=fgetc(arq)) != EOF)
    {
        printf("%c",ch);
        if(ch == 'D')//encontra um novo polígono
        {
            Desenho* novo = inicia_desenho();
            fgetc(arq);
            int contador = 0;

            char arrayCor[4][8]= {NULL};
            int index = 0;

            //Ler os valores das cores como "string"
            while((ch=fgetc(arq)) != '\n')
            {
                if(ch != ',')
                {
                    arrayCor[index][contador]=ch;
                    contador++;
                }
                else if(ch == ',')//proximo valor
                {
                    index++;
                    contador = 0;
                }
            }
            //Converte o valor das cores para float.
            novo->corPolig.r = atof(arrayCor[0]);
            novo->corPolig.g = atof(arrayCor[1]);
            novo->corPolig.b = atof(arrayCor[2]);
            novo->corPolig.t = atof(arrayCor[3]);

            char arrayVerticeX[100]= {NULL};
            char arrayVerticeY[100]= {NULL};
            index = 0;
            Vertice* n_vert = NULL;
            int flg_y=0;

            //Lê os vértices x,y...
            while((ch=fgetc(arq)) != '\n')//Enquanto estiver na mesma linha
            {
                if((ch != ',' && !isspace(ch))&& flg_y == 0)//lê todos os numeros para a coordenada X
                {
                    arrayVerticeX[index]=ch;
                    index++;
                }
                else if((ch != ',' && !isspace(ch))&& flg_y ==1)//Lê todos os numeros para a coordenada Y
                {
                    arrayVerticeY[index]=ch;
                    index++;
                }
                else if(ch == ',')//Terminou de ler X
                {
                    flg_y = 1;
                    index = 0;
                }
                else if(isspace(ch))//Terminou de ler X e Y
                {
                    flg_y = 0;
                    index = 0;
                    n_vert = cria_vertice(atoi(arrayVerticeX),atoi(arrayVerticeY)); //Converte de "string" para int
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

//calcula a distância entre os dois pontos e retorna.
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

void rotacao_desenho(Desenho* pol, float angulo){ //realiza a rotação do polígono ao redor do ponto médio por um ângulo
   //s e c recebem os valores de seno e cosseno do ângulo escolhido para realizar a rotação
   float s = sin(angulo); 
   float c = cos(angulo);
   point pm = ponto_medio(pol); //pm recebe o ponto médio do polígono selecionado

   int difX;
   int difY;

    if(pol->vertices)
    {
        Vertice* tmp_vert = pol->vertices;

        while(tmp_vert != NULL)
        {   
            /*Já que a rotação será feita ao redor do ponto médio e não ao redor da origem, é necessário o cálculo da diferença entre 
            os valores atuais de (x,y) e os valores (x,y) do ponto médio: */
            difX = (tmp_vert->x - pm.x);
            difY = (tmp_vert ->y - pm.y);
            
            /*Os novos valores de (x,y) agora podem ser calculados seguindo a fórmula padrão de rotação 2D:
                x’ = x . cos (q) – y . sen (q)
                y’ = x . sen (q) + y . cos (q)
            */
            float newX = ((difX * c) - (difY * s)) ;
            float newY = ((difX * s) + (difY * c));
            
            //Por fim, as diferenças calculadas anteriormente são acrescentadas novamente aos pontos:
            tmp_vert->x = newX + pm.x;
            tmp_vert->y = newY + pm.y;
            
            tmp_vert = tmp_vert->prox; //segue para o próximo vértice
        }

    }
}
