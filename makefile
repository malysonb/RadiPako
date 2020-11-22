default:
	gcc main.c -s -O2 ./RadiPakoLib/*.c -o RadiPako -Wall 
	cd ./RadiPakoLib/ && \
	make default