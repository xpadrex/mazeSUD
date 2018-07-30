/* misc.c - misc functions used throught mazeSUD */

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <sqlite3.h>
#include "misc.h"
#include "player.h"
#include "objects.h"
#include "locations.h"

int _number_of_objects = 0;    // local variable for the number of objects


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
  fprintf(file, "%d\n", player.str);
  fprintf(file, "%d\n", player.intel);
  fprintf(file, "%d\n", player.dex);
  fprintf(file, "%d\n", player.fort);
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
  fprintf(file, "%d\n", player.gold);

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
  fgets(str, sizeof(str), file);
  strcpy(tok, strtok(str, "\n"));
  player.combat_class = malloc(strlen(tok)+1);
  strcpy(player.combat_class,tok);
  fgets(str, sizeof(str), file);
  player.level = atoi(str);
  fgets(str, sizeof(str), file);
  player.xp = atoi(str);
  fgets(str, sizeof(str), file);
  player.armour = atoi(str);
  fgets(str, sizeof(str), file);
  player.health = atoi(str);
  fgets(str, sizeof(str), file);
  player.max_health = atoi(str);
  fgets(str, sizeof(str), file);
  player.energy = atoi(str);
  fgets(str, sizeof(str), file);
  player.damage = atoi(str);
  fgets(str, sizeof(str), file);
  player.str = atoi(str);
  fgets(str, sizeof(str), file);
  player.intel = atoi(str);
  fgets(str, sizeof(str), file);
  player.dex = atoi(str);
  fgets(str, sizeof(str), file);
  player.fort = atoi(str);
  fgets(str, sizeof(str), file);
  strcpy(tok, strtok(str, "\n"));
  if (strcasecmp(tok, "EMPTY") == 0) {
    player.hands = NULL;
  }
  else {
    load_equip(tok);
  }
  fgets(str, sizeof(str), file);
  strcpy(tok, strtok(str, "\n"));
  if (strcasecmp(tok, "EMPTY") == 0) {
    player.body = NULL;
  }
  else {
    load_equip(str);
  }
  fgets(str, sizeof(str), file);
  player.location = atoi(str);
  fgets(str, sizeof(str), file);
  player.gold = atoi(str);

  fclose(file);

  wait_for_keypress();
  return 0;
}

/* load_objects() function - loads the objects.dat database into an objects array */
void load_objects()
{
  FILE *file = NULL;
  int i = 0;              // counter for objects array
  char str[255];
  char tok[255];


  file = fopen("DATA/objects.dat", "r");

  if (file == NULL) {
    printf("Error loading object.dat.\n");
    wait_for_keypress();
    return;
  }

  while (fgets(str, sizeof(str), file) != NULL) {
    strcpy(tok, strtok(str, ","));
    objects[i].description = malloc(strlen(tok)+1);
    strcpy(objects[i].description, tok);
    strcpy(tok, strtok(NULL, ","));
    objects[i].tag = malloc(strlen(tok)+1);
    strcpy(objects[i].tag, tok);
    strcpy(tok, strtok(NULL, ","));
    objects[i].location = malloc(strlen(tok)+1);
    strcpy(objects[i].location, tok);
    if (strcasecmp(objects[i].location, "NULL") == 0) {
      objects[i].location = NULL;
    }
    strcpy(tok, strtok(NULL, ","));
    objects[i].look = malloc(strlen(tok)+1);
    strcpy(objects[i].look, tok);
    if (strcasecmp(objects[i].look, "NULL") == 0) {
      objects[i].look = NULL;
    }
    strcpy(tok, strtok(NULL, ","));
    objects[i].value = atoi(tok);
    strcpy(tok, strtok(NULL, ","));
    objects[i].damage = atoi(tok);
    strcpy(tok, strtok(NULL, ","));
    objects[i].armour = atoi(tok);

    i++;
  }
  fclose(file);
  _number_of_objects = i;

  return;
}

/* save_player() function - saves the player data to a file in binary format */
void save_objects()
{
  char *empty = "NULL";
  FILE *file = NULL;

  // open the file in write mode
  file = fopen("DATA/objects.dat", "w+");

  // always check return values to see if it was opened okay
  if(file == NULL) {
    fprintf(stderr, "Error opening objects.dat for writing.\n");
    return;
  }

  for (int i = 0; i < _number_of_objects; i++) {
    fprintf(file, "%s,%s,%s,", objects[i].description, objects[i].tag, objects[i].location);
    if (objects[i].look == NULL) {
      fprintf(file, "%s,", empty);
    }
    else {
      fprintf(file, "%s,", objects[i].look);
    }
    fprintf(file, "%d,%d,%d\n", objects[i].value, objects[i].damage, objects[i].armour);
  }

  // never forget to close the file
  fclose(file);
  
  return;
}