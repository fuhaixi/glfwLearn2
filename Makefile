VPATH = include/objects
objects := $(wildcard include/objects/*.o)
libs := -lglad -lglfw3dll

main: 
	g++ src/main.cpp $(objects) -o output/main -Iinclude -Llib $(libs)

run:
	output/main.exe

clear:
	rm output/main.exe