#include <windows.h>
#include <stdio.h>
#include "ListaDesenhos.h"
#include <math.h>

using namespace std;

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>

ListaD* listaPolig = (ListaD*)malloc(sizeof(ListaD)); 	//Cria e aloca memória a variável que armazenará o endereço para o primeiro polinogono na estrutura de Desenhos
Desenho* atual; 										//Variável para definir o poligono que está selecionado ou sendo desenhado.

/*
	<op>:
	Cria e inicializa a variável que controla os estados do editor, sendo esses estados:
		D : desenhando.
		X : Modo livre.
*/
char op = 'D';
int primeiro_vertice = 1;	//Flag para indicar quando o primeiro vértice é o proximo a ser desenhado.
int movendo = 0;			//Flag para indicar que está na opção de mover o polígono.
point rastro[2]; 			//Armazena os pontos para desenhar o rastro do mouse.
point drag[2];				//Armazena a variação do mouse quando move o desenho.
int gHeight;				//Utilizado para converter as coordenadas quando desenhar para o desenho não sair invertido.
float gColor[3]= {0,1,0};	//Variável temporária para definição de cores. Inicializada como verde.

/*
	<contorno_selecionado>:
	Desenha um contorno branco no poligono selecionado.
*/
void contorno_selecionado()
{
    if(atual->qtdVertices > 1)								//Verifica se o poligono atual tem mais de um vértice.
    {
        Vertice* tmp_vert = atual->vertices;				//Armazena em uma variável temporária o primeiro endereço dos vértices para poder percorrer todos.
        glColor3f(1.0,1.0,1.0);								//Cor: Branco.
        glBegin(GL_LINE_LOOP);								//Função de desenho do GLUT. GL_LINE_LOOP: Desenhará uma linha, e no último vértice ele desenhará automaticamente uma linha para o primeiro vértice.
        while(tmp_vert != NULL)								//Percorrer os vértices enquanto houver.
        {
            glVertex2i(tmp_vert->x,gHeight-tmp_vert->y);	//Adiciona na função de desenho do GLUT o vértice.
            tmp_vert = tmp_vert->prox;						//Vai para o próximo vertice.
        }
        glEnd();											//Sinaliza o fim dos vértices do desenho GLUT.
    }
}

/*
	<drawlines>:
	Varre a lista de desenhos, enviando para o GLUT seus respectivos vértices.
	Desenha o rastro.
	Indica a cor inicial do poligono a ser desenhado;
*/
void drawlines()
{

    glColor3fv(gColor); //Indica que a cor, temporária é a cor a ser utilizada para o próximo desenho a ser criado.
	
	//Adiciona a cor atual a estrutura do poligono.
    if(atual)
    {
        atual->corPolig.r = gColor[0];
        atual->corPolig.g = gColor[1];
        atual->corPolig.b = gColor[2];
        atual->corPolig.t = 1;
    }

	//Caso o usuário esteja desenhando, e o próximo vértice não é o primeiro, então ele desenhará o rastro
    if(op=='D' && primeiro_vertice == 0)				
    {
        glBegin(GL_LINES);
        glVertex2i(rastro[0].x, gHeight-rastro[0].y);
        glVertex2i(rastro[1].x, gHeight-rastro[1].y);
        glEnd();
    }

	//Se existe um desenho na lista ele irá percorrer todos os desenhos e vértices desenhando na tela.
    if(listaPolig->head)
    {
        Desenho* tmp_des = listaPolig->head;
        while(tmp_des != NULL)
        {
            Vertice* tmp_vert = tmp_des->vertices;

            if(tmp_vert)
            {
                glColor3d(tmp_des->corPolig.r,tmp_des->corPolig.g,tmp_des->corPolig.b); 	//Define que a cor do poligono é a da estrutura.
                glBegin(GL_LINES);  //Inicia o desenho
                while(tmp_vert!=NULL)
                {

                    glVertex2i(tmp_vert->x, gHeight-tmp_vert->y); //Adiciona vértice origem no GLUT
					
					//Caso exista um próximo vértice, ele adiciona ao GLUT o vértice destino, desenhando assim uma linha.
					//Caso não exista, o vértice destino será o primeiro vértice do desenho, fechando assim o polígono.
                    if(tmp_vert->prox != NULL)
                    {
                        glVertex2i(tmp_vert->prox->x, gHeight-tmp_vert->prox->y);
                    }
                    else if(tmp_des != atual)
                    {
                        glVertex2i(tmp_des->vertices->x, gHeight-tmp_des->vertices->y);
                    }

                    tmp_vert = tmp_vert->prox;
                }
                glEnd();
            }
            tmp_des = tmp_des->prox;
        }

    }

}

