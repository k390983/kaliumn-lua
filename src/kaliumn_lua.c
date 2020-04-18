//============================================================================//
// Lua bindings
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

//-------- kaliumn.h ---------------------------------------------------------//

#include "kaliumn.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Window
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

int E_getWinX(lua_State *L)
{
	lua_pushnumber(L, getWinX());

	return(1);
}

int E_getWinY(lua_State *L)
{
	lua_pushnumber(L, getWinY());

	return(1);
}

int E_setWinTitle(lua_State *L)
{
	setWinTitle(luaL_checkstring(L, 1));

	return(0);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Initialisation / Termination
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

int E_init(lua_State *L)
{
	init(luaL_checkstring(L, 1));

	return(0);
}

int E_term(lua_State *L)
{
	term();

	return(0);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Canvas
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

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

int E_displayCanvasAll(lua_State *L)
{
	displayCanvasAll();

	return(0);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Shapes
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

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

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Textures
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

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
			#define pixel texture.pixels[i * texture.width + j]

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
	texture.pixels = (Color4 *)malloc((width * height) * sizeof(Color4));
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

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Misc
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

int E_waitForKeyPress(lua_State *L)
{
	getchar();

	return(0);
}

int E_getTime(lua_State *L)
{
	lua_pushnumber(L, getTime());

	return(1);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Registering library
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

const struct luaL_Reg kaliumn_lua[] =
{
	// Window
	{"getWinX", E_getWinX},
	{"getWinY", E_getWinY},
	{"setWinTitle", E_setWinTitle},
	// Initialisation / Termination
	{"init", E_init},
	{"term", E_term},
	// Canvas
	{"initCanvas", E_initCanvas},
	{"cleanCanvas", E_cleanCanvas},
	{"displayCanvas", E_displayCanvas},
	{"displayCanvasAll", E_displayCanvasAll},
	// Shapes
	{"drawPixel", E_drawPixel},
	// Textures
	{"initTexture", E_initTexture},
	{"drawTexture", E_drawTexture},
	// Misc
	{"waitForKeyPress", E_waitForKeyPress},
	{"getTime", E_getTime},
	{NULL, NULL}
};

int luaopen_kaliumn(lua_State *L)
{
	luaL_newlib(L, kaliumn_lua);
	return 1;
}
