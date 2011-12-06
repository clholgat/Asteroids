#ifdef _M_IX86
#include <windows.h>
#else
#include <iostream>
#endif

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#include "explosion.h"

Explosion::Explosion(double x, double y){
	Model::modelInit(rand());
	centerX = x;
	centerY = y;
	life = 0;
}

void Explosion::draw(){
	glMatrixMode( GL_MODELVIEW );		// Setup model transformations
    glPushMatrix();
    
    glTranslatef(centerX, centerY, 0);
    for(int i = 0; i < 360; i += 20){
    	glRotatef(20, axis[0], axis[1], axis[2]);
    	glPointSize(1);
		glBegin(GL_POINTS);
		glColor3f(1, 1, 1);
		glNormal3f(0, 0, 1);
		for(double j = 0; j < life; j += 1){
			glVertex3f(j, 0, 0);
		}
		glEnd();
	}
	glPopMatrix();
}

bool Explosion::update(){
	life += 0.5;
	if(life > 5){
		return false;
	}else{
		return true;
	}
}
