/* 
 * objects.c - location struct, object struct and item and movement commands
 */

#include <stdio.h>
#include <strings.h>
#include "objects.h"
#include "player.h"
#include "misc.h"
#include "locations.h"

int player_items = 0;   // count of number of items carried by the player

/* load_objects() function - loads data from DATA/objects.dat into the objects[]
 * array 
 * THIS FUNCTION NEEDS MAJOR WORK.   I CAN'T SEEM TO GET THE HANG OF HOW TO MAKE IT WORK 
 * PROPERLY.   HAS BEEN PUT ON THE BACKBURNER FOR NOW
void load_objects()
{
  char line[250];
  int i = 0;

  FILE *in = fopen("DATA/objects.dat", "r");

  while (fscanf(in, "%99[^\n]\n", line) == 1) {
    fscanf(line, "%s,%s,%s,%d,%d,%d", &objects[i].description, &objects[i].tag,
                                &objects[i].location, objects[i].value, 
                                objects[i].damage, objects[i].armour);
    i++;
  }
  fclose(in);
  number_of_objects = i + 1;

  return;
}
*/

object objects[] = {
  {"crumpled map", "map", "town", 0, 0, 0},
  {"broken idol", "idol", "temple", 5, 0, 0},
  {"mug of ale", "ale", "tavern", 0, 0, 0},
  {"broken twig", "twig", "forest", 0, 1, 0},
  {"gold coin", "coin", "forest", 1, 0, 0},
  {"Weathered Axe", "axe", "town", 1, 5, 0},
  {"Tattered Leather Vest", "vest", "town", 1, 0, 1}
};

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
      strcasecmp(objects[i].location, locations[player.location].tag) == 0) {
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
        objects[i].location = locations[player.location].tag;
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
            printf("You ready the %s in your hand.\n",
                    player.hands->description);

            return;        
          }
          else if (objects[i].armour > 0) {
            player.body = &objects[i];
            printf("You put on the %s.\n", player.body->description);

            return;
          }
          else {
            printf("You can't equip a %s.\n", objects[i].description);

            return;
          }
        }
      } 
    }
    printf("You don't have a %s.\n", noun);
  }
  return;
}

/*
 * execute_unequip() function - removes your equipped item and returns it to
 * your inventory as a carried item
 */
void execute_unequip(const char *noun)
{
  if (noun != NULL) {
    if (player.hands == NULL && player.body == NULL) {
      printf("You don't have any items equipped.\n");

      return;
    }
    for (int i = 0; i < number_of_objects; i++) {
      if (strcasecmp(noun, objects[i].tag) == 0) {
        if (player.hands != NULL && 
            strcasecmp(player.hands->tag, objects[i].tag) == 0) {
          printf("You put the %s in your pack.\n", 
                  objects[i].description);
          player.hands = NULL;

          return;
        }
        else if (player.body != NULL && 
                strcasecmp(player.body->tag, objects[i].tag) == 0) {
          printf("You remove the %s and put it in your pack.\n",
                  objects[i].description);
          player.body = NULL;

          return;
        }
        else {
          printf("You don't have a %s equiped.\n", noun);

          return;
        }
      }
    }
    printf("You don't have a %s.\n", noun);
  }
  return;
}

int scan_objects(const char *search)
{
  for (int i = 0; i < number_of_objects; i++) {
    if (strcasecmp(objects[i].tag, search) == 0) {
      return 1;
    }
  }

  return 0;
}