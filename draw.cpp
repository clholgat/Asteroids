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
#include <stdlib.h>
#include <stdio.h>
#include <list>
#include <time.h>

#include "tardis.h"
#include "asteroid.h"
#include "dalek.h"
#include "draw.h"
#include "asteroids.h"
#include "explosion.h"
#include "shot.h"

using namespace std;

int w;
int h;
double timer = 0;
bool firstT = true;
Tardis tardis;
Dalek dalek;
bool bigDalek = false;
bool littleDalek = false;
bool doneBig = false;
bool doneLittle = false;
bool firstRun = true;
list<Asteroid*> asteroids;
list<Asteroid*>::iterator itA;
list<Explosion*> explosions;
list<Explosion*>::iterator itE;
int lives = 2;
int score = 0;
int extraLives = 0;
bool newGame = false;


void reboot(){
	bigDalek = false;
	littleDalek = false;
	doneBig = false;
	doneLittle = false;
	dalek.initialized = false;
	dalek.shots.clear();
	asteroids.clear();
	tardis.shots.clear();
	score = 0;
	lives = 2;
	firstT = true;
	firstRun = true;
	newGame = true;
}

void idle(){
	timer += 0.5;
	tardis.update();

	//printf("%d\n", lives);
	int oldExtra = extraLives;
	extraLives = (int)score/10000;
	if(extraLives != oldExtra){
		lives++;
	}

	float astTotal = checkAst();
	//printf("astTotal %f\n", astTotal);
	if(astTotal < 3 && !littleDalek && !bigDalek && !firstRun && !doneLittle){
		printf("little %f\n", astTotal);
		littleDalek = true;
		dalek.modelInit(time(NULL));
		dalek.init(1);
	}else if(astTotal < 6 && !littleDalek && !bigDalek && !firstRun && !doneBig){

		printf("big %f\n", astTotal);
		bigDalek = true;
		dalek.modelInit(time(NULL));
		dalek.init(2);
	}
	
	if((bigDalek || littleDalek) && dalek.shots.empty() && !asteroids.empty()){
		//printf("SHOOTING\n");
		dalek.shoot(&tardis, &asteroids);
	}


	if(bigDalek){
		bigDalek = dalek.update();
		if(!bigDalek){
			doneBig = true;
		}
	}else if(littleDalek){
		littleDalek = dalek.update();
		if(!littleDalek){
			doneLittle = true;
		}
	}

	list<Explosion*> newEx;
	for(itE = explosions.begin(); itE != explosions.end(); itE++){
		Explosion *it = *itE;
		if((*it).update()){
			newEx.push_back(it);
		}
	}
	explosions.clear();
	explosions = newEx;

	
	if(lives < 0){
		bigDalek = false;
		littleDalek = false;
		doneBig = false;
		doneLittle = false;
		dalek.shots.clear();
		asteroids.clear();
		tardis.shots.clear();
		score = 0;
		lives = 2;
		firstT = true;
	}

	glutPostRedisplay();
	firstRun = false;
}

