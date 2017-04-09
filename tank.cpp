#include "tank.h"  /* Include the header (not strictly necessary here) */


#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include <cmath>
#include <stdio.h>
#include <iostream>

#define PI 3.14159265
#define RAD_TO_DEG (180.0/M_PI)


using namespace std;

GLfloat disX = 0;
GLfloat disY = 0;
GLfloat mouseX2;
GLfloat mouseY2;
GLfloat angleCanhao2;
bool segue;

bool w = true;
bool s = true;

int idTank = 0;


int keyS[256];
GLfloat mouseX;
GLfloat mouseY;
int posTankJogador = 0;
    
vector <Tank> vetorTank;
Tank tanque;

Bullet* bolota;

void inicializaTank(GLfloat circle_x, GLfloat circle_y, float circle_R, float circle_raio)
{
  tanque.gX = circle_x - menorx;
  tanque.gY = circle_y - menory;
  tanque.tes = true;
  tanque.qwerty = 0;
  tanque.tt = 90;
  tanque.gg = 0;
  tanque.angleCanhao = 0;
  tanque.esteira = 0;
  tanque.ladoD = 0;
  tanque.ladoE = 0;
  tanque.contrarioD = false;
  tanque.contrarioE = false; 
  tanque.raio = circle_raio;
  tanque.qtdBullets = 0;
  tanque.height = circle_raio*0.83;
  tanque.width = circle_raio*0.66;
  tanque.alturaEsteira = circle_raio*1.16;
  tanque.larguraEsteira = circle_raio*0.33;
  tanque.raioBase = circle_raio*0.2;
  tanque.canhao = circle_raio*0.83;
  tanque.player = false;
  tanque.vivo = true;
  if (circle_R < 0.8)
  {
   posTankJogador = vetorTank.size();
   tanque.player = true;
   tanque.qwerty = 135;
  }
  tanque.id = idTank;
  idTank++;
  vetorTank.push_back(tanque);
}

void colisaoMouse()
{
  if (matrizDistance[int(vetorTank[posTankJogador].gY)][int(vetorTank[posTankJogador].gX)] < (vetorTank[posTankJogador].raio/2.1 ))
  {
    w = false;
    s = true;
  }
  else
   {
    w = true;
    s = true;
  }
}

bool colisaoTankTank(Tank* tanque)
{
  for(int j = 0; j < vetorTank.size(); j++)
  {
    if (tanque->id != vetorTank[j].id)
    {
      float dist = sqrt(pow((tanque->gX - vetorTank[j].gX), 2) + pow((tanque->gY - vetorTank[j].gY), 2));
      if(dist <= tanque->raio && vetorTank[j].vivo)
      {
	return true;
      }
      }
  }
  return false;
}

bool colisaoIA(Tank* tanque)
{
  if (matrizDistance[int(tanque->gY)][int(tanque->gX)] < (tanque->raio/2.1 ))
    return true;
  return false;
}

void IACPU(GLdouble timeDif)
{
  for(int i = 0; i < vetorTank.size(); i++)
  {
    if(!vetorTank[i].player)
    {
      if(colisaoIA(&vetorTank[i]) || colisaoTankTank(&vetorTank[i]))
      {
	vetorTank[i].gX -= cos(( vetorTank[i].angleCanhao)*PI/180) * timeDif*tankSpeed;
        vetorTank[i].gY -= sin(( vetorTank[i].angleCanhao)*PI/180) * timeDif*tankSpeed;
	vetorTank[i].angleCanhao += rand() % 360;
	  if (vetorTank[i].esteira > 5)
		vetorTank[i].esteira = 0;
	if (vetorTank[i].esteira < 0)
		vetorTank[i].esteira = 5;
	else
		vetorTank[i].esteira += 0.5;

      }
      else
      {
	vetorTank[i].gX += cos(( vetorTank[i].angleCanhao)*PI/180) * timeDif*tankSpeed;
        vetorTank[i].gY += sin(( vetorTank[i].angleCanhao)*PI/180) * timeDif*tankSpeed;
	if (vetorTank[i].esteira > 5)
		vetorTank[i].esteira = 0;
	if (vetorTank[i].esteira < 0)
		vetorTank[i].esteira = 5;
	else
		vetorTank[i].esteira += 0.5;
      }
    }
  }
}

