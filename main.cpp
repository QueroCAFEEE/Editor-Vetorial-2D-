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
int numLines;


// INI MOD
ListaD* listaPolig = (ListaD*)malloc(sizeof(ListaD));
Desenho* atual;

char op = 'D';

int primeiro_vertice = 1;

int movendo = 0;

// END MOD

point rastro[2];
point drag[2];

int gHeight;
float gColor[3]= {0,1,0};

void contorno_selecionado()
{
    if(atual->qtdVertices > 1)
    {
        Vertice* tmp_vert = atual->vertices;
        glColor3f(1.0,1.0,1.0);
        glBegin(GL_LINE_LOOP);
        while(tmp_vert != NULL)
        {
            glVertex2i(tmp_vert->x,gHeight-tmp_vert->y);
            tmp_vert = tmp_vert->prox;
        }
        glEnd();
    }
}

void drawlines()
{

    glColor3fv(gColor);
    if(atual)
    {
        atual->corPolig.r = gColor[0];
        atual->corPolig.g = gColor[1];
        atual->corPolig.b = gColor[2];
        atual->corPolig.t = 1;
    }


    if(op=='D' && primeiro_vertice == 0)
    {
        glBegin(GL_LINES);
        glVertex2i(rastro[0].x, gHeight-rastro[0].y);
        glVertex2i(rastro[1].x, gHeight-rastro[1].y);
        glEnd();
    }


    if(listaPolig->head)
    {
        Desenho* tmp_des = listaPolig->head;
        while(tmp_des != NULL)
        {
            Vertice* tmp_vert = tmp_des->vertices;

            if(tmp_vert)
            {
                glColor3d(tmp_des->corPolig.r,tmp_des->corPolig.g,tmp_des->corPolig.b);
                glBegin(GL_LINES);
                while(tmp_vert!=NULL)
                {

                    glVertex2i(tmp_vert->x, gHeight-tmp_vert->y);
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

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    drawlines();
    //glutSwapBuffers();
    if(atual != NULL && op == 'X')
    {
        contorno_selecionado();
    }
    glFlush();
}

void menufunc(int val)
{
    switch (val)
    {
    case 0:
        gColor[0]=1;
        gColor[1]=0;
        gColor[2]=0;
        break;
    case 1:
        gColor[0]=0;
        gColor[1]=1;
        gColor[2]=0;
        break;
    case 2:
        gColor[0]=0;
        gColor[1]=0;
        gColor[2]=1;
        break;

    case 3:
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
    case 'j':
        salva_arquivo("test.txt",listaPolig);
        break;
    case 'k':
        listaPolig = carrega_arquivo("test.txt");
        break;
    case 'i':
        imprime_info_desenhos(listaPolig);
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
    glClearColor(0,0,0,1);
    glMatrixMode(GL_PROJECTION);
    glOrtho(-1, 1.0, -1, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    createMenu();
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

    init();
    glutMainLoop();

    return 0;
}
