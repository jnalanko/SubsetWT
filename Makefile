all:
	g++ main.cpp ./sdsl-lite/build/lib/libsdsl.a -std=c++20 -I sdsl-lite/include/ -O3 -I include -I ./sdsl-lite/build/external/libdivsufsort/include/ -g -o main -Wno-deprecated-declarations
