# makefile for mazeSUD

main.o: main.c location.h object.h
	gcc -c main.c

location.o: location.c location.h object.h
	gcc -c location.c

object.o: object.c location.h object.h

mazeSUD: main.o location.o object.o
	gcc main.o location.o object.o -o mazeSUD
