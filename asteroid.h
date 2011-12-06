
#ifndef ASTEROID_H
#define ASTEROID_H

#include "model.h"

class Asteroid: public Model{
	public:
		void draw();
		Asteroid(float size);
		void init(int num);
	private:
		void drawPole(float pole[3], float points[][3]);
		void setPos();
		float poles[6][3];
		float xpos[8][3];
		float ypos[8][3];
		float xneg[8][3];
		float yneg[8][3];
		float zpos[8][3];
		float zneg[8][3];
		
};

#endif
