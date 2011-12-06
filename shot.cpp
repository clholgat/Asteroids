#ifdef _M_IX86
#include <windows.h>
#else
#include <iostream>
#endif

#include "tardis.h"
#include "model.h"
#include "asteroid.h"
#include "shot.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>
#include <math.h>

Shot::Shot(){
	return;
}

void Shot::drawSpecial(){
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(pos[0], pos[1], 0);
	glutSolidSphere(0.3, 10, 10);
	glPopMatrix();
}

void Shot::draw(){
	glMatrixMode(GL_MODELVIEW);
	glNormal3f(0, 0, 1);
	glPushMatrix();
	glTranslatef(pos[0], pos[1], 0);
	glPointSize(10);
	glBegin(GL_POINTS);
	glColor3f(color[0], color[1], color[1]);
	glVertex3f(0, 0, 0);
	glEnd();
	glPopMatrix();
}

void Shot::init(Model *model){
	movDir = (*model).movDir;
	dir = (*model).dir;
	rad = 0.3;
	color[0] = 1;
	color[1] = 1;
	color[2] = 1;
	pos[0] = (*model).pos[0];
	pos[1] = (*model).pos[1];
	spinRate = 0;
	axis[0] = 0;
	axis[1] = 0;
	axis[2] = 1;
}

void Shot::init(Model *model, int motionDir){
	movDir = motionDir;
	dir = (*model).dir;
	pos[0] = (*model).pos[0];
	pos[1] = (*model).pos[1];
	spinRate = 0;
	axis[0] = 0;
	axis[1] = 0;
	axis[2] = 1;
}
