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
    printf(LMAG "\n%s:\n" RESET, locations[player.location].tag);
    printf("    %s\n", locations[player.location].description);
    list_objects(locations[player.location].room_id);
    print_exits(player.location);
    printf("\n" RESET);
    for (int i = 0; i < number_of_monsters; i++) {
      if (player.location == monsters[i].location && monsters[i].health > 0) {
        if (monsters_near == 0) {
          if (monsters[i].in_combat == 1) {
            printf(YEL "There is a " LRED "%s" YEL, monsters[i].name);
          }
          else {
            printf(YEL "There is a %s", monsters[i].name);
          }
          aggro_monster(i);
          monsters_near++;
        }
        else if (monsters_near > 0) {
          printf(" and a ");
          if (monsters[i].in_combat == 1) {
            printf(RED "%s" YEL, monsters[i].name);
          }
          else {
            printf("%s", monsters[i].name);
          }
          aggro_monster(i);
          monsters_near++;
        }
      }
    }
    if (monsters_near > 0) {
      printf(" nearby.\n" RESET);
    }    
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

/* move_player() function - moves the player in the direction passed to the function */
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
  char buffer[500];
  char tok[500];

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
      //printf("room_id: %d\n", locations[i].room_id);
      fgets(buffer, sizeof(buffer), file);
      strcpy(tok, strtok(buffer, "\n"));
      locations[i].tag = malloc(strlen(tok)+1);
      strcpy(locations[i].tag, tok);
      //printf("Tag: %s\n", locations[i].tag);
      fgets(buffer, sizeof(buffer), file);
      strcpy(tok, strtok(buffer, "\n"));
      locations[i].description = malloc(strlen(tok)+1);
      strcpy(locations[i].description, tok);
      //printf("Description: %s\n", locations[i].description);
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
      //printf("movement: %d,%d,%d,%d,%d,%d\n", locations[i].north,locations[i].south,
      //  locations[i].east,locations[i].west,locations[i].up,locations[i].down);
      i++;
    }
  }
  fclose(file);

  number_of_locations = i;
  printf("Room data loaded...\n");

  return 0;
}

/* print_exits() function - will nicely format and print available exits in a room */
void print_exits(int loc)
{
  int number_of_exits = 0;
  char exits[6][10];
  int north = 0;
  int south = 0;
  int east = 0;
  int west = 0;
  int up = 0;
  int down = 0;

  if (locations[loc].north > 0) {
    number_of_exits++;
  }
  if (locations[loc].south > 0) {
    number_of_exits++;
  }
  if (locations[loc].east > 0) {
    number_of_exits++;
  }
  if (locations[loc].west > 0) {
    number_of_exits++;
  }
  if (locations[loc].up > 0) {
    number_of_exits++;
  }
  if (locations[loc].down > 0) {
    number_of_exits++;
  }

  for (int i = 0; i < number_of_exits; i++) {
    if (locations[loc].north > 0 && north == 0) {
      strcpy(exits[i], "north");
      north = 1;
    }
    else if (locations[loc].south > 0 && south == 0) {
      strcpy(exits[i], "south");
      south = 1;
    }
    else if (locations[loc].east > 0 && east == 0) {
      strcpy(exits[i], "east");
      east = 1;
    }
    else if (locations[loc].west > 0 && west == 0) {
      strcpy(exits[i], "west");
      west = 1;
    }
    else if (locations[loc].up > 0 && up == 0) {
      strcpy(exits[i], "up");
      up = 1;
    }
    else if (locations[loc].down > 0 && down == 0) {
      strcpy(exits[i], "down");
      down = 1;
    }
  }

  printf(LGRN "Exits: ");
  if (number_of_exits == 1) {
    printf("%s." RESET, exits[0]);
  }
  else if (number_of_exits == 2) {
    printf("%s and %s." RESET, exits[0], exits[1]);
  }
  else if (number_of_exits == 3) {
    printf("%s, %s and %s." RESET, exits[0], exits[1], exits[2]);
  }
  else if (number_of_exits == 4) {
    printf("%s, %s, %s and %s." RESET, exits[0], exits[1], exits[2], exits[3]);
  }
  else if (number_of_exits == 5) {
    printf("%s, %s, %s, %s and %s." RESET, exits[0], exits[1], exits[2], exits[3], exits[4]);
  }

  return;
}

/* execute_show_exits() function - shows a detailed list of the exits */
void execute_show_exits()
{
  printf(LCYN "Obvious Exits:\n" RESET);
  if (locations[player.location].north > 0) {
    printf("  NORTH - %s\n", locations[locations[player.location].north].tag);
  }
  if (locations[player.location].south > 0) {
    printf("  SOUTH - %s\n", locations[locations[player.location].south].tag);
  }
  if (locations[player.location].east > 0) {
    printf("  EAST  - %s\n", locations[locations[player.location].east].tag);
  }
  if (locations[player.location].west > 0) {
    printf("  WEST  - %s\n", locations[locations[player.location].west].tag);
  }
  if (locations[player.location].up > 0) {
    printf("  UP    - %s\n", locations[locations[player.location].up].tag);
  }
  if (locations[player.location].down > 0) {
    printf("  DOWN  - %s\n", locations[locations[player.location].down].tag);
  }

  return;
}