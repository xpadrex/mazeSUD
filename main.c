/*
 * main.c - Main program loop for the maze-rpg game
 */

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <unistd.h>
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
  show_prompt();
  return fgets(input, sizeof(input), stdin) != NULL;
}

/* parse_input() function - function to parse user input string into tokens 
 * 'verb' and 'noun' and run commands based on the verb/noun combination */
static int parse_input()
{ 
  char *verb = strtok(input, " \n");            // Break the input string down into 2 tokens
  char *noun = strtok(NULL, "\n");
  // char *noun1 = strtok(NULL, " \n");
  // if statements to run commands based on input verb
  if (verb != NULL) {
    if (strcasecmp(verb, "QUIT") == 0) {
      if (player.in_combat > 0) {
        printf("You cannot leave while you are in combat.\n");
      }
      else {
        printf("Setting up camp...\n");
        sleep(1);
        return 0;
      }
    }
    else if (strcasecmp(verb, "CLEAR") == 0) { 
      clear_screen();
    }
    else if (strcasecmp(verb, "LOOK") == 0) {
      execute_look(noun);
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
    else if (strcasecmp(verb, "U") == 0 &&
      locations[player.location].up > 0) {
      move_player(locations[player.location].up);
    }
    else if (strcasecmp(verb, "D") == 0 &&
      locations[player.location].down > 0) {
      move_player(locations[player.location].down);
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
    else if (strcasecmp(verb, "WEAR") == 0) {
      execute_equip(noun);
    }
    else if (strcasecmp(verb, "REMOVE") == 0) {
      execute_unequip(noun);
    }
    else if (strcasecmp(verb, "ATTACK") == 0 || strcasecmp(verb, "A") == 0) {
      execute_attack(noun);
    }
    else if (strcasecmp(verb, "REST") == 0) {
      execute_rest();
    }
    else if (strcasecmp(verb, "HELP") == 0) {
      print_file("help.txt");
    }
    else if (strcasecmp(verb, "TRAIN") == 0) {
      execute_training(noun);
    }
    else if (strcasecmp(verb, "LIST") == 0) {
      execute_list();
    }
    else if (strcasecmp(verb, "SELL") == 0) {
      execute_sell(noun);
    }
    else if (strcasecmp(verb, "BUY") == 0) {
      execute_buy(noun);
    }
    else if (strcasecmp(verb, "C") == 0 || strcasecmp(verb, "CAST") == 0) {
      execute_cast(noun);
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
  char name[20];
  char password[20];

  while (ok > 0) {
    printf("Enter your name, or NEW: ");
    while (fgets(name, sizeof(name), stdin) == NULL);
    remove_newline(name);

    if (strcasecmp(name, "NEW") == 0) {
      create_character();
      return;
    }
    else {
      // converts first character to upper case
      name[0] = toupper(name[0]);
      // converts the rest of the name to lower case
      for (int i = 1; i != strlen(name); i++) {
        name[i] = tolower(name[i]);
      }
      printf("Please enter your password: ");
      while (fgets(password, sizeof(password), stdin) == NULL);
      remove_newline(password);
      if (check_password(name, password) == 0) {
        load_player(name);
        return;
      }
      else {
        printf("Invalid player name or password.\n\n");
      }
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
  load_locations();
  //wait_for_keypress();

  clear_screen();
  print_file("title.txt");
  printf("\n  Welcome to mazeSUD v0.4 Alpha\n\n");
  
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
