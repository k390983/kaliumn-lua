//----------------------------------------------------------------------------//
// includes
//----------------------------------------------------------------------------//

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <math.h>
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

int initCanvas(const int WIDTH, const int HEIGHT, const int COLOR) {
	if(WIDTH <= 0) return(-1);
	if(HEIGHT <= 0) return(-1);
	if(checkColor(COLOR) == -1) return(-1);

	//Variable length array
	canvas = (int *)malloc((WIDTH * HEIGHT + 2) * sizeof(int));
	previousCanvas = (int *)malloc((WIDTH * HEIGHT) * sizeof(int));
	//The first element stores the width and the second the height
	canvas[0] = WIDTH;
	canvas[1] = HEIGHT;

	for(int i = 0; i < HEIGHT; ++i) {
		for(int j = 0; j < WIDTH; ++j) {
			canvas[i * WIDTH + j + 2] = COLOR;
			previousCanvas[i * WIDTH + j] = 0;

		}
	
	}

	return(1);
	
}

int cleanCanvas(const int COLOR) {
	if(checkColor(COLOR) == -1) return(-1);

	int width = canvas[0];
	int height = canvas[1];

	for(int i = 0; i < height; ++i) {
		for(int j = 0; j < width; ++j) {
			canvas[i * width + j + 2] = COLOR;

		}
	
	}

	return(1);

}

int display() {
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

	return(1);

}