/* monsters.c - file with the non player characters.   randomize their 
 * locations and create all npc structs 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "objects.h"
#include "player.h"
#include "locations.h"
#include "misc.h"
#include "monsters.h"

character types[] = {
  {"Troll", NULL, 1, 15, 15,  2, 15, 0, 8, NULL, NULL, 0},
  {"Orc", NULL, 1, 20, 20, 3, 18, 0, 10, NULL, NULL, 0},
  {"Wolf", NULL, 1, 12, 12, 2, 12, 0, 6, NULL, NULL, 0},
  {"Boar", NULL, 1, 8, 8, 2, 10, 0, 4, NULL, NULL, 0}
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
  for (int i = 0; i < (number_of_monsters + 1); i++) {
    monsters[i] = types[randomize(0, (types_of_monsters - 1))];
    monsters[i].location = randomize(3, number_of_locations);
  }

  return;
}

/* look_monsters() function - searches the list of monsters and shows the look description
 * for the monster if the tag matches the string passed to the function */
int look_monsters(const char *name)
{
  for (int i = 0; i < number_of_monsters; i++) {
    if (strcasecmp(name, monsters[i].name) == 0 && 
      player.location == monsters[i].location) {
      printf("You nonchalantly eye the level %d %s.  ", 
      monsters[i].level, monsters[i].name);
      if (monsters[i].hands != NULL) {
        printf("He is carrying a %s.\n", monsters[i].hands->description);
      }
      if (player.health < monsters[i].health) {
        printf("He looks alot stronger than you are.\n");
        return 0;
      }
      else if ((player.health / 2) > monsters[i].health) {
        printf("He doesn't look chalenging at all.\n");
        return 0;
      }
      else {
        printf("He looks to be a worthy aponent.\n");
        return 0;
      }            
    }
  } 
  return 1;
}