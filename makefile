# makefile for mazeSUD

mazeSUD: main.o objects.o player.o misc.o
	gcc main.o objects.o player.o misc.o -o mazeSUD

main.o: main.c objects.h player.h misc.h
	gcc -c main.c

objects.o: objects.c objects.h player.h misc.h
	gcc -c objects.c

player.o: player.c objects.h player.h misc.h
	gcc -c player.c

misc.o: misc.c objects.h player.h misc.h
	gcc -c misc.c

clean:
	rm mazeSUD main.o objects.o player.o misc.o 