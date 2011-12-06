#ifdef _M_IX86
#include <windows.h>
#else
#include <iostream>
#endif

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include "dalek.h"
#include "asteroids.h"
#include "shot.h"
#include "tardis.h"

#define BOT -1
#define TOP 0.8

Dalek::Dalek(){
	initialized = false;
	return;
}

void Dalek::init(double scaleFactor){
	initialized = true;
	scale = scaleFactor;

	corners[0][0] = -0.4*scale;
	corners[0][1] = TOP*scale;
	corners[1][0] = 0.4*scale;
	corners[1][1] = TOP*scale;
	corners[2][0] = 1*scale;
	corners[2][1] = BOT*scale;
	corners[3][0] = -1*scale;
	corners[3][1] = BOT*scale;
	float ANG = (((velocity/4)*cos(movDir*0.017))<= 0)?(-yAng):yAng;
	for(int i  = 0; i < 4; i++){
		float x = corners[i][0];
		float y = corners[i][1];
		corners[i][0] = x*cos(ANG*0.017)-y*sin(ANG*0.017);
		corners[i][1] = x*sin(ANG*0.017)+y*cos(ANG*0.017);
	}

	head[0] = 0;
	head[1] = TOP+0.1;

	float Base[9][3] = {
		{-0.6, BOT, 1},
		{-.8, BOT, 0.7},
		{-.8, BOT, 0},
		{-0.6, BOT, -0.6},
		{0, BOT, -1},
		{0.6, BOT, -0.6},
		{.8, BOT, -0},
		{.8, BOT, 0.7},
		{.6, BOT, 1},
	};
	
	float Top[9][3] =  {
		{0.6, TOP, 1},
		{.8, TOP, 0.7},
		{.8, TOP, 0},
		{0.6, TOP, -0.6},
		{0, TOP, -1},
		{-0.6, TOP, -0.6},
		{-.8, TOP, -0},
		{-.8, TOP, 0.7},
		{-.6, TOP, 1},
	};

	for(int i = 0; i < 9; i++){
		for(int j = 0; j < 3; j++){
			dalekTop[i][j] = Top[i][j];
			dalekBase[i][j] = Base[i][j];
		}
	}
	
	float scalef = .4;
	for(int i = 0; i < 9; i++){
		for(int j = 0; j < 3; j += 2){
			dalekTop[i][j] = dalekTop[i][j]*scalef;
		}
	}
	//printf("Inital pos %f %f\n", pos[0], pos[1]);
}

void Dalek::drawBase(float base[][3], float y){
	float pole[3] = {0, y, 0};
	for(int i = 0; i < 9; i++){
		glBegin(GL_TRIANGLES);
		glNormal3fv(norm(base[i], base[(i+1)%9], pole));
		glColor3f(1, 0, 0);
		glVertex3fv(base[i]);
		glVertex3fv(base[(i+1)%9]);
		glVertex3fv(pole);
		glEnd();
	}
}

int Dalek::mod(int num, int mod){
	return (num + mod)%mod;
}

void Dalek::drawSides(){
	int j = 0;
	for(int i = 0; i < 9; i++){
		j = 8-i;
		glBegin(GL_QUADS);
		glColor3f(1, 0 , 0);
		glNormal3fv(norm(dalekBase[mod(j-1, 9)], dalekTop[mod(i+1,9)], dalekTop[i]));
		glVertex3fv(dalekBase[j]);
		glVertex3fv(dalekBase[mod(j-1, 9)]);
		glVertex3fv(dalekTop[mod(i+1,9)]);
		glVertex3fv(dalekTop[i]);
		glEnd();
	}
}
		
void Dalek::drawSph(float top[][3], float bot[][3]){
	float cent[3] = {-0.8, -0.5, -0.2};
	double midTop[2];
	double midBot[2];
	double slopeX = 0;
	double slopeZ = 0;
	double x = 0;
	double z = 0;
	int j = 0;
	for(int i = 0; i < 9; i++){
		j = 8-i;
		midTop[0] = (top[i][0] + top[mod(i+1,9)][0])/2; 
		midTop[1] = (top[i][2] + top[mod(i+1,9)][2])/2;
		midBot[0] = (bot[j][0] + bot[mod(j-1,9)][0])/2;
		midBot[1] = (bot[j][2] + bot[mod(j-1,9)][2])/2;
		slopeX = (TOP-BOT)/(midTop[0]-midBot[0]);
		slopeZ = (TOP-BOT)/(midTop[1]-midBot[1]);
		
		for(int k = 0; k < 3; k++){
			x = midBot[0] + (1+cent[k])*(1/slopeX);
			z = midBot[1] + (1+cent[k])*(1/slopeZ);
			
			glTranslatef(x, cent[k], z);
			glutSolidSphere(0.1, 10, 10);
			glTranslatef(-x, -cent[k], -z);
		}
	}
}

