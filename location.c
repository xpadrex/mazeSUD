/* 
 * location.c - location struct and LOOK and GO functions
 */

#include <stdio.h>
#include <string.h>
#include "location.h"

typedef enum {town, tavern, temple, forest, clearing} path ;

struct location {
  const char *description;
  const char *tag;
  const int *exits[4];
}

areas[] = {
  {"town square", "town", {tavern, temple, forest, NULL}},
  {"seedy tavern", "tavern", {NULL, town, NULL, NULL}},
  {"temple of the old gods","temple", {town, NULL, NULL, NULL}},
  {"dark forest", "forest", {NULL, NULL, clearing, town}},
  {"large clearing", "clearing", {NULL, NULL, NULL, forest}}
};

#define number_of_locations (sizeof(areas) / sizeof(*areas))
static int player_location = 0;

/* execute_look() function - looks around the area of the player and 
 * reads the area description to them */

void execute_look(const char *noun)
{
  if (noun != NULL && strcasecmp(noun, "AROUND") == 0) {
    printf("You are in %s.\n", areas[player_location].description);
    printf("Possible exits: ");
    /*    
    for (int i = 1; areas[player_location].exits[i] != NULL; i++) {
      printf(", %s", areas[player_location].exits[i]);
    }
    printf("\n");
    */
    if (areas[player_location].exits[0] != NULL) {
      printf("North ");
    }
    if (areas[player_location].exits[1] != NULL) {
      printf("South ");
    }
    if (areas[player_location].exits[2] != NULL) {
      printf("East ");
     }
    if (areas[player_location].exits[3] != NULL) {
      printf("West");
    }
    printf("\n");
  }
  else {
    printf("I'm not sure what you want to look at.\n");
  }
}

/* execute_go() function - searches the locations list to make sure 
 * the location exists and then checks to make sure your not there 
 * already.  If your not already there moves you to that location */
void execute_go(const char *noun)
{
  if (noun != NULL) {
    if (strcasecmp(noun, "north") == 0 && areas[player_location].exits[0] != NULL) {
      move_player(areas[player_location].exits[0]);
    }
    else if (strcasecmp(noun, "south") == 0 && areas[player_location].exits[1] != NULL) {
      move_player(areas[player_location].exits[1]);
    }
    else if (strcasecmp(noun, "east") == 0 && areas[player_location].exits[2] != NULL) {
      move_player(areas[player_location].exits[2]);
    }
    else if (strcasecmp(noun, "west") == 0 && areas[player_location].exits[3] != NULL) {
      move_player(areas[player_location].exits[3]);
    }
    else {
      printf("Are you sure you know where your going?\n");
    }
  }
  return;
}


/* move_player() function - moves the player to the area passed to the function */
void move_player(const int *direction)
{
  printf("Walking...\n");
  player_location = direction;
  execute_look("around");

  return;
}
