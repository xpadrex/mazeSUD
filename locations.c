/* 
 * locations.c - location and movement related functions
 */

#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include "objects.h"
#include "player.h"
#include "misc.h"
#include "locations.h"
#include "monsters.h"
#include "combat.h"


location locations[250];
int number_of_locations; 
//int number_of_locations = (sizeof(locations) / sizeof(*locations));

/* execute_look() function - looks around the area of the player and 
 * reads the area description to them */
void execute_look(const char *noun)
{
  int monsters_near = 0;

  if (noun != NULL && strcasecmp(noun, "SELF") == 0) {
    look_self();
  }
  else if (noun == NULL || strcasecmp(noun, "AROUND") == 0) {
    printf("\n%s:\n", locations[player.location].tag);
    printf("  You are %s. There is", locations[player.location].description);
    if (locations[player.location].north > 0) {
      printf(" a %s to the north", locations[locations[player.location].north].tag);
    }
    if (locations[player.location].south > 0) {
      printf(" a %s to the south", locations[locations[player.location].south].tag);
    }
    if (locations[player.location].east > 0) {
      printf(" a %s to the east", locations[locations[player.location].east].tag);
    }
    if (locations[player.location].west > 0) {
      printf(" a %s to the west", locations[locations[player.location].west].tag);
    }
    if (locations[player.location].up > 0) {
      printf(" stairs leading up to a %s", locations[locations[player.location].up].tag);
    }
    if (locations[player.location].down > 0) {
      printf(" stairs leading down to a %s", locations[locations[player.location].down].tag);
    }
    printf(".\n");
    for (int i = 0; i < number_of_monsters; i++) {
      if (player.location == monsters[i].location && monsters[i].health > 0) {
        if (monsters_near == 0) {
          printf("There is a %s", monsters[i].name);
          aggro_monster(i);
          monsters_near++;
        }
        else if (monsters_near > 0) {
          printf(" and a %s", monsters[i].name);
          aggro_monster(i);
          monsters_near++;
        }
      }
    }
    if (monsters_near > 0) {
      printf(" nearby.\n");
    }
    list_objects(locations[player.location].room_id);
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
    locations[player.location].north > 0) {
      move_player(locations[player.location].north);
    }
    else if (strcasecmp(noun, "SOUTH") == 0 &&
    locations[player.location].south > 0) {
      move_player(locations[player.location].south);
    }
    else if (strcasecmp(noun, "EAST") == 0 &&
    locations[player.location].east > 0) {
      move_player(locations[player.location].east);
    }
    else if (strcasecmp(noun, "WEST") == 0 &&
    locations[player.location].west > 0) {
      move_player(locations[player.location].west);
    }
    else if (strcasecmp(noun, "DOWN") == 0 &&
    locations[player.location].down > 0) {
      move_player(locations[player.location].down);
    }
    else if (strcasecmp(noun, "UP") == 0 &&
    locations[player.location].up > 0) {
      move_player(locations[player.location].up);
    }
    else {
      printf("You can't go that way right now.\n");
    }

    return;
  }
  printf("What direction do you want to go?\n");

  return;
}

/* move_player() function - moves the player to the area passed to the function 
*/
void move_player(int direction)
{
  int i = 0;

  cancel_aggro();
  combat_off();
  
  while (direction != locations[i].room_id) {
    i++;
  }
  printf("Walking to %s\n\n", locations[i].tag);
  player.location = i;
  execute_look("around");

  return;
}

/* load_locations() function - loads the locations information from the file on disk into the locations array */
int load_locations()
{
  FILE *file = NULL;

  int i = 0;
  char buffer[256];
  char tok[256];

  file = fopen("DATA/locations.dat", "r");

  if (file == NULL) {
    printf("ERROR - Couldn't load locations.dat.\n");
    wait_for_keypress();
    return 1;
  }

  while (fgets(buffer, sizeof(buffer), file) != NULL) {
    if (strstr(buffer, "#") != NULL) {
      continue;
    }
    else {
      strcpy(tok, strtok(buffer, "\n"));
      locations[i].room_id = atoi(tok);
      printf("room_id: %d\n", locations[i].room_id);
      fgets(buffer, sizeof(buffer), file);
      strcpy(tok, strtok(buffer, "\n"));
      locations[i].tag = malloc(strlen(tok)+1);
      strcpy(locations[i].tag, tok);
      printf("Tag: %s\n", locations[i].tag);
      fgets(buffer, sizeof(buffer), file);
      strcpy(tok, strtok(buffer, "\n"));
      locations[i].description = malloc(strlen(tok)+1);
      strcpy(locations[i].description, tok);
      printf("Description: %s\n", locations[i].description);
      fgets(buffer, sizeof(buffer), file);
      strcpy(tok, strtok(buffer, ","));
      locations[i].north = atoi(tok);
      strcpy(tok, strtok(NULL, ","));
      locations[i].south = atoi(tok);
      strcpy(tok, strtok(NULL, ","));
      locations[i].east = atoi(tok);
      strcpy(tok, strtok(NULL, ","));
      locations[i].west = atoi(tok);
      strcpy(tok, strtok(NULL, ","));
      locations[i].up = atoi(tok);
      strcpy(tok, strtok(NULL, "\n"));
      locations[i].down = atoi(tok);
      printf("movement: %d,%d,%d,%d,%d,%d\n", locations[i].north,locations[i].south,
        locations[i].east,locations[i].west,locations[i].up,locations[i].down);
      i++;
    }
  }
  fclose(file);

  number_of_locations = i;
  printf("Room data loaded...\n");

  return 0;
}