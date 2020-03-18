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

#include "etc.h"

#include "interface.h"
#include "draw.h"
#include "canvas.h"

//----------------------------------------------------------------------------//
// global variables
//----------------------------------------------------------------------------//

int *canvas;
int *previousCanvas;

int hideCursor() {
	printf("\e[?25l");
	return(1);

}

int showCursor() {
	printf("\e[?25h");
	return(1);

}

int clearScreen() {
	printf("\033[2J");
	return(1);

}

int moveCursor(const int X, const int Y) {
	printf("\033[%d;%dH",Y + 1, X + 1);

	return(1);

}

int setTitle(const char TITLE[]) {
	printf("\033]0;%s\a", TITLE);

	return(1);

}

