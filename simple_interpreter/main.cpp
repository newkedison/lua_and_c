/*
 * =============================================================================
 *       Filename:  main.cpp
 *    Description:  
 *        Version:  1.0
 *        Created:  2013-01-01 20:52:38
 *         Author:  newk (newk), newkedison@gmail.com
 *        Company:  
 *          Copyright (c) 2013, newk
 * =============================================================================
 */

// example from: http://www.lua.org/pil/24.1.html
// modify by newk to run with lua 5.2
// compile with: gcc main.cpp -llua5.2
#include <stdio.h>
#include <string.h>
extern "C"
{
#include <lua5.2/lua.h>
#include <lua5.2/lauxlib.h>
#include <lua5.2/lualib.h>
}

int main (void) {
  char buff[256];
  int error;
  lua_State *L = luaL_newstate();   /* opens Lua */
  luaL_openlibs(L);  // open all lua libs

  while (fgets(buff, sizeof(buff), stdin) != NULL) {
    error = luaL_loadbuffer(L, buff, strlen(buff), "line") ||
      lua_pcall(L, 0, 0, 0);
    if (error) {
      fprintf(stderr, "%s", lua_tostring(L, -1));
      lua_pop(L, 1);  /* pop error message from the stack */
    }
  }

  lua_close(L);
  return 0;
}
