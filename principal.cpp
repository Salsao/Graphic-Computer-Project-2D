#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include <cmath>
#include <stdio.h>
#include <iostream>
#include "leitor.h"
#include "tank.h"

using namespace std;

void display(void)
	{
		
		/* Limpar todos os pixels */
		glClear (GL_COLOR_BUFFER_BIT);
		

		glFlush ();
		for (int h = 0; h < vetorTank.size(); h++)
		{
		  desenhaTank(vetorTank[h]);
		  for (int i = 0; i < vetorRetangulo.size(); i++)
		  {
		    drawRectangulo(vetorRetangulo[i]);
		    if(vetorRetangulo[i].teletransporte)
		      for(int z = i + 1; z < vetorRetangulo.size(); z++)
			if(vetorRetangulo[z].teletransporte)
			  teletransporte(vetorRetangulo[i].altura, vetorRetangulo[i].largura, vetorRetangulo[i].x, vetorRetangulo[i].y, vetorRetangulo[z].x, vetorRetangulo[z].y, &(vetorTank[h]));
		  glFlush ();
		}
		  glFlush();   
		}
		glFlush ();
		glutSwapBuffers();
	}

void init (void)
	{
		/* selecionar cor de fundo (Branco) (RGB e transparencia) */
		glClearColor (1.0, 1.0, 1.0, 0.0);
		/* inicializar sistema de viz. */
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, widthPmg, heightPmg, 0, 100, -100);

	}

int main (int argc, char** argv) {
	openXml("./config.xml");
	openSvg(nomeArquivoSvg.c_str());
	openPgm(nomeArquivoPmg.c_str());
	openPgm(nomeArquivoPmg2.c_str());
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize (widthPmg, heightPmg);
	glutInitWindowPosition (0, 0);
	glutCreateWindow ("Trabalho Curto 4");
	glutDisplayFunc(display);
	glutIdleFunc(Idle);
	glutKeyboardFunc(Tecl);
	glutKeyboardUpFunc(keyUP);
	glutPassiveMotionFunc(moveCanhao);
	glutMouseFunc(shootClick);
	glutTimerFunc(25,update,0);
        glutTimerFunc(1/shootsRate,inimigoAtira,0); 
	init ();
	glutMainLoop();
	/* C ANSI requer que main retorne um
	inteiro */
	return 0;
}
