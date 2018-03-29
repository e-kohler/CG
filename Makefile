all: compile run

compile:
		g++ `pkg-config --cflags  gtk+-3.0` -o cg_trab.o *.cpp `pkg-config --libs gtk+-3.0` -std=c++11
run:
		./cg_trab.o

clean: 
	rm -f main.o

