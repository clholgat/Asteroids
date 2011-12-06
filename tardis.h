
#ifndef TARDIS_H
#define TARDIS_H

#include "model.h"
#include "asteroid.h"
#include "shot.h"
#include <list>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

using namespace std;


class Tardis: public Model{
	public:
		Tardis();
		void draw();
		void init();
		void shoot();
		void accelerate();
		void left();
		void right();
		void update();
		bool checkShots(Asteroid *a);
		bool checkTardis(Asteroid *a);
		list<Shot*> shots;
		list<Shot*>::iterator itS;
		GLuint texture;
	private:
		void drawPointer();
		float tardisRect[44][3];
		int tardisLen;
};

#endif


