both: default_x86 default_x64
	echo "usage: 'RadiPako filename.* -o output.rpk'" \
	>./Release/Readme.txt
	cp LICENSE ./Release/LICENSE

default_x86:
	i686-w64-mingw32-gcc main.c -s -O2 ./RadiPakoLib/*.c -o ./Release/32bit/RadiPako appinfo.o
	cd ./RadiPakoLib/ && \
	make default_x86

default_x64:
	gcc main.c -s -O2 ./RadiPakoLib/*.c -o ./Release/64bit/RadiPako appinfo_x64.o
	cd ./RadiPakoLib/ && \
	make default_x64