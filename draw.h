#ifndef DRAW_H
#define DRAW_H

void drawScene();
void specialKeys(int key, int x, int y);
void keys(unsigned char key, int x, int y);
float checkAst();
void reboot();

typedef struct spin {
	float axis[3];
	int rate;
} *Spin;

typedef struct props {
	int movDir;
	float pos[2];
	int dir;
	float velocity;
	Spin wonky;
	int ang;
} *Props;

Tardis getTardis();
void idle();


#endif