void colisaoFrente(Tank* tanque)
{
  if (matrizDistance[int(tanque->gY)][int(tanque->gX)] < (tanque->raio/2.1 ))
  {
    w = false;
    s = true;
  }
  else
   {
    w = true;
    s = true;
  }
}

void colisaoAtras(Tank* tanque)
{
  if (matrizDistance[int(tanque->gY)][int(tanque->gX)] < (tanque->raio/2.1 ))
    {
    w = true;
    s = false;
  }
  else
  {
    w = true;
    s = true;
  }
}

void colisaoTiro(Bullet* bullet)
{
  if (bullet->posX < widthPmg && bullet->posX > 0 && bullet->posY < heightPmg && bullet->posY > 0)
    if (matrizDistance[int(bullet->posY)][int(bullet->posX)] < 9)
    {
      bullet->posicao = 1000;
      bullet->posX = 1000;
      bullet->posY = 1000;
    }
}

void teletransporte(float alturaR, float larguraR, float x, float y, float x2, float y2, Tank *tanque)
{
	float a = (y - menory) - ((y - menory) + alturaR);
	float b = 0;
	float c = (x - menorx + larguraR)*((y - menory) + alturaR) - (x - menorx + larguraR)*(y - menory);
	float eq = tanque->gX * a + tanque->gY * b + c;
	float distanciaPR = fabs(eq)/sqrt(a*a);

	if(distanciaPR <= tanque->raio && !(tanque->gY >= ((y - menory) + alturaR) || tanque->gY <= (y - menory)))
	{
		tanque->gX = widthPmg - tanque->gX + 50;
		return;
	}	
	
	a = (y2 - menory) - ((y2 - menory) + alturaR);
	b = 0;
	c = (x2 - menorx)*((y2 - menory) + alturaR) - (x2 - menorx)*(y2 - menory);
	eq = tanque->gX * a + tanque->gY * b + c;
	distanciaPR = fabs(eq)/sqrt(a*a);
	
	if(distanciaPR <= tanque->raio && !(tanque->gY >= ((y2 - menory) + alturaR) || tanque->gY <= (y2 - menory)))
	{
		tanque->gX = widthPmg - tanque->gX - 50;
		return;
	}
	
}


void moveInX(GLfloat dx, Tank *tanque){
  if(w)
  {
	tanque->gY += sin(( tanque->angleCanhao)*PI/180) * dx;
	tanque->gX += cos(( tanque->angleCanhao)*PI/180) * dx;
	colisaoFrente(tanque);
	if (tanque->esteira > 5)
		tanque->esteira = 0;
	if (tanque->esteira < 0)
		tanque->esteira = 5;
	else
		tanque->esteira += 0.5;
  }
}

void moveInY(GLfloat dy, Tank *tanque){
  if(s)
  {
	tanque->gY -= sin((tanque->angleCanhao)*PI/180) * dy;
	tanque->gX -= cos(( tanque->angleCanhao)*PI/180) * dy;
	colisaoAtras(tanque);
	if (tanque->esteira > 5)
		tanque->esteira = 0;
	if (tanque->esteira < 0)
		tanque->esteira = 5;
	else
		tanque->esteira += 0.5;
  }
}

void moveAngle(GLfloat gg, Tank *tanque){
  if(s && w)
  {
	tanque->angleCanhao -= gg;
	if (tanque->esteira > 5)
		tanque->esteira = 0;
	if (tanque->esteira < 0)
		tanque->esteira = 5;
	else
		tanque->esteira += 0.5;
  }
}


