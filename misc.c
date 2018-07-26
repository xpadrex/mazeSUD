/* misc.c - misc functions used throught mazeSUD */

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include "misc.h"
#include "player.h"

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
  FILE *file = NULL;

  // open the file in write binary mode
  file = fopen(name, "wb");

  // always check return values to see if it was opened okay
  if(file == NULL) {
    fprintf(stderr, "Error opening file for writing.\n");
    return 1;
  }

  // write character type struct 'player'
  fwrite(player.name, sizeof(char), strlen(player.name)+1, file);
  fwrite(player.combat_class, sizeof(char), strlen(player.combat_class)+1, file);
  fwrite(&player.level, sizeof(player.level), 1, file);
  fwrite(&player.xp, sizeof(player.xp), 1, file);
  fwrite(&player.armour, sizeof(player.armour), 1, file);
  fwrite(&player.health, sizeof(player.health), 1, file);
  fwrite(&player.max_health, sizeof(player.max_health), 1, file);
  fwrite(&player.energy, sizeof(player.energy), 1, file);
  fwrite(&player.damage, sizeof(player.damage), 1, file);
  fwrite(player.hands->tag, sizeof(char), strlen(player.hands->tag)+1, file);
  fwrite(player.body->tag, sizeof(char), strlen(player.hands->tag)+1, file);
  fwrite(&player.location, sizeof(player.location), 1, file);
  //fwrite(&player, sizeof(player), 1, file);
  // never forget to close the file
  fclose(file);

  return 0;
}

/* load_player() function - loads player data from a file in binary format */
int load_player(const char *name)
{
  FILE *file = NULL;
  file = fopen(name, "rb");

  if (file == NULL) {
    fprintf(stderr, "Error opening file for writing.\n");
    return 1;
  }

  while (1) {
    fread(&player, sizeof(player), 1, file);
    if (feof(file)) {
      break;
    }
  }

  return 0;
}