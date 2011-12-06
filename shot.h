#ifndef SHOT_H
#define SHOT_H

#include "model.h"
#include "asteroid.h"
#include <list>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

using namespace std;

class Shot: public Model{
	public:
		Shot();
		void draw();
		void drawSpecial();
		void init(Model *model);
		void init(Model *model, int motionDir);
		float color[3];
};

#endif