void Idle (){

	static GLdouble previousTime = 0;
	GLdouble currentTime;
	GLdouble timeDif;

	currentTime = glutGet(GLUT_ELAPSED_TIME);
	timeDif = currentTime - previousTime;
	previousTime = currentTime;
	IACPU(timeDif);
	if (keyS['s']){
		moveInY(1*timeDif*tankSpeed, &(vetorTank[posTankJogador]));
		vetorTank[posTankJogador].ladoD = 0;
		vetorTank[posTankJogador].ladoE = 0;
		vetorTank[posTankJogador].contrarioE = false;
		vetorTank[posTankJogador].contrarioD = false;
		if (keyS['d'])
			vetorTank[posTankJogador].ladoD = 2;
		if (keyS['a'])
			vetorTank[posTankJogador].ladoE = 2;
		}
	if (keyS['w']){
		moveInX(1*timeDif*tankSpeed, &(vetorTank[posTankJogador]));
		vetorTank[posTankJogador].ladoD = 0;
		vetorTank[posTankJogador].ladoE = 0;
		vetorTank[posTankJogador].contrarioE = true;
		vetorTank[posTankJogador].contrarioD = true;
		if (keyS['d'])
			vetorTank[posTankJogador].ladoE = 2;
		if (keyS['a'])
			vetorTank[posTankJogador].ladoD = 2;
		}
	if (keyS['d']){
		moveAngle(-1*timeDif*tankSpeed, &(vetorTank[posTankJogador]));
		if(!keyS['a'])
			vetorTank[posTankJogador].ladoE = 2;
		else
			vetorTank[posTankJogador].ladoD = 0;
		if(!keyS['a'] && !keyS['w'] && !keyS['s'])
		{
			vetorTank[posTankJogador].contrarioE = true;
			vetorTank[posTankJogador].contrarioD = false;
		}
		else
			vetorTank[posTankJogador].contrarioE = false;
		}
	if (keyS['a']){
		moveAngle(1*timeDif*tankSpeed, &(vetorTank[posTankJogador]));
		if(!keyS['d'])
			vetorTank[posTankJogador].ladoD = 2;
		else
			vetorTank[posTankJogador].ladoE = 0;
		if(!keyS['d'] && !keyS['w'] && !keyS['s'])
		{
			vetorTank[posTankJogador].contrarioD = true;
			vetorTank[posTankJogador].contrarioE = false;
		}
		else
			vetorTank[posTankJogador].contrarioD = false;
		}
	if (!keyS['d']){
		if (!keyS['w'])
			vetorTank[posTankJogador].ladoD = 0;
		}
	if (!keyS['a']){
		if (!keyS['w'])
			vetorTank[posTankJogador].ladoE = 0;
		}
	glutPostRedisplay();
}

void desenhaCanhao (Tank tanque)
{
	if(tanque.player)
		glColor3f(0.2333,0.4450,0.1333);
	else
		glColor3f(0.2333,0.4450,0.6333);
	glBegin(GL_QUADS);
	glVertex2f (-8/2.0, 0.0);
	glVertex2f (-8/2.0, tanque.canhao);
	glVertex2f (8/2.0, tanque.canhao);
	glVertex2f (8/2.0, 0.0);
	glEnd();
}

void desenhaBaseTank (Tank tanque)
{
	if(tanque.player)
		glColor3f(0.4196,0.5568,0.1372);
	else
		glColor3f(0.5196,0.7568,0.8372);
	glBegin(GL_QUADS);
	glVertex2f (-tanque.width/2.0, -tanque.height/2);
	glVertex2f (-tanque.width/2.0, tanque.height/2);
	glVertex2f (tanque.width/2.0, tanque.height/2);
	glVertex2f (tanque.width/2.0, -tanque.height/2);
	glEnd();
}

void desenhaBaseCanhao(Tank tanque) 
{
	float x = tanque.raioBase;
	float y = 0; 
	float theta = 2*3.1415/tanque.raioBase; 
	float cose = cos(theta);
	float seno = sin(theta);
	float trans;
	if(tanque.player)
		glColor3f(0.1333,0.5450,0.1333);
	else
		glColor3f(0.3333,0.2450,0.6333);
	glBegin(GL_POLYGON); 
	for(int i = 0; i < 1080; i++) 
	{ 
		glVertex2f(x, y);
		trans = x;
		x = cose * x - seno * y;
		y = seno * trans + cose * y;
	}
	glEnd();
}

void desenhaFormatoBullet(float raio, float R, float G, float B) 
{
	float x = raio;
	float y = 0; 
	float theta = 2*3.1415/raio; 
	float cose = cos(theta);
	float seno = sin(theta);
	float trans;
	glColor3f(R,G,B);
	glBegin(GL_POLYGON); 
	for(int i = 0; i < 1080; i++) 
	{ 
		glVertex2f(x, y);
		trans = x;
		x = cose * x - seno * y;
		y = seno * trans + cose * y;
	}
	glEnd();
}

