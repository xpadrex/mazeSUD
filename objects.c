/* 
 * objects.c - location struct, object struct and item and movement commands
 */

#include <stdio.h>
#include <strings.h>
#include "objects.h"
#include "player.h"
#include "misc.h"

int player_location = 1;            // start location of player
int player_items = 0;               /* count of number of items carried by the  
                                     * player */

typedef struct {
  char *description;
  char *tag;
  char *north;
  char *south;
  char *east;
  char *west;
} location;

location locations[] = {
  {"Yourself", "player", NULL, NULL, NULL, NULL},
  {"town square", "town", "tavern", "temple", "forest", NULL},
  {"seedy tavern", "tavern", NULL, "town", NULL, NULL},
  {"temple of the old gods","temple", "town", NULL, NULL, NULL},
  {"dark forest", "forest", NULL, NULL, "clearing", "town"},
  {"large clearing", "clearing", NULL, NULL, "river", "forest"},
  {"raging river", "river", NULL, NULL, NULL, "clearing"}
};

object objects[] = {
  {"crumpled map", "map", "town", 0, 0, 0},
  {"broken idol", "idol", "temple", 5, 0, 0},
  {"mug of ale", "ale", "tavern", 0, 0, 0},
  {"broken twig", "twig", "forest", 0, 1, 0},
  {"gold coin", "coin", "forest", 1, 0, 0},
  {"Weathered Axe", "axe", "town", 1, 5, 0},
  {"Tattered Leather Vest", "vest", "town", 1, 0, 1}
};

/* reads the size of the locations array to get the number of locations */
#define number_of_locations (sizeof(locations) / sizeof(*locations))

/* reads the size of the objects array to get the number of objects in game */
#define number_of_objects (sizeof(objects) / sizeof(*objects))

/* list_objects() - function to list the objects in a location */
void list_objects(const char *here)
{
  for (int i = 0; i < number_of_objects; i++) {
    if (strcasecmp(objects[i].location, here) == 0) {
      printf("You see a %s\n", objects[i].description);
    }
  }

  return;
}

/* execute_get() function - takes an item from the players location and puts it 
 * in their inventory */
void execute_get(const char *noun)
{
  if (noun != NULL) {  
    for (int i = 0; i < number_of_objects; i++) {
      if (strcasecmp(objects[i].tag, noun) == 0 && 
      strcasecmp(objects[i].location, locations[player_location].tag) == 0) {
        objects[i].location = "player";
        printf("You take the %s and put it in your pack.\n", 
        objects[i].description);
        player_items++;

        return;
      }
    }

    printf("There is no %s here.\n", noun);

    return;
  }
  else {
    printf("I don't know what you want to get.\n");
  }

  return;
}

/* execute_drop() function - drops an object from the players inventory */
void execute_drop(const char *noun)
{
  if (noun != NULL) {
    for (int i = 0; i < number_of_objects; i++) {
      if (strcasecmp(objects[i].tag, noun) == 0 && 
      objects[i].location == "player") {
        if (player.hands != NULL && player.hands->tag == objects[i].tag) {
          player.hands = NULL;
        }
        else if (player.body != NULL && player.body->tag == objects[i].tag) {
          player.body = NULL;
        }
        objects[i].location = locations[player_location].tag;
        player_items--;

        printf("You drop the %s on the ground.\n", objects[i].description);

        return;
      }
    }
    printf("You don't have a %s.\n", noun);

    return;    
  }
  else {
    printf("What do you wan't to drop?\n");
  }

  return;
}

/* list_inventory() - fuction to list all items on the player */
void list_inventory()
{
  if (player_items < 1) {
    printf("You aren't carrying anything.\n");

    return;
  }

  for (int i = 0; i < number_of_objects; i++) {
    if (strcasecmp(objects[i].location, "player") == 0) {
      printf("You have a %s\n", objects[i].description);
    }
  }

  return;  
}

/* execute_look() function - looks around the area of the player and 
 * reads the area description to them */
void execute_look(const char *noun)
{
  if (noun != NULL && strcasecmp(noun, "SELF") == 0) {
    look_self();
  }
  else if (noun != NULL && strcasecmp(noun, "AROUND") == 0) {
    printf("You are in a %s. There is", locations[player_location].description);
    if (locations[player_location].north != NULL) {
      printf(" a %s to the north", locations[player_location].north);
    }
    if (locations[player_location].south != NULL) {
      printf(" a %s to the south", locations[player_location].south);
    }
    if (locations[player_location].east != NULL) {
      printf(" a %s to the east", locations[player_location].east);
    }
    if (locations[player_location].west != NULL) {
      printf(" a %s to the west", locations[player_location].west);
    }
    printf(".\n");
    list_objects(locations[player_location].tag);
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
    locations[player_location].north !=NULL) {
      move_player(locations[player_location].north);
    }
    else if (strcasecmp(noun, "south") == 0 &&
    locations[player_location].south != NULL) {
      move_player(locations[player_location].south);
    }
    else if (strcasecmp(noun, "east") == 0 &&
    locations[player_location].east != NULL) {
      move_player(locations[player_location].east);
    }
    else if (strcasecmp(noun, "west") == 0 &&
    locations[player_location].west != NULL) {
      move_player(locations[player_location].west);
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
  player_location = i;
  execute_look("around");

  return;
}

/* execute_equip() function - checks if the item can be equiped and if so equip
 * the item either on the body or in the hand */
void execute_equip(const char *noun)
{
  if (noun != NULL) {
    if (player_items == 0) {
      printf("You don't have any items to equip");

      return;
    }

    for (int i = 0; i < number_of_objects; i++) {
      if (strcasecmp(noun, objects[i].tag) == 0) {
        if (strcasecmp("player", objects[i].location) == 0) {
          if (objects[i].damage > 0) {            
            player.hands = &objects[i];
            printf("You equip the %s.\n", player.hands->description);
        
          }
          else if (objects[i].armour > 0) {
            player.body = &objects[i];
            printf("You put on the %s.\n", player.body->description);
          }
          else {
            printf("You can't equip a %s", objects[i].description);
          }
        }
      } 
    }
  }

  return;
}