all:
	$(MAKE) clean
	$(MAKE) linux
	$(MAKE) clean
	$(MAKE) win

CC=
RANLIB=
OBJECT=
EXTRA_LIB=
AR=

win:
	$(MAKE) "CC=i586-mingw32msvc-gcc" "RANLIB=i586-mingw32msvc-ranlib" \
		"OBJECT=lua.exe" "EXTRA_LIB=" "AR=ar rcs -o" "SYSCFLAGS=" \
		"SYSLIBS=" lua.exe

linux:
	$(MAKE) "CC=gcc" "RANLIB=ranlib" "OBJECT=lua" "EXTRA_LIB=-lm -ldl" \
		"AR=ar rcu -o" "SYSCFLAGS=-DLUA_USE_LINUX" \
		"SYSLIBS=-Wl,-E -ldl -lreadline -lncurses" lua

$(OBJECT): main.cpp lib
	$(CC) -o $@ main.cpp -Lluasrc -llua $(EXTRA_LIB)

# lua: main.cpp lib
#   $(CC) -o lua main.cpp -Lliblua -llua -lm -ldl

# lua.exe: main.cpp lib
#   $(CC) -o lua.exe main.cpp -Lluasrc -llua

lib:
	@echo "CC=$(CC)"
	@echo "AR=$(AR)"
	cd luasrc && $(MAKE) "CC=$(CC)" "LUA_A=liblua.a" "AR=$(AR)" \
		"RANLIB=$(RANLIB)" "SYSCFLAGS=$(SYSCFLAGS)" "SYSLIBS=$(SYSLIBS)" \
		liblua.a

clean:
	cd luasrc && $(MAKE) clean

cleanall: clean
	rm lua.exe lua

run: lua
	./lua

.PHONY: lib clean win linux
