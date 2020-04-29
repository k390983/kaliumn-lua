//============================================================================//
// Kaliumn
//============================================================================//

// View kaliumn.h for documentation

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

#define _BSD_SOURCE
#define _DEFAULT_SOURCE

//-------- standard libraries ------------------------------------------------//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

//-------- POSIX libraries ---------------------------------------------------//

#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/select.h>

//-------- external libraries ------------------------------------------------//

// reading images <https://github.com/nothings/stb>
#define STB_IMAGE_IMPLEMENTATION
#include "includes/stb_image.h"

//-------- kaliumn.h ---------------------------------------------------------//

#include "kaliumn.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Global variables
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

Canvas kal_canvas;
float kal_dt;
struct termios kal_origTerm;
char *keys;
char *arrowKeys;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Internal functions
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#define moveCursor(X, Y) printf("\033[%d;%dH",Y + 1, X + 1)

#define setDefaultColor() printf("\x1b[0m")

#define consoleFrontColor(R, G, B) printf("\x1b[38;2;%d;%d;%dm", R, G, B)

#define consoleBackColor(R, G, B) printf("\x1b[48;2;%d;%d;%dm", R, G, B)

#define hideCursor() printf("\e[?25l")

#define showCursor() printf("\e[?25h")

#define clearScreen() printf("\033[2J")

void resetTerminal()
{
    tcsetattr(0, TCSANOW, &kal_origTerm);
}

void initTerminal()
{
    struct termios new_termios;

    tcgetattr(0, &kal_origTerm);
    memcpy(&new_termios, &kal_origTerm, sizeof(new_termios));

    atexit(resetTerminal);
    cfmakeraw(&new_termios);
    tcsetattr(0, TCSANOW, &new_termios);
}

int keyHit()
{
    struct timeval tv = { 0L, 0L };
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(0, &fds);
    return select(1, &fds, NULL, NULL, &tv);
}

int getKey()
{
    int r;
    unsigned char c;
    if ((r = read(0, &c, sizeof(c))) < 0) {
        return r;
    } else {
        return c;
    }
}

static int compareColor3(const Color3 pixel1, const Color3 pixel2)
{
	if(pixel1.r != pixel2.r || pixel1.g != pixel2.g || pixel1.b != pixel2.b)
		return(1);

	return(0);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Window
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

int getWinX()
{
	struct winsize size;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);

	return(size.ws_col);
}

int getWinY()
{
	struct winsize size;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);

	return(size.ws_row * 2);
}

