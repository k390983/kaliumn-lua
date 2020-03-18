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


#include "includes/interface.h"
#include "includes/draw.h"
#include "includes/canvas.h"
#include "includes/etc.h"

//----------------------------------------------------------------------------//
// global variables
//----------------------------------------------------------------------------//

extern int *canvas;
extern int *previousCanvas;

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
	{"drawTexture", E_drawTexture},
	{"display", E_display},
	{"printString", E_printString},
	{"waitForKeyPress", E_waitForKeyPress},
	{"getTime", E_getTime},
	{NULL, NULL}

};

int luaopen_kaliumn (lua_State *L){
	luaL_newlib(L, kaliumn);
	return 1;

}