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

#include "etc.h"

#include "interface.h"
#include "draw.h"
#include "canvas.h"

//----------------------------------------------------------------------------//
// global variables
//----------------------------------------------------------------------------//

int *canvas;
int *previousCanvas;

//----------------------------------------------------------------------------//
// colors
//----------------------------------------------------------------------------//

#define BLACK 30
#define RED 31
#define GREEN 32
#define YELLOW 33
#define BLUE 34
#define MAGENTA 35
#define CYAN 36
#define LIGHT_GRAY 37
#define DARK_GRAY 90
#define LIGHT_RED 91
#define LIGHT_GREEN 92
#define LIGHT_YELLOW 93
#define LIGHT_BLUE 94
#define LIGHT_MAGENTA 95
#define LIGHT_CYAN 96
#define WHITE 97

#define DEFAULT 39

#define TRANSPARENT 99 // only for textures

void hideCursor() {
	printf("\e[?25l");

}

void showCursor() {
	printf("\e[?25h");

}

void clearScreen() {
	printf("\033[2J");

}

void setColor(const int FOREGROUND, const int BACKGROUND) {
	printf("\e[%dm", FOREGROUND);
	printf("\e[%dm", BACKGROUND + 10);

}

void moveCursor(const int X, const int Y) {
	printf("\033[%d;%dH",Y + 1, X + 1);

}

void setTitle(const char TITLE[]) {
	printf("\033]0;%s\a", TITLE);

}

int colorCode(const char COLOR[]) {
	if (strcmp(COLOR, "black") == 0){
		return(BLACK);
		
	} else if (strcmp(COLOR, "red") == 0){
		return(RED);
	
	} else if (strcmp(COLOR, "green") == 0){
		return(GREEN);
		
	} else if (strcmp(COLOR, "yellow") == 0){
		return(YELLOW);
		
	} else if (strcmp(COLOR, "blue") == 0){
		return(BLUE);
		
	} else if (strcmp(COLOR, "magenta") == 0){
		return(MAGENTA);
		
	} else if (strcmp(COLOR, "cyan") == 0){
		return(CYAN);
		
	} else if (strcmp(COLOR, "light gray") == 0){
		return(LIGHT_GRAY);
		
	} else if (strcmp(COLOR, "dark gray") == 0){
		return(DARK_GRAY);
		
	} else if (strcmp(COLOR, "light red") == 0){
		return(LIGHT_RED);
		
	} else if (strcmp(COLOR, "light green") == 0){
		return(LIGHT_GREEN);
	
	} else if (strcmp(COLOR, "light yellow") == 0){
		return(LIGHT_YELLOW);
		
	} else if (strcmp(COLOR, "light blue") == 0){
		return(LIGHT_BLUE);
		
	} else if (strcmp(COLOR, "light magenta") == 0){
		return(LIGHT_MAGENTA);
		
	} else if (strcmp(COLOR, "light cyan") == 0){
		return(LIGHT_CYAN);
		
	} else if (strcmp(COLOR, "white") == 0){
		return(WHITE);

	} else {
		return(DEFAULT);
	
	}

}