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