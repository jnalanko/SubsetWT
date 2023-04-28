main:
	g++ main.cpp ./sdsl-lite/build/lib/libsdsl.a -std=c++20 -I sdsl-lite/include/ -O3 -I include -I ./sdsl-lite/build/external/libdivsufsort/include/ -g -o main -Wno-deprecated-declarations

tests:
	g++ tests.cpp ./sdsl-lite/build/lib/libsdsl.a -std=c++20 -I sdsl-lite/include/ -I include -I ./sdsl-lite/build/external/libdivsufsort/include/ -g -o tests -Wno-deprecated-declarations
