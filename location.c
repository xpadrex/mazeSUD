/* 
 * location.c - location struct and LOOK and GO functions
 */

#include <stdio.h>
#include <string.h>

struct location {
  const char *description;
  const char *tag;
}

areas[] = {
  {"town square", "town"},
  {"seedy tavern", "tavern"}
};

#define number_of_locations (sizeof(areas) / sizeof(*areas))

static unsigned player_location = 0;