void desenhaEsteira (Tank tanque, bool de)
{
	if(tanque.player)
		glColor3f(0.4196,0.5568,0.1372);
	else
		glColor3f(0.1196,0.3568,0.5372);
	glBegin(GL_QUADS);
	glVertex2f (-tanque.larguraEsteira/2.0, -tanque.alturaEsteira/2);
	glVertex2f (-tanque.larguraEsteira/2.0, tanque.alturaEsteira/2);
	glVertex2f (tanque.larguraEsteira/2.0, tanque.alturaEsteira/2);
	glVertex2f (tanque.larguraEsteira/2.0, -tanque.alturaEsteira/2);
	glEnd();
	glColor3f(0.0,0.0,0.0);
	glLineWidth(2.f);
	GLfloat j;
	
	
	//verifica se eh direita ou esquerda
	if (de)  //esquerda
		if (tanque.contrarioE){
			for (j = tanque.alturaEsteira - tanque.esteira; j > 0; j-= 6 - tanque.ladoE) {
				glBegin(GL_LINES);
				glVertex2f(-tanque.larguraEsteira/2, -tanque.alturaEsteira/2 + j);
				glVertex2f(tanque.larguraEsteira/2, -tanque.alturaEsteira/2 + j);
				glEnd();
			}
		}
		else{
			for (j = tanque.esteira; j < tanque.alturaEsteira; j+= 6 - tanque.ladoE) {
				glBegin(GL_LINES);
				glVertex2f(-tanque.larguraEsteira/2, -tanque.alturaEsteira/2 + j);
				glVertex2f(tanque.larguraEsteira/2, -tanque.alturaEsteira/2 + j);
				glEnd();
			}
		}
	
	else //direita
		if (tanque.contrarioD){
			for (j = tanque.alturaEsteira - tanque.esteira; j > 0; j-= 6 - tanque.ladoD) {
				glBegin(GL_LINES);
				glVertex2f(-tanque.larguraEsteira/2, -tanque.alturaEsteira/2 + j);
				glVertex2f(tanque.larguraEsteira/2, -tanque.alturaEsteira/2 + j);
				glEnd();
			}
		}
		else{
			for (j = tanque.esteira; j < tanque.alturaEsteira ; j+= 6 - tanque.ladoD) {
			glBegin(GL_LINES);
			glVertex2f(-tanque.larguraEsteira/2, -tanque.alturaEsteira/2 + j);
			glVertex2f(tanque.larguraEsteira/2, -tanque.alturaEsteira/2 + j);
			glEnd();
			}
		}
}

void apontaCanhao(Tank alvo, Tank *inimigo)
{
    inimigo->tt = 180 + (atan2(inimigo->gY - alvo.gY, inimigo->gX - alvo.gX)*RAD_TO_DEG);
}

void moveCanhao (int x, int y)
{
	vetorTank[posTankJogador].qwerty = 0;
	mouseX = x;
	mouseY = y;
	if (vetorTank[posTankJogador].gX < mouseX)
		vetorTank[posTankJogador].tt = atan(1.0*(vetorTank[posTankJogador].gY -  mouseY)/(vetorTank[posTankJogador].gX  - mouseX))*RAD_TO_DEG;
	else if (vetorTank[posTankJogador].gX == mouseX)
		if (vetorTank[posTankJogador].gY < mouseY)
		  vetorTank[posTankJogador].tt = 90;
		else
		  vetorTank[posTankJogador].tt = 270;
	else
		vetorTank[posTankJogador].tt = atan(1.0*(mouseY - vetorTank[posTankJogador].gY)/(mouseX - vetorTank[posTankJogador].gX))*RAD_TO_DEG + 180;

}

