/* monsters.h - header file for monsters.c to define all globals */

#include "player.h"

extern void init_monsters();
extern int number_of_monsters;

#ifndef __MONSTERS__
#define __MONSTERS__
extern character monsters[];
#endif