/* misc.c - misc functions used throught mazeSUD */

#include <stdio.h>
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
  char any_key[10];

  printf("\nPress <ENTER> to continue...\n");
  fgets(any_key, sizeof(any_key), stdin);

  return;
}

int yes_or_no()
{
  char answer[4];

  printf("[Y/n]? ");
  fgets(answer, sizeof(answer), stdin);

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