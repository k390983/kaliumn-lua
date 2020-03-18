//----------------------------------------------------------------------------//
// includes
//----------------------------------------------------------------------------//

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

#include <sys/ioctl.h>
#include <unistd.h>
#include <stdlib.h>
#include <termios.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>

#include "canvas.h"

#include "interface.h"
#include "draw.h"
#include "etc.h"

//----------------------------------------------------------------------------//
// global variables
//----------------------------------------------------------------------------//

int *canvas;
int *previousCanvas;

void initCanvas(int width, int height, int color) {
	//Variable length array
	canvas = (int *)malloc((width * height + 2) * sizeof(int));
	previousCanvas = (int *)malloc((width * height) * sizeof(int));
	//The first element stores the width and the second the height
	canvas[0] = width;
	canvas[1] = height;

	for(int i = 0; i < height; ++i) {
		for(int j = 0; j < width; ++j) {
			canvas[i * width + j + 2] = color;
			previousCanvas[i * width + j] = 0;

		}
	
	}
	
}

void cleanCanvas(int color) {
	int width = canvas[0];
	int height = canvas[1];

	for(int i = 0; i < height; ++i) {
		for(int j = 0; j < width; ++j) {
			canvas[i * width + j + 2] = color;

		}
	
	}

}

void display() {
	int width = canvas[0];
	int height = canvas[1];
	
	moveCursor(0, 0);

	for(int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			if(canvas[(i * width) + j + 2] != previousCanvas[(i * width) + j]) {
				moveCursor(j * 2, i);
				setColor(colorCode("default"), canvas[(i * width) + j + 2]);
				printf("  ");

			}

		}

	}

}