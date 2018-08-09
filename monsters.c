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
  {"Wolf", NULL, 1, 8, 2, 12, 12, 0, 5, 10, 3, 8, 3, NULL, NULL, 0, 2, 0, 0, 0},
  {"Boar", NULL, 1, 5, 3, 10, 10, 0, 5, 8, 3, 5, 3, NULL, NULL, 0, 2, 0, 0, 0},
  {"Troll", NULL, 1, 12, 5,  15, 15, 0, 7, 12, 5, 8, 5, NULL, NULL, 0, 5, 0, 0, 1},
  {"Orc", NULL, 1, 15, 6, 16, 16, 0, 7, 14, 4, 7, 4, NULL, NULL, 0, 5, 0, 0, 1},
  {"Ogre", NULL, 2, 24, 9, 22, 22, 0, 8, 18, 5, 5, 8, NULL, NULL, 0, 8, 0, 0, 1},
  {"Goblin", NULL, 2, 22, 8, 18, 18, 0, 8, 18, 5, 5, 8, NULL, NULL, 0, 14, 0, 0, 1},
  {"Dire Wolf", NULL, 3, 38, 6, 20, 20, 0, 9, 18, 5, 14, 10, NULL, NULL, 0, 6, 0, 0, 0}
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
  int duplicate = 0;    // int to keep count of how many monster locations are duplicated

  for (int i = 0; i < 8; i++) {             // initialize random lvl 1 monsters in start zone
    monsters[i] = types[randomize(0, 2)];
    monsters[i].location = randomize(5, 9);
    /* code for checking for duplicate monster locations
    do {
      monsters[i].location = randomize(5, 9);
      // check to see if the location is duplicated
      for (int check; check < i; check++) {     
        if (monsters[check].location = monsters[i].location) {
          duplicate++;
        }
      }
      if (duplicate < 2) {
        break;
      }
    } while (duplicate != 0);  // (duplicate < 1); will let only one monster per location
    duplicate = 0; */
  }

  for (int i = 6; i < number_of_monsters; i++) {   // random level 2 monsters for Dungeon
    monsters[i] = types[randomize(3, (types_of_monsters - 1))];
    monsters[i].location = randomize(10, 24);
  }
  
  
  for (int i = 0; i < number_of_monsters; i++) {    // Give monsters unique ID number
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
  sleep(300);
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