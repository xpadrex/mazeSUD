/*
 * main.c - Main program loop for the maze-rpg game
 */

#include <stdio.h>
#include <string.h>
#include "location.h"
#include "object.h"


static char input[60];                         // buffer for keyboard input

/* get_input() function - gets keyboard input from the user and stores it in the static array 'input[]' */
static int get_input()
{
  printf("> ");                              // Player prompt
  return fgets(input, sizeof(input), stdin) != NULL;
}

/* parse_input() funstion - function to parse user input string into tokens 'verb' and 'noun' and run commands 
 * based on the verb/noun combination */
static int parse_input()
{
  char *verb = strtok(input, " \n");            // Break the input string down into 2 tokens
  char *noun = strtok(NULL, "\n");

  // if statements to run commands based on input verb
  if (verb != NULL) {
    if (strcasecmp(verb, "QUIT") == 0) {
      return 0;
    }
    else if (strcasecmp(verb, "LOOK") == 0) {
      execute_look(noun);
    }
    else if (strcasecmp(verb, "GO") == 0) {
      execute_go(noun);
    }
    else {
      printf("I'm sorry, I don't know how to %s.\n", verb);
    }
  }

  return 1;
}

int main()
{
  printf("\n--Welcome to mazeSUD v0.1 Alpha--\n\n");
  printf("uncopyright 2018 - by: xpadrex\n\n");

  execute_look("around");

  while (get_input() && parse_input());

  printf("See you soon!\n");

  return 0;
}
