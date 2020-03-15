
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

//----------------------------------------------------------------------------//
// colors
//----------------------------------------------------------------------------//

#define DEFAULT 39
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

//----------------------------------------------------------------------------//
// global variables
//----------------------------------------------------------------------------//

int *canvas;
int *previousCanvas;

//----------------------------------------------------------------------------//
// internal functions / macros
//----------------------------------------------------------------------------//

#define hideCursor() printf("\e[?25l");

#define showCursor() printf("\e[?25h");

#define clearScreen() printf("\033[2J");

void setColor(const int FOREGROUND, const int BACKGROUND) {
	printf("\e[%dm", FOREGROUND);
	printf("\e[%dm", BACKGROUND + 10);

}

void moveCursor(const int X, const int Y) {
	printf("\033[%d;%dH",Y, X);

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

//----------------------------------------------------------------------------//
// external functions
//----------------------------------------------------------------------------//

//-------- initialization / termination --------------------------------------//

int E_initialize(lua_State *L) {
	setTitle(luaL_checkstring(L, 1));
	hideCursor();
	moveCursor(0, 0);
	clearScreen();

	return(0);

}

int E_terminate(lua_State *L) {
	setColor(colorCode("default"), colorCode("default"));
	showCursor();
	moveCursor(0, 0);
	clearScreen();

	return(0);

}

//-------- window ------------------------------------------------------------//

int E_setTitle(lua_State *L) {
	setTitle(luaL_checkstring(L, 1));

	return(0);

}

int E_getWinX(lua_State *L) {
	struct winsize size;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);

	lua_pushnumber(L, size.ws_col / 2);

	return(1);

}

int E_getWinY(lua_State *L) {
	struct winsize size;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);

	lua_pushnumber(L, size.ws_row);

	return(1);

}

//-------- cursor ------------------------------------------------------------//

int E_setColor(lua_State *L) {
	setColor(
		colorCode(luaL_checkstring(L, 1)),
		colorCode(luaL_checkstring(L, 2))

	);

	return(0);

}

int E_moveCursor(lua_State *L) {
	int x = luaL_checknumber(L, 1);
	int y = luaL_checknumber(L, 2);

	moveCursor(x, y);

	return(0);

}

//-------- canvas functions --------------------------------------------------//

int E_initCanvas(lua_State *L) {
	int width = luaL_checknumber(L, 1);
	int height = luaL_checknumber(L, 2);
	int color = colorCode(luaL_checkstring(L, 3));

	//Variable length array
	canvas = (int *)malloc((width * height + 2) * sizeof(int));
	previousCanvas = (int *)malloc((width * height) * sizeof(int));
	//The first element stores the width and the second the height
	canvas[0] = width;
	canvas[1] = height;

	for(int i = 0; i < height; ++i) {
		for(int j = 0; j < width; ++j) {
			canvas[(i * width) + j + 2] = color;
			previousCanvas[(i * width) + j] = 0;

		}
	
	}

	return(0);
	
}

int E_cleanCanvas(lua_State *L) {
	int color = colorCode(luaL_checkstring(L, 3));
	int x = canvas[0];
	int y = canvas[1];

	for(int i = 0; i < y; ++i) {
		for(int j = 0; j < x; ++j) {
			canvas[(i * x) + j + 2] = color;

		}

	}

	return(0);

}

int E_display() {
	int width = canvas[0];
	int height = canvas[1];
	moveCursor(0, 0);

	for(int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			if(canvas[(i * width) + j + 2] != previousCanvas[(i * width) + j]) {
				moveCursor(j * 2, i);
				setColor(DEFAULT, canvas[(i * width) + j + 2]);
				printf("  ");

			}

			previousCanvas[(i * width) + j] = canvas[(i * width) + j + 2];

		}

		if(i < height - 1) {
			printf("\n");

		}

	}

	return(0);

}

//-------- textures ----------------------------------------------------------//

/*int E_drawTexture(lua_State *L) {
	int x = luaL_checknumber(L, 1);
	int y = luaL_checknumber(L, 2);

	char texture[25];
	sprintf(texture, "%s", luaL_checkstring(L, 3));

	return(0);

}*/

//-------- drawing -----------------------------------------------------------//

int E_drawPixel(lua_State *L) {
	int x = luaL_checknumber(L, 1);
	int y = luaL_checknumber(L, 2);

	int color = colorCode(luaL_checkstring(L, 3));
	int width = canvas[0];

	canvas[y * width + x + 2] = color;

	return(0);

}



//-------- utility -----------------------------------------------------------//

int E_printString(lua_State *L) {
	printf("%s", luaL_checkstring(L, 1));

	return(0);

}

int E_waitForKeyPress(lua_State *L) {
	getchar();

	return(0);

}

//----------------------------------------------------------------------------//
// registering library
//----------------------------------------------------------------------------//

const struct luaL_Reg kaliumn[] = {
	{"initialize", E_initialize},
	{"terminate", E_terminate},
	{"setTitle", E_setTitle},
	{"getWinX", E_getWinX},
	{"getWinY", E_getWinY},
	{"setColor", E_setColor},
	{"moveCursor", E_moveCursor},
	{"initCanvas", E_initCanvas},
	{"cleanCanvas", E_cleanCanvas},
	{"drawPixel", E_drawPixel},
	{"display", E_display},
	{"printString", E_printString},
	{"waitForKeyPress", E_waitForKeyPress},
	{NULL, NULL}

};

int luaopen_kaliumn (lua_State *L){
	luaL_newlib(L, kaliumn);
	return 1;

}