//============================================================================//
// KALIUMN CONSOLE INTERFACE LIBRARY
//============================================================================//

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Licence
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

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

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Includes
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

//-------- lua ---------------------------------------------------------------//

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

//-------- standard libraries ------------------------------------------------//

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <unistd.h>

//-------- POSIX libraries ---------------------------------------------------//

#include <sys/ioctl.h>
#include <sys/time.h>

//-------- external libraries ------------------------------------------------//

// reading .bmp files <https://github.com/marc-q/libbmp>
#include "includes/libbmp.h"
// reading .png files <https://github.com/lvandeve/lodepng>
#include "includes/lodepng.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Types
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

typedef struct Color
{
	int r;
	int g;
	int b;
	int a;
}Color;

typedef struct Canvas
{
	int width;
	int height;
	int drawMode;
	Color currentColor;
	Color *pixels;
	Color *previousPixels;
}Canvas;

typedef struct Texture
{
	int width;
	int height;
	Color *pixels;
} Texture;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Global variables
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

Canvas kal_canvas;
Color kal_color;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Internal functions
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#define moveCursor(X, Y) printf("\033[%d;%dH",Y + 1, X + 1)

#define setDefaultColor() printf("\x1b[0m")

#define setFrontColor(R, G, B) printf("\x1b[38;2;%d;%d;%dm", R, G, B)

#define setBackColor(R, G, B) printf("\x1b[48;2;%d;%d;%dm", R, G, B)

#define hideCursor() printf("\e[?25l")

#define showCursor() printf("\e[?25h")

#define clearScreen() printf("\033[2J")

#define setTitle(TITLE) printf("\033]0;%s\a", TITLE)

static int comparePixels(const Color pixel1, const Color pixel2)
{
	if(pixel1.r != pixel2.r || pixel1.g != pixel2.g || pixel1.b != pixel2.b)
		return(1);

	return(0);
}

static void drawToCanvas
(
	Color *pixel,
	const int R,
	const int G,
	const int B,
	const int A
)
{
	float t = (float)A / (float)255;

	pixel->r = (int)(pixel->r * (1 - t) + R * t);
	pixel->g = (int)(pixel->g * (1 - t) + G * t);
	pixel->b = (int)(pixel->b * (1 - t) + B * t);
}

static void assignColor
(
	Color *pixel,
	const int R,
	const int G,
	const int B,
	const int A
)
{
	pixel->r = R;
	pixel->g = G;
	pixel->b = B;
	pixel->a = A;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Canvas
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

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
	kal_canvas.pixels = (Color *)malloc((WIDTH * HEIGHT) * sizeof(Color));
	kal_canvas.previousPixels = (Color *)malloc((WIDTH * HEIGHT) * sizeof(Color));

	kal_canvas.width = WIDTH;
	kal_canvas.height = HEIGHT;

	for(int i = 0; i < HEIGHT; ++i)
	{
		for(int j = 0; j < WIDTH; ++j)
		{
			assignColor(&kal_canvas.pixels[i * WIDTH + j], R, G, B, 255);
			assignColor(&kal_canvas.previousPixels[i * WIDTH + j], -1, -1, -1, 255);
		}
	}

	return(0);
}

int cleanCanvas(const int R, const int G, const int B)
{
	for(int i = 0; i < kal_canvas.height; ++i)
	{
		for(int j = 0; j < kal_canvas.width; ++j)
		{
			assignColor(&kal_canvas.pixels[i * kal_canvas.width + j], R, G, B, 255);
		}
	}

	return(0);
}

