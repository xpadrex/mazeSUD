# makefile for mazeSUD

mazeSUD: main.o objects.o player.o misc.o locations.o
	gcc main.o objects.o player.o misc.o locations.o -o mazeSUD

main.o: main.c objects.h player.h misc.h locations.h
	gcc -c main.c

objects.o: objects.c objects.h player.h misc.h locations.h
	gcc -c objects.c

player.o: player.c objects.h player.h misc.h locations.h
	gcc -c player.c

misc.o: misc.c objects.h player.h misc.h locations.h
	gcc -c misc.c

locations.o: locations.c objects.h player.h misc.h locations.h
	gcc -c locations.c

clean:
	rm mazeSUD main.o objects.o player.o misc.o locations.o
