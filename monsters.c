/* monsters.c - file with the non player characters.   randomize their 
 * locations and create all npc structs 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <time.h>
#include "objects.h"
#include "player.h"
#include "locations.h"
#include "misc.h"
#include "monsters.h"

character types[] = {
  {"Troll", 2, 15, 0, NULL, NULL, 0},
  {"Orc", 3, 18, 0, NULL, NULL, 0},
  {"Dire wolf", 2, 12, 0, NULL, NULL, 0},
  {"Bore", 2, 10, 0, NULL, NULL, 0}
};

int number_of_monsters = 4;  // const for total monsters on map
character monsters[4];       // set array size to value of int number_of_monsters

/* reads the size of the monsters array to get the number of types in game */
#define types_of_monsters (sizeof(types) / sizeof(*types))


/* init_monsters() function - randomizes the types and location of all
 * monster npc
 */
void init_monsters()
{
  /* Intializes random number generator */
  srand(time(0));

  for (int i = 0; i < number_of_monsters; i++) {
    monsters[i] = types[rand() % number_of_monsters];
    monsters[i].location = rand() % number_of_locations;
  }

  return;
}