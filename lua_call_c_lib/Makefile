all: lua lib.so

lib.so: lib.o
	gcc -shared -fPIC -Wl,-soname,lib.so.1 -o lib.so.1.0 lib.o -llua5.2
	ln -f -s lib.so.1.0 lib.so.1
	ln -f -s lib.so.1 lib.so

lib.o: lib.cpp lib.h
	gcc -c -o lib.o lib.cpp

lua: main.cpp
	gcc -o lua main.cpp -llua5.2

run: lua caller.lua
	./lua

clean:
	-rm *.so*
	-rm *.o
	-rm lua
