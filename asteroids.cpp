#ifdef _M_IX86
#include <windows.h>
#else
#include <iostream>
#endif

#include <string.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>

#include "tardis.h"
#include "asteroid.h"
#include "dalek.h"
#include "asteroids.h"
#include "draw.h"
	
	
void handle_menu(int ID){
	switch(ID){
		case 0:
			exit(0);
		case 1:
			reboot();
	}
}

void reshape(int h, int w){
	glMatrixMode(GL_PROJECTION);     // Select The Projection Matrix
	glLoadIdentity();                // Reset The Projection Matrix
	// Calculate The Aspect Ratio And Set The Clipping Volume
	if (h == 0) h = 1;
	glOrtho(-w/40, w/40, -h/40, h/40, -10, 10);
	glMatrixMode( GL_MODELVIEW );		// Setup model transformations
 	glLoadIdentity();
  	gluLookAt( 0, 0, 5, 0, 0, -1, 0, 1, 0 );             // Reset The Modelview Matrix
}

int main(int argc, char *argv[]){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    
    int width = 600;
    int height = 600;
    glutInitWindowSize(width, height);
    glutCreateWindow("Asteroids");
    glutDisplayFunc( drawScene );  	
    
    glMatrixMode( GL_MODELVIEW );		// Setup model transformations
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glEnable( GL_CULL_FACE );
	glCullFace( GL_BACK );
	glShadeModel( GL_FLAT );		// Flat shading
  	glEnable( GL_NORMALIZE );		// Normalize normals
  	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_TEXTURE_2D);

  	glClearDepth( 1.0 );			// Setup background colour
  	glClearColor( 0, 0, 0, 0 );
  	glEnable( GL_DEPTH_TEST );
	
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-width/40, width/40, -height/40, height/40, -5, 5);
    
    glMatrixMode( GL_MODELVIEW );		// Setup model transformations
 	glLoadIdentity();
  	gluLookAt( 0, 0, 5, 0, 0, -1, 0, 1, 0 );
	
    glutCreateMenu(handle_menu);
    glutAddMenuEntry("Quit", 0);
	glutAddMenuEntry("New Game", 1);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    glutReshapeFunc(reshape);
    glutSpecialFunc(specialKeys);
    glutKeyboardFunc(keys);
    glutIdleFunc(idle);
    glutMainLoop();			// Enter GLUT main loop
	return 1;
}
    
