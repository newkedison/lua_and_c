/*
 * =============================================================================
 *       Filename:  main.cpp
 *    Description:  
 *        Version:  1.0
 *        Created:  2013-01-03 09:43:50
 *         Author:  newk (newk), newkedison@gmail.com
 *        Company:  
 *          Copyright (c) 2013, newk
 * =============================================================================
 */

//This demo show how to call C function from lua
//Some code from: http://www.lua.org/pil/26.1.html

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern "C"
{
#include <lua5.2/lua.h>
#include <lua5.2/lauxlib.h>
#include <lua5.2/lualib.h>
}
//This lua function call the sum() to add two value, but there is no definition
//of this function in the lua code
//We will define this function by C++
const char* lua_code = "a = 11111\n"
                       "b = 23456\n"
                       "c = sum(a, b)\n"
                       "print('In lua: c = ' .. c)";

//Here is the definition of sum function,
//the prototype must be the same as bellow
static int c_sum(lua_State* L)
{
  //Use luaL_check* to ensure the type of argument
  //if type mismatch and cannot be auto convert, this will raise an error
  //(NOTE, '111' can auto convert to 111 if we use luaL_checkint)
  //if type match, the value will be return
  int a = luaL_checkint(L, -2);
  printf("c_sum: a = %d\n", a);
  int b = luaL_checkint(L, -1);
  printf("c_sum: b = %d\n", b);
  //We do not need to balance the stack, because the lua will only pop 
  //the result from stack and delete the whole stack
  lua_pushnumber(L, a + b);
  //return how many result is push to the stack
  return 1;
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

int main(int argc, const char* argv[])
{
  lua_State *L = luaL_newstate();   /* opens Lua */
  luaL_openlibs(L);  // open all lua libs
  //register the c function to lua and give it a name
  //then the lua code can call this C function by the name "sum"
  //NOTE: this is a quick-and-dirty way(see: http://www.lua.org/pil/26.1.html)
  //for a good way example, see ../lua_call_c_lib/
  lua_pushcfunction(L, c_sum);
  lua_setglobal(L, "sum");

  if (luaL_loadbuffer(L, lua_code, strlen(lua_code), "test"))
    error(L, "load lua code fail. %s\n", lua_tostring(L, -1));
  if (lua_pcall(L, 0, 0, 0))
    error(L, "lua_pcall fail. %s\n", lua_tostring(L, -1));

  lua_getglobal(L, "c");
  printf("main: c = %d", (int)lua_tonumber(L, -1));
  lua_pop(L, 1);
  lua_close(L);
  return 0;
}
