//----------------------------------------------------------------------------//
// c/lua interface
//----------------------------------------------------------------------------//

#ifndef INTERFACE
#define INTERFACE

//-------- initialization / termination --------------------------------------//

int E_initialize(lua_State *L);

int E_terminate(lua_State *L);

//-------- window ------------------------------------------------------------//

int E_setTitle(lua_State *L);

int E_getWinX(lua_State *L);

int E_getWinY(lua_State *L);

//-------- canvas functions --------------------------------------------------//

int E_initCanvas(lua_State *L);

int E_cleanCanvas(lua_State *L);

int E_display(lua_State *L);

//-------- drawing -----------------------------------------------------------//

int E_drawPixel(lua_State *L);

int E_drawTexture(lua_State *L);

//-------- utility -----------------------------------------------------------//

int E_printString(lua_State *L);

int E_waitForKeyPress(lua_State *L);

int E_setColor(lua_State *L);

int E_moveCursor(lua_State *L);

int E_getTime(lua_State *L); // returns time in miliseconds

#endif
