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

//static char input_name[20];             // variable for input of player name

/* declaring the player variable (character type) */
character player = {NULL, NULL, 1, 0, 0, 25, 25, 100, 5, NULL, NULL, 1};

attack warrior[] = {
  {"Attack", "Attack the target for 100%% of your attack power", 1.0, 0},
  {"Crushing Blow", "Land a crushing blow on the target for 200%% of your attack power", 2.0, 25}
};

attack mage[] = {
  {"Bolt", "Fire a bolt of energy at the target for 100%% of your attack power", 1.0, 0},
  {"Magic Missle", "Fire a magical missle at the target for 200%% of your attack power", 2.0, 25}
};

/* 
 * create_character() function - creates the player character when you first 
 * start the game 
 */
void create_character()
{
  int ok;                                 // check for do/while loop
  static char input_name[20];             // variable for input of player name
  static char input_class[20];             // variable for input of player name

  do {
    printf("By what name do you wish to be called? ");
    while (fgets(input_name, sizeof(input_name), stdin) == NULL);
    remove_newline(input_name);
    /* removes the newline from the end of the string */
    //input_name[strlen(input_name) - 1] = '\0';
    // converts the first letter of the name to uppercase if it isn't already
    input_name[0] = toupper(input_name[0]);
    // converts the rest of the name to lower case
    for (int i = 1; i != strlen(input_name); i++) {
      input_name[i] = tolower(input_name[i]);
    }
    printf("%s, is that the name you would like to use? ", input_name);

    ok = yes_or_no();
  } while (ok < 1);

  player.name = input_name;

  ok = 0; 
  do {
    printf("\nIn mazeSUD we have 2 types of player classes, close combat melee fighters\n");
    printf("and ranged casters.\nWhat would you like to be, a fighter or a caster? ");
    while (fgets(input_class, sizeof(input_class), stdin) == NULL);
    remove_newline(input_class);
    if (strcasecmp(input_class, "CASTER") == 0) {
      player.combat_class = "Caster";
      ok = 1;
    }
    else if (strcasecmp(input_class, "FIGHTER") == 0) {
      player.combat_class = "Fighter";
      ok = 1;
    }
    else {
      printf("Sorry, %s is not a valid option.\n", input_class);
    }
  } while (ok < 1);

  printf("Hello %s the %s, welcome to mazeSUD.\n", player.name, player.combat_class);
  wait_for_keypress();

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
  printf("      Health : %d/%d\n", player.health, player.max_health);
  printf("      Energy : %d/100\n", player.energy);
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