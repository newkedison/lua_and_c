/*
 * =============================================================================
 *       Filename:  main.cpp
 *    Description:  
 *        Version:  1.0
 *        Created:  2013-01-06 18:52:12
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

//This DEMO show lua code how to use userdata to access data struct created by c

//This function copy from: http://www.lua.org/pil/24.1.html
void error (lua_State *L, const char *fmt, ...) {
  va_list argp;
  va_start(argp, fmt);
  vfprintf(stderr, fmt, argp);
  va_end(argp);
  lua_close(L);
  exit(-1);
}

//Identify for metatable save in lua registy
//Every type of struct need a different ID,
//this can prevent the lua code from passing other struct to the c function
//see: http://www.lua.org/pil/28.2.html 
const char* ARRAY_ID = "lua_demo.array";

//This struct is defined in c code, we will write some function 
//to help the lua code to access it
struct c_array
{
  int size;
  int array[1]; //As an dynamic array, take one element for a hole
};

//Create a c_array for the lua code, store as userdata
static int newarray(lua_State* L)
{
  //Current stack
  //    |                 |                                             
  //    +-----------------+                                             
  //    |   array_size    |                                             
  //    +-----------------+                                             
  int size = luaL_checkint(L, -1);
  //This function help to check the range of arguments
  luaL_argcheck(L, size > 0, -1, "invalid size");
  //lua_newuserdata malloc n byte of memory and return the pointer
  c_array* arr = (c_array*)lua_newuserdata(L,
    sizeof(c_array) + sizeof(int) * (size - 1));
  //Current stack
  //    |                 |
  //    +-----------------+
  //    |userdata(c_array)|
  //    +-----------------+
  arr->size = size;
  memset(arr->array, 0, sizeof(int) * size);
  //Get the metatable in registy, ARRAY_ID is the key
  luaL_getmetatable(L, ARRAY_ID);
  //Current stack
  //    |                 |                                             
  //    +-----------------+                                             
  //    |  metatable      |                                             
  //    +-----------------+                                             
  //    |userdata(c_array)|                                             
  //    +-----------------+                                             
  //                                                                    
  //userdata.metatable = metatable
  lua_setmetatable(L, -2);
  //Current stack
  //    |                                |
  //    +--------------------------------+
  //    |userdata(c_array) with metatable|
  //    +--------------------------------+
  printf("Create a new c_array, size = %d\n", size);
  return 1;
}

//This function check the bottom element of the stack is a c_array userdata
c_array* check_array(lua_State* L)
{
  //Check the metatable of the bottome element,
  //make sure is the same as registy[ARRAY_ID]
  c_array* arr = (c_array*)luaL_checkudata(L, 1, ARRAY_ID);
  luaL_argcheck(L, arr != NULL, 1, "invalid array");
  //Return the pointer to the struct
  return arr;
}

//This function check the #2 element, it must be an int and in the range [1, size]
int check_index(lua_State* L, c_array* arr)
{
  int index = luaL_checkint(L, 2);
  luaL_argcheck(L, index >= 1 && index <= arr->size, 2, "invalid index");
  return index;
}

//Set the value of an element in the array
static int array_set_value(lua_State* L)
{
  //Current stack
  //    |                 |                                             
  //    +-----------------+                                             
  //    |    value        |                                             
  //    +-----------------+                                             
  //    |    index        |                                             
  //    +-----------------+                                             
  //    |userdata(c_array)|                                             
  //    +-----------------+                                             
  c_array* arr = check_array(L);
  int index = check_index(L, arr);
  int value = luaL_checkint(L, 3);
  arr->array[index - 1] = value;
  return 0;
}

//Get the value of an element in the array
static int array_get_value(lua_State* L)
{
  //Current stack
  //    |                 |                                             
  //    +-----------------+                                             
  //    |    index        |                                             
  //    +-----------------+                                             
  //    |userdata(c_array)|                                             
  //    +-----------------+                                             
  c_array* arr = check_array(L);
  int index = check_index(L, arr);
  lua_pushnumber(L, arr->array[index - 1]);
  return 1;
}

//Get the size of array
static int array_size(lua_State* L)
{
  //Current stack
  //    |                 |                                             
  //    +-----------------+                                             
  //    |userdata(c_array)|                                             
  //    +-----------------+                                             
  c_array* arr = check_array(L);
  lua_pushnumber(L, arr->size);
  return 1;
}

//This function will be call when lua code `print(arr)`
static int array_tostring(lua_State* L)
{
  //Current stack
  //    |                 |                                             
  //    +-----------------+                                             
  //    |userdata(c_array)|                                             
  //    +-----------------+                                             
  c_array* arr = check_array(L);
  lua_pushfstring(L, "Array size = %d", arr->size);
  //Current stack
  //    |                 |                                             
  //    +-----------------+                                             
  //    |Array size = XX  |                                             
  //    +-----------------+                                             
  //    |userdata(c_array)|                                             
  //    +-----------------+                                             
  return 1;
}

//This function will be call when lua garbage collection destroy the object
static int array_gc(lua_State* L)
{
  printf("Array is destroy\n");
}

//These functions will register to the array itself, like
//arr = {{"new", function() ... end},
//       {"set", function() ... end},
//       ...}
static const struct luaL_Reg function_list[] = 
{
  {"new", newarray},
  {"set", array_set_value},
  {"get", array_get_value},
  {"size", array_size},
  {NULL, NULL}
};

//These functions will register to the metatable of array, like
//local metatable = getmetatable(arr)
//metatable.__newindex = fcuntion() ... end
//metatable.__index = fcuntion() ... end
//...
static const struct luaL_Reg metatable_function_list[] =
{
  {"__newindex", array_set_value},
  {"__index", array_get_value},
  {"__len", array_size},
  {"__tostring", array_tostring},
  {"__gc", array_gc},
  {NULL, NULL}
};

//Here is the lua code to use the struct defined in c
const char* lua_code =
  //Create a new userdata
  "arr = array.new(100)\n"
  "print('Type: ' .. type(arr))\n" //Type: userdata
  //Call the __tostring in metatable
  "print(arr)\n" //Array size: 100
  "print('arr[1] = ' .. array.get(arr, 1))\n" //arr[1] = 0
  "array.set(arr, 1, 10)\n"
  "print('arr[1] = ' .. array.get(arr, 1))\n" //arr[1] = 10
  "print('array size = ' .. array.size(arr))\n" //array size = 100
  "arr[3] = 13\n"
  //arr[3] call the __index() function in metatable
  "print('arr[3] = ' .. arr[3])\n" //arr[3] = 13
  //In lua code, we can not use setmetatable to replace a metatable of a userdata
  //but we can use getmetatable to get it and change its content
  "local metatable = getmetatable(arr)\n"
  //Here is a magic, see bellow
  "metatable.__index = metatable\n"
  //arr.set equal to arr.metatable.__index.set, then with the above line
  //equal to arr.metatable.set and then equal to array.set
  "metatable.set = array.set\n"
  "metatable.get = array.get\n"
  "metatable.size = array.size\n"
  //Equal to arr.set(arr, 2, 12)
  "arr:set(2, 12)\n"
  "print('arr[2] = ' .. arr:get(2))\n"
  "print('array size = ' .. arr:size())\n";

int main(int argc, const char* argv[])
{
  lua_State *L = luaL_newstate();   /* opens Lua */
  luaL_openlibs(L);  // open all lua libs
  //Create a table, add to registy, and put in on the top of stack
  luaL_newmetatable(L, ARRAY_ID);
  //Current stack
  //    |                 |                                             
  //    +-----------------+                                             
  //    |empty metatable  |                                             
  //    +-----------------+                                             
  //Add function to the metatable
  luaL_setfuncs(L, metatable_function_list, 0);
  //Create a new table and add function to it
  luaL_newlib(L, function_list);
  //Current stack
  //    |                         |                                             
  //    +-------------------------+                                             
  //    |    table with functions |                                             
  //    +-------------------------+                                             
  //    |metatable with functions |                                             
  //    +-------------------------+                                             
  //Put the top element in stack to lua and named it as "array"
  //In lua code: array = {...}
  lua_setglobal(L, "array");
  //Load and call the lua_code
  if (luaL_loadbuffer(L, lua_code, strlen(lua_code), "test"))
    error(L, "load lua code fail. %s\n", lua_tostring(L, -1));
  if (lua_pcall(L, 0, 0, 0))
    error(L, "lua_pcall fail. %s\n", lua_tostring(L, -1));
  //After lua_close(), lua will call the garbage collection to destroy all object
  //if an object has a __gc metamethod, it will be called
  lua_close(L);
  return 0;
}
