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
	{"initCanvas", E_initCanvas},
	{"cleanCanvas", E_cleanCanvas},
	{"display", E_display},
	{"drawPixel", E_drawPixel},
	{"drawTexture", E_drawTexture},
	{"printString", E_printString},
	{"waitForKeyPress", E_waitForKeyPress},
	{"setColor", E_setColor},
	{"moveCursor", E_moveCursor},
	{"getTime", E_getTime},
	{NULL, NULL}

};

int luaopen_kaliumn (lua_State *L){
	luaL_newlib(L, kaliumn);
	return 1;

}
