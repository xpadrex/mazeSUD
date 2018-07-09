# makefile for mazeSUD

main.o: main.c objects.h player.h
	gcc -c main.c

objects.o: objects.c objects.h player.h
	gcc -c objects.c

player.o: player.c objects.h player.h
	gcc -c player.c

mazeSUD: main.o objects.o player.o
	gcc main.o objects.o player.o -o mazeSUD
