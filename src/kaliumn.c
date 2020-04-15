//============================================================================//
// KALIUMN CONSOLE INTERFACE LIBRARY
//============================================================================//

//----------------------------------------------------------------------------//
// Licence
//----------------------------------------------------------------------------//

/*
MIT License

Copyright (c) 2020 Kai Kitagawa-Jones

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

//----------------------------------------------------------------------------//
// Includes
//----------------------------------------------------------------------------//

//-------- lua ---------------------------------------------------------------//

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

//-------- standard libraries ------------------------------------------------//

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

//-------- POSIX libraries ---------------------------------------------------//

#include <sys/ioctl.h>
#include <sys/time.h>

//-------- external libraries ------------------------------------------------//

// reading .bmp files < https://github.com/marc-q/libbmp >
#include "includes/libbmp.h"

//----------------------------------------------------------------------------//
// Types
//----------------------------------------------------------------------------//

typedef struct Pixel{
	int r;
    int g;
    int b;

} Pixel;

typedef struct Canvas{
	int width;
	int height;
	Pixel *pixels;
	Pixel *previousPixels;

} Canvas;

//----------------------------------------------------------------------------//
// Global variables
//----------------------------------------------------------------------------//

Canvas canvas;

//----------------------------------------------------------------------------//
// Internal functions
//----------------------------------------------------------------------------//

#define moveCursor(X, Y) printf("\033[%d;%dH",Y + 1, X + 1)

#define setDefaultColor() printf("\x1b[0m")

#define setFrontColor(R, G, B) printf("\x1b[38;2;%d;%d;%dm", R, G, B)

#define setBackColor(R, G, B) printf("\x1b[48;2;%d;%d;%dm", R, G, B)

#define hideCursor() printf("\e[?25l")

#define showCursor() printf("\e[?25h")

#define clearScreen() printf("\033[2J")

#define setTitle(TITLE) printf("\033]0;%s\a", TITLE)

int comparePixels(const Pixel pixel1, const Pixel pixel2)
{
	if(pixel1.r != pixel2.r || pixel1.g != pixel2.g || pixel1.b != pixel2.b)
		return(1);

	return(0);
}

void assignColor(Pixel pixel, const int R, const int G, const int B)
{
	pixel.r = R;
	pixel.g = G;
	pixel.b = B;
}

//----------------------------------------------------------------------------//
// Canvas
//----------------------------------------------------------------------------//

int initCanvas(
	const int WIDTH,
	const int HEIGHT,
	const int R,
	const int G,
	const int B
)
{
	if(WIDTH <= 0) return(1);
	if(HEIGHT <= 0) return(1);

	//Variable length array
	canvas.pixels = (Pixel *)malloc((WIDTH * HEIGHT) * sizeof(Pixel));
	canvas.previousPixels = (Pixel *)malloc((WIDTH * HEIGHT) * sizeof(Pixel));
	//The first element stores the width and the second the height
	canvas.width = WIDTH;
	canvas.height = HEIGHT;

	for(int i = 0; i < HEIGHT; ++i)
	{
		for(int j = 0; j < WIDTH; ++j)
		{
			assignColor(canvas.pixels[i * WIDTH + j], R, G, B);
			assignColor(canvas.previousPixels[i * WIDTH + j], -1, -1, -1);
		}
	}

	return(0);
}

int cleanCanvas(const int R, const int G, const int B)
{
	for(int i = 0; i < canvas.height; ++i)
	{
		for(int j = 0; j < canvas.width; ++j)
		{
			assignColor(canvas.pixels[i * canvas.width + j], R, G, B);
		}
	}

	return(0);
}

int display()
{
	moveCursor(0, 0);

	for(int i = 0; i < canvas.height; i += 2)
	{
		for (int j = 0; j < canvas.width; j++)
		{
			Pixel pixel1 = canvas.pixels[(i * canvas.width) + j];
			Pixel previousPixel1 = canvas.pixels[(i * canvas.width) + j];
			Pixel pixel2 = canvas.pixels[(i * canvas.width) + j];
			Pixel previousPixel2 = canvas.pixels[(i * canvas.width) + j];

			/*if(comparePixels(pixel1, previousPixel1) || comparePixels(pixel2, previousPixel2))
			{
				moveCursor(j, i / 2);
				setBackColor(pixel1.r, pixel1.g, pixel1.b);
				setFrontColor(pixel2.r, pixel2.g, pixel2.b);
				printf("▄");
			}*/

			setBackColor(pixel1.r, pixel1.g, pixel1.b);
			setFrontColor(pixel2.r, pixel2.g, pixel2.b);
			printf("▄");
		}
		printf("\n");
	}

	return(0);
}

