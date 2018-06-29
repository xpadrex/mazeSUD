/* 
 * location.c - location struct and LOOK and GO functions
 */

#include <stdio.h>
#include <string.h>
#include "location.h"

struct location {
  const char *description;
  const char *tag;
  const char *north;
  const char *south;
  const char *east;
  const char *west;
}

areas[] = {
  {"town square", "town", "tavern", "temple", "forest", NULL},
  {"seedy tavern", "tavern", NULL, "town", NULL, NULL},
  {"temple of the old gods","temple", "town", NULL, NULL, NULL},
  {"dark forest", "forest", NULL, NULL, "clearing", "town"},
  {"large clearing", "clearing", NULL, NULL, NULL, "forest"}
};

#define number_of_locations (sizeof(areas) / sizeof(*areas))
static int player_location = 0;

/* execute_look() function - looks around the area of the player and 
 * reads the area description to them */

void execute_look(const char *noun)
{
  if (noun != NULL && strcasecmp(noun, "AROUND") == 0) {
    printf("You are in %s.\n", areas[player_location].description);
    if (areas[player_location].north != NULL) {
      printf("There is a %s to the north.  ", areas[player_location].north);
    }
    if (areas[player_location].south != NULL) {
      printf("To the south is a %s.  ", areas[player_location].south);
    }
    if (areas[player_location].east != NULL) {
      printf("There is a %s to the east.  ", areas[player_location].east);
    }
    if (areas[player_location].west != NULL) {
      printf("To the west is a %s.  ", areas[player_location].west);
    }
    printf("\n");
  }
  else {
    printf("I'm not sure what you want to look at.\n");
  }
}

/* execute_go() function - checks the direction the player input to make sure it is a valid path,
 * if it is, calles the move_player() function */
void execute_go(const char *noun)
{
  if (noun != NULL) {
    if (strcasecmp(noun, "north") == 0 && areas[player_location].north != NULL) {
      move_player(areas[player_location].north);
    }
    else if (strcasecmp(noun, "south") == 0 && areas[player_location].south != NULL) {
      move_player(areas[player_location].south);
    }
    else if (strcasecmp(noun, "east") == 0 && areas[player_location].east != NULL) {
      move_player(areas[player_location].east);
    }
    else if (strcasecmp(noun, "west") == 0 && areas[player_location].west != NULL) {
      move_player(areas[player_location].west);
    }
    else {
      printf("You can't go that way right now.\n");
    }
  }
  return;
}


/* move_player() function - moves the player to the area passed to the function */
void move_player(const char *direction)
{
  int i = 0;

  while (strcasecmp(direction, areas[i].tag) != 0) {
    i++;
  }
  printf("Walking...\n");
  player_location = i;
  execute_look("around");

  return;
}