float checkAst(){
	float astTotal = 0;
	for(itA = asteroids.begin(); itA != asteroids.end(); ++itA){
		(**itA).update();
	}
	list<Asteroid*> newAst;
	for(itA = asteroids.begin(); itA != asteroids.end(); ++itA){
		Asteroid *it = *itA;
		astTotal += (*it).rad;
		if(tardis.checkTardis((it))){
			if((**itA).rad <= 2.0/4.0){
				lives -= 1;
				astTotal -= (*it).rad;
			}else{
				lives -= 1;
				Asteroid *newA = new Asteroid((*it).rad/2);
				Asteroid *newB = new Asteroid((*it).rad/2);
				(*newA).modelInit(time(NULL));
				(*newA).init(time(NULL)+1);
				(*newB).modelInit(time(NULL)+2);
				(*newB).init(time(NULL)+3);
				(*newA).pos[0] = (*it).pos[0];
				(*newA).pos[1] = (*it).pos[1];
				(*newB).pos[0] = (*it).pos[0];
				(*newB).pos[1] = (*it).pos[1];
				newAst.push_back(newA);
				newAst.push_back(newB);
			}
			explosions.push_back(new Explosion((*it).pos[0], (*it).pos[1]));
			tardis.init();
			continue;
		}
		if(tardis.checkShots((it))){
			if((*it).rad <= 2.0/4.0){
				astTotal -= (*it).rad;
			}else{
				if((*it).rad == 2){
					score += 20;
				}else if((*it).rad == 1){
					score += 50;
				}else{
					score += 100;
				}
				Asteroid *newA = new Asteroid((*it).rad/2);
				Asteroid *newB = new Asteroid((*it).rad/2);
				(*newA).modelInit(time(NULL));
				(*newA).init(time(NULL)+1);
				(*newB).modelInit(time(NULL)+2);
				(*newB).init(time(NULL)+3);
				(*newA).pos[0] = (*it).pos[0];
				(*newA).pos[1] = (*it).pos[1];
				(*newB).pos[0] = (*it).pos[0];
				(*newB).pos[1] = (*it).pos[1];
				newAst.push_back(newA);
				newAst.push_back(newB);
			}
			explosions.push_back(new Explosion((*it).pos[0], (*it).pos[1]));
			continue;
		}
		if((littleDalek || bigDalek) && dalek.checkShots(it)){
			if((*it).rad <= 2.0/4.0){
				astTotal -= (*it).rad;
			}else{
				Asteroid *newA = new Asteroid((*it).rad/2);
				Asteroid *newB = new Asteroid((*it).rad/2);
				(*newA).modelInit(time(NULL));
				(*newA).init(time(NULL)+1);
				(*newB).modelInit(time(NULL)+2);
				(*newB).init(time(NULL)+3);
				(*newA).pos[0] = (*it).pos[0];
				(*newA).pos[1] = (*it).pos[1];
				(*newB).pos[0] = (*it).pos[0];
				(*newB).pos[1] = (*it).pos[1];
				newAst.push_back(newA);
				newAst.push_back(newB);
			}
			explosions.push_back(new Explosion((*it).pos[0], (*it).pos[1]));
			continue;
		}
		if((littleDalek || bigDalek) && dalek.checkDalek(it)){
			if((*it).rad <= 2.0/4.0){
				astTotal -= (*it).rad;
			}else{
				Asteroid *newA = new Asteroid((*it).rad/2);
				Asteroid *newB = new Asteroid((*it).rad/2);
				(*newA).modelInit(time(NULL));
				(*newA).init(time(NULL)+1);
				(*newB).modelInit(time(NULL)+2);
				(*newB).init(time(NULL)+3);
				(*newA).pos[0] = (*it).pos[0];
				(*newA).pos[1] = (*it).pos[1];
				(*newB).pos[0] = (*it).pos[0];
				(*newB).pos[1] = (*it).pos[1];
				newAst.push_back(newA);
				newAst.push_back(newB);
			}
			/*dalek.initialized = false;
			if(bigDalek){
				bigDalek = false;
				doneBig = true;
			}else{
				littleDalek = false;
				doneLittle = true;
			}*/
			explosions.push_back(new Explosion((*it).pos[0], (*it).pos[1]));
			continue;
		}
		newAst.push_back(it);
	}
	asteroids.clear();
	asteroids = newAst;

	list<Shot*> newShot;
	list<Shot*>::iterator itS;

	if((bigDalek || littleDalek) && dalek.checkDalek(&tardis)){
		printf("Checkig Tardis\n");
		lives--;
		tardis.init();
	}

	if(bigDalek || littleDalek){
	if(dalek.checkShots(&tardis)){
		lives--;
		tardis.init();	
	}

	for(itS = tardis.shots.begin(); itS != tardis.shots.end(); ++itS){
		Shot *it = *itS;
		if(dalek.checkDalek(it)){
			dalek.initialized = false;
			if(bigDalek){
				bigDalek = false;
				doneBig = true;
				score += 200;
			}else{
				littleDalek = false;
				doneLittle = true;
				score += 1000;
			}
			explosions.push_back(new Explosion((*it).pos[0], (*it).pos[1]));
			continue;
		}else{
			newShot.push_back(it);
		}
	}
	tardis.shots.clear();
	tardis.shots = newShot;
	}
	

	if(asteroids.empty()){
		firstT = true;
	}

	return astTotal;
}

