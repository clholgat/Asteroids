#ifndef MODEL_H
#define MODEL_H

class Model{
	public:
		Model();
		void wrap();
		void update();
		void draw();
		void applyTrans();
		void modelInit(int num);
		void init();
		float *norm(float one[], float two[], float three[]);
		float axis[3];
		int spinRate;
		int movDir;
		float pos[2];
		int dir;
		int rate;
		float velocity;
		int ang;
		float rad;
};

#endif