//----------------------------------------------------------------------------//
// Drawing
//----------------------------------------------------------------------------//

int drawPixel(const int X, const int Y, const int R, const int G, const int B)
{
	if(X < 0 || X > canvas.width) return(1);
	if(Y < 0 || Y > canvas.height) return(1);

	assignColor(canvas.pixels[Y * canvas.width + X], R, G, B);

	return(1);
}

//----------------------------------------------------------------------------//
// Input
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// Texture
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// Misc
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// Window
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
// Interface
//----------------------------------------------------------------------------//

int E_initialize(lua_State *L) {
	setTitle(luaL_checkstring(L, 1));
	hideCursor();
	moveCursor(0, 0);
	clearScreen();

	return(0);

}

int E_terminate(lua_State *L) {
	setDefaultColor();
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

	lua_pushnumber(L, floor(size.ws_col / 2));

	return(1);

}

int E_getWinY(lua_State *L) {
	struct winsize size;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);

	lua_pushnumber(L, size.ws_row);

	return(1);

}

//-------- canvas --------------------------------------------------//

int E_initCanvas(lua_State *L) {
	int width = luaL_checknumber(L, 1);
	int height = luaL_checknumber(L, 2);
	int r = luaL_checknumber(L, 3);
	int g = luaL_checknumber(L, 4);
	int b = luaL_checknumber(L, 5);

	initCanvas(width, height, r, g, b);

	return(0);

}

int E_cleanCanvas(lua_State *L) {
	int r = luaL_checknumber(L, 1);
	int g = luaL_checknumber(L, 2);
	int b = luaL_checknumber(L, 3);

	cleanCanvas(r, g, b);

	return(0);

}

int E_display(lua_State *L) {
	display();

	return(0);

}

//-------- drawing -----------------------------------------------------------//

int E_drawPixel(lua_State *L) {
	int x = luaL_checknumber(L, 1);
	int y = luaL_checknumber(L, 2);
	int r = luaL_checknumber(L, 3);
	int g = luaL_checknumber(L, 4);
	int b = luaL_checknumber(L, 5);

	drawPixel(x, y, r, g, b);

	return(0);

}

/*int E_drawTexture(lua_State *L) {
	int x = luaL_checknumber(L, 1);
	int y = luaL_checknumber(L, 2);

	char path[25];
	sprintf(path, "%s", luaL_checkstring(L, 3));

	printf("%s", path);
	getchar();

	drawTexture(x, y, path);

	return(0);

}*/

//-------- utility -----------------------------------------------------------//

int E_printString(lua_State *L) {
	printf("%s", luaL_checkstring(L, 1));

	return(0);

}

int E_waitForKeyPress(lua_State *L) {
	getchar();

	return(0);

}

/*int E_setColor(lua_State *L) {
	setColor(
		colorCode(luaL_checkstring(L, 1)),
		colorCode(luaL_checkstring(L, 2))

	);

	return(0);

}*/

int E_moveCursor(lua_State *L) {
	int x = luaL_checknumber(L, 1);
	int y = luaL_checknumber(L, 2);

	moveCursor(x, y);

	return(0);

}

// returns time in miliseconds

int E_getTime(lua_State *L) {
	struct timeval  tv;
	gettimeofday(&tv, NULL);

	long int time = (long int)((tv.tv_sec) * 1000 + (tv.tv_usec) / 1000);

	lua_pushnumber(L, time);

	return(1);

}

//----------------------------------------------------------------------------//
// Registering library
//----------------------------------------------------------------------------//

const struct luaL_Reg kaliumn[] = {
	{"initialize", E_initialize},
	{"terminate", E_terminate},
	{"setTitle", E_setTitle},
	{"getWinX", E_getWinX},
	{"getWinY", E_getWinY},
	{"initCanvas", E_initCanvas},
	{"cleanCanvas", E_cleanCanvas},
	{"display", E_display},
	{"drawPixel", E_drawPixel},
	//{"drawTexture", E_drawTexture},
	{"printString", E_printString},
	{"waitForKeyPress", E_waitForKeyPress},
	//{"setColor", E_setColor},
	{"moveCursor", E_moveCursor},
	{"getTime", E_getTime},
	{NULL, NULL}

};

int luaopen_kaliumn (lua_State *L){
	luaL_newlib(L, kaliumn);
	return 1;

}
