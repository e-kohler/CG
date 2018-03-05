all: compile run

compile:
		g++ `pkg-config --cflags gtk+-3.0` -o GTKTest.o Element.cpp GTKTest.cpp `pkg-config --libs gtk+-3.0`

run:
		./GTKTest.o

