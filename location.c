/* 
 * location.c - location struct and LOOK and GO functions
 */

#include <stdio.h>
#include <string.h>
#include "location.h"

struct location {
  const char *description;
  const char *tag;
}

areas[] = {
  {"town square", "town"},
  {"seedy tavern", "tavern"},
  {"temple of the old gods","temple"},
  {"dark forest", "forest"},
  {"open field", "field"}
};

#define number_of_locations (sizeof(areas) / sizeof(*areas))

static unsigned player_location = 0;

/* execute_look() function - looks around the area of the player and 
 * reads the area description to them */

void execute_look(const char *noun)
{
  if (noun != NULL && strcasecmp(noun, "AROUND") == 0) {
    printf("You are in %s.\n", areas[player_location].description);
  }
  else {
    printf("I'm not sure what you want to look at.\n");
  }
}

/* execute_go() function - searches the locations list to make sure the location 
 * exists and then checks to make sure your not there already.  If your not already
 * there moves you to that location */
void execute_go(const char *noun)
{
  unsigned i;
  for (i = 0; i < number_of_locations; i++) {
    if (noun != NULL && strcasecmp(noun, areas[i].tag) == 0) {
      if (i == player_location) {
        printf("You are already standing there.\n");
      }
      else {
        printf("Walking...\n");
        player_location = i;
        execute_look("around");
      }
      return;
    }
  }

  printf("I'm not sure you know where your going.\n");
}
