/* monsters.h - header file for monsters.c to define all globals */

#include "player.h"

extern void init_monsters();
extern int number_of_monsters;
extern int look_monsters(const char *name);

#ifndef __MONSTERS__
#define __MONSTERS__
extern character monsters[];
#endif