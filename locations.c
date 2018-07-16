/* locations.c */

#include <stdio.h>
#include <strings.h>
#include "objects.h"
#include "player.h"
#include "misc.h"
#include "locations.h"
#include "monsters.h"

location locations[] = {
  {"Yourself", "player", NULL, NULL, NULL, NULL},
  {"town square", "town", "tavern", "temple", "forest", NULL},
  {"seedy tavern", "tavern", NULL, "town", NULL, NULL},
  {"temple of the old gods","temple", "town", NULL, NULL, NULL},
  {"dark forest", "forest", NULL, NULL, "clearing", "town"},
  {"large clearing", "clearing", NULL, NULL, "river", "forest"},
  {"raging river", "river", NULL, NULL, NULL, "clearing"}
};

/* reads the size of the locations array to get the number of locations */
//#define number_of_locations (sizeof(locations) / sizeof(*locations))
int number_of_locations = (sizeof(locations) / sizeof(*locations));

/* execute_look() function - looks around the area of the player and 
 * reads the area description to them */
void execute_look(const char *noun)
{
  if (noun != NULL && strcasecmp(noun, "SELF") == 0) {
    look_self();
  }
  else if (noun != NULL && strcasecmp(noun, "AROUND") == 0) {
    printf("You are in a %s. There is", locations[player.location].description);
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
        printf("You see a %s in the area.\n", 
        monsters[i].name);
      }

    list_objects(locations[player.location].tag);
    }
  }
  else {
    printf("I'm not sure what you want to look at.\n");
  }

  return;
}

/* execute_go() function - checks the direction the player input to make sure  
 * it is a valid path, if it is, calles the move_player() function */
void execute_go(const char *noun)
{
  if (noun != NULL) {
    if (strcasecmp(noun, "north") == 0 &&
    locations[player.location].north !=NULL) {
      move_player(locations[player.location].north);
    }
    else if (strcasecmp(noun, "south") == 0 &&
    locations[player.location].south != NULL) {
      move_player(locations[player.location].south);
    }
    else if (strcasecmp(noun, "east") == 0 &&
    locations[player.location].east != NULL) {
      move_player(locations[player.location].east);
    }
    else if (strcasecmp(noun, "west") == 0 &&
    locations[player.location].west != NULL) {
      move_player(locations[player.location].west);
    }
    else {
      printf("You can't go that way right now.\n");
    }
  }
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