//SENAC 6o BCC
//ALEXANDRE MIZUNO
//RAFAEL TEODORO

#include <windows.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>
#include <IK/irrKlang.h>  // biblioteca para o sistema de som
#include <conio.h>
#include "imageloader.h"
#include "camera.h"

using namespace std;
#pragma comment(lib, "irrKlang.lib") // linka o  irrKlang.dll (necessario estar na pasta do projeto)

//variaveis globais
static float a1=1, a2=1, a3=2, girar=90, bx=0,by=-3.6,bz=0, bf=0,bpower=26,fx=0,fy=-3.6,fz=0, inc=0;
int fl=0;
GLuint _objetoTextura, GroundTextura, Ground2Textura, Ground3Textura, Ground4Textura, Ground5Textura, Ground6Textura, Ground7Textura, TorcidaTextura, EstacTextura,HoloTextura; //Nome das texturas
GLUquadric *quad;
float rotacionar;
CCamera Camera; //Cria uma camera
irrklang::ISoundEngine* engine = irrklang::createIrrKlangDevice(); // inicializa o engine de som

//definicao daz configuracoes da luz
const GLfloat light_ambient[]  = { 0.0, 0.0, 0.0, 1.0 }; //cor da luz
const GLfloat light_diffuse[]  = { 1.0, 1.0, 1.0, 1.0 }; //luz difusa
const GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 }; //luz especular
const GLfloat light_position[] = { 2.0, 5.0, 5.0, 0.0 }; //definir posicao da luz no ambiente

//definicao dos materiais dos objetos
const GLfloat mat_ambient[]    = { 1, 1, 1, 1 };
const GLfloat mat_diffuse[]    = { 1, 1, 1, 1.0 };
const GLfloat mat_specular[]   = { 1.0, 1.0, 1.0, 1.0 };
const GLfloat high_shininess[] = { 100.0 };

bool LightEnabled = true;

bool* keyStates = new bool[256]; // Cria um array de 256 posicoes com valores booleanos

//Declara o metodo para ele aparecer
void desenhar(void);
void atualizar(int);



void keyOperations (void) 
{  
	if (keyStates[27]) // Tecla ESC - Sai do Programa
	{ 
		PostQuitMessage(0);
	}  
	if (keyStates['a']) // Desliza/Move a bola para a esquerda
	{ 
		if (fl==1)
		{
			if (bx>-10)
			{
				bx -= 0.1;
				fx -= 0.1;
			}
			glTranslatef(bx, by, bz);
		}
		else
		{	
			Camera.StrafeRight(-0.1);
		}
	}  
	if (keyStates['d']) // Desliza/Move a bola para a direita
	{ 
		if (fl==1)
		{
			if (bx<10)
			{
				bx += 0.1;
				fx += 0.1;
			}
			glTranslatef(bx,by,bz);
		}
		else
			Camera.StrafeRight(0.1);
	}  
	if (keyStates['w']) //Move para frente / bola para frente
	{ 
		if (fl==1)
		{
			if (bz>-18.5)
			{
				bz -= 0.1;
				fz -= 0.1;
			}
			glTranslatef(bx, by, bz);
		}
		else
			Camera.MoveForwards( -0.5 ) ;
	}  
	if (keyStates['s']) //Move para tras / bola para tras
	{ 
		if (fl==1)
		{
			if (bz<84.5)
			{
				bz += 0.1;
				fz += 0.1;
			}
			glTranslatef(bx, by, bz);
		}
		else
			Camera.MoveForwards( 0.1 ) ;
	}  
	if (keyStates['x']) // Olha para cima
	{ 
		Camera.RotateX(5.0);
	}  
	if (keyStates['z']) // Olha para baixo
	{ 
		Camera.RotateX(-5.0);
	}  
	if (keyStates['q']) // Olha para a esquerda
	{ 
		Camera.RotateY(5.0);
	}  
	if (keyStates['e']) // Olha para a Direita
	{ 
		Camera.RotateY(-5.0);
	}  
	if (keyStates['f']) // Desce
	{ 
		Camera.Move(F3dVector(0.0,-0.3,0.0));
	}  
	if (keyStates['r']) //Sobe
	{ 
		Camera.Move(F3dVector(0.0,0.3,0.0));
	}  
	if (keyStates[32]) // Tecla ESPACO - Lanca a bola
	{ 
		fl=0;
		if (bf<bpower)
		{
			if (bf<bpower/2)
				by += 0.1;
			else
				by -= 0.1;
			
			bz -= 0.1;
			if (fx < bx)
			{
				//inc = fx/bpower;
				bx += inc*0.5;
			}
			if (fx > bx)
			{
				//inc = fx/bpower;
				bx += inc*0.5;
			}
			girar = girar - 5;
			glTranslatef(bx, by, bz);
			bf = bf + 0.1;

		}

		if ((bx>-1.4) && (bx < 2.4) && (by>1.4) && (bz< -20)) // Condicao de vitoria
		{
			engine->play2D("../Sounds/good.ogg");
			//MessageBox(NULL, "The Field Goal is GOOD", "Voce Ganhou",  MB_OK | MB_ICONEXCLAMATION);
			by=-3.6;
		}
		else
		{
			if (bf>=bpower)  // Condicao de derrota
			{
				engine->play2D("../Sounds/nogood.ogg");
				//MessageBox(NULL, "The Field Goal is NO GOOD", "Voce Perdeu",  MB_OK | MB_ICONEXCLAMATION);
				bpower=26;
				bf = 0;
			}
		}

		

		
	}  
	if (keyStates['m']) // Reseta a posicao da bola
	{ 
		bx=0;
		fx=0;
		by=-3.6;
		bz=0;
		fz=0;
		glTranslatef(bx, by, bz);
		girar = 90;
		bf=0;
		bpower=26;
		inc=0;
		
	}  

	if (keyStates['n'])  // Ativa a mudanca de posicionamento da bola
	{
		if ((by < -3.5)&& (bx>-10) && (bx<10) && (bz>-18.5) && (bz<84.5))
		{
			if (fl==0)
				fl=1;
			else
				fl=0;
		}
		
	} 

	if (keyStates['1']) // forca 1
	{
		bpower = 26;
	} 
	if (keyStates['2']) // forca 2
	{
		bpower = 36;
	} 
	if (keyStates['3']) // forca 3
	{
		bpower = 46;
	} 
	if (keyStates['4']) // forca 4
	{
		bpower = 56;
	} 
	if (keyStates['5']) // forca maxima
	{
		bpower = 66;
	} 
	if (keyStates['j']) // mira para a esquerda
	{
		if ((by < -3.5) && (fl==0) && (bx>-10) && (bx<10) && (bz>-18.5) && (bz<84.5))
		{
			fx -= 0.1;
			inc = fx/bpower;
		}
	} 
	if (keyStates['l']) // mira para a direita
	{
		if ((by < -3.5) && (fl==0) && (bx>-10) && (bx<10) && (bz>-18.5) && (bz<84.5))
		{
			fx += 0.1; 
			inc = fx/bpower;
		}
	} 
	if (keyStates['t'])
	{
		LightEnabled = !LightEnabled;
		if (LightEnabled) glEnable(GL_LIGHT0);
			else glDisable(GL_LIGHT0);
	}
}

