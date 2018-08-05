/* monsters.c - file with the non player characters.   randomize their 
 * locations and create all npc structs 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <pthread.h>
#include <unistd.h>
#include "objects.h"
#include "player.h"
#include "locations.h"
#include "misc.h"
#include "monsters.h"

character types[] = {
  {"Troll", NULL, 1, 12, 5,  18, 18, 0, 6, 12, 5, 8, 5, NULL, NULL, 0, 5, 0, 0},
  {"Orc", NULL, 1, 15, 6, 20, 20, 0, 7, 14, 4, 7, 4, NULL, NULL, 0, 5, 0, 0},
  {"Wolf", NULL, 1, 8, 2, 14, 14, 0, 5, 10, 3, 8, 3, NULL, NULL, 0, 2, 0, 0},
  {"Boar", NULL, 1, 5, 3, 10, 10, 0, 4, 8, 3, 5, 3, NULL, NULL, 0, 2, 0, 0}
};

character monsters[number_of_monsters];       // set array size to value of int number_of_monsters
pthread_t respawn[number_of_monsters];        // creat array of thread pointers for respawing monsters
int monster_respawn_id[number_of_monsters];

/* reads the size of the monsters array to get the number of types in game */
#define types_of_monsters (sizeof(types) / sizeof(*types))


/* init_monsters() function - randomizes the types and location of all
 * monster npcs and creates UID's for the monsters
 */
void init_monsters()
{
  for (int i = 0; i < number_of_monsters; i++) {
    monsters[i] = types[randomize(0, (types_of_monsters - 1))];
    monsters[i].location = randomize(5, number_of_locations - 1);
    monsters[i].id = 100001 + i;
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
      printf("You eye the level %d %s.  ", monsters[i].level, monsters[i].name);
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

/* respawn_wait() thread - waits for 2min then respawns dead monster */
void *respawn_wait(void *target)
{
  int i = *(int *)target;
  sleep(45);
  monsters[i].health = monsters[i].max_health;
  pthread_exit(NULL);
  return NULL;
}

/* respawn_monster() function - respawns a killed monster after set time limit */
void respawn_monster(int m)
{
  monster_respawn_id[m] = m;

  if (monsters[m].health < 1) {
    pthread_create(&respawn[m], NULL, respawn_wait, &monster_respawn_id[m]);
  }
  return;
}