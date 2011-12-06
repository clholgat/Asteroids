#ifndef _BITMAP_H
#define _BITMAP_H

#ifdef _M_IX86
#include <windows.h>
#else
#include <iostream>
#endif

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <bmp.h>

GLuint loadTexture(char *file);

class Bitmap {
	public:
		//variables
	    RGBQUAD *colours;
		BYTE *pixelData;
		bool loaded;
		LONG width,height;
		WORD bpp;
		//methods
		Bitmap(void);
		Bitmap(char *);
		~Bitmap();
		bool loadBMP(char *);
	private:
		//variables
		BITMAPFILEHEADER bmfh;
		BITMAPINFOHEADER bmih;
		//methods
		void reset(void);
};

#endif //_BITMAP_H
