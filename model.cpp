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

#include "model.h"


Model::Model(){
return;
}

void Model::modelInit(int num){
	srand(num);
	int w = glutGet(GLUT_WINDOW_WIDTH);
	int h = glutGet(GLUT_WINDOW_HEIGHT);
	
	float offset = (float)RAND_MAX*2;
	
	pos[0] = rand()%w - w/2;
	pos[1] = rand()%h - h/2;
	wrap();
	//printf("x %f y %f\n", pos[0], pos[1]);
	dir = rand()%360;
	movDir = dir;
	axis[0] = (float)rand()/RAND_MAX;
	axis[1] = (float)rand()/RAND_MAX;
	axis[2] = (float)rand()/RAND_MAX;
	rate = rand()%4;
	velocity = ((float)rand()/(offset));
	//printf("rate %d velocity %lf dir %d\n", rate, velocity, dir);
}

float *Model::norm(float one[], float two[], float three[]){
	float *ret = (float *)malloc(sizeof(float)*3);
	ret[0] = (two[1]-one[1])*(three[2]-two[2])-(two[2]-one[2])*(three[1]-two[1]);
	ret[1] = (two[2]-one[2])*(three[0]-two[0])-(two[0]-one[0])*(three[2]-two[2]);
	ret[2] = (two[0]-one[0])*(three[1]-two[1])-(two[1]-one[1])*(three[0]-two[0]);
	return ret;
}

void Model::update(){
	pos[0] += (velocity/4)*cos(movDir*0.017);
	pos[1] += (velocity/4)*sin(movDir*0.017);
	ang += rate;
	ang %= 360;
	
	wrap();
}

void Model::wrap(){
	int w = glutGet(GLUT_WINDOW_WIDTH);
	int h = glutGet(GLUT_WINDOW_HEIGHT);
	
	if(pos[0] > w/40){
		pos[0] = -w/40;
	}else if(pos[0] < -w/40){
		pos[0] = w/40;
	}
	
	if(pos[1] > h/40){
		pos[1] = -h/40;
	}else if(pos[1] < -h/40){
		pos[1] = h/40;
	}
}

void Model::applyTrans(){
	glTranslatef(pos[0], pos[1], 0);
    glRotatef(ang, axis[0], axis[1], 0);
}