//Função para transformar a imagem em textura do objeto
GLuint carregarTextura(Image* imagem)
{
	GLuint objetoTextura;
	glGenTextures(1, &objetoTextura); //Gera o nome para textura ou seja guarda a textura
	glBindTexture(GL_TEXTURE_2D, objetoTextura); //Edição de textura

	//mapear a imagem de textura
	glTexImage2D(GL_TEXTURE_2D,                //local
				 0,                            //Especifica o grau de detalhe, no caso 0 tira como base o a imagem
				 GL_RGB,                       //Especifica o número de cores na textura
				 imagem->width, imagem->height,//Largura e altura
				 0,                            //Borda da imagem
				 GL_RGB,                       //GL_RGB, porque os pixels estao no formato RGB
				 GL_UNSIGNED_BYTE,             //GL_UNSIGNED_BYTE, são armazenados com números sem sinal
				 imagem->pixels);               //Especifica um ponteiro para os dados de imagem na memória

	return objetoTextura; //retorna o nome da textura
}

//Importa a imagem para textura e habilita funções para a caracterização do objeto
void renderizar()
{
	quad = gluNewQuadric();

	//TEXTURA DA BOLA
	Image* imagem = loadBMP("bola.bmp"); //carregar imagem
	_objetoTextura = carregarTextura(imagem); //carrega textura
	delete imagem; //depois do uso deleta a imagem para reiniciar a aplicação
	
	// TEXTURA CAMPO 1/7
	Image* imagemGround = loadBMP("ground.bmp"); //carregar imagem
	GroundTextura = carregarTextura(imagemGround); //carrega textura
	delete imagemGround; //depois do uso deleta a imagem para reiniciar a aplicação

	// TEXTURA CAMPO 2/7
	Image* imagemGround2 = loadBMP("ground2.bmp"); //carregar imagem
	Ground2Textura = carregarTextura(imagemGround2); //carrega textura
	delete imagemGround2; //depois do uso deleta a imagem para reiniciar a aplicação

	// TEXTURA CAMPO 3/7
	Image* imagemGround3 = loadBMP("ground3.bmp"); //carregar imagem
	Ground3Textura = carregarTextura(imagemGround3); //carrega textura
	delete imagemGround3; //depois do uso deleta a imagem para reiniciar a aplicação

	// TEXTURA CAMPO 4/7
	Image* imagemGround4 = loadBMP("ground4.bmp"); //carregar imagem
	Ground4Textura = carregarTextura(imagemGround4); //carrega textura
	delete imagemGround4; //depois do uso deleta a imagem para reiniciar a aplicação

	// TEXTURA CAMPO 5/7
	Image* imagemGround5 = loadBMP("ground5.bmp"); //carregar imagem
	Ground5Textura = carregarTextura(imagemGround5); //carrega textura
	delete imagemGround5; //depois do uso deleta a imagem para reiniciar a aplicação

	// TEXTURA CAMPO 6/7
	Image* imagemGround6 = loadBMP("ground6.bmp"); //carregar imagem
	Ground6Textura = carregarTextura(imagemGround6); //carrega textura
	delete imagemGround6; //depois do uso deleta a imagem para reiniciar a aplicação
	
	// TEXTURA CAMPO 7/7
	Image* imagemGround7 = loadBMP("ground7.bmp"); //carregar imagem
	Ground7Textura = carregarTextura(imagemGround7); //carrega textura
	delete imagemGround7; //depois do uso deleta a imagem para reiniciar a aplicação

	// TEXTURA TORCIDA
	Image* imagemTorcida = loadBMP("torcida.bmp"); //carregar imagem
	TorcidaTextura = carregarTextura(imagemTorcida); //carrega textura
	delete imagemTorcida; //depois do uso deleta a imagem para reiniciar a aplicação

	// TEXTURA ESTACIONAMENTO
	Image* imagemEstacionamento = loadBMP("estacionamento.bmp"); //carregar imagem
	EstacTextura = carregarTextura(imagemEstacionamento); //carrega textura
	delete imagemEstacionamento; //depois do uso deleta a imagem para reiniciar a aplicação

	// TEXTURA HOLOFOTE
	Image* imagemHolofote = loadBMP("refletor.bmp"); //carregar imagem
	HoloTextura = carregarTextura(imagemHolofote); //carrega textura
	delete imagemHolofote; //depois do uso deleta a imagem para reiniciar a aplicação
}

