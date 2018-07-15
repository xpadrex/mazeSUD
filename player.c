/*
 * player.c - player struct and player related program functions
 */

#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include "objects.h"
#include "player.h"
#include "misc.h"

static char input_name[20];             // variable for input of player name

/* declaring the player variable (character type) */
character player = {NULL, 1, 25, 100, NULL, NULL, 1};

/* 
 * create_character() function - creates the player character when you first 
 * start the game 
 */
void create_character()
{
  printf("By what name do you wish to be called? ");
  while (fgets(input_name, sizeof(input_name), stdin) == NULL);
  /* removes the newline from the end of the string */
  input_name[strlen(input_name) - 1] = '\0';
  /* converts the first letter of the name to uppercase if it isn't already */
  input_name[0] = toupper(input_name[0]);
  for (int i = 1; i != strlen(input_name); i++) {
    input_name[i] = tolower(input_name[i]);
  }
  player.name = input_name;

  printf("Hello %s, welcome to mazeSUD.\n", player.name);

  return;
}

/*
 * look_self() function - looks at the player and shows all relevant player 
 * information 
 */
void look_self() 
{
  printf("\n        Name : %s\n", player.name);
  printf("Armour Class : %d\n", player.armour);
  printf("      Health : %d\n", player.health);
  printf("      Energy : %d\n", player.energy);
  if (player.hands == NULL) {
    printf("\nYou have nothing in your hands.\n");
  }
  else {
    printf("\nYou have a %s in your main hand.\n", player.hands->description);
  }
  if (player.body == NULL) {
    printf("You aren't wearing any armour.\n");
  }
  else {
    printf("You are wearing a %s.\n", player.body->description);
  }
  
  return;
}