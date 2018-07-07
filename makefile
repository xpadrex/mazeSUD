# makefile for mazeSUD

main.o: main.c objects.h
	gcc -c main.c

objects.o: objects.c objects.h
	gcc -c objects.c

mazeSUD: main.o objects.o
	gcc main.o objects.o -o mazeSUD
