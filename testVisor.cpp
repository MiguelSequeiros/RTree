#include <string>
#include "NumParser.h"
#include "RandGenerator.h"
#include "Mbr.h"
#include <GL/glut.h>
#include <iostream>
#include <math.h>

double rotate_x = 0;
double rotate_y = 0;
double rotate_z = 0;
Mbr** g_setOfMbr;
int g_min;
int g_max;

void specialKeys( int key, int x, int y ) 
{
   //  La flecha derecha: incrementa su rotación en 5 grados
   if (key == GLUT_KEY_RIGHT)
     rotate_y += 2;
    
   //  La flecha izquierda: disminuye su rotación en 5 grados
   else if (key == GLUT_KEY_LEFT)
     rotate_y -= 2;
    
   else if (key == GLUT_KEY_UP)
     rotate_x += 2;
    
   else if (key == GLUT_KEY_DOWN)
     rotate_x -= 2;
    
   else if (key == GLUT_KEY_PAGE_UP)
     rotate_z += 2;
    
   else if (key == GLUT_KEY_PAGE_DOWN)
     rotate_z -= 2;
   //  Solicitud para actualizar la pantalla
   glutPostRedisplay();
}

void init(void) 
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   //glClearDepth (0.0);
}

void reshape(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho( g_min , g_max , g_min , g_max , g_min , g_max );
    glMatrixMode(GL_MODELVIEW);

    glEnable(GL_DEPTH_TEST);
}

void display(void)
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //   glColor3f(1.0,1.0,1.0);
    glLoadIdentity();

    glRotatef(rotate_x,1,0,0);
    glRotatef(rotate_y,0,1,0);
    glRotatef(rotate_z,0,0,1);

    glBegin(GL_LINES);

        glColor3f(1,0,0);
            glVertex3f(0,0,0);
            glVertex3f(g_max,0,0);

        glColor3f(0,1,0);
            glVertex3f(0,0,0);
            glVertex3f(0,g_max,0);

        glColor3f(0,0,1);
            glVertex3f(0,0,0);
            glVertex3f(0,0,g_max);

/* Desde aqui tendria que ir en la void Mbr::DrawMbrLines */

        glColor3f( 1 , 1 , 1 );

        for( int i=0 ; i<100 ; i++ )
        	g_setOfMbr[i]->drawMbrLines(true);

/* Hasta aqui */
    glEnd();
    glFlush();
/* Desde aqui va la funcion Mbr::draw */
    for( int i=0 ; i<100 ; i++ )
       	g_setOfMbr[i]->draw(true);
}



int main( int argc , char** argv )
{
	int min = 1;
	int max = 1000;
	int N = 3;
  std::string fileName = "RandTest.txt";

	RandGenerator rg1( min , max , 1000 , fileName , N );
	rg1.writeOnAFile();

	NumParser np1( N , fileName );

	Mbr** setOfMbr = np1.readFromFile();

	g_setOfMbr = setOfMbr;
	g_min = -1500;
	g_max = 1500;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB );//| GLUT_DEPTH);

	glutInitWindowPosition (100, 100);
	glutInitWindowSize (500 , 500); 
	glutCreateWindow (" VISOR::RTREE:");

	init ();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutSpecialFunc(specialKeys);
	glutMainLoop();

	return 0;
};