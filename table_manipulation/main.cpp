/*
 * =============================================================================
 *       Filename:  main.cpp
 *    Description:  
 *        Version:  1.0
 *        Created:  2013-01-02 20:11:13
 *         Author:  newk (newk), newkedison@gmail.com
 *        Company:  
 *          Copyright (c) 2013, newk
 * =============================================================================
 */

//This demo show how to operate the lua table from C code

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern "C"
{
#include <lua5.2/lua.h>
#include <lua5.2/lauxlib.h>
#include <lua5.2/lualib.h>
}

//This function copy from: http://www.lua.org/pil/24.1.html
void error (lua_State *L, const char *fmt, ...) {
  va_list argp;
  va_start(argp, fmt);
  vfprintf(stderr, fmt, argp);
  va_end(argp);
  lua_close(L);
  exit(-1);
}

//From: http://www.lua.org/pil/25.1.html
//Before calling this function, must push the table to the top of stack
int get_field_int(lua_State* L, const char* field_name)
{
  int result;
  lua_pushstring(L, field_name);
  lua_gettable(L, -2);  /* -2 indicate the table in the stack */
  //After call lua_gettable(), the top of stack now is the result
  if (!lua_isnumber(L, -1))
    error(L, "can not read field '%s'", field_name);
  result = (int)lua_tonumber(L, -1);
  lua_pop(L, 1);  /* remove number, now the top of stack is still the table */
  return result;
}

//From: http://www.lua.org/pil/25.1.html
//Before calling this function, must push the table to the top of stack
void set_field_int(lua_State*L, const char* field_name, int value)
{
  lua_pushstring(L, field_name);
  lua_pushnumber(L, value);
  lua_settable(L, -3);
}

int main(int argc, const char* argv[])
{
  lua_State *L = luaL_newstate();
  luaL_openlibs(L);
  lua_newtable(L); //Create a empty table, and it is on the top of stack
  set_field_int(L, "a", 1);
  set_field_int(L, "b", 2);
  printf("%d %d\n", get_field_int(L, "b"), get_field_int(L, "a"));
  lua_close(L);
  return 0;
}
