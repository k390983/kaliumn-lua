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

#include "interface.h"

#include "draw.h"
#include "canvas.h"
#include "etc.h"

//----------------------------------------------------------------------------//
// global variables
//----------------------------------------------------------------------------//

int *canvas;
int *previousCanvas;

//-------- initialization / termination --------------------------------------//

int E_initialize(lua_State *L) {
	if(setTitle(luaL_checkstring(L, 1)) == -1) luaL_error(L, "invalid title");
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
	int color = colorCode(luaL_checkstring(L, 3));

	initCanvas(width, height, color);

	return(0);

}

int E_cleanCanvas(lua_State *L) {
	int color = colorCode(luaL_checkstring(L, 1));

	cleanCanvas(color);

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
	int color = colorCode(luaL_checkstring(L, 3));

	drawPixel(x, y, color);

	//printf("draw");

	return(0);

}

int E_drawTexture(lua_State *L) {
	int x = luaL_checknumber(L, 1);
	int y = luaL_checknumber(L, 2);

	char path[25];
	sprintf(path, "%s", luaL_checkstring(L, 3));

	printf("%s", path);
	getchar();

	drawTexture(x, y, path);

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

// returns time in miliseconds

int E_getTime(lua_State *L) {
	struct timeval  tv;
	gettimeofday(&tv, NULL);

	long int time = (long int)((tv.tv_sec) * 1000 + (tv.tv_usec) / 1000);

	lua_pushnumber(L, time);

	return(1);

}
