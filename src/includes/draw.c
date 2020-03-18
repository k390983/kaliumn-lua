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

#include "draw.h"

#include "canvas.h"
#include "etc.h"
#include "interface.h"

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

int checkColor(const int COLOR) {
	if(
		COLOR != 30 &&
		COLOR != 31 &&
		COLOR != 32 &&
		COLOR != 33 &&
		COLOR != 34 &&
		COLOR != 35 &&
		COLOR != 36 &&
		COLOR != 37 &&
		COLOR != 90 &&
		COLOR != 91 &&
		COLOR != 92 &&
		COLOR != 93 &&
		COLOR != 94 &&
		COLOR != 95 &&
		COLOR != 96 &&
		COLOR != 97 &&
		COLOR != 39
	) {
		return(-1);

	} else {
		return(1);

	}

}

int colorCode(const char COLOR[]) {
	if(strcmp(COLOR, "black") == 0){
		return(BLACK);
		
	} else if(strcmp(COLOR, "red") == 0){
		return(RED);
	
	} else if(strcmp(COLOR, "green") == 0){
		return(GREEN);
		
	} else if(strcmp(COLOR, "yellow") == 0){
		return(YELLOW);
		
	} else if(strcmp(COLOR, "blue") == 0){
		return(BLUE);
		
	} else if(strcmp(COLOR, "magenta") == 0){
		return(MAGENTA);
		
	} else if(strcmp(COLOR, "cyan") == 0){
		return(CYAN);
		
	} else if(strcmp(COLOR, "light gray") == 0){
		return(LIGHT_GRAY);
		
	} else if(strcmp(COLOR, "dark gray") == 0){
		return(DARK_GRAY);
		
	} else if(strcmp(COLOR, "light red") == 0){
		return(LIGHT_RED);
		
	} else if(strcmp(COLOR, "light green") == 0){
		return(LIGHT_GREEN);
	
	} else if(strcmp(COLOR, "light yellow") == 0){
		return(LIGHT_YELLOW);
		
	} else if(strcmp(COLOR, "light blue") == 0){
		return(LIGHT_BLUE);
		
	} else if(strcmp(COLOR, "light magenta") == 0){
		return(LIGHT_MAGENTA);
		
	} else if(strcmp(COLOR, "light cyan") == 0){
		return(LIGHT_CYAN);
		
	} else if(strcmp(COLOR, "white") == 0){
		return(WHITE);

	} else if(strcmp(COLOR, "default") == 0) {
		return(DEFAULT);
	
	} else {
		return(-1);

	}

}

int setColor(const int FOREGROUND, const int BACKGROUND) {
	if(checkColor(FOREGROUND) == -1 || checkColor(BACKGROUND) == -1) return(-1);

	printf("\e[%dm", FOREGROUND);
	printf("\e[%dm", BACKGROUND + 10);

	return(1);

}

int drawTexture(const int X, const int Y, const char PATH[]) {
	int tWidth, tHeight;
	FILE *fp = fopen(PATH, "r");

	if(fp == NULL) return(-1);

	fscanf(fp, "%d, %d", &tWidth, &tHeight);

	for(int i = 0; i < tHeight; ++i) {
		for(int j = 0; j < tWidth; ++j) {
			int color;

			fscanf(fp, "%d ", &color);

			if(color != 99) {
				if(drawPixel(X + j, Y + i, color) == -1) return(-1);

			}

		}

	}

	fclose(fp);

	return(1);

}

int drawPixel(const int X, const int Y, const int COLOR) {
	if(checkColor(COLOR == -1)) return(-1);
	if(X < 0 || X > canvas[0] - 1) return(-1);
	if(Y < 0 || Y > canvas[1] - 1) return(-1);

	canvas[Y * canvas[0] + X + 2] = COLOR;

	return(1);

}