void Dalek::drawHead(){
	glPushMatrix();
	glTranslatef(0, TOP+0.1, 0.05);
	glutSolidSphere(0.52, 10, 10);
	//glTranslatef(0, -(TOP+0.1), -0.05);
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(0, TOP-0.3, 0.05);
	glRotatef(90, 1, 0, 0);
	glutSolidTorus(0.02, 0.6, 10, 10);
	glTranslatef(0, 0, 0.25);
	glutSolidTorus(0.02, 0.69, 10, 10);
	glPopMatrix();
	
}

void Dalek::drawThing(float len, float scale){
	float armBot[][3] = {
		{1, 0, 0.5},
		{0.5, 0, 1},
		{-0.5, 0, 1},
		{-1, 0, 0.5},
		{-1, 0, -0.5},
		{-0.5, 0, -1},
		{0.5, 0, -1},
		{1, 0, -0.5},
	};
	
	float armTop[8][3] = {
		{1, len, -0.5},
		{0.5, len, -1},
		{-0.5, len, -1},
		{-1, len, -0.5},
		{-1, len, 0.5},
		{-0.5, len, 1},
		{0.5, len, 1},
		{1, len, 0.5},
	};
	
	for(int i = 0; i < 8; i++){
		for(int j = 0; j < 3; j += 2){
			armTop[i][j] *= scale;
			armBot[i][j] *= scale;
		}
	}
	
	int j = 0;
	for(int i = 0; i < 8; i++){
		j = 7-i;
		glBegin(GL_QUADS);
		glNormal3fv(norm(armBot[j], armBot[mod(j-1,8)], armTop[mod(i+1,8)]));
		glColor3f(1, 0 , 0);
		glVertex3fv(armBot[j]);
		glVertex3fv(armBot[mod(j-1,8)]);
		glVertex3fv(armTop[mod(i+1,8)]);
		glVertex3fv(armTop[i]);
		glEnd();
	}
	
	float top[3] = {0, len, 0};
	float bot[3] = {0, 0, 0};
	
	for(int i = 0; i < 8; i++){
		glBegin(GL_TRIANGLES);
		glNormal3fv(norm(armBot[i], armBot[mod(i+1,8)], bot));
		glColor3f(1, 0, 0);
		glVertex3fv(armBot[i]);
		glVertex3fv(armBot[mod(i+1,8)]);
		glVertex3fv(bot);
		glEnd();
		
		glBegin(GL_TRIANGLES);
		glNormal3fv(norm(armTop[i], armTop[mod(i+1,8)], top));
		glVertex3fv(armTop[i]);
		glVertex3fv(armTop[mod(i+1,8)]);
		glVertex3fv(top);
		glEnd();
	}
	
}

void Dalek::drawArms(){
	glPushMatrix();
	glTranslatef(-0.2, 0.3, -0.2);
	glRotatef(110, -1, 1, 0);
	drawThing(1.2, 0.03);
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(0.2, 0.3, -0.2);
	glRotatef(130, -1, -1, 0);
	drawThing(1.2, 0.03);
	glTranslatef(0, 1.2, 0);
	glutSolidSphere(0.1, 10, 10);
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(0.3, 1.3, 0);
	glRotatef(30, 0, 0, -1);
	drawThing(0.15, 0.1);
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(-0.3, 1.3, 0);
	glRotatef(30, 0, 0, 1);
	drawThing(0.15, 0.1);
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(0, 0.8, -0.8);
	glRotatef(100, 1, 0, 0);
	drawThing(0.4, 0.03);
	glTranslatef(0, 0, 0);
	glutSolidSphere(0.1, 10, 10);
	glPopMatrix();
}


void Dalek::draw(){
	
	
	glMatrixMode(GL_MODELVIEW);
	glNormal3f(0, 0, 1);
	glPushMatrix();
	applyTrans();
	glRotatef(yAng, 1, 0, 0);
	glScaled(scale, scale, scale);

	drawBase(dalekBase, BOT);
	drawBase(dalekTop, TOP);
	drawSides();
	drawSph(dalekTop, dalekBase);
	drawHead();
	drawArms();
	
	glPopMatrix();
	if(!shots.empty()){
		Shot *shot = shots.back();
		(*shot).drawSpecial();
	}
	glFlush();
}

bool Dalek::update(){
	srand(time(NULL));
	int yVel = rand();
	if(yVel%2 == 1){
		yVel = 2;
	}else{
		yVel = 1;
	}

	pos[0] += (velocity/4)*cos(movDir*0.017);
	pos[1] += yVel*(velocity/4)*sin(movDir*0.017);
	ang += rate;
	ang %= 360;
	
	bool retval = true;
	float w = glutGet(GLUT_WINDOW_WIDTH);
	//printf("%d %d\n", pos[0], w/40);
	if(pos[0] > w/40 || pos[0] < -w/40){
		printf("HIT THE EDGE\n");
		initialized = false;
		retval = false;
		shots.clear();
	}

	for(int i = 0; i < 4; i++){
		current[i][0] = corners[i][0]+pos[0];
		current[i][1] = corners[i][1]+pos[1];
	}

	currentHead[0] = head[0]+pos[0];
	currentHead[1] = head[1]+pos[1];

	wrap();

	if(!shots.empty()){
		Shot *shot = shots.back();
		(*shot).update();
		//printf("SHOT LOCATION %f %f\n", (*shot).pos[0], (*shot).pos[1]);
	}
	return retval;

}

