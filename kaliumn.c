
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

// foreground

#define F_DEFAULT 39
#define F_BLACK 30
#define F_RED 31
#define F_GREEN 32
#define F_YELLOW 33
#define F_BLUE 34
#define F_MAGENTA 35
#define F_CYAN 36
#define F_LIGHT_GRAY 37
#define F_DARK_GRAY 90
#define F_LIGHT_RED 91
#define F_LIGHT_GREEN 92
#define F_LIGHT_YELLOW 93
#define F_LIGHT_BLUE 94
#define F_LIGHT_MAGENTA 95
#define F_LIGHT_CYAN 96
#define F_WHITE 97

// background

#define B_DEFAULT 49
#define B_BLACK 40
#define B_RED 41
#define B_GREEN 42
#define B_YELLOW 43
#define B_BLUE 44
#define B_MAGENTA 45
#define B_CYAN 46
#define B_LIGHT_GRAY 47
#define B_DARK_GRAY 100
#define B_LIGHT_RED 101
#define B_LIGHT_GREEN 102
#define B_LIGHT_YELLOW 103
#define B_LIGHT_BLUE 104
#define B_LIGHT_MAGENTA 105
#define B_LIGHT_CYAN 106
#define B_WHITE 107

//----------------------------------------------------------------------------//
// global variables
//----------------------------------------------------------------------------//

int *KAL_canvas;
int *KAL_previousCanvas;

//----------------------------------------------------------------------------//
// internal functions / macros
//----------------------------------------------------------------------------//

#define hideCursor() printf("\e[?25l");

#define showCursor() printf("\e[?25h");

#define clearScreen() printf("\033[2J");

void setColor(const char FOREGROUND[], const char BACKGROUND[]) {
	// foreground

	if (strcmp(FOREGROUND, "black") == 0){
		printf("\e[%dm", F_BLACK);
		
	} else if (strcmp(FOREGROUND, "red") == 0){
		printf("\e[%dm", F_RED);
	
	} else if (strcmp(FOREGROUND, "green") == 0){
		printf("\e[%dm", F_GREEN);
		
	} else if (strcmp(FOREGROUND, "yellow") == 0){
		printf("\e[%dm", F_YELLOW);
		
	} else if (strcmp(FOREGROUND, "blue") == 0){
		printf("\e[%dm", F_BLUE);
		
	} else if (strcmp(FOREGROUND, "magenta") == 0){
		printf("\e[%dm", F_MAGENTA);
		
	} else if (strcmp(FOREGROUND, "cyan") == 0){
		printf("\e[%dm", F_CYAN);
		
	} else if (strcmp(FOREGROUND, "light gray") == 0){
		printf("\e[%dm", F_LIGHT_GRAY);
		
	} else if (strcmp(FOREGROUND, "dark gray") == 0){
		printf("\e[%dm", F_DARK_GRAY);
		
	} else if (strcmp(FOREGROUND, "light red") == 0){
		printf("\e[%dm", F_LIGHT_RED);
		
	} else if (strcmp(FOREGROUND, "light green") == 0){
		printf("\e[%dm", F_LIGHT_GREEN);
	
	} else if (strcmp(FOREGROUND, "light yellow") == 0){
		printf("\e[%dm", F_LIGHT_YELLOW);
		
	} else if (strcmp(FOREGROUND, "light blue") == 0){
		printf("\e[%dm", F_LIGHT_BLUE);
		
	} else if (strcmp(FOREGROUND, "light magenta") == 0){
		printf("\e[%dm", F_LIGHT_MAGENTA);
		
	} else if (strcmp(FOREGROUND, "light cyan") == 0){
		printf("\e[%dm", F_LIGHT_CYAN);
		
	} else if (strcmp(FOREGROUND, "white") == 0){
		printf("\e[%dm", F_DEFAULT);

	} else {
		printf("\e[%dm", F_DEFAULT);
	
	}

	// background

	if (strcmp(BACKGROUND, "black") == 0){
		printf("\e[%dm", B_BLACK);
		
	} else if (strcmp(BACKGROUND, "red") == 0){
		printf("\e[%dm", B_RED);
	
	} else if (strcmp(BACKGROUND, "green") == 0){
		printf("\e[%dm", B_GREEN);
		
	} else if (strcmp(BACKGROUND, "yellow") == 0){
		printf("\e[%dm", B_YELLOW);
		
	} else if (strcmp(BACKGROUND, "blue") == 0){
		printf("\e[%dm", B_BLUE);
		
	} else if (strcmp(BACKGROUND, "magenta") == 0){
		printf("\e[%dm", B_MAGENTA);
		
	} else if (strcmp(BACKGROUND, "cyan") == 0){
		printf("\e[%dm", B_CYAN);
		
	} else if (strcmp(BACKGROUND, "light gray") == 0){
		printf("\e[%dm", B_LIGHT_GRAY);
		
	} else if (strcmp(BACKGROUND, "dark gray") == 0){
		printf("\e[%dm", B_DARK_GRAY);
		
	} else if (strcmp(BACKGROUND, "light red") == 0){
		printf("\e[%dm", B_LIGHT_RED);
		
	} else if (strcmp(BACKGROUND, "light green") == 0){
		printf("\e[%dm", B_LIGHT_GREEN);
	
	} else if (strcmp(BACKGROUND, "light yellow") == 0){
		printf("\e[%dm", B_LIGHT_YELLOW);
		
	} else if (strcmp(BACKGROUND, "light blue") == 0){
		printf("\e[%dm", B_LIGHT_BLUE);
		
	} else if (strcmp(BACKGROUND, "light magenta") == 0){
		printf("\e[%dm", B_LIGHT_MAGENTA);
		
	} else if (strcmp(BACKGROUND, "light cyan") == 0){
		printf("\e[%dm", B_LIGHT_CYAN);
		
	} else if (strcmp(BACKGROUND, "white") == 0){
		printf("\e[%dm", B_DEFAULT);

	} else {
		printf("\e[%dm", B_DEFAULT);
	
	}

}