/*
	<display>:
	O GLUT chamará essa função diversas vezes para redesenhar os polígonos na tela.
*/
void display()
{
	/*
		<glClear>:
		Serve para limpar buffers utilizados pelo OpenGL com valores pré-definidos. A máscara utilizada neste exemplo, (GL_COLOR_BUFFER_BIT, diz à função glClear() que apenas o buffer de desenho deve ser limpo. Após a execução desta função, a tela ficará preta, uma vez que a init() define (R, G, B)=(0.0, 0.0, 0.0) como cor de limpeza de tela. 
	*/
    glClear(GL_COLOR_BUFFER_BIT);
    drawlines();
	
	//Se existir um desenho selecionado, e a opção estiver no modo livre chama a função
    if(atual != NULL && op == 'X')
    {
        contorno_selecionado();
    }
    glFlush();	//Faz com que qualquer comando OpenGL ainda não executado seja executado o mais rápido possível pelo mecanismo de exibição.
}
/*
	<menufunc>:
	Define os comandos a serem executados pelo menu do botão direito do mouse dependendo da opção selecionada;
*/
void menufunc(int val)
{
    switch (val)
    {
    case 0: //Opção: Cores->Red
        gColor[0]=1;
        gColor[1]=0;
        gColor[2]=0;
        break;
    case 1: //Opção: Cores->Green
        gColor[0]=0;
        gColor[1]=1;
        gColor[2]=0;
        break;
    case 2: //Opção: Cores->Blue
        gColor[0]=0;
        gColor[1]=0;
        gColor[2]=1;
        break;

    case 3: //Opção: Cores->Outra cor.
        printf("Informe a nova cor [Ponto Flutuante]:\nVermelho:");
        scanf(" %f",&gColor[0]);
        printf("Verde:");
        scanf(" %f",&gColor[1]);
        printf("Azul:");
        scanf(" %f",&gColor[2]);
        break;

    case 4:
        movendo = 1;
        break;

    case 5:
        if(atual != NULL)
        {
            apaga_desenho(listaPolig,atual);
            glutPostRedisplay();
        }
        break;

    case 6:
        salva_arquivo("test.txt",listaPolig);
        break;

    case 7:
        listaPolig = carrega_arquivo("test.txt");
        break;

    case 9:
        exit(0);
    }
}

static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 27 :
    case 'q':
        exit(0);
        break;

    case 'd'://DESENHAR
        op='D';
        break;

    case 'x'://LIVRE/Selecionar
        op='X';
        atual=NULL;
        primeiro_vertice = 1;
        break;
    case 'm'://Mover
        movendo = !movendo;
        break;

    case '+':
        //redimensionar
        if(atual != NULL)
        {
            escala_desenho(atual,1.02);
        }
        break;
    case '-':
        //redimensionar
        if(atual != NULL)
        {
            escala_desenho(atual,0.98);
        }
        break;
            
    case 'r':
        espelhamento(atual);
        break;            
            
    case 'j':
        salva_arquivo("test.txt",listaPolig);
        break;
    case 'k':
        listaPolig = carrega_arquivo("test.txt");
        break;
    case 'i':
        imprime_info_desenhos(listaPolig);
        break;
     case 'p':
        rotacao_desenho(atual, 1);
        break;

    }

    glutPostRedisplay();
}

void createMenu()
{
    int menuCor = glutCreateMenu(menufunc);
    glutAddMenuEntry("Red", 0);
    glutAddMenuEntry("Green", 1);
    glutAddMenuEntry("Blue", 2);
    glutAddMenuEntry("Outra cor.(Inserir via prompt)",3);

    int menuEditar = glutCreateMenu(menufunc);
    glutAddMenuEntry("Mover",4);
    glutAddMenuEntry("Apagar",5);

    int menuArquivo = glutCreateMenu(menufunc);
    glutAddMenuEntry("Salvar",6);
    glutAddMenuEntry("Carregar",7);

    int menu=glutCreateMenu(menufunc);
    glutAddSubMenu("Editar",menuEditar);
    glutAddSubMenu("Cores",menuCor);
    glutAddSubMenu("Arquivo",menuArquivo);
    glutAddMenuEntry("Sair",9);

    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

/*
    <init>:
    Inicializa alguns aspectos básicos para o funcionamento da aplicação.
*/
void init()
{
    glClearColor(0,0,0,1);					//Define a cor de fundo da janela de visualização como preta
    glMatrixMode(GL_PROJECTION);			//Define que as funções usadas a após a chamada desta rotina irão referir-se ao observador e não aos objetos do cenário 3D.
    glOrtho(-1, 1.0, -1, 1.0, -1.0, 1.0);  	//Define a janela de seleção onde se deseja trabalhar. Os parâmetros são os seguintes glOrtho(Xmin, Xmax, Ymin, Ymax, Zmin, Zmax).
    glMatrixMode(GL_MODELVIEW); 			//Define que as funções usadas a após a chamada desta rotina irão referir-se ao aos objetos do cenário 3D e não ao observador.
    createMenu(); 							//Chama função que cria o menu do botão direito do mouse.
}

void reshape(int width, int height)
{
    gHeight=height;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, width, 0,height);

    glMatrixMode(GL_MODELVIEW);
}