//Projecao do objeto no ambiente
void projetar(int largura, int altura)
{
	if (altura == 0 || largura == 0) return;  //Nothing is visible then, so return
	
	//Set a new projection matrix
	glMatrixMode(GL_PROJECTION);  
	glLoadIdentity();
	//Angle of view:40 degrees
	//Near clipping plane distance: 0.5
	//Far clipping plane distance: 20.0
	gluPerspective(40.0,(GLdouble)largura/(GLdouble)altura,0.5,150);

	glMatrixMode(GL_MODELVIEW);
	glViewport(0,0,largura,altura);  //Use the whole window for rendering
}

//Desenha o objeto e o cenario
void desenhar(void)
{
	keyOperations();
	glEnable(GL_LIGHTING);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //limpa e coloca os valores pre definido
    glLoadIdentity(); // inicia a matriz de projeção corrente como a matriz identidade
	Camera.Render();

	//BOLA
	glEnable(GL_TEXTURE_2D); //habilitar textura
	glBindTexture(GL_TEXTURE_2D, _objetoTextura); //vincular GL_TEXTURE_2D com a texturização feita

	glPushMatrix(); //insere a matriz de transformação corrente na pilha
	glTexParameteri(GL_TEXTURE_2D, //Especifica a textura de destino da unidade de textura ativa
                    GL_TEXTURE_MIN_FILTER, //Especifica o nome de um parâmetro de textura
                    GL_NEAREST); //Especifica o valor
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // especificacao igual de cima

	glTranslatef(bx, by, bz);

	glRotatef(girar,1.0f,0.0f,0.0f); //rotaciona o objeto verticalmente
	glRotatef(rotacionar,0.0f,0.0f,1.0f); //rotaciona o objeto horizontalmente
	glScalef (a1/5,a2/5, a3/5); //altera o formato do objeto
	gluQuadricTexture(quad,1); //especifica se as coordenadas de textura devem ser gerados
    gluSphere(quad,1,20,20); //cria o objeto, no caso uma esfera
    glPopMatrix(); //retira a matriz do topo da pilha e torna esta última a matriz de transformação corrente
	
	//CAMPO PARTE 1/7
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, GroundTextura); 
	glPushMatrix(); //insere a matriz de transformação corrente na pilha
	glTexParameteri(GL_TEXTURE_2D, //Especifica a textura de destino da unidade de textura ativa
                    GL_TEXTURE_MIN_FILTER, //Especifica o nome de um parâmetro de textura
                    GL_NEAREST); //Especifica o valor
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // especificacao igual de cima
	float HalfGroundSize = 10.0f;

	glTranslatef(0.0,-4.0,0.0);
	glBegin(GL_POLYGON);
	  
	  glNormal3f(0.0,1.0,0.0);
	  
	  glTexCoord2f(0.0,0.0);
	  glVertex3f(-HalfGroundSize, 0.0, -23.0);
	  glTexCoord2f(0.0,1.0);
	  glVertex3f(-HalfGroundSize, 0.0, -7);
	  glTexCoord2f(1.0,1.0);
	  glVertex3f(+HalfGroundSize, 0.0, -7);
	  glTexCoord2f(1.0,0.0);
	  glVertex3f(+HalfGroundSize, 0.0, -23.0);
	glEnd();
	glPopMatrix(); //retira a matriz do topo da pilha e torna esta última a matriz de transformação corrente

	//CAMPO PARTE 2/7
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, Ground2Textura); 
	glPushMatrix(); //insere a matriz de transformação corrente na pilha
	glTexParameteri(GL_TEXTURE_2D, //Especifica a textura de destino da unidade de textura ativa
                    GL_TEXTURE_MIN_FILTER, //Especifica o nome de um parâmetro de textura
                    GL_NEAREST); //Especifica o valor
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // especificacao igual de cima
	
	glTranslatef(0.0,-4.0,0.0);
	glBegin(GL_POLYGON);
	  
	  glNormal3f(0.0,1.0,0.0);
	  
	  glTexCoord2f(0.0,0.0);
	  glVertex3f(-HalfGroundSize, 0.0, -7);
	  glTexCoord2f(0.0,1.0);
	  glVertex3f(-HalfGroundSize, 0.0, 9);
	  glTexCoord2f(1.0,1.0);
	  glVertex3f(+HalfGroundSize, 0.0, 9);
	  glTexCoord2f(1.0,0.0);
	  glVertex3f(+HalfGroundSize, 0.0, -7);
	glEnd();
	glPopMatrix(); //retira a matriz do topo da pilha e torna esta última a matriz de transformação corrente

	//CAMPO PARTE 3/7
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, Ground3Textura); 
	glPushMatrix(); //insere a matriz de transformação corrente na pilha
	glTexParameteri(GL_TEXTURE_2D, //Especifica a textura de destino da unidade de textura ativa
                    GL_TEXTURE_MIN_FILTER, //Especifica o nome de um parâmetro de textura
                    GL_NEAREST); //Especifica o valor
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // especificacao igual de cima
	
	glTranslatef(0.0,-4.0,0.0);
	glBegin(GL_POLYGON);
	  
	  glNormal3f(0.0,1.0,0.0);
	  
	  glTexCoord2f(0.0,0.0);
	  glVertex3f(-HalfGroundSize, 0.0, 9);
	  glTexCoord2f(0.0,1.0);
	  glVertex3f(-HalfGroundSize, 0.0, 25);
	  glTexCoord2f(1.0,1.0);
	  glVertex3f(+HalfGroundSize, 0.0, 25);
	  glTexCoord2f(1.0,0.0);
	  glVertex3f(+HalfGroundSize, 0.0, 9);
	glEnd();
	glPopMatrix(); //retira a matriz do topo da pilha e torna esta última a matriz de transformação corrente

	//CAMPO PARTE 4/7
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, Ground4Textura); 
	glPushMatrix(); //insere a matriz de transformação corrente na pilha
	glTexParameteri(GL_TEXTURE_2D, //Especifica a textura de destino da unidade de textura ativa
                    GL_TEXTURE_MIN_FILTER, //Especifica o nome de um parâmetro de textura
                    GL_NEAREST); //Especifica o valor
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // especificacao igual de cima
	
	glTranslatef(0.0,-4.0,0.0);
	glBegin(GL_POLYGON);
	  
	  glNormal3f(0.0,1.0,0.0);
	  
	  glTexCoord2f(0.0,0.0);
	  glVertex3f(-HalfGroundSize, 0.0, 25);
	  glTexCoord2f(0.0,1.0);
	  glVertex3f(-HalfGroundSize, 0.0, 41);
	  glTexCoord2f(1.0,1.0);
	  glVertex3f(+HalfGroundSize, 0.0, 41);
	  glTexCoord2f(1.0,0.0);
	  glVertex3f(+HalfGroundSize, 0.0, 25);
	glEnd();
	glPopMatrix(); //retira a matriz do topo da pilha e torna esta última a matriz de transformação corrente
	
	//CAMPO PARTE 5/7	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, Ground5Textura); 
	glPushMatrix(); //insere a matriz de transformação corrente na pilha
	glTexParameteri(GL_TEXTURE_2D, //Especifica a textura de destino da unidade de textura ativa
                    GL_TEXTURE_MIN_FILTER, //Especifica o nome de um parâmetro de textura
                    GL_NEAREST); //Especifica o valor
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // especificacao igual de cima
	
	glTranslatef(0.0,-4.0,0.0);
	glBegin(GL_POLYGON);
	  
	  glNormal3f(0.0,1.0,0.0);
	  
	  glTexCoord2f(0.0,0.0);
	  glVertex3f(-HalfGroundSize, 0.0, 41);
	  glTexCoord2f(0.0,1.0);
	  glVertex3f(-HalfGroundSize, 0.0, 57);
	  glTexCoord2f(1.0,1.0);
	  glVertex3f(+HalfGroundSize, 0.0, 57);
	  glTexCoord2f(1.0,0.0);
	  glVertex3f(+HalfGroundSize, 0.0, 41);
	glEnd();
	glPopMatrix(); //retira a matriz do topo da pilha e torna esta última a matriz de transformação corrente
	
	//CAMPO PARTE 6/7 	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, Ground6Textura); 
	glPushMatrix(); //insere a matriz de transformação corrente na pilha
	glTexParameteri(GL_TEXTURE_2D, //Especifica a textura de destino da unidade de textura ativa
                    GL_TEXTURE_MIN_FILTER, //Especifica o nome de um parâmetro de textura
                    GL_NEAREST); //Especifica o valor
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // especificacao igual de cima
	
	glTranslatef(0.0,-4.0,0.0);
	glBegin(GL_POLYGON);
	  
	  glNormal3f(0.0,1.0,0.0);
	  
	  glTexCoord2f(0.0,0.0);
	  glVertex3f(-HalfGroundSize, 0.0, 57);
	  glTexCoord2f(0.0,1.0);
	  glVertex3f(-HalfGroundSize, 0.0, 73);
	  glTexCoord2f(1.0,1.0);
	  glVertex3f(+HalfGroundSize, 0.0, 73);
	  glTexCoord2f(1.0,0.0);
	  glVertex3f(+HalfGroundSize, 0.0, 57);
	glEnd();
	glPopMatrix(); //retira a matriz do topo da pilha e torna esta última a matriz de transformação corrente
	
	//CAMPO PARTE 7/7
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, Ground7Textura); 
	glPushMatrix(); //insere a matriz de transformação corrente na pilha
	glTexParameteri(GL_TEXTURE_2D, //Especifica a textura de destino da unidade de textura ativa
                    GL_TEXTURE_MIN_FILTER, //Especifica o nome de um parâmetro de textura
                    GL_NEAREST); //Especifica o valor
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // especificacao igual de cima
	
	glTranslatef(0.0,-4.0,0.0);
	glBegin(GL_POLYGON);
	  
	  glNormal3f(0.0,1.0,0.0);
	  
	  glTexCoord2f(0.0,0.0);
	  glVertex3f(-HalfGroundSize, 0.0, 73);
	  glTexCoord2f(0.0,1.0);
	  glVertex3f(-HalfGroundSize, 0.0, 89);
	  glTexCoord2f(1.0,1.0);
	  glVertex3f(+HalfGroundSize, 0.0, 89);
	  glTexCoord2f(1.0,0.0);
	  glVertex3f(+HalfGroundSize, 0.0, 73);
	  

	glEnd();
	glPopMatrix(); //retira a matriz do topo da pilha e torna esta última a matriz de transformação corrente
		
	//TRAVES
	glColor3f(1.0,1.0,0.0); // COR AMARELA
	
	glDisable(GL_TEXTURE_2D);
	
	//TRAVE 1 - z<0
	glBegin(GL_QUADS);
		glVertex3d (0.45, -4.0, -20.0);
		glVertex3d (0.45, -1.5, -20.0);
		glVertex3d (0.55, -1.5, -20.0);
		glVertex3d (0.55, -4.0, -20.0);
	glEnd();
	glBegin(GL_QUADS);
		glVertex3d (-1.5, -1.5, -20.0);
		glVertex3d (-1.5, -1.40, -20.0);
		glVertex3d (2.5, -1.40, -20.0);
		glVertex3d (2.5, -1.5, -20.0);
	glEnd();
	glBegin(GL_QUADS);
		glVertex3d (-1.5, -1.40, -20.0);
		glVertex3d (-1.5, 3.3, -20.0);
		glVertex3d (-1.4, 3.3, -20.0);
		glVertex3d (-1.4, -1.40, -20.0);
	glEnd();
	glBegin(GL_QUADS);
		glVertex3d (2.4, 3.3, -20.0);
		glVertex3d (2.5, 3.3, -20.0);
		glVertex3d (2.5, -1.40, -20.0);
		glVertex3d (2.4, -1.40, -20.0);
	glEnd();

	//TRAVE 2 - z>0
	glBegin(GL_QUADS);
		glVertex3d (0.45, -4.0, 86.0);
		glVertex3d (0.45, -1.5, 86.0);
		glVertex3d (0.55, -1.5, 86);
		glVertex3d (0.55, -4.0, 86);
	glEnd();
	glBegin(GL_QUADS);
		glVertex3d (-1.5, -1.5, 86);
		glVertex3d (-1.5, -1.40, 86);
		glVertex3d (2.5, -1.40, 86);
		glVertex3d (2.5, -1.5, 86);
	glEnd();
	glBegin(GL_QUADS);
		glVertex3d (-1.5, -1.40, 86);
		glVertex3d (-1.5, 3.3, 86);
		glVertex3d (-1.4, 3.3, 86);
		glVertex3d (-1.4, -1.40, 86);
	glEnd();
	glBegin(GL_QUADS);
		glVertex3d (2.4, 3.3, 86);
		glVertex3d (2.5, 3.3, 86);
		glVertex3d (2.5, -1.40, 86);
		glVertex3d (2.4, -1.40, 86);
	glEnd();

	glPushMatrix();
	glPopMatrix();
	glColor3f(1.0,1.0,1.0);

	//SETA DE MIRA
	glColor3f(1.0,0.0,0.0); // COR VERMELHA
	
	glDisable(GL_TEXTURE_2D);

	glBegin(GL_LINE_LOOP);
		glVertex3d (bx, -3.6, fz);
		glVertex3d (fx+0.2, -3.0, fz-2);
		glVertex3d (fx+0.6, -3.0, fz-2);
		glVertex3d (fx, -2.5, fz-3.2);
		glVertex3d (fx-0.6, -3.0, fz-2);
		glVertex3d (fx-0.2, -3.0, fz-2);
	glEnd();
	glPushMatrix();
	glPopMatrix();
	glColor3f(1.0,1.0,1.0); // RESETA A COR PARA BRANCO

	// ARQUIBANCADA 
	glColor3f(0.3,0.3,0.3); // COR CINZA ESCURO

	glDisable(GL_TEXTURE_2D);

	//Parte externa
	glBegin(GL_QUADS); 
		glVertex3d (-25, 5.0, -38.0);
		glVertex3d (25, 5.0, -38.0);
		glVertex3d (25, -4.0, -38.0);
		glVertex3d (-25, -4.0, -38.0);
	glEnd();

	glBegin(GL_QUADS);
		glVertex3d (-25, 5.0, 104.0);
		glVertex3d (25, 5.0, 104.0);
		glVertex3d (25, -4.0, 104.0);
		glVertex3d (-25, -4.0, 104.0);
	glEnd();

	glBegin(GL_QUADS);
		glVertex3d (25, 5.0, -38.0);
		glVertex3d (25, -4.0, -38.0);
		glVertex3d (25, -4.0, 104.0);
		glVertex3d (25, 5.0, 104.0);
	glEnd();

	glBegin(GL_QUADS);
		glVertex3d (-25, 5.0, -38.0);
		glVertex3d (-25, -4.0, -38.0);
		glVertex3d (-25, -4.0, 104.0);
		glVertex3d (-25, 5.0, 104.0);
	glEnd();

	//Parte Superior
	glBegin(GL_QUADS);
		glVertex3d (-25, 5.0, -38.0);
		glVertex3d (-25, 5.0, 104.0);
		glVertex3d (-24, 5.0, 104.0);
		glVertex3d (-24, 5.0, -38.0);
	glEnd();
	glBegin(GL_QUADS);
		glVertex3d (25, 5.0, -38.0);
		glVertex3d (25, 5.0, 104.0);
		glVertex3d (24, 5.0, 104.0);
		glVertex3d (24, 5.0, -38.0);
	glEnd();
	glBegin(GL_QUADS);
		glVertex3d (24, 5.0, -38.0);
		glVertex3d (24, 5.0, -37.0);
		glVertex3d (-24, 5.0, -37.0);
		glVertex3d (-24, 5.0, -38.0);
	glEnd();
	glBegin(GL_QUADS);
		glVertex3d (24, 5.0, 104.0);
		glVertex3d (24, 5.0, 103.0);
		glVertex3d (-24, 5.0, 103.0);
		glVertex3d (-24, 5.0, 104.0);
	glEnd();

	//Parte Inferior
	glBegin(GL_QUADS);
		glVertex3d (-10, -4, -23.0);
		glVertex3d (-10, -4, 89.0);
		glVertex3d (-10, -3, 89.0);
		glVertex3d (-10, -3, -23);
	glEnd();
	glBegin(GL_QUADS);
		glVertex3d (-10, -4, -23.0);
		glVertex3d (-10, -3, -23.0);
		glVertex3d (10, -3, -23.0);
		glVertex3d (10, -4, -23.0);
	glEnd();
	glBegin(GL_QUADS);
		glVertex3d (10, -4, -23.0);
		glVertex3d (10, -4, 89.0);
		glVertex3d (10, -3, 89.0);
		glVertex3d (10, -3, -23.0);
	glEnd();
	glBegin(GL_QUADS);
		glVertex3d (10, -4, 89.0);
		glVertex3d (10, -3, 89.0);
		glVertex3d (-10, -3, 89.0);
		glVertex3d (-10, -4, 89.0);
	glEnd();

	glPushMatrix();
	glPopMatrix();
	glColor3f(1.0,1.0,1.0);


	//TORCIDA
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, TorcidaTextura); 
	glPushMatrix(); //insere a matriz de transformação corrente na pilha
	glTexParameteri(GL_TEXTURE_2D, //Especifica a textura de destino da unidade de textura ativa
                    GL_TEXTURE_MIN_FILTER, //Especifica o nome de um parâmetro de textura
                    GL_NEAREST); //Especifica o valor
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // especificacao igual de cima

	glBegin(GL_POLYGON);
	  glNormal3f(0.0,1.0,0.0);
	  
	  glTexCoord2f(0.0,0.0);
	  glVertex3f(-10, -3.0, -23);
	  glTexCoord2f(0.0,1.0);
	  glVertex3f(-10, -3.0, 89);
	  glTexCoord2f(1.0,1.0);
	  glVertex3f(-24, 5, 89);
	  glTexCoord2f(1.0,0.0);
	  glVertex3f(-24, 5, -23);
	glEnd();

	glBegin(GL_POLYGON);
	  glNormal3f(0.0,1.0,0.0);
	  
	  glTexCoord2f(0.0,0.0);
	  glVertex3f(10, -3.0, -23);
	  glTexCoord2f(0.0,1.0);
	  glVertex3f(10, -3.0, 89);
	  glTexCoord2f(1.0,1.0);
	  glVertex3f(24, 5, 89);
	  glTexCoord2f(1.0,0.0);
	  glVertex3f(24, 5, -23);
	glEnd();

	glBegin(GL_POLYGON);
	  glNormal3f(0.0,1.0,0.0);
	  
	  glTexCoord2f(0.0,0.0);
	  glVertex3f(-10, -3.0, -23);
	  glTexCoord2f(0.0,1.0);
	  glVertex3f(10, -3.0, -23);
	  glTexCoord2f(1.0,1.0);
	  glVertex3f(10, 5, -37);
	  glTexCoord2f(1.0,0.0);
	  glVertex3f(-10, 5, -37);
	glEnd();

	glBegin(GL_POLYGON);
	  glNormal3f(0.0,1.0,0.0);
	  
	  glTexCoord2f(0.0,0.0);
	  glVertex3f(-10, -3.0, 89);
	  glTexCoord2f(0.0,1.0);
	  glVertex3f(10, -3.0, 89);
	  glTexCoord2f(1.0,1.0);
	  glVertex3f(10, 5, 104);
	  glTexCoord2f(1.0,0.0);
	  glVertex3f(-10, 5, 104);
	glEnd();

	//Cantos diagonais
	glBegin(GL_POLYGON);
	  glNormal3f(0.0,1.0,0.0);
	  
	  glTexCoord2f(0.0,0.0);
	  glVertex3f(-10, -3.0, 89);
	  glTexCoord2f(0.0,1.0);
	  glVertex3f(-10, 5.0, 104);
	  glTexCoord2f(1.0,1.0);
	  glVertex3f(-24, 5, 104);
	  glTexCoord2f(1.0,0.0);
	  glVertex3f(-24, 5, 89);
	glEnd();

	glBegin(GL_POLYGON);
	  glNormal3f(0.0,1.0,0.0);
	  
	  glTexCoord2f(0.0,0.0);
	  glVertex3f(10, -3.0, 89);
	  glTexCoord2f(0.0,1.0);
	  glVertex3f(10, 5.0, 104);
	  glTexCoord2f(1.0,1.0);
	  glVertex3f(24, 5, 104);
	  glTexCoord2f(1.0,0.0);
	  glVertex3f(24, 5, 89);
	glEnd();

	glBegin(GL_POLYGON);
	  glNormal3f(0.0,1.0,0.0);
	  
	  glTexCoord2f(0.0,0.0);
	  glVertex3f(-10, -3.0, -23);
	  glTexCoord2f(0.0,1.0);
	  glVertex3f(-10, 5.0, -37);
	  glTexCoord2f(1.0,1.0);
	  glVertex3f(-24, 5, -37);
	  glTexCoord2f(1.0,0.0);
	  glVertex3f(-24, 5, -23);
	glEnd();

	glBegin(GL_POLYGON);
	  glNormal3f(0.0,1.0,0.0);
	  
	  glTexCoord2f(0.0,0.0);
	  glVertex3f(10, -3.0, -23);
	  glTexCoord2f(0.0,1.0);
	  glVertex3f(10, 5.0, -37);
	  glTexCoord2f(1.0,1.0);
	  glVertex3f(24, 5, -37);
	  glTexCoord2f(1.0,0.0);
	  glVertex3f(24, 5, -23);
	glEnd();

	glPopMatrix(); //retira a matriz do topo da pilha e torna esta última a matriz de transformação corrente

	// HOLOFOTES 
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, HoloTextura); 
	glPushMatrix(); //insere a matriz de transformação corrente na pilha
	glTexParameteri(GL_TEXTURE_2D, //Especifica a textura de destino da unidade de textura ativa
                    GL_TEXTURE_MIN_FILTER, //Especifica o nome de um parâmetro de textura
                    GL_NEAREST); //Especifica o valor
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // especificacao igual de cima
	
	glBegin(GL_QUADS); 
		glVertex3d (-24.95, 15.0, -23.0);
		glVertex3d (-25.05, 15.0, -23.0);
		glVertex3d (-25.05, -4.0, -23.0);
		glVertex3d (-24.95, -4.0, -23.0);
	glEnd();

	glBegin(GL_POLYGON); 
	glNormal3f(0.0,1.0,0.0);
	  
		glTexCoord2f(0.0,0.0);
		glVertex3d (-24.95, 15.0, -25.0);
		glTexCoord2f(0.0,1.0);
		glVertex3d (-24.95, 15.0, -21.0);
		glTexCoord2f(1.0,1.0);
		glVertex3d (-22.95, 17.0, -21.0);
		glTexCoord2f(1.0,0.0);
		glVertex3d (-22.95, 17.0, -25.0);
	glEnd();
	
   
	
	glBegin(GL_QUADS);
		glVertex3d (-24.95, 15.0, 89.0);
		glVertex3d (-25.05, 15.0, 89.0);
		glVertex3d (-25.05, -4.0, 89.0);
		glVertex3d (-24.95, -4.0, 89.0);
	glEnd();
	glBegin(GL_POLYGON); 
	glNormal3f(0.0,1.0,0.0);
	  
		glTexCoord2f(0.0,0.0);
		glVertex3d (-24.95, 15.0, 87.0);
		glTexCoord2f(0.0,1.0);
		glVertex3d (-24.95, 15.0, 91.0);
		glTexCoord2f(1.0,1.0);
		glVertex3d (-22.95, 17.0, 91.0);
		glTexCoord2f(1.0,0.0);
		glVertex3d (-22.95, 17.0, 87.0);
	glEnd();

	glBegin(GL_QUADS);
		glVertex3d (24.95, 15.0, 89.0);
		glVertex3d (25.05, 15.0, 89.0);
		glVertex3d (25.05, -4.0, 89.0);
		glVertex3d (24.95, -4.0, 89.0);
	glEnd();
	glBegin(GL_POLYGON); 
	glNormal3f(0.0,1.0,0.0);
	  
		glTexCoord2f(0.0,0.0);
		glVertex3d (24.95, 15.0, 87.0);
		glTexCoord2f(0.0,1.0);
		glVertex3d (24.95, 15.0, 91.0);
		glTexCoord2f(1.0,1.0);
		glVertex3d (22.95, 17.0, 91.0);
		glTexCoord2f(1.0,0.0);
		glVertex3d (22.95, 17.0, 87.0);
	glEnd();

	glBegin(GL_QUADS);
		glVertex3d (24.95, 15.0, -23.0);
		glVertex3d (25.05, 15.0, -23.0);
		glVertex3d (25.05, -4.0, -23.0);
		glVertex3d (24.95, -4.0, -23.0);
	glEnd();
	glBegin(GL_POLYGON); 
	glNormal3f(0.0,1.0,0.0);
	  
		glTexCoord2f(0.0,0.0);
		glVertex3d (24.95, 15.0, -25.0);
		glTexCoord2f(0.0,1.0);
		glVertex3d (24.95, 15.0, -21.0);
		glTexCoord2f(1.0,1.0);
		glVertex3d (22.95, 17.0, -21.0);
		glTexCoord2f(1.0,0.0);
		glVertex3d (22.95, 17.0, -25.0);
	glEnd();


	glPopMatrix();
	glColor3f(1.0,1.0,1.0); // RESETA A COR PARA BRANCO


	//ESTACIONAMENTO 
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glBindTexture(GL_TEXTURE_2D, EstacTextura); 
	glPushMatrix(); //insere a matriz de transformação corrente na pilha
	glTexParameteri(GL_TEXTURE_2D, //Especifica a textura de destino da unidade de textura ativa
                    GL_TEXTURE_MIN_FILTER, //Especifica o nome de um parâmetro de textura
                    GL_NEAREST); //Especifica o valor
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // especificacao igual de cima

	glBegin(GL_POLYGON);

		glNormal3f(0.0,1.0,0.0);
	  
		glTexCoord2f(0.0,0.0);
		glVertex3d (-150, -4.0, -188.0);
		glTexCoord2f(0.0,1.0);
		glVertex3d (150, -4.0, -188.0);
		glTexCoord2f(1.0,1.0);
		glVertex3d (150, -4.0, -38.0);
		glTexCoord2f(1.0,0.0);
		glVertex3d (-150, -4.0, -38.0);

	glEnd();
	
	glBegin(GL_POLYGON);

		glNormal3f(0.0,1.0,0.0);
	  
		glTexCoord2f(0.0,0.0);
		glVertex3d (-150, -4.0, 250.0);
		glTexCoord2f(0.0,1.0);
		glVertex3d (150, -4.0, 250.0);
		glTexCoord2f(1.0,1.0);
		glVertex3d (150, -4.0, 104.0);
		glTexCoord2f(1.0,0.0);
		glVertex3d (-150, -4.0, 104.0);

	glEnd();
	
	glBegin(GL_POLYGON);

		glNormal3f(0.0,1.0,0.0);
	  
		glTexCoord2f(0.0,0.0);
		glVertex3d (25, -4.0, -38.0);
		glTexCoord2f(0.0,1.0);
		glVertex3d (150, -4.0, -38.0);
		glTexCoord2f(1.0,1.0);
		glVertex3d (150, -4.0, 104.0);
		glTexCoord2f(1.0,0.0);
		glVertex3d (25, -4.0, 104.0);

	glEnd();
	
	glBegin(GL_POLYGON);

		glNormal3f(0.0,1.0,0.0);
	  
		glTexCoord2f(0.0,0.0);
		glVertex3d (-25, -4.0, -38.0);
		glTexCoord2f(0.0,1.0);
		glVertex3d (-150, -4.0, -38.0);
		glTexCoord2f(1.0,1.0);
		glVertex3d (-150, -4.0, 104.0);
		glTexCoord2f(1.0,0.0);
		glVertex3d (-25, -4.0, 104.0);

	glEnd();
	glPopMatrix(); //retira a matriz do topo da pilha e torna esta última a matriz de transformação corrente

	glutSwapBuffers();//é usada no lugar da glFlush porque quando é feita a troca (ou swap) de buffers,
                      //é realizada implicitamente uma operação de flush
}