void setWinTitle(const char TITLE[])
{
	printf("\033]0;%s\a", TITLE);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Initialisation / Termination
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

void init(const char TITLE[])
{
	setWinTitle(TITLE);
	initTerminal();
	hideCursor();
	moveCursor(0, 0);
	clearScreen();
}

void term()
{
	setDefaultColor();
	showCursor();
	moveCursor(0, 0);
	clearScreen();
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Canvas
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

void initCanvas(
	const int WIDTH,
	const int HEIGHT,
	const int R,
	const int G,
	const int B
)
{
	//Variable length array
	kal_canvas.pixels = (Color3 *)malloc((WIDTH * HEIGHT) * sizeof(Color3));
	kal_canvas.previousPixels
		= (Color3 *)malloc((WIDTH * HEIGHT) * sizeof(Color3));

	kal_canvas.width = WIDTH;
	kal_canvas.height = HEIGHT;

	kal_canvas.prevDraw = getTime();
	kal_dt = 0;

	for(int i = 0; i < HEIGHT; ++i)
	{
		for(int j = 0; j < WIDTH; ++j)
		{
			#define pixel kal_canvas.pixels[i * WIDTH + j]
			#define previousPixel kal_canvas.previousPixels[i * WIDTH + j]
			pixel.r = R;
			pixel.g = B;
			pixel.b = B;
			previousPixel.r = R;
			previousPixel.g = B;
			previousPixel.b = G;
		}
	}
}

void cleanCanvas(const int R, const int G, const int B)
{
	for(int i = 0; i < kal_canvas.height; ++i)
	{
		for(int j = 0; j < kal_canvas.width; ++j)
		{
			#define pixel kal_canvas.pixels[i * kal_canvas.width + j]
			pixel.r = R;
			pixel.g = B;
			pixel.b = B;
		}
	}
}

void displayCanvas()
{
	moveCursor(0, 0);

	for(int i = 0; i < kal_canvas.height; i += 2)
	{
		for (int j = 0; j < kal_canvas.width; j++)
		{
			#define topPixel kal_canvas.pixels[(i * kal_canvas.width) + j]
			#define topPrevPixel kal_canvas.previousPixels[(i * kal_canvas.width) + j]
			#define bottomPixel kal_canvas.pixels[((i + 1) * kal_canvas.width) + j]
			#define bottomPrevPixel kal_canvas.previousPixels[((i + 1) * kal_canvas.width) + j]

			if
			(
				compareColor3(topPixel, topPrevPixel)
				|| compareColor3(bottomPixel, bottomPrevPixel)
			)
			{
				moveCursor(j, i / 2);
				consoleBackColor(topPixel.r, topPixel.g, topPixel.b);
				consoleFrontColor(bottomPixel.r, bottomPixel.g, bottomPixel.b);
				printf("▄");
			}

			topPrevPixel.r = topPixel.r;
			topPrevPixel.g = topPixel.g;
			topPrevPixel.b = topPixel.b;

			bottomPrevPixel.r = bottomPixel.r;
			bottomPrevPixel.g = bottomPixel.g;
			bottomPrevPixel.b = bottomPixel.b;
		}
	}

	refreshKeys();

	kal_dt = getTime() - kal_canvas.prevDraw;
	kal_canvas.prevDraw = getTime();
}

void displayCanvasAll()
{
	moveCursor(0, 0);

	for(int i = 0; i < kal_canvas.height; i += 2)
	{
		for (int j = 0; j < kal_canvas.width; j++)
		{
			#define topPixel kal_canvas.pixels[(i * kal_canvas.width) + j]
			#define topPrevPixel kal_canvas.previousPixels[(i * kal_canvas.width) + j]
			#define bottomPixel kal_canvas.pixels[((i + 1) * kal_canvas.width) + j]
			#define bottomPrevPixel kal_canvas.previousPixels[((i + 1) * kal_canvas.width) + j]

			moveCursor(j, i / 2);
			consoleBackColor(topPixel.r, topPixel.g, topPixel.b);
			consoleFrontColor(bottomPixel.r, bottomPixel.g, bottomPixel.b);
			printf("▄");

			topPrevPixel.r = topPixel.r;
			topPrevPixel.g = topPixel.g;
			topPrevPixel.b = topPixel.b;

			bottomPrevPixel.r = bottomPixel.r;
			bottomPrevPixel.g = bottomPixel.g;
			bottomPrevPixel.b = bottomPixel.b;
		}
	}

	refreshKeys();

	kal_dt = getTime() - kal_canvas.prevDraw;
	kal_canvas.prevDraw = getTime();
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Shapes
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

void drawPixel
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
		#define pixel kal_canvas.pixels[Y * kal_canvas.width + X]
		float t = (float)A / (float)255;

		pixel.r = (int)(pixel.r * (1 - t) + R * t);
		pixel.g = (int)(pixel.g * (1 - t) + G * t);
		pixel.b = (int)(pixel.b * (1 - t) + B * t);
	}
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Texture
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

Texture initTexture(const char *PATH)
{
	int width, height;
	int components;

	unsigned char* image = stbi_load(PATH, &width, &height, &components, 4);

	Texture texture;
	texture.width = width;
	texture.height = height;
	texture.pixels = (Color4 *)malloc((width * height) * sizeof(Color4));

	for(int i = 0; i < height; i++)
	{
		for(int j = 0; j < width; j++)
		{
			#define pixel texture.pixels[i * width + j]

			pixel.r = image[i * width * 4 + j * 4];
			pixel.g = image[i * width * 4 + j * 4 + 1];
			pixel.b = image[i * width * 4 + j * 4 + 2];
			pixel.a = image[i * width * 4 + j * 4 + 3];
		}
	}

	return(texture);
}

void drawTexture(const Texture image, const int X, const int Y)
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
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Input
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

void refreshKeys()
{
	int count = 0;
	int aCount = 0;

	free(keys);
	free(arrowKeys);

	while(!keyHit())
	{
		char c = getKey();
		if(c != 27)
		{
			count++;
			keys = realloc(keys, count + 1 * sizeof(char));
			keys[count] = c;
		}
		else
		{
			aCount++;
			arrowKeys = realloc(arrowKeys, aCount + 1 * sizeof(char));
			getKey();
			arrowKeys[aCount] = getKey();
		}
	}
	system("clear");
	printf("%s", keys);
}

int isCharPressed(char ch)
{
	int i = 0;
	while(keys[i] != EOF)
	{
		if(keys[i] == ch)
			return(1);
		i++;
	}
	return(0);
}

int isArrowPressed(char dir)
{
	int i = 0;
	while(keys[i] != EOF)
	{
		if(keys[i] == dir)
			return(1);
		i++;
	}
	return(0);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Misc
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

float getTime()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);

	long int time = (long int)((tv.tv_sec) * 1000 + (tv.tv_usec) / 1000);
	float timeF = (float)(time % 10000000) / 1000;

	return(timeF);
}

float getDT()
{
	return(kal_dt);
}