void updateCanhao(){
	    if (vetorTank[posTankJogador].gX < mouseX)
		    vetorTank[posTankJogador].tt = atan(1.0*(vetorTank[posTankJogador].gY -  mouseY)/(vetorTank[posTankJogador].gX  - mouseX))*RAD_TO_DEG;
	    else if (vetorTank[posTankJogador].gX == mouseX)
		    if (vetorTank[posTankJogador].gY < mouseY)
		      vetorTank[posTankJogador].tt = 90;
		    else
		      vetorTank[posTankJogador].tt = 270;
	    else
		    vetorTank[posTankJogador].tt = atan(1.0*(mouseY - vetorTank[posTankJogador].gY)/(mouseX - vetorTank[posTankJogador].gX))*RAD_TO_DEG + 180;

	  for (int i = 0; i < vetorTank.size(); i++)
	    if(!(vetorTank[i].player))
	      apontaCanhao(vetorTank[posTankJogador], &(vetorTank[i]));
}

void desenhaBullet(int x, int y, GLfloat tt, GLfloat bocaCanhao, Tank *tanque, int i){
 
	glPushMatrix();
	glTranslatef(x, y, 0);
        glRotatef(tt, 0,0,1);
        glTranslatef(0, bocaCanhao, 0);
        desenhaFormatoBullet(7,0.3,0.4,0.8);

    glPopMatrix();
}

void tiro(Tank *tanque)
{	
     int i;
     for(i = 0;i < tanque->bullets.size(); i++){

	  desenhaBullet(tanque->bullets[i]->x,tanque->bullets[i]->y, tanque->bullets[i]->angulo, tanque->bullets[i]->posicao, tanque, i);
          tanque->bullets[i]->posicao += shootSpeed;  
	  tanque->bullets[i]->posX -= sin(( tanque->bullets[i]->angulo)*PI/180) * shootSpeed;    
	  tanque->bullets[i]->posY += cos(( tanque->bullets[i]->angulo)*PI/180) * shootSpeed;
	  colisaoTiro(tanque->bullets[i]);

          for(int j = 0; j < vetorTank.size(); j++)
             if(tanque->gX != vetorTank[j].gX)
                  if(sqrt(pow((tanque->bullets[i]->posX - vetorTank[j].gX), 2) + pow((tanque->bullets[i]->posY - vetorTank[j].gY), 2)) <= vetorTank[j].raio*1.7 && vetorTank[j].vivo)
		  {

		    vetorTank[j].vivo = false;
		      tanque->bullets[i]->posicao = 1000;
		      tanque->bullets[i]->posX = 1000;
		      tanque->bullets[i]->posY = 1000;
		    
		  }

        }
}
     


void movemove()
{
	if(disX != 0 && disY != 0){
	  if(w){
	    colisaoMouse();
		if (segue){
			if (vetorTank[posTankJogador].gX < mouseX)
					vetorTank[posTankJogador].angleCanhao = atan(1.0*(vetorTank[posTankJogador].gY -  mouseY)/(vetorTank[posTankJogador].gX  - mouseX))*RAD_TO_DEG;
					else if (vetorTank[posTankJogador].gX == mouseX)
						if (vetorTank[posTankJogador].gY < mouseY)
							vetorTank[posTankJogador].angleCanhao = 90;
						else
							vetorTank[posTankJogador].angleCanhao = 270;
			else
				vetorTank[posTankJogador].angleCanhao = atan(1.0*(mouseY - vetorTank[posTankJogador].gY)/(mouseX - vetorTank[posTankJogador].gX))*RAD_TO_DEG + 180;
				}
		disX = mouseX2 - vetorTank[posTankJogador].gX;
		disY = mouseY2 - vetorTank[posTankJogador].gY;
		if (segue)
		{
			angleCanhao2 = vetorTank[posTankJogador].angleCanhao;
			segue = false;
		}
		
			if (sqrt(disX*disX + disY*disY) < 10)
			{
				vetorTank[posTankJogador].gX = mouseX2;
				vetorTank[posTankJogador].gY = mouseY2;
			}
			else
			{
				vetorTank[posTankJogador].gY += sin(( angleCanhao2)*PI/180) * 0.5;
				vetorTank[posTankJogador].gX += cos(( angleCanhao2)*PI/180) * 0.5;
				if (vetorTank[posTankJogador].esteira > 5)
					vetorTank[posTankJogador].esteira = 0;
				if (vetorTank[posTankJogador].esteira < 0)
					vetorTank[posTankJogador].esteira = 5;
				else
					vetorTank[posTankJogador].esteira += 0.5;
			}
		}
		else
		{
		  segue = false;
		  disX = 0;
		  disY = 0;
		}
	}
	else
		segue = true;
}

