/*
 * =============================================================================
 *       Filename:  main.cpp
 *    Description:  
 *        Version:  1.0
 *        Created:  2013-01-05 20:03:38
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

//This DEMO introduce some useful functions for controlling array and string
//See: http://www.lua.org/pil/27.1.html
//     http://www.lua.org/pil/27.2.html

//This function copy from: http://www.lua.org/pil/24.1.html
void error (lua_State *L, const char *fmt, ...) {
  va_list argp;
  va_start(argp, fmt);
  vfprintf(stderr, fmt, argp);
  va_end(argp);
  lua_close(L);
  exit(-1);
}

//This function will join the given array of strings to one string 
//and add a separator between each of them
int c_join(lua_State* L)
{
  //ensure the first parameter is a table
  luaL_checktype(L, 1, LUA_TTABLE);
  //ensure the second parameter is a string and get it
  const char* spliter = luaL_checkstring(L, 2);
  //get how many elements in the table(use luaL_getn() before lua5.2)
  int len = luaL_len(L, 1);
  //create a buffer to save the result temporarily, 
  //this way will prevent form pushing many temp value to the stack, 
  //so that we can get a better performance
  luaL_Buffer buf;
  //The buffer must be init first
  luaL_buffinit(L, &buf);
  for (int i = 1; i <= len - 1; ++i)
  {
    //Get the #i element in the array which stay in the #1 place of stack
    lua_rawgeti(L, 1, i);
    //Add the value in the top of stack to the buffer
    //Althougth the string is in the top of stack, we can not use:
    //  luaL_addstring(&buf, tostring(L, -1))
    //to add it, because that will break the balance of the stack
    //To deal with this common work, lua provide this luaL_addvalue() function
    //to add the top of stack to the buffer
    //see the last part in http://www.lua.org/pil/27.2.html for more detail
    luaL_addvalue(&buf);
    //Add the spliter to buffer
    luaL_addstring(&buf, spliter);
  }
  //Note we do not add the last element to buffer, because we don't need
  //a separator after it. We must add it after the loop
  lua_rawgeti(L, 1, len);
  luaL_addvalue(&buf);
  //After all work done, push the whole buffer to the stack
  luaL_pushresult(&buf);
  return 1;
}

//Here is the example lua code for using the c_join function
const char* lua_code = 
  "ss = {'aaa', 'ccc', 'xxx'}\n"
  "s = cjoin(ss, '.')\n"
  "print('lua:' .. s)";

int main(int argc, const char* argv[])
{
  lua_State* L = luaL_newstate();
  luaL_openlibs(L);
  //Add the c_join to lua global area, named cjoin
  lua_pushcfunction(L, c_join);
  lua_setglobal(L, "cjoin");

  //Load and run the lua code
  if (luaL_loadbuffer(L, lua_code, strlen(lua_code), "test"))
    error(L, "load lua code fail. %s\n", lua_tostring(L, -1));
  if (lua_pcall(L, 0, 0, 0))
    error(L, "lua_pcall fail. %s\n", lua_tostring(L, -1));

  //Get the result
  lua_getglobal(L, "s");
  printf("main: s = %s", lua_tostring(L, -1));
  lua_pop(L, 1);
  lua_close(L);
  return 0;
}
