#include <windows.h>
#include <stdio.h>
#include "ListaDesenhos.h"

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
double escala = 0.0;
char op = 'D';

int primeiro_vertice = 1;

int movendo = 0;

// END MOD

typedef struct point
{
    int x;
    int y;
} point;

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
                glBegin(GL_POLYGON);
                while(tmp_vert!=NULL)
                {
                    glVertex2i(tmp_vert->x, gHeight-tmp_vert->y);
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
        if(atual != NULL){
            apaga_desenho(listaPolig,atual);
            glutPostRedisplay();
        }
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
        //escala += 0.20
        escala_desenho(atual,1.02);
        break;
    case '-':
        //redimensionar
        //escala -= 0.20
    escala_desenho(atual,0.98);
    break;

    case 'e':
    rotacao_desenho(atual,0.01);
    break;

    case 'k':
    rotacao_desenho(atual,-0.01);
    break;

    }

    /*if(escala < 0.2) {
		escala = 0.2;
	} else if(escala > 1.02) {
		escala = 1.02;
	}*/

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

    int menu=glutCreateMenu(menufunc);
    glutAddSubMenu("Editar",menuEditar);
    glutAddSubMenu("Cores",menuCor);
    glutAddMenuEntry("Sair",9);

    glutAttachMenu(GLUT_RIGHT_BUTTON);
}
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
                Vertice* v = cria_vertice(x,y);
                adiciona_vertice_desenho(atual,v);
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
    listaPolig->head = NULL;
    atual = NULL;

    // ###
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(800,600);
    glutCreateWindow("Editor 2D OpenGL/GLUT");
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutMouseFunc(mouseclick);
    glutPassiveMotionFunc(mousedrag);

    init();
    glutMainLoop();

    return 0;
}
