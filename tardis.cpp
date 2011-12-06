#ifdef _M_IX86
#include <windows.h>
#else
#include <iostream>
#endif

#include "tardis.h"
#include "model.h"
#include "asteroid.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>
#include <math.h>


		
bool Tardis::checkShots(Asteroid *a){
	for(itS = shots.begin(); itS != shots.end(); itS++){
		if((pow((**itS).pos[0]-(*a).pos[0],2)+pow((**itS).pos[1]-(*a).pos[1],2)) <= pow((*a).rad,2)){
			delete *itS;
			shots.erase(itS);
			return true;
		}
	}
	return false;
}

bool Tardis::checkTardis(Asteroid *a){
	if((pow(pos[0]-(*a).pos[0], 2)+pow(pos[1]-(*a).pos[1],2)) <= pow(1.2+(*a).rad, 2)){
		//printf("%f\n", pow(pos[0]-(*a).pos[0], 2)+pow(pos[1]-(*a).pos[1],2));
		return true;
	}
	return false;
}
		
Tardis::Tardis(){
	texture = 0;
	return;
}

void Tardis::init(){
	tardisLen = 44;
	velocity = 0;
	movDir = 0;
	dir = 90;
	
	float rect[44][3] = {
	{0.66, 1, 0.66},	//front
	{0.66, -1, 0.66},
	{-0.66, -1, 0.66},
	{-0.66, 1, 0.66},
	
	{-0.66, 1, -0.66},	//back
	{-0.66, -1, -0.66},
	{0.66, -1, -0.66},
	{0.66, 1, -0.66},
	
	{0.66, 1, -0.66},	//right
	{0.66, -1, -0.66},
	{0.66, -1, 0.66},
	{0.66, 1, 0.66},
	
	{-0.66, 1, 0.66},	//left
	{-0.66, -1, 0.66},
	{-0.66, -1, -0.66},
	{-0.66, 1, -0.66},
	
	{0.66, 1, 0.66},	//top
	{-0.66, 1, 0.66},
	{-0.66, 1, -0.66},
	{0.66, 1, -0.66},
	
	{0.66, -1, 0.66},	//bottom
	{0.66, -1, -0.66},
	{-0.66, -1, -0.66},
	{-0.66, -1, 0.66},
	
	{0.5, 1.2, 0.5}, 	//top thing (tt) front
	{0.5, 1, 0.5},
	{-0.5, 1, 0.5},
	{-0.5, 1.2, 0.5},
	
	{-0.5, 1.2, -0.5},	//tt back
	{-0.5, 1, -0.5},
	{0.5, 1, -0.5},
	{0.5, 1.2, -0.5},
	
	{0.5, 1.2, -0.5},	//tt left
	{0.5, 1, -0.5},
	{0.5, 1, 0.5},
	{0.5, 1.2, 0.5},
	
	{-0.5, 1.2, 0.5},	//tt left
	{-0.5, 1, 0.5},
	{-0.5, 1, -0.5},
	{-0.5, 1.2, -0.5},
	
	{0.5, 1.2, 0.5},	//tt top
	{-0.5, 1.2, 0.5},
	{-0.5, 1.2, -0.5},
	{0.5, 1.2, -0.5}
	};

	for(int i = 0; i < 44; i++){
		for(int j = 0; j < 3; j++){
			tardisRect[i][j] = rect[i][j];
		}
	}
	
	pos[0] = 0;
	pos[1] = 0;
	//printf("INIT\n");
}

void Tardis::drawPointer(){
	glColor3f(0.2, 0.2, 0.2);
	glutSolidTorus(0.05, 1.2, 20, 20);
	rad = 1.2;
	glBegin(GL_TRIANGLES);
	glColor3f(1, 1, 0.2);
	glNormal3f(0, 0, 1);
	glVertex3f(-.5, 1.3, 0);
	glVertex3f(.5, 1.3, 0);
	glVertex3f(0, 2, 0);
	glEnd();
}

