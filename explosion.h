#ifndef EXPLOSION_H
#define EXPLOSION_H

#include "model.h"

class Explosion: public Model{
	public:
		void draw();
		Explosion(double x, double y);
		bool update();
	private:
		double centerX;
		double centerY;
		double life;
};

#endif