void mouseclick(int button, int state,int x, int y)
{
    if(button==GLUT_LEFT_BUTTON && state==GLUT_UP)
    {
        if(op == 'D')
        {
            if(primeiro_vertice == 1)
            {
                Desenho* novo = inicia_desenho();
                Vertice* v = cria_vertice(x,y);
                adiciona_vertice_desenho(novo,v);
                adiciona_novo_desenho(listaPolig,novo);
                atual = novo;
                primeiro_vertice = 0;
            }
            else
            {
                if(distancia_entre_2pt(atual->vertices->x,x,atual->vertices->y,y) < 5.0)
                {
                    op = 'X';
                    primeiro_vertice = 1;
                }
                else
                {
                    Vertice* v = cria_vertice(x,y);
                    adiciona_vertice_desenho(atual,v);
                }
            }
            rastro[0].x=x;
            rastro[0].y=y;
            rastro[1].x=x;
            rastro[1].y=y;
        }
    }
    else if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        if(op == 'X')
        {
            atual = clica_desenho(listaPolig,x,y);
            if(atual)
            {
                gColor[0] = atual->corPolig.r;
                gColor[1] = atual->corPolig.g;
                gColor[2] = atual->corPolig.b;
            }
            drag[0].x = x;
            drag[0].y = y;
            glutPostRedisplay();
        }
        if(movendo)
        {
            movendo = 0;
        }

    }
    else if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
    {
        if(op == 'X')
        {
            atual = clica_desenho(listaPolig,x,y);
            gColor[0] = atual->corPolig.r;
            gColor[1] = atual->corPolig.g;
            gColor[2] = atual->corPolig.b;
        }
    }
    glutPostRedisplay();
}

void mousedrag(int x, int y)
{

    if(!primeiro_vertice)
    {
        rastro[1].x=x;
        rastro[1].y=y;
    }
    if(movendo)
    {
        drag[1].x = x;
        drag[1].y = y;
        if(atual)
        {
            move_desenho(atual,drag[1].x - drag[0].x,drag[1].y - drag[0].y);
            drag[0].x = x;
            drag[0].y = y;
        }
    }
    glutPostRedisplay();
}

int main(int argc, char ** argv)
{
    //Inicializações
    listaPolig->head = NULL;
    atual = NULL;

    /*
        <glutInit>:
        Inicializa a biblioteca GLUT e negocia a sessão junto com o sistema de janela.
        Neste processo, glutInit pode provocar a finalização da aplicação GLUT, enviando
        uma mensagem de erro ao usuário indicando que não pode ser inicializada apropriadamente.
    */
    glutInit(&argc, argv);

    /*
        <glutInitDisplayMode>:
        Avisa a GLUT que tipo de modo de exibição deve ser usado quando a janela é criada.
        Neste caso os argumentos indicam a criação de uma janela single-buffered (GLUT_SINGLE)
        com o modo de cores RGBA (GLUT_RGB). O primeiro significa que todos os comandos de
        desenho são feitos na janela de exibição.O modo de cores RGBA significa que as cores
        são especificadas através do fornecimento de intensidades dos componentes red, green
        e blue separadas.

    */
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    /*
        <glutInitWindowPosition>:
        Define a posição inicial da janela.
    */
    glutInitWindowPosition(100,100);

    /*
        <glutInitWindowSize>:
        Define o tamanho inicial da janela;
    */
    glutInitWindowSize(800,600);

    /*
        <glutCreateWindow>:
        é o comando da biblioteca GLUT que cria a janela. Neste caso, é criada uma janela com o
        nome "Editor 2D OpenGL/GLUT". Este argumento corresponde a legenda para a barra de título
        da janela.
    */
    glutCreateWindow("Editor 2D OpenGL/GLUT");

    /*
        <glutReshapeFunc>:
        Define a função que será chamada quando a janela atual sofrer alterações em suas dimensões.
        Passa como argumento as novas dimensões para a função.
    */
    glutReshapeFunc(reshape);

    /*
        <glutDisplayFunc>:
        Define a função que será chamada quando a janela atual necessita ser redesenhada/desenhada.
    */
    glutDisplayFunc(display);

    /*
        <glutKeyboardFunc>:
        Determina a função de manipulação de comandos do teclado da janela atual.
    */
    glutKeyboardFunc(key);

    /*
        <glutMouseFunc>:
        Determina a função de controle para comandos do mouse da janela atual.

    */
    glutMouseFunc(mouseclick);

    /*
        <glutPassiveMotionFunc>:
        Estabelece a função que é chamada pela GLUT cada vez que o mouse é movido sobre a janela corrente
        enquanto nenhum de seus botões está pressionado. Parâmetros de entrada da função callback:
        (int x, int y). Os parâmetros x e y indicam a posição do mouse em coordenadas da janela.
    */
    glutPassiveMotionFunc(mousedrag);

	/*
		<init>:
		Inicia alguns componentes básicos para utilização do editor.
	*/
    init();
	
	/*
		<glutMainLoop>:
		Inicia o loop de processamento de desenhos com GLUT. Esta rotina deve ser chamada pelo menos uma vez em um programa que utilize a biblioteca GLUT.
	*/
    glutMainLoop();

    return 0;
}
