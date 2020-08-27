default_x86:
	i686-w64-mingw32-gcc main.c -s -O2 ./RadiPakoLib/*.c -o ./Release/32bit/RadiPako appinfo.o
	cd ./RadiPakoLib/ && \
	make default_x86
	cd ..

default_x64:
	gcc main.c -s -O2 ./RadiPakoLib/*.c -o ./Release/64bit/RadiPako appinfo_x64.o
	cd ./RadiPakoLib/ && \
	make default_x64
	cd ..