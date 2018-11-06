typedef struct pt
{
    int x;
    int y;
} point;

typedef struct color{
    double r;
    double g;
    double b;
    double t;
}Cor;

typedef struct vert{
    //Info
    int x;
    int y;

    //Ponteiros
    struct vert *prox;
    struct vert *ant;
}Vertice;

typedef struct des{
    //Info
    int ordemDesenho;
    Cor corPolig;
    int qtdVertices;
    //Ponteiros
    Vertice *vertices;
    struct des *prox;
    struct des *ant;
}Desenho;

 typedef struct listaDesenhos{
    Desenho* head;
 }ListaD;

/*
 *  <Função>:             inicia_desenho.
 *  <Parametros>:         Não recebe parametros.
 *  <Funcionalidade>:     Inicializa a estrutura de desenho, aloca o espaço na memoria.
 *  <Retorno>:            Ponteiro para o Desenho.
 */
Desenho* inicia_desenho();

/*
 *  <Função>:             cria_vertice
 *  <Parametros>:         Recebe como parametros os as coordenadas X e Y do vertice (Double)
 *  <Funcionalidade>:     Inicializa a estrutura de vertice, aloca o espaço na memoria, atribui os valores.
 *  <Retorno>:            Ponteiro para o Desenho.
 */
Vertice* cria_vertice(int x,int y);

void imprime_cor(Cor c);

void imprime_vertices(Vertice* ve);

void imprime_info_desenhos(ListaD* lista);

void adiciona_vertice_desenho(Desenho* pol,Vertice* ve);

void adiciona_novo_desenho(ListaD* lista, Desenho* novo);

Desenho* clica_desenho(ListaD* lista, int x, int y);

void move_desenho(Desenho* pol, int varx,int vary);

void apaga_desenho(ListaD* lista,Desenho* pol);

void escala_desenho(Desenho* pol, double escala);

void espelhamento(Desenho* pol);

int salva_arquivo(char* nome,ListaD* desenhos);

ListaD* carrega_arquivo(char* nome);

double distancia_entre_2pt(int x1,int x2,int y1,int y2);

point ponto_medio(Desenho* poligono);

void rotacao_desenho(Desenho* pol, float angulo);
/*função para rotacionar o polígono em torno dele mesmo, utilizando o ponto médio do envelope
é utilizada a forma matemática para rotacionar todos os pontos do poligono, por isso a função 
recebe como parâmetros o desenho e o angulo pelo qual deseja-se rotacionar o desenho. Foi seguida esta estrutura:
xu := (xo-xr) * cos (ang) - (yo-yr) * sin (ang) + xr
yu := (yo-yr) * cos (ang) + (xo-xr) * sin (ang) + yr
em que xu e yu eram novos vértices já rotacionados, xo e yo os vértices do polígono desenhado 
e xr e yr os vértices do ponto médio*/
