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

#include "draw.h"

#include "canvas.h"
#include "etc.h"
#include "interface.h"

//----------------------------------------------------------------------------//
// global variables
//----------------------------------------------------------------------------//

int *canvas;
int *previousCanvas;

void drawTexture(int x, int y, char path[]) {
	int width, height;
	FILE *fp = fopen(path, "r");

	fscanf(fp, "%d, %d", &width, &height);

	for(int i = 0; i < height; ++i) {
		for(int j = 0; j < width; ++j) {
			int color;

			fscanf(fp, "%d ", &color);

			if(color != 99) {
				drawPixel(x + j, y + i, color);

			}

		}

	}

	fclose(fp);

}

void drawPixel(int x, int y, int color) {
	canvas[y * canvas[0] + x + 2] = color;

}