/* 
 * objects.c - object and item related commands
 */

#include <stdio.h>
#include <strings.h>
#include "objects.h"
#include "player.h"
#include "misc.h"
#include "locations.h"
#include "monsters.h"

// int player_items = 0;   // count of number of items carried by the player

object objects[100];
/*
object objects[] = {
  {"crumpled map", "map", "town", 
  "The map is too damaged to be useful, it looks like it was for this area.",
   0, 0, 0},
  {"broken idol", "idol", "temple",
  "Although useless, it looks expensive.", 5, 0, 0},
  {"mug of ale", "ale", "tavern",
  "I'm sure it tastes as good as it looks.", 0, 0, 0},
  {"broken twig", "twig", "forest",
  "Looks can be decieving.", 0, 10, 0},
  {"gold coin", "coin", "forest",
  "There is a picture of a dumb faced man with a combover on one side.", 1, 0, 0},
  {"Weathered Axe", "axe", "town", "Looks like it was crafted by dwarven smiths.  Bet it will come in handy.", 1, 5, 0},
  {"Tattered Leather Vest", "vest", "town", "Everyone looks good in leather!",
   1, 0, 1},
  {"Broken Sword","sword", NULL, NULL, 10, 0, 0},
  {"Bag of goodies", "bag", NULL, NULL, 20, 0, 0},
  {"Dead Rabbit", "rabbit", NULL, NULL, 1, 0, 0}
};
*/

/* reads the size of the objects array to get the number of objects in game */
//#define number_of_objects (sizeof(objects) / sizeof(*objects))
#define number_of_objects (_number_of_objects)

/* list_objects() - function to list the objects in a location */
void list_objects(const char *here)
{
  int obj = 0;

  for (int i = 0; i < number_of_objects; i++) {
    if (strcasecmp(objects[i].location, here) == 0 && obj == 0) {
      printf("You see a %s", objects[i].description);
      obj = 1;
    }
    else if (strcasecmp(objects[i].location, here) == 0 && obj != 0) {
      printf(", a %s", objects[i].description);
    }
  }
  printf("\n");

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
        objects[i].location = player.name;
        printf("You take the %s and put it in your pack.\n", 
        objects[i].description);

        return;
      }
    }

    printf("There is no %s here.\n", noun);

    return;
  }
//  else if (player_items > 19) {
//    printf("You can't carry any more items.\n");
//
//    return;
//  }
  else {
    printf("I don't know what you want to get.\n");
  }

  return;
}

/* execute_drop() function - drops an object or equipped item from the players inventory */
void execute_drop(const char *noun)
{
  if (noun != NULL) {
    for (int i = 0; i < number_of_objects; i++) {
      if (strcasecmp(objects[i].tag, noun) == 0 && 
      objects[i].location == player.name) {
        if (player.hands != NULL && player.hands->tag == objects[i].tag) {
          player.damage = player.damage - player.hands->damage;
          player.hands = NULL;
        }
        else if (player.body != NULL && player.body->tag == objects[i].tag) {
          player.armour = player.armour - player.body->armour;
          player.body = NULL;
        }
        objects[i].location = locations[player.location].tag;

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

/* list_inventory() fuction - list all items on the player */
void list_inventory()
{
  printf("You are carrying: \n");
  for (int i = 0; i < number_of_objects; i++) {
    if (strcasecmp(objects[i].location, player.name) == 0) {
      printf("  A %s\n", objects[i].description);
    }
  }
  printf("  %d Gold coins.\n", player.gold);

  return;  
}

/* execute_equip() function - checks if the item can be equiped and if so equip
 * the item either on the body or in the hand */
void execute_equip(const char *noun)
{
  if (noun != NULL) {
    /*if (player_items == 0) {
      printf("You don't have any items to equip");

      return;
    }*/

    for (int i = 0; i < number_of_objects; i++) {
      if (strcasecmp(noun, objects[i].tag) == 0) {
        if (strcasecmp(player.name, objects[i].location) == 0) {
          if (objects[i].damage > 0) {            
            player.hands = &objects[i];
            player.damage = player.damage + player.hands->damage;
            printf("You ready the %s in your hand.\n",
                    player.hands->description);

            return;        
          }
          else if (objects[i].armour > 0) {
            player.body = &objects[i];
            player.armour = player.armour + player.body->armour;
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
          player.damage = player.damage - player.hands->damage;
          player.hands = NULL;

          return;
        }
        else if (player.body != NULL && 
                strcasecmp(player.body->tag, objects[i].tag) == 0) {
          printf("You remove the %s and put it in your pack.\n",
                  objects[i].description);
          player.armour = player.armour - player.body->armour;
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

/* this function is not complete for now */
int scan_objects(const char *search)
{
  for (int i = 0; i < number_of_objects; i++) {
    if (strcasecmp(objects[i].tag, search) == 0) {
      return 1;
    }
  }

  return 0;
}

/* init_loot() function - Takes loot with a NULL location and equips it 
 * onto an enemy NPC */
void init_loot()
{
  for (int m = 0; m < number_of_monsters; m++) {
    for (int i = 0; i < number_of_objects; i++) {
      if (objects[i].location == NULL) {
        objects[i].location = monsters[m].name;
        monsters[m].hands = &objects[i];

        break;
      }
    }
  }
  
  return;
}

/* load_equip() function - equips the item based on the tag loaded from player
 * data file */
void load_equip(const char *item)
{
  for (int i = 0; i < number_of_objects; i++) {
    if (strcasecmp(objects[i].tag, item) == 0) {
      objects[i].location = player.name;
      if (objects[i].damage > 0) {            
        player.hands = &objects[i];

        return;        
      }
      else if (objects[i].armour > 0) {
        player.body = &objects[i];
        
        return;
      }  
    }
  }
}

/* look_objects() function - searches the objects and displayes the look  
 * description if it finds the object passed to the function */
int look_objects(const char *item)
{
  for (int i = 0; i < number_of_objects; i++) {
    if (strcasecmp(objects[i].tag, item) == 0) {
      if (strcasecmp(locations[player.location].tag, objects[i].location) == 0 || 
          strcasecmp(objects[i].location, player.name) == 0) {
        printf("%s:\n  %s\n", objects[i].description, objects[i].look);
        if (objects[i].damage > 0) {
          printf("  Can be equipped in the main hand for +%d to damage.\n", objects[i].damage);
        }
        else if (objects[i].armour > 0) {
          printf("  Can be equipped for +%d armour.\n", objects[i].armour);
        }

        return 0;
      }
    }
  }

  return 1;
}
