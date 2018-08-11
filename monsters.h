/* monsters.h - header file for monsters.c to define all globals */

#include "player.h"

extern void init_monsters();
extern int look_monsters(const char *name);
extern void respawn_monster(int m);
void check_for_duplicate(int current, int low, int high);

#ifndef __MONSTERS__
#define __MONSTERS__
extern character monsters[];
#define number_of_monsters 20
#endif