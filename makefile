default:
	gcc main.c -s -O2 ./RadiPakoLib/*.c -o RadiPako
	cd ./RadiPakoLib/ && \
	make default