# makefile for mazeSUD

main.o: main.c location.h
	gcc -c main.c

location.o: location.c location.h
	gcc -c location.c

mazeSUD: main.o location.o
	gcc main.o location.o -o mazeSUD
