#ifdef _M_IX86
#include <windows.h>
#else
#include <iostream>
#endif

#include "bitmap.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>


GLuint loadTexture(char *file) {
        Bitmap *image; 
		GLuint texture = 1;
        image=new Bitmap();

        if(image==NULL) {
				//printf("returning NUL\n");
                return 0;
        }

        if (image->loadBMP(file)) {
				//printf("here\n");
                glGenTextures(1, &texture); 
				glBindTexture(GL_TEXTURE_2D, texture);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
                glTexImage2D(GL_TEXTURE_2D, 0, 3, image->width, image->height, 0, 
                                        GL_RGB, GL_UNSIGNED_BYTE, image->pixelData);
				printf("Returning %d\n", (int)texture);
				return texture;
        }
        else {
                return 0;
        }
}

Bitmap::Bitmap(){
	return;
}

Bitmap::~Bitmap(){
	return;
}

bool Bitmap::loadBMP(char *file) {
        //file handle used in all file operations
    FILE *in;

    //open the file for reading in binary mode    
    fopen_s(&in, file,"rb");
	if(!in){
		return false;
	}
	fread(&bmfh,sizeof(BITMAPFILEHEADER),1,in);
	fread(&bmih,sizeof(BITMAPINFOHEADER),1,in);
	int numColours=1 << bmih.biBitCount;

	//load the palette for 8 bits per pixel
	if(bmih.biBitCount == 8) {
		colours=new RGBQUAD[numColours];
		fread(colours,sizeof(RGBQUAD),numColours,in);
	}

	DWORD size=bmfh.bfSize-bmfh.bfOffBits;
	BYTE *tempPixelData=new BYTE[size];

	if(tempPixelData==NULL) {
		fclose(in);
		return false;
	}
	fread(tempPixelData,sizeof(BYTE),size,in);

	//byteWidth is the width of the actual image in bytes
	//padWidth is the width of the image plus the extra padding
	LONG byteWidth,padWidth;

	//initially set both to the width of the image
	width = bmih.biWidth;
	byteWidth=padWidth=(LONG)((float)width*(float)bpp/8.0);

	//add any extra space to bring each line to a DWORD boundary
	while(padWidth%4!=0) {
	padWidth++;
	}

	DWORD diff;
int offset;


height=bmih.biHeight;
//set diff to the actual image size(no padding)
diff=height*byteWidth;
//allocate memory for the image
pixelData=new BYTE[diff];
if(pixelData==NULL) {
    fclose(in);
    return false;
}
//bitmap is inverted, so the padding needs to be removed
//and the image reversed
//Here you can start from the back of the file or the front,
//after the header.  The only problem is that some programs
//will pad not only the data, but also the file size to
//be divisible by 4 bytes.
if(height>0) {
    int j= size-3;
    offset=padWidth-byteWidth;
    for(int i=0 ; i < (int)size; i+=3) {
        if((i+1)%padWidth==0) {
            i+=offset;
		}

        *(pixelData+j+2)=*(tempPixelData+i);
        *(pixelData+j+1)=*(tempPixelData+i+1);
        *(pixelData+j)=*(tempPixelData+i+2);
        j++;
    }
}
//the image is not reversed.  Only the padding needs to be removed.
else {
    height=height*-1;
    offset=0;
    do {
        memcpy((pixelData+(offset*byteWidth)),
               (tempPixelData+(offset*padWidth)),
                byteWidth);
        offset++;
    } while(offset<height);
}
//printf("returning\n");
return true;

}