void update(int value)
{
	movemove();
	glutPostRedisplay();
	glutTimerFunc(10,update,0);
}

void inimigoAtira(int value)
{
     for(int i = 0; i < vetorTank.size(); i++)
     {
          if (i != posTankJogador)
          {
               bolota = (Bullet*)malloc(sizeof(Bullet*));
		     bolota->angulo = vetorTank[i].tt - 90;
		     bolota->x = vetorTank[i].gX;
		     bolota->y = vetorTank[i].gY;
		     bolota->posX = vetorTank[i].gX;
		     bolota->posY = vetorTank[i].gY;
		     bolota->posicao = 25;
               vetorTank[i].bullets.push_back(bolota);
               vetorTank[i].qtdBullets = (vetorTank[i].qtdBullets + 1)%400;
               glutPostRedisplay();
	          glutTimerFunc(1/shootsRate,inimigoAtira,0);
	     }
     }
}


void shootClick(int button, int state, int x, int y) {
	if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) 
	{
		if(disX == 0 && disY == 0)
		{

		  mouseX2 = x;
		  mouseY2 = y; 
		  if(w)
		  {
		  disX = mouseX2 - vetorTank[posTankJogador].gX;
		  disY = mouseY2 - vetorTank[posTankJogador].gY;
		  colisaoMouse();

		  }
		}

	}

	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		bolota = (Bullet*)malloc(sizeof(Bullet*));
		bolota->angulo = vetorTank[posTankJogador].tt - 90; 
		bolota->x = vetorTank[posTankJogador].gX;
		bolota->y = vetorTank[posTankJogador].gY;
		bolota->posX = vetorTank[posTankJogador].gX;
		bolota->posY = vetorTank[posTankJogador].gY;
		bolota->posicao = 25;
                vetorTank[posTankJogador].bullets.push_back(bolota);
                vetorTank[posTankJogador].qtdBullets = (vetorTank[posTankJogador].qtdBullets + 1)%400;
	}
}

void Tecl(unsigned char key, int x, int y)
{
	switch(key){
	case 'W':
	case 'w':
		keyS[(int)('w')] = 1;
	break;

	case 'S':
	case 's':
		keyS[(int)('s')] = 1;
	break;

	case 'A':
	case 'a':
		keyS[(int)('a')] = 1;
	break;

	case 'D':
	case 'd':
		keyS[(int)('d')] = 1;
	break;

	}
}

void killKey(){
	for(int j = 0; j < 256; j++)
		keyS[j] = 0;
}

void keyUP(unsigned char key, int x, int y){

	switch(key){
	case 'W':
	case 'w':
		keyS[(int)('w')] = 0;
	break;

	case 'S':
	case 's':
		keyS[(int)('s')] = 0;
	break;

	case 'A':
	case 'a':
		keyS[(int)('a')] = 0;
	break;

	case 'D':
	case 'd':
		keyS[(int)('d')] = 0;
	break;

	}
}

void desenhaTank(Tank tanque)
{
	if(tanque.vivo){
	  glPushMatrix();
	  tiro(&tanque);
	  glPopMatrix();
	  glPushMatrix();
	  glTranslatef(tanque.gX,tanque.gY,0);
	  glRotatef(90,0,0,1); 
	  glRotatef(tanque.angleCanhao,0,0,1);
	  desenhaBaseTank(tanque); 
	  glTranslatef(tanque.width/2 + tanque.larguraEsteira/2 ,0,0); 
	  desenhaEsteira(tanque, false); 
	  glTranslatef(-2*(tanque.width/2 + tanque.larguraEsteira/2) ,0,0); 
	  desenhaEsteira(tanque, true); 
	  glTranslatef(tanque.width/2 + tanque.larguraEsteira/2 ,0,0); 
	  desenhaBaseCanhao(tanque); 
	  glPopMatrix();
	  glPushMatrix();
	  glTranslatef(tanque.gX,tanque.gY,0);
	  glRotatef(tanque.tt - 90 ,0,0,1);
	  glRotatef(tanque.qwerty,0,0,1);
	  desenhaCanhao(tanque);
	  glPopMatrix();
	  updateCanhao();
	  glFlush ();
	  }
}
