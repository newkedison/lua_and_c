/*
 * =============================================================================
 *       Filename:  main.cpp
 *    Description:  
 *        Version:  1.0
 *        Created:  2013-01-01 22:00:01
 *         Author:  newk (newk), newkedison@gmail.com
 *        Company:  
 *          Copyright (c) 2013, newk
 * =============================================================================
 */

//Call a simple function(written by lua) from C++

#include <stdio.h>
#include <string.h>
extern "C"
{
#include <lua5.2/lua.h>
#include <lua5.2/lauxlib.h>
#include <lua5.2/lualib.h>
}

int main(int argc, const char* argv[])
{
  //lua function define
  const char* code = "function sum(a, b)\n"
                     "  print('Lua:' .. a .. '+' .. b .. '=' .. a + b)\n"
                     "  return a + b\n"
                     "end";
  int error;
  lua_State *L = luaL_newstate();   /* opens Lua */
  luaL_openlibs(L);  // open all lua libs
  error = luaL_loadbuffer(L, code, strlen(code), "Sum");
  if (error) 
  {
    fprintf(stderr, "luaL_loadbuffer:%s", lua_tostring(L, -1));
    lua_pop(L, 1);  /* pop error message from the stack */
    return -1;
  }
  //The lua function must compile first
  error = lua_pcall(L, 0, 0, 0);
  if (error)
  {
    fprintf(stderr, "lua_pcall_compile:%s\n", lua_tostring(L, -1));
    lua_pop(L, 1);  /* pop error message from the stack */
    return -1;
  }
  //Start to call the lua function
  lua_getglobal(L, "sum"); //push function name to the statck
  lua_pushinteger(L, 14);  //push first argument
  lua_pushinteger(L, 33);  //push second argument
  error = lua_pcall(L, 2, 1, 0); //call the lua function
  if (error)
  {
    fprintf(stderr, "lua_pcall:%s\n", lua_tostring(L, -1));
    lua_pop(L, 1);  /* pop error message from the stack */
    return -1;
  }
  //Get the return value from stack, check if it is an integer and get it
  int ret = (int)lua_tonumber(L, -1);
  //Balance the stack
  lua_pop(L, 1);
  printf("Result: %d\n", ret);
  lua_close(L);
  return 0;
}
