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
#include "combat.h"

// int player_items = 0;   // count of number of items carried by the player

object objects[100];

#define number_of_objects (_number_of_objects)

/* list_objects() - function to list the objects in a location */
void list_objects(int here)
{
  int obj = 0;

  for (int i = 0; i < number_of_objects; i++) {
    if (objects[i].location ==  here && obj == 0) {
      printf("You see a %s", objects[i].description);
      obj = 1;
    }
    else if (objects[i].location == here && obj != 0) {
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
  if (in_combat != 0) {    // can't do this action in combat
    combat_off();
  }

  if (noun != NULL) {  
    for (int i = 0; i < number_of_objects; i++) {
      if (strcasecmp(objects[i].tag, noun) == 0 && objects[i].location == player.location) {
        objects[i].location = player.id;
        printf("You take the %s and put it in your pack.\n", 
        objects[i].description);

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

/* execute_drop() function - drops an object or equipped item from the players inventory */
void execute_drop(const char *noun)
{
  if (in_combat != 0) {    // can't do this action in combat
    combat_off();
  }

  if (noun != NULL) {
    for (int i = 0; i < number_of_objects; i++) {
      if (strcasecmp(objects[i].tag, noun) == 0 && 
      objects[i].location == player.id) {
        if (player.hands != NULL && player.hands->tag == objects[i].tag) {
          player.damage = player.damage - player.hands->damage;
          player.hands = NULL;
        }
        else if (player.body != NULL && player.body->tag == objects[i].tag) {
          player.armour = player.armour - player.body->armour;
          player.body = NULL;
        }
        objects[i].location = player.location;

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
    if (objects[i].location == player.id) {
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
  if (in_combat != 0) {    // can't do this action in combat
    combat_off();
  }

  if (noun != NULL) {
    for (int i = 0; i < number_of_objects; i++) {
      if (strcasecmp(noun, objects[i].tag) == 0) {
        if (player.id == objects[i].location) {
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
  else {
    printf("What do you want to equip?\n");
  }
  return;
}

/*
 * execute_unequip() function - removes your equipped item and returns it to
 * your inventory as a carried item
 */
void execute_unequip(const char *noun)
{
  if (in_combat != 0) {    // can't do this action in combat
    combat_off();
  }

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
      if (objects[i].location == 0) {
        objects[i].location = monsters[m].id;
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
      objects[i].location = player.id;
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
      if (locations[player.location].room_id == objects[i].location ||
          objects[i].location == player.id) {
        printf("%s:\n  %s\n", objects[i].description, objects[i].look);
        if (objects[i].damage > 0) {
          printf("  Can be equipped in the main hand for +%d to damage.\n", objects[i].damage);
        }
        else if (objects[i].armour > 0) {
          printf("  Can be equipped on the body for +%d armour.\n", objects[i].armour);
        }

        return 0;
      }
    }
  }

  return 1;
}
