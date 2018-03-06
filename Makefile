all: compile run

compile:
		g++ `pkg-config --cflags gtk+-3.0` -o GTKTest.o Figure.cpp Coord.cpp main.cpp  `pkg-config --libs gtk+-3.0`

run:
		./GTKTest.o

