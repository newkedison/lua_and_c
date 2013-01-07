/*
 * =============================================================================
 *       Filename:  main.cpp
 *    Description:  
 *        Version:  1.0
 *        Created:  2013-01-02 13:26:44
 *         Author:  newk (newk), newkedison@gmail.com
 *        Company:  
 *          Copyright (c) 2013, newk
 * =============================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern "C"
{
#include <lua5.2/lua.h>
#include <lua5.2/lauxlib.h>
#include <lua5.2/lualib.h>
}

// This DEMO show how to load a lua file to read some config

//This function copy from: http://www.lua.org/pil/24.1.html
void error (lua_State *L, const char *fmt, ...) {
  va_list argp;
  va_start(argp, fmt);
  vfprintf(stderr, fmt, argp);
  va_end(argp);
  lua_close(L);
  exit(-1);
}

//This function copy from: http://www.lua.org/pil/25.html
//Modify some function name to use in lua 5.2
bool load_config (const char *filename, int *width, int *height) 
{
  lua_State *L = luaL_newstate();
  luaL_openlibs(L);

  //Load a lua file and run it
  if (luaL_loadfile(L, filename) || lua_pcall(L, 0, 0, 0))
    error(L, "cannot run configuration file: %s\n",
      lua_tostring(L, -1));

  //Let lua push the two global variables to the stack
  lua_getglobal(L, "width");
  lua_getglobal(L, "height");
  //Check the type of two variables before read them
  if (!lua_isnumber(L, -2))
    error(L, "`width' should be a number\n");
  if (!lua_isnumber(L, -1))
    error(L, "`height' should be a number\n");
  *width = (int)lua_tonumber(L, -2);
  *height = (int)lua_tonumber(L, -1);

  lua_close(L);
}
int main(int argc, const char* argv[])
{
  int width, height;
  load_config("config.lua", &width, &height);
  printf("width=%d, height=%d\n", width, height);
  return 0;
}
