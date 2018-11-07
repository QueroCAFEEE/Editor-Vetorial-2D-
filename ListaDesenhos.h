//Estrutura para representar de maneira simplificada um ponto, armazenando as coordenadas do mesmo.
typedef struct pt
{
    int x;
    int y;
} point;

//Estrutura para simplificar a definição de cores.
typedef struct color{
    double r;
    double g;
    double b;
    double t;//alpha
}Cor;

//Estrutura para representar os vértices do polígono.
typedef struct vert{
    //Info
    int x;
    int y;

    //Ponteiros
    struct vert *prox;  //Próximo vértice, caso exita.
    struct vert *ant;   //Vértice anterior, caso exista.
}Vertice;

//Estrutura para armazenar as informações dos poligonos na memória.
typedef struct des{
    //Info
    int ordemDesenho;   //Ordem que o polígono foi desenhado.
    Cor corPolig;       //Cor do polígono
    int qtdVertices;    //Quantidade de vértices que o polígono possui.
    //Ponteiros
    Vertice *vertices;  //Primeiro endereço da lista de vértices caso exista.
    struct des *prox;   //Próximo vértice, caso exista.
    struct des *ant;    //Vértice anterior, caso exista.
}Desenho;

//Lista de desenhos, para armazenar o primeiro endereço da lista, ou null caso não exista desenhos na lista.
 typedef struct listaDesenhos{
    Desenho* head;
 }ListaD;

/*
 *  <Função>:               inicia_desenho.
 *  <Parametros>:           Não recebe parametros.
 *  <Funcionalidade>:       Inicializa a estrutura de desenho, aloca o espaço na memoria.
 *  <Retorno>:              Ponteiro para o Desenho.
 */
Desenho* inicia_desenho();

/*
 *  <Função>:               cria_vertice
 *  <Parametros>:           Recebe como parametros os as coordenadas X e Y do vertice (Double).
 *  <Funcionalidade>:       Inicializa a estrutura de vertice, aloca o espaço na memoria, atribui os valores.
 *  <Retorno>:              Ponteiro para o Desenho.
 */
Vertice* cria_vertice(int x,int y);

/*
 *  <Função>:               imprime_cor
 *  <Parametros>:           Estrutura Cor .
 *  <Funcionalidade>:       Imprime os valores RGBA da estrutura.
 *  <Retorno>:              Não retorna nada.
 */
void imprime_cor(Cor c);

/*
 *  <Função>:               imprime_vertices
 *  <Parametros>:           Estrutura Vertice.
 *  <Funcionalidade>:       Imprime as coordenadas dos vértices na lista.
 *  <Retorno>:              Não retorna nada.
 */
void imprime_vertices(Vertice* ve);

/*
 *  <Função>:               imprime_info_desenhos
 *  <Parametros>:           Estrutura ListaD, que representa a lista de desenhos.
 *  <Funcionalidade>:       Imprime todas as informações da estrutura Desenho de todos os poligonos da lista.
 *  <Retorno>:              Não retorna nada.
 */
void imprime_info_desenhos(ListaD* lista);

/*
 *  <Função>:               adiciona_vertice_desenho
 *  <Parametros>:           Estrutura Desenho a qual o vértice pertence, Estutura vértice a ser adicionado ao desenho.
 *  <Funcionalidade>:       Adiciona o vértice a lista de vértices do desenho.
 *  <Retorno>:              Não retorna nada.
 */
void adiciona_vertice_desenho(Desenho* pol,Vertice* ve);

/*
 *  <Função>:               adiciona_novo_desenho
 *  <Parametros>:           Estrutura ListaD onde o desenho vai ser adicionado, Estrutura Desenho a ser adicionado à lista de polígonos.
 *  <Funcionalidade>:       Adiciona o desenho à lista de polígonos informada.
 *  <Retorno>:              Não retorna nada.
 */
void adiciona_novo_desenho(ListaD* lista, Desenho* novo);

