# makefile for mazeSUD

mazeSUD: main.o objects.o player.o misc.o locations.o monsters.o combat.o
	gcc main.o objects.o player.o misc.o locations.o monsters.o combat.o  -o mazeSUD -lpthread

main.o: main.c objects.h player.h misc.h locations.h combat.h
	gcc -c main.c

objects.o: objects.c objects.h player.h misc.h locations.h monsters.h combat.h
	gcc -c objects.c

player.o: player.c objects.h player.h misc.h locations.h
	gcc -c player.c

misc.o: misc.c objects.h player.h misc.h locations.h 
	gcc -c misc.c

locations.o: locations.c objects.h player.h misc.h locations.h monsters.h
	gcc -c locations.c

monsters.o: monsters.c objects.h player.h locations.h misc.h monsters.h
	gcc -c monsters.c

combat.o: combat.c player.h monsters.h locations.h misc.h combat.h
	gcc -c combat.c

clean:
	rm mazeSUD main.o objects.o player.o misc.o locations.o monsters.o combat.o