void moveCursor(const int X, const int Y) {
	printf("\033[%d;%dH",Y, X);

}

void setTitle(const char TITLE[]) {
	printf("\033]0;%s\a", TITLE);

}

//----------------------------------------------------------------------------//
// external functions
//----------------------------------------------------------------------------//

int E_initialize(lua_State *L) {
	char title[100];
	sprintf(title, "%s", luaL_checkstring(L, 1));

	setTitle(title);
	hideCursor();
	moveCursor(0, 0);
	clearScreen();

	return(0);

}

int E_terminate(lua_State *L) {
	setColor("default", "default");
	showCursor();
	moveCursor(0, 0);
	clearScreen();

	return(0);

}

int E_setTitle(lua_State *L) {
	char title[100];
	sprintf(title, "%s", luaL_checkstring(L, 1));

	setTitle(title);

	return(0);

}

int E_getWinX(lua_State *L) {
	struct winsize size;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);

	lua_pushnumber(L, size.ws_col);

	return(1);

}

int E_getWinY(lua_State *L) {
	struct winsize size;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);

	lua_pushnumber(L, size.ws_row);

	return(1);

}

int E_setColor(lua_State *L) {
	char foreground[25], background[25];
	sprintf(foreground, "%s", luaL_checkstring(L, 1));
	sprintf(background, "%s", luaL_checkstring(L, 2));

	setColor(foreground, background);

	return(0);

}

int E_moveCursor(lua_State *L) {
	int x = luaL_checknumber(L, 1);
	int y = luaL_checknumber(L, 2);

	moveCursor(x, y);

	return(0);

}

int E_printString(lua_State *L) {
	printf("%s", luaL_checkstring(L, 1));

	return(0);

}

int E_waitForKeyPress(lua_State *L) {
	getchar();

	return(0);

}

int E_initCanvas(lua_State *L) {
	int width = luaL_checknumber(L, 1);
	int height = luaL_checknumber(L, 2);
	int color = luaL_checknumber(L, 3);

	//Variable length array
	KAL_canvas = (int *)malloc((width * height + 2) * sizeof(int));
	KAL_previousCanvas = (int *)malloc((width * height) * sizeof(int));
	//The first element stores the width and the second the height
	KAL_canvas[0] = width;
	KAL_canvas[1] = height;
	//Initialize with background color
	for(int i = 0; i < height; ++i) {
		for(int j = 0; j < width; ++j) {
			KAL_canvas[(i * width) + j + 2] = color;
			KAL_previousCanvas[(i * width) + j] = 99;

		}
	
	}

	return(0);
	
}

int E_cleanCanvas(lua_State *L) {
	int color = luaL_checknumber(L, 1);

	//Get width and height
	int width = KAL_canvas[0];
	int height = KAL_canvas[1];
	//Clean with background color
	for(int i = 0; i < height; ++i)
	{
		for(int j = 0; j < width; ++j)
		{
			KAL_canvas[(i * width) + j + 2] = color;
		}
	}

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
	{"printString", E_printString},
	{"waitForKeyPress", E_waitForKeyPress},
	{NULL, NULL}

};

int luaopen_kaliumn (lua_State *L){
	luaL_newlib(L, kaliumn);
	return 1;

}