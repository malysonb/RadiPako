default:
	g++ main.cpp -s -O2 ./src/*.cpp -o RadiPako -Wall 
	cd ./src/ && \
	make default