void atualizar(int valor)
{
	// Se quiser que a bola gire em loop no eixo Y ative o método abaixo
	/* 
	girar-=5.0f;
	if(girar>360.f) //condição para caso chegue no grau 360 ai comeca decremetar
    {
        girar-=360;
	}

	*/

	// Se quiser que a bola gire em loop no eixo X ative o método abaixo
	/*
    rotacionar+=2.0f;
    if(rotacionar>360.f) //condição para caso chegue no grau 360 ai comeca decremetar
    {
        rotacionar-=360;
    }
	*/
	

    glutPostRedisplay(); //re-exibe janela ou seja atualiza o frame
    glutTimerFunc(25,atualizar,0); //Pega a função 'callback' que deve ser chamada e o tempo que ela deve esperar antes de chamar a função
}


void keyPressed (unsigned char key, int x, int y) {  
keyStates[key] = true; // Muda o status da tecla para pressionada
}  
void keyUp (unsigned char key, int x, int y) {  
keyStates[key] = false; // Muda o status da tecla para nao-pressionada
}  

//Inicializa as funções necessarias para ver a aplicação
int main(int argc, char** argv)
{
	glutInit(&argc, argv); //inicializa a biblioteca GLUT
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); //avisa GLUT para utilizar dois buffers no desenho de cenas: um principal e outro auxiliar
	glutInitWindowSize(400,400); //definar tamanho da janela
	
	//INICIALIZA AS TECLAS COMO FALSO
	for (int j=0; j<256; j++)
	{
		keyStates[j]=  false;
	}

	// inicia o sistema de som e cria uma engine
	
	//engine->play2D("../Sounds/getout.ogg", true); // toca o som que esta localizado na pasta Sounds em loop

	glutCreateWindow("Field Goal"); //criar janela e nome
	renderizar();

	glClearColor(0.8,0.8,0.8,0.0); // COR DO CEU

    glutTimerFunc(25,atualizar,0); //Pega a função 'callback' que deve ser chamada e o tempo que ela deve esperar antes de chamar a função
	
	Camera.Move( F3dVector(0.0, 0.0, 13.0 ));
	Camera.MoveForwards( 1.0 );
	glutDisplayFunc(desenhar);
	glutKeyboardFunc(keyPressed);
	glutKeyboardUpFunc(keyUp);
	glutReshapeFunc(projetar);
	glShadeModel(GL_SMOOTH);	//GL_FLAT would look much worse
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);

	// Cria os tipos de luzes
	glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	 
    // Cria os tipos de materiais 
	glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
	
	glutMainLoop(); //repetição necessaria para parar quando houver uma chamada de saida
	engine->drop(); // deleta engine de som
	return 0;
}









