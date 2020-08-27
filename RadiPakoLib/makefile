default_x86:
	i686-w64-mingw32-gcc RadiPako.c -c
	i686-w64-mingw32-gcc RadiPako.o -shared -s -O2 -o ../Release/32bit/libRPK.dll libinfo.o

default_x64:
	gcc RadiPako.c -c
	gcc RadiPako.o -shared -s -O2 -o ../Release/64bit/libRPK.dll libinfo_x64.o