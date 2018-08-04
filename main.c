/*
 * main.c - Main program loop for the maze-rpg game
 */

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "objects.h"
#include "player.h"
#include "misc.h"
#include "locations.h"
#include "monsters.h"
#include "combat.h"


static char input[60];                         // buffer for keyboard input

/* get_input() function - gets keyboard input from the user and stores it in 
 * the static array 'input[]' */
static int get_input()
{
  printf("\n[%d|%d]> ", player.health, player.energy);                              // Player prompt
  return fgets(input, sizeof(input), stdin) != NULL;
}

/* parse_input() function - function to parse user input string into tokens 
 * 'verb' and 'noun' and run commands based on the verb/noun combination */
static int parse_input()
{
  char *verb = strtok(input, " \n");            // Break the input string down into 2 tokens
  char *noun = strtok(NULL, "\n");
  // if statements to run commands based on input verb
  if (verb != NULL) {
    if (strcasecmp(verb, "QUIT") == 0) {
      return 0;
    }
    else if (strcasecmp(verb, "CLEAR") == 0) { 
      clear_screen();
    }
    else if (strcasecmp(verb, "LOOK") == 0) {
      execute_look(noun);
    }
    else if (strcasecmp(verb, "GO") == 0) {
      execute_go(noun);
    }
    /* Commands for singe letter directions */
    else if (strcasecmp(verb, "N") == 0 &&
      locations[player.location].north > 0) {
      move_player(locations[player.location].north);
    }
    else if (strcasecmp(verb, "S") == 0 &&
      locations[player.location].south > 0) {
      move_player(locations[player.location].south);
    }
    else if (strcasecmp(verb, "E") == 0 &&
      locations[player.location].east > 0) {
      move_player(locations[player.location].east);
    }
    else if (strcasecmp(verb, "W") == 0 &&
      locations[player.location].west > 0) {
      move_player(locations[player.location].west);
    }
    /* end of direction comands */
    else if (strcasecmp(verb, "INVENTORY") == 0) {
      list_inventory();
    }
    else if (strcasecmp(verb, "GET") == 0) {
      execute_get(noun);
    }
    else if (strcasecmp(verb, "DROP") == 0) {
      execute_drop(noun);
    }
    else if (strcasecmp(verb, "EQUIP") == 0) {
      execute_equip(noun);
    }
    else if (strcasecmp(verb, "UNEQUIP") == 0) {
      execute_unequip(noun);
    }
    else if (strcasecmp(verb, "ATTACK") == 0) {
      execute_attack(noun);
    }
    else if (strcasecmp(verb, "REST") == 0) {
      execute_rest();
    }
    else {
      printf("I'm sorry, I don't know what you mean by %s.\n", verb);
    }
  }

  return 1;
}

/* main login menu to load or create new character */
void main_menu()
{
  int ok = 1;
  char name[15];

  while (ok > 0) {
    printf("Enter your name, or NEW: ");
    while (fgets(name, sizeof(name), stdin) == NULL);
    remove_newline(name);

    if (strcasecmp(name, "NEW") == 0) {
      create_character();
      ok = 0;
    }
    else {
      // converts first character to upper case
      name[0] = toupper(name[0]);
      // converts the rest of the name to lower case
      for (int i = 1; i != strlen(name); i++) {
      name[i] = tolower(name[i]);
    }
      ok = load_player(name);
    }
  }

  return;
}

/* mazeSUD main program loop */
int main()
{
  /* Intializes random number generator - ONCE ONLY */
  srand(time(0));
  
  load_objects();

  clear_screen();
  printf("\nWelcome to mazeSUD v0.4 Alpha\n\n");
  printf("uncopyright 2018  by: xpadrex\n");
  printf("Design and lore   by: toiletbrush\n\n");

  load_player_list(); 
  init_monsters();      // randomize monster locations
  init_loot();          // randomize loot for mosters
  main_menu();   
  clear_screen();
  execute_look("around");

  while (get_input() && parse_input() && player.health >= 1);

  printf("\nThanks for playing!\n");
  
  save_player(player.name);
  save_objects();
  

  return 0;
}