int displayCanvas()
{
	moveCursor(0, 0);

	for(int i = 0; i < kal_canvas.height; i += 2)
	{
		for (int j = 0; j < kal_canvas.width; j++)
		{
			Color pixel1 = kal_canvas.pixels[(i * kal_canvas.width) + j];
			Color previousPixel1 = kal_canvas.previousPixels[(i * kal_canvas.width) + j];
			Color pixel2 = kal_canvas.pixels[((i + 1) * kal_canvas.width) + j];
			Color previousPixel2 = kal_canvas.previousPixels[((i + 1) * kal_canvas.width) + j];

			if(comparePixels(pixel1, previousPixel1) || comparePixels(pixel2, previousPixel2))
			{
				moveCursor(j, i / 2);
				setBackColor(pixel1.r, pixel1.g, pixel1.b);
				setFrontColor(pixel2.r, pixel2.g, pixel2.b);
				printf("▄");
			}

		}
	}

	return(0);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Drawing
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

int drawPixel
(
	const int R,
	const int G,
	const int B,
	const int A,
	const int X,
	const int Y
)
{
	if(X > 0 && X < kal_canvas.width && Y > 0 && Y < kal_canvas.height)
	{
		drawToCanvas(&kal_canvas.pixels[Y * kal_canvas.width + X], R, G, B, A);
	}
	else
		return(1);

	return(0);
}

/*int drawTexture(const int X, const int Y, const char PATH[])
{
	unsigned char* image = 0;
	unsigned width, height;

	lodepng_decode32_file(&image, &width, &height, PATH);

	for(int i = 0; i < height; i++)
	{
		for(int j = 0; j < width; j++)
		{
			int r = image[i * width * 4 + j * 4];
			int g = image[i * width * 4 + j * 4 + 1];
			int b = image[i * width * 4 + j * 4 + 2];
			int a = image[i * width * 4 + j * 4 + 3];

			drawPixel(X + j, Y + i, r, g, b, a);
		}
	}

	return(0);
}*/

int drawTexture(const Texture image, const int X, const int Y)
{
	for(int i = 0; i < image.height; i++)
	{
		for(int j = 0; j < image.width; j++)
		{
			int r = image.pixels[i * image.width + j].r;
			int g = image.pixels[i * image.width + j].g;
			int b = image.pixels[i * image.width + j].b;
			int a = image.pixels[i * image.width + j].a;

			drawPixel(r, g, b, a, X + j, Y + i);
		}
	}

	return(0);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Input
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Texture
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

Texture initTexture(const char *PATH)
{
	unsigned char* image = 0;
	unsigned width, height;

	lodepng_decode32_file(&image, &width, &height, PATH);

	Texture texture;
	texture.width = width;
	texture.height = height;
	texture.pixels = (Color *)malloc((width * height) * sizeof(Color));

	for(int i = 0; i < height; i++)
	{
		for(int j = 0; j < width; j++)
		{
			assignColor
			(
				&texture.pixels[i * width + j],
				image[i * width * 4 + j * 4],
				image[i * width * 4 + j * 4 + 1],
				image[i * width * 4 + j * 4 + 2],
				image[i * width * 4 + j * 4 + 3]
			);
			//printf("(%d, %d, %d, %d)\n", image[i * width * 4 + j * 4], image[i * width * 4 + j * 4 + 1], image[i * width * 4 + j * 4 + 2], image[i * width * 4 + j * 4 + 3]);
		}
	}

	//getchar();

	return(texture);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Misc
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Window
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Interface
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

//-------- canvas ------------------------------------------------------------//

int E_initCanvas(lua_State *L) {
	int width = luaL_checknumber(L, 1);
	int height = luaL_checknumber(L, 2);
	int r = luaL_checknumber(L, 3);
	int g = luaL_checknumber(L, 4);
	int b = luaL_checknumber(L, 5);

	initCanvas(width, height, r, g, b);

	return(0);

}

int E_cleanCanvas(lua_State *L)
{
	int r = luaL_checknumber(L, 1);
	int g = luaL_checknumber(L, 2);
	int b = luaL_checknumber(L, 3);
	cleanCanvas(r, g, b);

	return(0);
}

int E_displayCanvas(lua_State *L)
{
	displayCanvas();

	return(0);
}

//-------- drawing -----------------------------------------------------------//

/*int E_setColor(lua_State *L) {
	setColor(
		colorCode(luaL_checkstring(L, 1)),
		colorCode(luaL_checkstring(L, 2))

	);

	return(0);

}*/

int E_drawPixel(lua_State *L)
{
	int r = luaL_checknumber(L, 1);
	int g = luaL_checknumber(L, 2);
	int b = luaL_checknumber(L, 3);
	int a = luaL_checknumber(L, 4);
	int x = luaL_checknumber(L, 5);
	int y = luaL_checknumber(L, 6);
	drawPixel(r, g, b, a, x, y);

	return(0);
}

/*int E_drawTexture(lua_State *L)
{
	int x = luaL_checknumber(L, 1);
	int y = luaL_checknumber(L, 2);

	char path[FILENAME_MAX];
	sprintf(path, "%s", luaL_checkstring(L, 3));

	drawTexture(x, y, path);

	return(0);
}*/

int E_drawTexture(lua_State *L)
{
	int *data = malloc(sizeof(int));
	int width, height;

	int x = lua_tonumber(L, 2);
	int y = lua_tonumber(L, 3);

	lua_pushnil(L);

	while(lua_next(L, 1) != 0)
	{
		if(lua_istable(L, -1))
		{
			int i = 1;

			//read pixel info data to data[]
			lua_pushnil(L);
			while(lua_next(L, -2) != 0)
			{
				data = realloc(data, i * sizeof(int));
				data[i - 1] = (int)lua_tonumber(L, -1);
				lua_pop(L, 1);
				i++;
			}
		}
		else if(strcmp(lua_tostring(L, -2), "width") == 0)
		{
			width = lua_tonumber(L, -1);
		}
		else if(strcmp(lua_tostring(L, -2), "height") == 0)
		{
			height = lua_tonumber(L, -1);
		}

		lua_pop(L, 1);
	}

	//convert data[] to texture
	Texture texture;
	texture.pixels = (Color *)malloc((width * height) * sizeof(Color));
	texture.width = width;
	texture.height = height;

	for(int i = 0; i < height; i++)
	{
		for(int j = 0; j < width; j++)
		{
			texture.pixels[i * width + j].r = data[(i * width + j) * 4];
			texture.pixels[i * width + j].g = data[(i * width + j) * 4 + 1];
			texture.pixels[i * width + j].b = data[(i * width + j) * 4 + 2];
			texture.pixels[i * width + j].a = data[(i * width + j) * 4 + 3];
		}
	}

	drawTexture(texture, x, y);

	return(0);
}

//-------- input -------------------------------------------------------------//

//-------- texture -----------------------------------------------------------//

int E_initTexture(lua_State *L)
{
	char path[FILENAME_MAX];

	sprintf(path, "%s", luaL_checkstring(L, 1));

	Texture texture = initTexture(path);

	lua_newtable(L);

	lua_pushstring(L, "width");
	lua_pushnumber(L, texture.width);
	lua_settable(L, -3);

	lua_pushstring(L, "height");
	lua_pushnumber(L, texture.height);
	lua_settable(L, -3);

	lua_pushstring(L,"data");
	lua_newtable(L);

	for(int i = 0; i < texture.width; i++)
	{
		for(int j = 0; j < texture.width; j++)
		{
			Color pixel = texture.pixels[i * texture.width + j];

			lua_pushnumber(L, (i * texture.width + j) * 4 + 1);
			lua_pushnumber(L, pixel.r);
			lua_settable(L, -3);

			lua_pushnumber(L, (i * texture.width + j) * 4 + 1 + 1);
			lua_pushnumber(L, pixel.g);
			lua_settable(L, -3);

			lua_pushnumber(L, (i * texture.width + j) * 4 + 2 + 1);
			lua_pushnumber(L, pixel.b);
			lua_settable(L, -3);

			lua_pushnumber(L, (i * texture.width + j) * 4 + 3 + 1);
			lua_pushnumber(L, pixel.a);
			lua_settable(L, -3);
		}
	}

	lua_settable(L, -3);

	return(1);
}

//-------- misc --------------------------------------------------------------//

int E_waitForKeyPress(lua_State *L)
{
	getchar();

	return(0);
}

// returns time in miliseconds
int E_getTime(lua_State *L)
{
	struct timeval  tv;
	gettimeofday(&tv, NULL);

	long int time = (long int)((tv.tv_sec) * 1000 + (tv.tv_usec) / 1000);

	lua_pushnumber(L, time);

	return(1);

}

//-------- window ------------------------------------------------------------//

int E_init(lua_State *L)
{
	setTitle(luaL_checkstring(L, 1));
	hideCursor();
	moveCursor(0, 0);
	clearScreen();

	return(0);
}

int E_term(lua_State *L)
{
	setDefaultColor();
	showCursor();
	moveCursor(0, 0);
	clearScreen();

	return(0);
}

int E_getWinWidth(lua_State *L)
{
	struct winsize size;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
	lua_pushnumber(L, size.ws_col);

	return(1);
}

int E_getWinHeight(lua_State *L)
{
	struct winsize size;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
	lua_pushnumber(L, size.ws_row * 2);

	return(1);
}

int E_setWindowTitle(lua_State *L)
{
	setTitle(luaL_checkstring(L, 1));

	return(0);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Registering library
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

const struct luaL_Reg kaliumn[] =
{
	{"initCanvas", E_initCanvas},
	{"cleanCanvas", E_cleanCanvas},
	{"displayCanvas", E_displayCanvas},
	{"drawPixel", E_drawPixel},
	{"drawTexture", E_drawTexture},
	{"initTexture", E_initTexture},
	{"waitForKeyPress", E_waitForKeyPress},
	{"getTime", E_getTime},
	{"init", E_init},
	{"term", E_term},
	{"getWinWidth", E_getWinWidth},
	{"getWinHeight", E_getWinHeight},
	{"setWindowTitle", E_setWindowTitle},
	{NULL, NULL}
};

int luaopen_kaliumn (lua_State *L)
{
	luaL_newlib(L, kaliumn);
	return 1;
}
