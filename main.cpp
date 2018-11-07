#include <windows.h>
#include <stdio.h>
#include "ListaDesenhos.h"
#include <math.h>
#define PI 3.14159265359
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
int gHeight;				//Utilizado para converter as coordenadas quando desenhar para não sair invertido.
float gColor[3]= {0,1,0};	//Variável temporária para definição de cores. Inicializada como verde.

void info()
{
    MessageBox(0,

               "Para mover o poligono basta apertar o botão direito do mouse basta "
               "selecionar 'Editar' e 'mover'o poligono grudara no cursor, \n\n"
               "Para solta-lo basta clicar em qualquer parte da janela de desenho \n\n"
               "Para mudar a cor do poligono, basta termina-lo e clicar com botao direito em cima"
               "e escolher a opcao 'Cores' e clicar na desejada \n\n"
               "Para salvar o poligono desenhado basta clicar com botao direito do mouse em cima do desenho "
               "e escolher a opcao 'Arquivo' e selecionar a opcao 'Salvar'\n\n"
               "Ja para carregar algum poligono ja salvo em qualquer parte da janela de desenho "
               "basta clicar com botao direito na janela de desenho, selecionar a opcao 'Arquivo' e selecionar a opcao 'Carregar' \n\n"
               "Para apagar um poligono basta clicar em cima do desenho com o botao direito do mouse "
               "e selecionar a opcao 'Editar' e selecionar a opcao 'Apagar', e o desenho sera totalmente apagado\n\n"
               "Para sair do editor basta clicar com botao direito do mouse em qualquer parte da tela e selecionar a opcao 'Sair' \n\n"
               "Para aumentar o poligono de tamanho basta apertar o '+' do teclado\n\n"
               "Ja para diminuir o poligono, basta apertar a tecla '-' do teclado\n\n"
               "Para desenhar seu poligono basta seguir com o cursor ate onde queira uma linha de seu poligono"
               "logo depois apertar o botao esquerdo do mouse, para que grave aquela aresta"
               "e assim sucessivamente ate fechar o poligono e o desenho mudar a cor de verde para branco"

               , "Ajuda", MB_OK );
}

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

    case 4: //Opção: Editar->Mover
        movendo = 1;
        break;

    case 5: //Opção: Editar->Apagar
        if(atual != NULL)
        {
            apaga_desenho(listaPolig,atual);
            glutPostRedisplay();    //Chama a função display novamente, fazendo com que a janela corrente seja redesenhada.
        }
        break;

    case 6: //Opção: Arquivo->Salvar
        salva_arquivo("test.txt",listaPolig);
        break;

    case 7: //Opção: Arquivo->Carregar
        listaPolig = carrega_arquivo("test.txt");
        break;

    case 8: //Opção: Editar->Rotacionar(+90º)
        if(atual != NULL)
        {
            rotacao_desenho(atual, PI/2);
        }
        break;

    case 9: //Opção: Editar->Rotacionar(-90º)
        if(atual != NULL)
        {
            rotacao_desenho(atual, -PI/2);
        }
        break;

    case 10: //Opção: Editar->Espelhar
        if(atual != NULL)
        {
            espelhamento(atual);
        }
        break;

    case 11: //Opção: Desenhar
        atual=NULL;
        op='D';
        break;

    case 12: //Opção: Editar-> Aumentar Escala
        if(atual != NULL)
        {
            escala_desenho(atual,1.2);
        }
        break;

    case 13: //Opção: Editar-> Diminuir Escala
        if(atual != NULL)
        {
            escala_desenho(atual,0.8);
        }
        break;

    case 98: //Opção: Sair
        info();
        break;

    case 99: //Opção: Sair
        exit(0);
    }
}

