default_x86:
	i686-w64-mingw32-gcc main.c -s -O2 ./RadiPakoLib/*.c -o RadiPako appinfo.o

default_x64:
	gcc main.c -s -O2 ./RadiPakoLib/*.c -o RadiPako appinfo.o