void Tardis::draw(){
	double coords[][2]={
		{0, 0},
		{0, 1},
		{1, 1},
		{1, 0}
	};

	glMatrixMode( GL_MODELVIEW );		// Setup model transformations

  	glPushMatrix();
  	glTranslatef(pos[0], pos[1], 0);
  	glRotatef(dir-90, 0, 0, 1);
  	drawPointer();
  	glPopMatrix();
  	
  	glPushMatrix();
  	glTranslatef(pos[0], pos[1], 0);
  	glRotatef(ang, axis[0], axis[1], 0);
    glScalef(0.9, 0.9, 0.9);
    
    
    int pointLen = tardisLen/4;
    for(int i = 0; i < pointLen; i++){
		if(texture){
			glBindTexture(GL_TEXTURE_2D, texture);
		}
    	glBegin( GL_QUADS );
    	glNormal3fv(norm(tardisRect[i*4], tardisRect[i*4+1], tardisRect[i*4+2]));
    	//glColor3f(0.129, 0.024, 0.392);
    	glColor3f(0, 0.234, 0.410);
    	for(int j = 0; j < 4; j++){
			glTexCoord2dv(coords[j]);
    		glVertex3fv(tardisRect[(i*4)+j]);
    	}
    	glEnd();
    }
    glPopMatrix();
    
    for(itS = shots.begin(); itS != shots.end(); itS++){
    	(**itS).draw();
    }
    
    
	glFlush();				// Flush OpenGL queue
}

void Tardis::shoot(){
	if(shots.size() == 4){
		return;
	}
	int vel = velocity;
	float movx = vel*cos(movDir*0.0175);
	float movy = vel*sin(movDir*0.0175);
	float xx = 2*cos(dir*0.0175);
	float yy = 2*sin(dir*0.0175);
	float xs = 3*cos(dir*0.0175);
	float ys = 3*sin(dir*0.0175);
	float mag = sqrt((xx+movx)*(xx+movx)+(yy+movy)*(yy+movy));
	float mags = sqrt((xs+movx)*(xs+movx)+(ys+movy)*(ys+movy));
	float normx = (movx+xx)/mag;
	float normy = (movy+yy)/mag;
	float normxs = (movx+xs)/mags;
	float normys = (movy+ys)/mags;
	float delta = 0.001;
	
	Shot *newShot = new Shot();
	(*newShot).modelInit(rand());
	(*newShot).init(this);
	(*newShot).dir = acos(normxs)*57.296, 
	(*newShot).velocity = mag*0.5;
	if(normxs < 0+delta && normys < 0+delta){
		(*newShot).dir = -(*newShot).dir;
	}else if(normxs > 0-delta && normys < 0+delta){
		(*newShot).dir = -(*newShot).dir;
	}
	(*newShot).movDir = (*newShot).dir;
	shots.push_back(newShot);
	return;

}

void Tardis::accelerate(){
	int vel = velocity;
	float movx = vel*cos((movDir)*0.0175);
	float movy = vel*sin((movDir)*0.0175);
	float xx = 3*cos(dir*0.0175);
	float yy = 3*sin(dir*0.0175);
	//float xs = 3*cos(dir*0.0175);
	//float ys = 3*sin(dir*0.0175);
	float mag = sqrt((xx+movx)*(xx+movx)+(yy+movy)*(yy+movy));
	//float mags = sqrt((xs+movx)*(xs+movx)+(ys+movy)*(ys+movy));
	float normx = (movx+xx)/mag;
	float normy = (movy+yy)/mag;
	//float normxs = (movx+xs)/mags;
	//float normys = (movy+ys)/mags;
	float delta = 0.001;
	
	movDir = acos(normx)*57.296;
	if(normx < 0+delta && normy < 0+delta){
		movDir = -movDir;
	}else if(normx > 0-delta && normy < 0+delta){
		movDir = -movDir;
	}
	velocity = mag;
	if(velocity > 10){
		velocity = 10;
	}
}

void Tardis::right(){
	dir -= 15;
	dir += 360;
	dir %= 360;
}

void Tardis::left(){
	dir += 15;
	dir %= 360;
}

void Tardis::update(){
	pos[0] += (velocity/4)*cos(movDir*0.017);
	pos[1] += (velocity/4)*sin(movDir*0.017);
	//printf("%f %f\n", pos[0], pos[1]);
	float friction = 1;
	if(velocity >= friction){
		velocity -= friction;
	}
	if(velocity < friction){
		velocity = 0;
	}
	ang += rate;
	ang %= 360;
	
	for(itS = shots.begin(); itS != shots.end(); itS++){
		(**itS).update();
	}
	
	wrap();

}
	
