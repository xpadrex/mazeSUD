/* 
 * locations.c - location and movement related functions
 */

#include <stdio.h>
#include <strings.h>
#include "objects.h"
#include "player.h"
#include "misc.h"
#include "locations.h"
#include "monsters.h"

/* location information - description, tag, N, S, E, W */
location locations[] = {
  {"Yourself", "player", NULL, NULL, NULL, NULL},
  {"in the town square", "Town", "tavern", "temple", "forest", "hall"},
  {"at the temple of the old gods","Temple", "town", NULL, NULL, NULL},
  {"in a seedy tavern", "Tavern", NULL, "town", NULL, NULL},
  {"in the Hall of Training.  Statues of decorated warriors line the halls.  \
  There is a battle hardened trainer in the corner, and an arms merchant in the back", "hall", NULL, NULL, "town", NULL},
  {"in a thick dark forest", "Forest", NULL, NULL, "clearing", "town"},
  {"in a large clearing", "Clearing", NULL, NULL, "river", "forest"},
  {"at a washed out river bank", "River", NULL, NULL, NULL, "clearing"}
};

/* reads the size of the locations array to get the number of locations */
//#define number_of_locations (sizeof(locations) / sizeof(*locations))
int number_of_locations = (sizeof(locations) / sizeof(*locations));

/* execute_look() function - looks around the area of the player and 
 * reads the area description to them */
void execute_look(const char *noun)
{
  int monsters_near = 0;

  if (noun != NULL && strcasecmp(noun, "SELF") == 0) {
    look_self();
  }
  else if (noun == NULL || strcasecmp(noun, "AROUND") == 0) {
    clear_screen();
    printf("%s:\n", locations[player.location].tag);
    printf("  You are %s. There is", locations[player.location].description);
    if (locations[player.location].north != NULL) {
      printf(" a %s to the north", locations[player.location].north);
    }
    if (locations[player.location].south != NULL) {
      printf(" a %s to the south", locations[player.location].south);
    }
    if (locations[player.location].east != NULL) {
      printf(" a %s to the east", locations[player.location].east);
    }
    if (locations[player.location].west != NULL) {
      printf(" a %s to the west", locations[player.location].west);
    }
    printf(".\n");
    for (int i = 0; i < number_of_monsters; i++) {
      if (player.location == monsters[i].location) {
        if (monsters_near == 0) {
          printf("There is a %s", monsters[i].name);
          monsters_near++;
        }
        else if (monsters_near > 0) {
          printf(" and a %s", monsters[i].name);
          monsters_near++;
        }
      }
    }
    if (monsters_near > 0) {
      printf(" nearby.\n");
    }
    list_objects(locations[player.location].tag);
  }
  else {
    if (look_objects(noun) == 0) {

      return;
    }
    else if (look_monsters(noun) == 0) {

      return;
    }
    printf("There isn't any %s here.\n", noun);
  }

  return;
}

/* execute_go() function - checks the direction the player input to make sure  
 * it is a valid path, if it is, calles the move_player() function */
void execute_go(const char *noun)
{
  if (noun != NULL) {
    if (strcasecmp(noun, "NORTH") == 0 &&
    locations[player.location].north !=NULL) {
      move_player(locations[player.location].north);
    }
    else if (strcasecmp(noun, "SOUTH") == 0 &&
    locations[player.location].south != NULL) {
      move_player(locations[player.location].south);
    }
    else if (strcasecmp(noun, "EAST") == 0 &&
    locations[player.location].east != NULL) {
      move_player(locations[player.location].east);
    }
    else if (strcasecmp(noun, "WEST") == 0 &&
    locations[player.location].west != NULL) {
      move_player(locations[player.location].west);
    }
    else {
      printf("You can't go that way right now.\n");
    }

    return;
  }
  printf("Where do you want to go?\n");

  return;
}

/* move_player() function - moves the player to the area passed to the function 
*/
void move_player(const char *direction)
{
  int i = 0;

  while (strcasecmp(direction, locations[i].tag) != 0) {
    i++;
  }
  printf("Walking to %s\n\n", locations[i].tag);
  player.location = i;
  execute_look("around");

  return;
}