/*
 * =============================================================================
 *       Filename:  lib.cpp
 *    Description:  
 *        Version:  1.0
 *        Created:  2013-01-04 08:50:02
 *         Author:  newk (newk), newkedison@gmail.com
 *        Company:  
 *          Copyright (c) 2013, newk
 * =============================================================================
 */

#include "lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern "C"
{
#include <lua5.2/lua.h>
#include <lua5.2/lauxlib.h>
#include <lua5.2/lualib.h>
}
//This file show how to create some lib function and a load function


//This function copy from: http://www.lua.org/pil/24.1.html
void error (lua_State *L, const char *fmt, ...) {
  va_list argp;
  va_start(argp, fmt);
  vfprintf(stderr, fmt, argp);
  va_end(argp);
  lua_close(L);
  exit(-1);
}

//c_sum and c_sub will be call by the lua code, these two functions do not 
//need to be export and extern in c-style

//Here is the definition of sum function,
//the prototype must be the same as bellow
static int c_sum(lua_State* L)
{
  int a = luaL_checkint(L, -2);
  printf("c_sum: a = %d\n", a);
  int b = luaL_checkint(L, -1);
  printf("c_sum: b = %d\n", b);
  lua_pushnumber(L, a + b);
  return 1;
}
static int c_sub(lua_State* L)
{
  int a = luaL_checkint(L, -2);
  printf("c_sub: a = %d\n", a);
  int b = luaL_checkint(L, -1);
  printf("c_sub: b = %d\n", b);
  lua_pushnumber(L, a - b);
  return 1;
}

//This array define which function need to be call by the lua code
static const struct luaL_Reg mylib [] = {
  {"sum", c_sum},
  {"sub", c_sub},
  {NULL, NULL}  /* sentinel */
};

//This function is the only one which need to export in c-style(see lib.h)
//The prototype can not change, because it will be called by lua
//You would better not change the name of function, so that it can support
//require() method in lua
int luaopen_lib(lua_State* L)
{
  luaL_newlib(L, mylib);
  //The #2 arg indicate the name of this lib, this name will be use in lua code
  //No need to be the same as file name, but I think that would be better
  lua_setglobal(L, "lib");
  //If lua version small than 5.2, use bellow
  //luaL_openlib(L, "lib", mylib, 0);
  return 1;
}
