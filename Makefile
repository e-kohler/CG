all: compile run

compile:
		g++ `pkg-config --cflags gtk+-3.0` -o cg_trab.o View.cpp Figure.cpp Coord.cpp main.cpp  `pkg-config --libs gtk+-3.0`

run:
		./cg_trab.o

