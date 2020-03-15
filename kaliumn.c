
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
// internal functions
//----------------------------------------------------------------------------//

static void clear() {
	printf("\033[H\033[J");

}

//----------------------------------------------------------------------------//
// external functions
//----------------------------------------------------------------------------//

static int initialize(lua_State *L) {
	clear();

}

static int terminate(lua_State *L) {
	printf("\e[%dm\e[%dm", F_DEFAULT, B_DEFAULT);
	//clear();

	return(0);

}

static int getWinX(lua_State *L) {
	struct winsize size;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);

	lua_pushnumber(L, size.ws_col);

	return(1);

}

static int getWinY(lua_State *L) {
	struct winsize size;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);

	lua_pushnumber(L, size.ws_row);

	return(1);

}

static int setColor(lua_State *L) {
	char foreground[25], background[25];
	sprintf(foreground, "%s", luaL_checkstring(L, 1));
	sprintf(background, "%s", luaL_checkstring(L, 2));

	// foreground

	if (strcmp(foreground, "black") == 0){
		printf("\e[%dm", F_BLACK);
		
	} else if (strcmp(foreground, "red") == 0){
		printf("\e[%dm", F_RED);
	
	} else if (strcmp(foreground, "green") == 0){
		printf("\e[%dm", F_GREEN);
		
	} else if (strcmp(foreground, "yellow") == 0){
		printf("\e[%dm", F_YELLOW);
		
	} else if (strcmp(foreground, "blue") == 0){
		printf("\e[%dm", F_BLUE);
		
	} else if (strcmp(foreground, "magenta") == 0){
		printf("\e[%dm", F_MAGENTA);
		
	} else if (strcmp(foreground, "cyan") == 0){
		printf("\e[%dm", F_CYAN);
		
	} else if (strcmp(foreground, "light gray") == 0){
		printf("\e[%dm", F_LIGHT_GRAY);
		
	} else if (strcmp(foreground, "dark gray") == 0){
		printf("\e[%dm", F_DARK_GRAY);
		
	} else if (strcmp(foreground, "light red") == 0){
		printf("\e[%dm", F_LIGHT_RED);
		
	} else if (strcmp(foreground, "light green") == 0){
		printf("\e[%dm", F_LIGHT_GREEN);
	
	} else if (strcmp(foreground, "light yellow") == 0){
		printf("\e[%dm", F_LIGHT_YELLOW);
		
	} else if (strcmp(foreground, "light blue") == 0){
		printf("\e[%dm", F_LIGHT_BLUE);
		
	} else if (strcmp(foreground, "light magenta") == 0){
		printf("\e[%dm", F_LIGHT_MAGENTA);
		
	} else if (strcmp(foreground, "light cyan") == 0){
		printf("\e[%dm", F_LIGHT_CYAN);
		
	} else if (strcmp(foreground, "white") == 0){
		printf("\e[%dm", F_DEFAULT);

	} else {
		printf("\e[%dm", F_DEFAULT);
	
	}

	// background

	if (strcmp(background, "black") == 0){
		printf("\e[%dm", B_BLACK);
		
	} else if (strcmp(background, "red") == 0){
		printf("\e[%dm", B_RED);
	
	} else if (strcmp(background, "green") == 0){
		printf("\e[%dm", B_GREEN);
		
	} else if (strcmp(background, "yellow") == 0){
		printf("\e[%dm", B_YELLOW);
		
	} else if (strcmp(background, "blue") == 0){
		printf("\e[%dm", B_BLUE);
		
	} else if (strcmp(background, "magenta") == 0){
		printf("\e[%dm", B_MAGENTA);
		
	} else if (strcmp(background, "cyan") == 0){
		printf("\e[%dm", B_CYAN);
		
	} else if (strcmp(background, "light gray") == 0){
		printf("\e[%dm", B_LIGHT_GRAY);
		
	} else if (strcmp(background, "dark gray") == 0){
		printf("\e[%dm", B_DARK_GRAY);
		
	} else if (strcmp(background, "light red") == 0){
		printf("\e[%dm", B_LIGHT_RED);
		
	} else if (strcmp(background, "light green") == 0){
		printf("\e[%dm", B_LIGHT_GREEN);
	
	} else if (strcmp(background, "light yellow") == 0){
		printf("\e[%dm", B_LIGHT_YELLOW);
		
	} else if (strcmp(background, "light blue") == 0){
		printf("\e[%dm", B_LIGHT_BLUE);
		
	} else if (strcmp(background, "light magenta") == 0){
		printf("\e[%dm", B_LIGHT_MAGENTA);
		
	} else if (strcmp(background, "light cyan") == 0){
		printf("\e[%dm", B_LIGHT_CYAN);
		
	} else if (strcmp(background, "white") == 0){
		printf("\e[%dm", B_DEFAULT);

	} else {
		printf("\e[%dm", B_DEFAULT);
	
	}

	return(0);

}

void setCursorPosition(lua_State *L) {
	int x = luaL_checknumber(L, 1);
	int y = luaL_checknumber(L, 2);

    printf("\033[%d;%dH",y, x);

}

static int printString(lua_State *L) {
	printf("%s", luaL_checkstring(L, 1));

	return(0);

}

//----------------------------------------------------------------------------//
// registering library
//----------------------------------------------------------------------------//

static const struct luaL_Reg kaliumn[] = {
	{"initialize", initialize},
	{"terminate", terminate},
	{"getWinX", getWinX},
	{"getWinY", getWinY},
	{"setColor", setColor},
	{"setCursorPosition", setCursorPosition},
	{"printString", printString},
	{NULL, NULL}

};

int luaopen_kaliumn (lua_State *L){
	luaL_newlib(L, kaliumn);
	return 1;

}