void specialKeys(int key, int x, int y){
	float th = 0;
	switch(key){
		case GLUT_KEY_LEFT:
			tardis.left();
			//printf("LEFT\n");
			break;
		case GLUT_KEY_RIGHT:
			tardis.right();
			//printf("RIGHT\n");
			break;
		default:
			break;
	}
}

void keys(unsigned char key, int x, int y){
	switch(key){
		case 'x':	
			tardis.accelerate();
			//printf("ACC\n");
			break;
		case 'z':
			tardis.shoot();
			//printf("SHOOT\n");
			break;
		default:
			break;
	}
}

void drawScene(){
	if(!newGame){
		return;
	}
	glMatrixMode( GL_MODELVIEW );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
	glPushMatrix();
	glLoadIdentity();
	
	if(firstT){
		asteroids.clear();
		tardis.modelInit(1);
		tardis.init();
		firstT = false;
		doneBig = false;
		doneLittle = false;
		for(int i = 0; i < 4; i++){
			asteroids.push_back(new Asteroid(2));
		}
		int i = 0;
		for(itA = asteroids.begin(); itA != asteroids.end(); itA++){
			(**itA).modelInit(time(NULL)+i);
			(**itA).init(time(NULL)-i);
			i++;
		}

	}

	if(bigDalek || littleDalek){
		if(dalek.initialized){
			dalek.draw();
		}else{
			printf("\nRESETTING\n");
			bigDalek = littleDalek = false;
		}
	}

	
	float  amb[] = { 0, 0, 0, 1 };	// Ambient material property
	float  lt_amb[] = { .2, .2, .2, 1 };	// Ambient light property
  	float  lt_dif[] = { .8, .8, .8, 1 };	// Ambient light property
  	float  lt_pos[] = {0, .39392, .91914, 0};
  	float  lt_spc[] = { 0, 0, 0, 1 };	// Specular light property
  	
  	glEnable( GL_LIGHT0 );
  	glEnable( GL_LIGHTING );
	glLightModelfv( GL_LIGHT_MODEL_AMBIENT, amb );

  	glLightfv( GL_LIGHT0, GL_POSITION, lt_pos );
  	glLightfv( GL_LIGHT0, GL_AMBIENT, lt_amb );
  	glLightfv( GL_LIGHT0, GL_DIFFUSE, lt_dif );
  	glLightfv( GL_LIGHT0, GL_SPECULAR, lt_spc );
	
	glPopMatrix();
	
	tardis.draw();
	for(itA = asteroids.begin(); itA != asteroids.end(); itA++){
		(**itA).draw();
		
	}
	for(itE = explosions.begin(); itE != explosions.end(); itE++){
		(**itE).draw();
	}
	
	char *stringScore = (char *)malloc(sizeof(char)*256);
	//_itoa_s(score, stringScore, 256, 10);
	sprintf(stringScore, "%d", score);

	glMatrixMode( GL_MODELVIEW );
	glNormal3f(0, 0, 1);
	glColor3f(1, 1, 1);
	//glRasterPos2i(10, 10);
	glRasterPos3i(-glutGet(GLUT_WINDOW_WIDTH)/40+2, glutGet(GLUT_WINDOW_HEIGHT)/40-2, 3);
	for(int i = 0; i < (int)strlen(stringScore); i++){
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *(stringScore+i));
	}

	char *stringLives = (char *)malloc(sizeof(char)*256);
	//_itoa_s(lives, stringLives, 256, 10);
	sprintf(stringLives, "%d", lives);
	char *livesGreet = "Remaining Lives: ";
	glRasterPos3i(glutGet(GLUT_WINDOW_WIDTH)/40-10, glutGet(GLUT_WINDOW_HEIGHT)/40-2, 3);
	for(int i = 0; i < (int)strlen(livesGreet); i++){
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *(livesGreet+i));
	}
	for(int i = 0; i < (int)strlen(stringLives); i++){
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *(stringLives+i));
	}

	glFlush();				// Flush OpenGL queue
    glutSwapBuffers();
    //glutTimerFunc(35, drawScene, 0);
}
