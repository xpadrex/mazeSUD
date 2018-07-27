/* misc.c - misc functions used throught mazeSUD */

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include "misc.h"
#include "player.h"
#include "objects.h"

/* clear_screen() function - sends ascii codes to clear the terminal */
void clear_screen()
{
  printf("\e[1;1H\e[2J");

  return;
}

void wait_for_keypress()
{
  char any_key[2];

  printf("\nPress <ENTER> to continue...\n");
  fgets(any_key, sizeof(any_key), stdin);

  return;
}

/* yes_or_no() function - returns 0 for no, and 1 for yes */
int yes_or_no()
{
  char answer[10];

  printf("[Y/n] ");
  fgets(answer, sizeof(answer), stdin);
  answer[strlen(answer) - 1] = '\0';

  if (strcasecmp(answer, "N") == 0) {
    return 0;
  }
  else if (strcasecmp(answer, "NO") == 0) {
    return 0;
  }
  else {
    return 1;
  }
}

/* randomize() function - returns a random number in the range of min/max */
int randomize(int min, int max)
{
   return min + rand() / (RAND_MAX / (max - min + 1) + 1);
}

/* remove_newline() - removes the "\n" from the end of a string */
void remove_newline(char *s)
{
  if (strlen(s) > 1) {
    s[strlen(s) - 1] = '\0';
  }

  return;
}

/* save_player() function - saves the player data to a file in binary format */
int save_player(const char *name)
{
  char *empty = "EMPTY";
  FILE *file = NULL;

  // open the file in write mode
  file = fopen(name, "w+");

  // always check return values to see if it was opened okay
  if(file == NULL) {
    fprintf(stderr, "Error opening file for writing.\n");
    return 1;
  }

  fprintf(file, "%s\n", player.name);
  fprintf(file, "%s\n", player.combat_class);
  fprintf(file, "%d\n", player.level);
  fprintf(file, "%d\n", player.xp);
  fprintf(file, "%d\n", player.armour);
  fprintf(file, "%d\n", player.health);
  fprintf(file, "%d\n", player.max_health);
  fprintf(file, "%d\n", player.energy);
  fprintf(file, "%d\n", player.damage);
  if (player.hands == NULL) {
    fprintf(file, "%s\n", empty);
  }
  else {
    fprintf(file, "%s\n", player.hands->tag);
  }
  if (player.body == NULL) {
    fprintf(file, "%s\n", empty);
  }
  else {
    fprintf(file, "%s\n", player.body->tag);
  }
  fprintf(file, "%d\n", player.location);

  // never forget to close the file
  fclose(file);
  return 0;
}

/* load_player() function - loads player data from a file in binary format */
int load_player(const char *name)
{
  FILE *file = NULL;
  char str[25];
  char tok[25];


  file = fopen(name, "r");

  if (file == NULL) {
    printf("That user name does not exist.\n");
    return 1;
  }

  fgets(str, sizeof(str), file);
  strcpy(tok, strtok(str, "\n"));
  player.name = malloc(strlen(tok)+1);
  strcpy(player.name, tok);
  printf("%s\n", tok);
  fgets(str, sizeof(str), file);
  strcpy(tok, strtok(str, "\n"));
  player.combat_class = malloc(strlen(tok)+1);
  strcpy(player.combat_class,tok);
  printf("%s\n", tok);
  fgets(str, sizeof(str), file);
  printf("%s\n", str);
  player.level = atoi(str);
  fgets(str, sizeof(str), file);
  printf("%s\n", str);
  player.xp = atoi(str);
  fgets(str, sizeof(str), file);
  printf("%s\n", str);
  player.armour = atoi(str);
  fgets(str, sizeof(str), file);
  printf("%s\n", str);
  player.health = atoi(str);
  fgets(str, sizeof(str), file);
  printf("%s\n", str);
  player.max_health = atoi(str);
  fgets(str, sizeof(str), file);
  printf("%s\n", str);
  player.energy = atoi(str);
  fgets(str, sizeof(str), file);
  printf("%s\n", str);
  player.damage = atoi(str);
  fgets(str, sizeof(str), file);
  printf("%s\n", str);
  strcpy(tok, strtok(str, "\n"));
  if (strcasecmp(tok, "EMPTY") == 0) {
    player.hands = NULL;
  }
  else {
    load_equip(tok);
  }
  fgets(str, sizeof(str), file);
  printf("%s\n", str);
  strcpy(tok, strtok(str, "\n"));
  if (strcasecmp(tok, "EMPTY") == 0) {
    player.body = NULL;
  }
  else {
    load_equip(str);
  }
  fgets(str, sizeof(str), file);
  printf("%s\n", str);
  player.location = atoi(str);

  fclose(file);

  wait_for_keypress();
  return 0;
}