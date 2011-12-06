
#ifndef DALEK_H
#define DALEK_H

#include "model.h"
#include "shot.h"
#include "tardis.h"


class Dalek: public Model{
	public:
		Dalek();
		void draw();
		void init(double scaleFactor);
		void modelInit(int num);
		bool update();
		double scale;
		bool initialized;
		list<Shot*> shots;
		list<Shot*>::iterator itS;
		void shoot(Tardis *tardis, list<Asteroid*> *asteroids);
		bool checkShots(Model *model);
		bool checkDalek(Model *model);
	private:
		void drawBase(float base[][3], float y);
		void drawSides();
		void drawHead();
		void drawArms();
		void drawSph(float bot[][3], float top[][3]);
		int mod(int num, int mod);
		void drawThing(float len, float scale);
		float dalekBase [9][3];
		float dalekTop [9][3];
		float yAng;
		float corners[4][2];
		float current[4][2];
		float head[2];
		float currentHead[2];
};

#endif