void Dalek::modelInit(int num){
	srand(num);
	int w = glutGet(GLUT_WINDOW_WIDTH);
	int h = glutGet(GLUT_WINDOW_HEIGHT);
	dir = rand()%360;

	movDir = dir;

	float offset = (float)RAND_MAX*2;
	velocity = ((float)rand()/(offset));
	while(velocity == 0){
		velocity = ((float)rand()/(offset));
	}
	
	double xV = (velocity/4)*cos(movDir*0.017);
	if(xV >= 0){
		pos[0] = -w/40;
		ang = -90;
	}else{
		pos[0] = w/40;
		ang = 90;
	}
	pos[1] = rand()%h - h/2;
	wrap();
	printf("Initial velocity x %f y %f \n", (velocity/4)*cos(movDir*0.017), (velocity/4)*sin(movDir*0.017));
	printf("Real INITIAL POS %f %f\n", pos[0], pos[1]);
	//printf("x %f y %f\n", pos[0], pos[1]);
	
	axis[0] = 0;
	axis[1] = 1;
	axis[2] = 0;
	rate = 0;
	yAng = (movDir > 90 && movDir < 270)?180-movDir:((movDir>=270)?movDir-360:movDir);
	
	//printf("DALEK VELOCITY %f\n", velocity);
	//printf("rate %d velocity %lf dir %d\n", rate, velocity, dir);
}

void Dalek::shoot(Tardis *tardis, list<Asteroid*> *asteroids){
	//printf("SHOOTING\n");
	srand(time(NULL));
	int ast, ship, random;
	if(scale == 2){
		ast = 3;
		ship = 2;
		random = 5;
	}else{
		ast = 4;
		ship = 5;
		random = 5;
	}

	Shot *newShot = new Shot();
	(*newShot).modelInit(rand());
	(*newShot).init(this);
	(*newShot).dir = 0, 
	(*newShot).velocity = 0.5;
	//printf("Shot stuff dir %d velocity %f\n", (*newShot).dir, (*newShot).velocity);
	int check = rand();
	while(check == 0){
		check = rand();
	}
	check = check%10;
	printf("check %d\n", check);
	Asteroid *target = (*asteroids).front();
	if(check < ast){
		printf("ast\n");
		(*newShot).dir = (int)(atan((pos[1]-(*target).pos[1])/(pos[0]-(*target).pos[0]))*57.2957795);
	}else if(check > ast+ship){
		printf("rand\n");
		(*newShot).dir = rand();
	}else{
		(*newShot).dir = (int)(atan((pos[1]-(*tardis).pos[1])/(pos[0]-(*tardis).pos[0]))*57.2957795);
		printf("ship\n");
	}
	//printf("TRIG %f %f\n", atan((pos[1]-(*target).pos[1])/(pos[0]-(*target).pos[0])), atan((pos[1]-(*target).pos[1])/(pos[0]-(*target).pos[0]))*57.295);
	(*newShot).movDir = (*newShot).dir;
	(*newShot).color[0] = 1;
	(*newShot).color[1] = 0;
	(*newShot).color[2] = 0;
	//printf("Shot stuff dir %d velocity %f\n", (*newShot).dir, (*newShot).velocity);
	shots.push_back(newShot);
}

bool Dalek::checkShots(Model *model){
	if(shots.empty()){
		return false;
	}
	Shot *shot = shots.front();
	if((pow((*shot).pos[0]-(*model).pos[0],2)+pow((*shot).pos[1]-(*model).pos[1],2)) <= pow((*model).rad,2)){
		shots.clear();
		return true;
	}
	return false;
}

bool Dalek::checkDalek(Model *model){
	if((pow(currentHead[0]-(*model).pos[0],2)+pow(currentHead[1]-(*model).pos[1],2)) <= pow((*model).rad+0.52*scale,2)){
		return true;
	}
	float dx, dy, d, D, delta, x1, y1, x2, y2, x, y;
	for(int i = 0; i < 4; i++){
		x1 = current[i][0];
		y1 = current[i][1];
		x2 = current[mod(i+1, 4)][0];
		y2 = current[mod(i+1, 4)][1];
		dx = x2-x1;
		dy = y2-y1;
		d = pow(dx,2)+pow(dy,2);
		D = (x1-(*model).pos[0])*(y2-(*model).pos[1])-(x2-(*model).pos[0])*(y2-(*model).pos[1]);
		delta = pow((*model).rad,2)*d-pow(D,2);
		if(delta < 0){
			return false;
		}else{
			int sgndy = (dy<0)?(-1):(1);
			int sgndx = (dx<0)?(-1):(1);
			x = (*model).pos[0]+(D*dy+sgndy*dx*sqrt(delta))/d;
			y = (*model).pos[1]+(-D*dx+sgndy*dy*sqrt(delta))/d;
			if(((x < x1 && x > x2) || (x > x1 && x < x2)) && ((y < y1 && y > y2) || (y > y1 && y < y2))){
				return true;
			}else{
				return false;
			}
		}
	}
	return false;
}




