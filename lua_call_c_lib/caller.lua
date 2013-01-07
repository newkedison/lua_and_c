require("package")
--First method to load c library
--local path = os.getenv("HOME") .. "/codetest/c_with_lua/lua_call_c_lib/lib.so"
--local f = package.loadlib(path, "luaopen_lib")
--f()

--Second method to load c library
--NOTE: There must exist a file called lib.so in package.cpath
--      and lib.so must export a function call luaopen_lib
require("lib")
a = 11
b = 223
-- These two function is difined in the lib.so
c = lib.sum(a, b)
d = lib.sub(b, a)

