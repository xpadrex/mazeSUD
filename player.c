/*
 * player.c - player struct and player related program functions
 */

#include <stdio.h>
#include <string.h>
#include <strings.h>
#include "objects.h"
#include "player.h"
#include "misc.h"

static char i[20];             // variable for input of player name

/* declaring the player variable (character type) */
character player = {NULL, 1, 25, 100, NULL, "stylish clothing"};

/* create_character() function - creates the player character when you first start the game */
void create_character()
{
  printf("By what name do you wish to be called? ");
  //scanf("%19s", i);
  while (fgets(i, sizeof(i), stdin) == NULL);
  i[strlen(i) - 1] = '\0';            // removes the "\n" from the end of fgets string
  player.name = i;

  printf("Hello %s, welcome to mazeSUD.\n", player.name);

  return;
}

/* look_self() function - looks at the player and shows all relevant player information */
void look_self() 
{
  printf("        Name : %s\n", player.name);
  printf("Armour Class : %d\n", player.armour);
  printf("      Health : %d\n", player.health);
  printf("      Energy : %d\n", player.energy);
  if (player.hands == NULL) {
    printf("\nYou have nothing in your hands.\n");
  }
  else {
    printf("\nYou have a %s equiped.\n", player.hands);
  }
  if (player.body == NULL) {
    printf("You are naked\n");
  }
  else {
    printf("You are wearing %s.\n", player.body);
  }
  list_inventory();

  return;
}