/*
 *  <Função>:               clica_desenho
 *  <Parametros>:           Lista de poligonos, coordenadas x e y do clique do mouse.
 *  <Funcionalidade>:       Varre a lista de polígonos verificando se o ponto onde ocorreu o clique se encontra dentro do intervalo dos vértices do polígono.
 *  <Retorno>:              Caso exista um poligono, retorta o endereço desse poligono. Caso exista mais de um poligono, ele retorna o mais à cima (com maior odem).
 */
Desenho* clica_desenho(ListaD* lista, int x, int y);

/*
 *  <Função>:               move_desenho
 *  <Parametros>:           Desenho a ser movido, variação x e y.
 *  <Funcionalidade>:       Soma todas as coordenadas X com a varx e as coordenadas Y com vary do vértice, movendo assim o polígono.
 *  <Retorno>:              Não retorna nada.
 */
void move_desenho(Desenho* pol, int varx,int vary);

/*
 *  <Função>:               apaga_desenho
 *  <Parametros>:           Lista de desenhos, desenho a ser apagado
 *  <Funcionalidade>:       Varre a lista de desenhos procurando o desenho selecionado, caso ache o algoritmo aplica a lógica de remover da lista duplamente encadeada e libera a memória do desenho.
 *  <Retorno>:              Não retorna nada.
 */
void apaga_desenho(ListaD* lista,Desenho* pol);

/*
 *  <Função>:               escala_desenho
 *  <Parametros>:           Desenho a ser escalado, nova escala do poligono.
 *  <Funcionalidade>:       Escala os vértices do polígono em relação ao ponto médio do "envelope".
 *  <Retorno>:              Não retorna nada.
 */
void escala_desenho(Desenho* pol, double escala);

/*
 *  <Função>:               espelhamento
 *  <Parametros>:           Desenho a ser espelhado.
 *  <Funcionalidade>:       Espelha os vértices do polígono em relação a o eixo Y.
 *  <Retorno>:              Não retorna nada.
 */
void espelhamento(Desenho* pol);

/*
 *  <Função>:               salva_arquivo
 *  <Parametros>:           Nome do arquivo, lista de desenhos à ser salva.
 *  <Funcionalidade>:       Salva as informações contidas na estrutura em um arquivo para que possa ser carregada mais tarde.
 *  <Retorno>:              Não retorna nada.
 */
int salva_arquivo(char* nome,ListaD* desenhos);

/*
 *  <Função>:               carrega_arquivo
 *  <Parametros>:           Nome do arquivo.
 *  <Funcionalidade>:       Carrega as informações salvas no arquivo para a estrutura do programa.
 *  <Retorno>:              Retorna a lista de desenhos gerada pelo algoritmo.
 */
ListaD* carrega_arquivo(char* nome);

/*
 *  <Função>:               distancia_entre_2pt
 *  <Parametros>:           Coordenadas x e y de dois pontos.
 *  <Funcionalidade>:       Função genérica para calcular a distãncia entre dois pontos.
 *  <Retorno>:              Retorna a distãncia entre os dois pontos.
 */
double distancia_entre_2pt(int x1,int x2,int y1,int y2);

/*
 *  <Função>:               ponto_medio
 *  <Parametros>:           Desenho.
 *  <Funcionalidade>:       Utiliza a idéia do envelope e calcula o centro do retângulo, achando assim aproximadamente o ponto médio do polígono.
 *  <Retorno>:              Estrutura point com as coordenadas do ponto médio.
 */
point ponto_medio(Desenho* poligono);

void rotacao_desenho(Desenho* pol, float angulo);
/*função para rotacionar o polígono em torno dele mesmo, utilizando o ponto médio do envelope
é utilizada a forma matemática para rotacionar todos os pontos do poligono, por isso a função
recebe como parâmetros o desenho e o angulo pelo qual deseja-se rotacionar o desenho. Foi seguida esta estrutura:
xu := (xo-xr) * cos (ang) - (yo-yr) * sin (ang) + xr
yu := (yo-yr) * cos (ang) + (xo-xr) * sin (ang) + yr
em que xu e yu eram novos vértices já rotacionados, xo e yo os vértices do polígono desenhado
e xr e yr os vértices do ponto médio*/
