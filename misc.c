/* misc.c - misc functions used throught mazeSUD */

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include "misc.h"

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