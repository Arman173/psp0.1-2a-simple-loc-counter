# makefile for compiling and build the project
CC      = g++
CFLAGS  = -std=c++11 -Wall
LDFLAGS = -lm

all: program-2a clean

program-2a: main.o locc.o
	$(CC) -o $@ $^ $(LDFLAGS)

main.o: main.cpp
	$(CC) -o $@ -c $(CFLAGS) $<

# COMPILAMOS LOS COMPONENTES/MODULOS DEL PROGRAMA
locc.o: components/locc.cpp components/locc.h
	$(CC) -o $@ -c $(CFLAGS) $<

.PHONY: clean cleanest

clean:
	rm *.o

cleanest: clean
	rm program-1a