/*
    <key>:
    Define o que fazer quando determinada tecla for pressionada.
*/
static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 27 : //Sair
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
            escala_desenho(atual,1.2);
        }
        break;
    case '-':
        //redimensionar
        if(atual != NULL)
        {
            escala_desenho(atual,0.8);
        }
        break;

    case 'r':
        //espelhar
        if(atual!=NULL)
        {
            espelhamento(atual);
        }
        break;

    case 'j':
        //Salva os poligonos atuais em um arquivo
        salva_arquivo("test.txt",listaPolig);
        break;
    case 'k':
        //Carrega os poligonos de um arquivo
        listaPolig = carrega_arquivo("test.txt");
        break;
    case 'i':
        //Debuga as informações contidas nas estruturas dos poligonos
        imprime_info_desenhos(listaPolig);
        break;
    case 'p':
        if(atual != NULL)
        {
            rotacao_desenho(atual, PI/2);
        }
        break;

    }

    glutPostRedisplay();
}
/*
    <createMenu>:
    Função que define as opções do menu do click direito.
*/
void createMenu()
{
    int menuCor = glutCreateMenu(menufunc); //Define que a função a ser chamada é a menufunc
    glutAddMenuEntry("Red", 0); //Adiciona uma opção ao menu, com o primeiro parâmetro o nome que aparecerá, e o segundo o valor que passará para a função (menufunc)
    glutAddMenuEntry("Green", 1);
    glutAddMenuEntry("Blue", 2);
    glutAddMenuEntry("Outra cor.(Inserir via prompt)",3);

    int menuEditar = glutCreateMenu(menufunc);
    glutAddMenuEntry("Mover",4);
    glutAddMenuEntry("Espelhar",10);
    glutAddMenuEntry("Aumentar Escala",12);
    glutAddMenuEntry("Diminuir Escala",13);
    glutAddMenuEntry("Rotacionar(+90º)",8);
    glutAddMenuEntry("Rotacionar(-90º)",9);
    glutAddMenuEntry("Apagar",5);

    int menuArquivo = glutCreateMenu(menufunc);
    glutAddMenuEntry("Salvar",6);
    glutAddMenuEntry("Carregar",7);

    int menu=glutCreateMenu(menufunc);
    glutAddMenuEntry("Desenhar",11);
    glutAddSubMenu("Editar",menuEditar);//adiciona submenu
    glutAddSubMenu("Cores",menuCor);
    glutAddSubMenu("Arquivo",menuArquivo);
    glutAddMenuEntry("Ajuda",98);
    glutAddMenuEntry("Sair",99);

    glutAttachMenu(GLUT_RIGHT_BUTTON); //Define que o menu criado aparecerá ao click do botão direito.
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
    info();
}

void reshape(int width, int height)
{
    gHeight=height;                         //Salva em uma variável global a nova altura da janela.
    glMatrixMode(GL_PROJECTION);            //Define que as funções usadas a após a chamada desta rotina irão referir-se ao observador e não aos objetos do cenário 3D.
    glLoadIdentity();                       //inicia a matriz de projeção corrente como a matriz identidade
    gluOrtho2D(0, width, 0,height);         //Define a janela de seleção onde se deseja trabalhar. Os parâmetros são os seguintes glOrtho(Xmin, Xmax, Ymin, Ymax, Zmin, Zmax).
    glMatrixMode(GL_MODELVIEW);             //Define que as funções usadas a após a chamada desta rotina irão referir-se ao aos objetos do cenário 3D e não ao observador.
}

/*
    <mouseclick>:
    Função que é chamada sempre que há um clique no mouse dentro do programa.
*/
void mouseclick(int button, int state,int x, int y)
{
    if(button==GLUT_LEFT_BUTTON && state==GLUT_UP) //Se o botão esquerdo for solto
    {
        if(op == 'D')//Verifica se está no modo desenhar
        {
            if(primeiro_vertice == 1) //Se for primeiro vértice
            {
                Desenho* novo = inicia_desenho();   //cria um novo desenho
                Vertice* v = cria_vertice(x,y);
                adiciona_vertice_desenho(novo,v);
                adiciona_novo_desenho(listaPolig,novo);
                atual = novo;
                primeiro_vertice = 0;
            }
            else
            {
                if(distancia_entre_2pt(atual->vertices->x,x,atual->vertices->y,y) < 5.0)//verifica se o usuário clicou proximo ao primeiro ponto para fechar o poligono.
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
            //atualiza as posições do rastro
            rastro[0].x=x;
            rastro[0].y=y;
            rastro[1].x=x;
            rastro[1].y=y;
        }
    }
    else if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)//Se o botão esquerdo for pressionado
    {
        if(op == 'X')//Verifica se está no modo livre/selecionar
        {
            atual = clica_desenho(listaPolig,x,y);//busca se na área clicada possui algum poligono.
            if(atual) //caso ache um desenho, atualiza a cor temporária com a cor do poligono.
            {
                gColor[0] = atual->corPolig.r;
                gColor[1] = atual->corPolig.g;
                gColor[2] = atual->corPolig.b;
            }
            //atualiza valores para mover
            drag[0].x = x;
            drag[0].y = y;

            glutPostRedisplay();
        }
        if(movendo)//Caso esteja movendo e clicar com o botão esquerdo, solta o poligono.
        {
            movendo = 0;
        }

    }
    else if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)//Se o botão direito for pressionado
    {
        if(op == 'X')//Verifica se está no modo livre/selecionar
        {
            atual = clica_desenho(listaPolig,x,y);//busca se na área clicada possui algum poligono.
            if(atual) //caso ache um desenho, atualiza a cor temporária com a cor do poligono.
            {
                gColor[0] = atual->corPolig.r;
                gColor[1] = atual->corPolig.g;
                gColor[2] = atual->corPolig.b;
            }
            drag[0].x = x;
            drag[0].y = y;
        }
    }
    glutPostRedisplay();
}

/*
    <mousedrag>:
    Função que é chamada quando o mouse se move.
*/
void mousedrag(int x, int y)
{

    if(!primeiro_vertice)//Caso não seja o primeiro vértice, atualiza as posições do rastro.
    {
        rastro[1].x=x;
        rastro[1].y=y;
    }
    if(movendo)//Calcula a variação da posicão do mouse para mover o desenho.
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
