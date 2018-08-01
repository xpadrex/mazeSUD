/*
 * combat.c - combat related functions
 */

#include <stdio.h>
#include <pthread.h>    
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <string.h>
#include "player.h"
#include "monsters.h"
#include "locations.h"
#include "misc.h"
#include "combat.h"

pthread_t combat;

/* combat_on(void * target) - function that is run in its own thread for player autoattacks and 
 *                            and mosnter attacks
 */
void *combat_on(void *target)
{ 
  int i;
  int player_atk;
  int monster_atk;
  int player_dmg;
  int monster_dmg;

  /* find the monster that we are attacking in the list of monsters */
  for (i = 0; i < number_of_monsters; i++) {
    if (strcasecmp(monsters[i].name, target) == 0 && 
        player.location == monsters[i].location) {
          break;
        }
  }

  printf("\n**combat on**");
  show_prompt();

  do {
    player_atk = (player.dex / 10) + 1;
    monster_atk = (monsters[i].dex / 10) + 1;

    do {
      sleep(4);
      /* player rolls initiative */
      if (randomize(1,20) >= randomize(1,20)) { 
        if (player_atk > 0 ) {
          player_dmg = randomize(player.damage / 2, player.damage) - (monsters[i].armour * 0.25);
          if (player_dmg <= 0) {
            printf("\nYou charge the %s and miss.\n", monsters[i].name);
            player_atk--;
          }
          else {
            printf("\nYou bash %s for %d damage.\n", monsters[i].name, player_dmg);
            monsters[i].health -= player_dmg;
            player_atk--;
          }
        }
        if (monster_atk > 0) {
          monster_dmg = randomize(monsters[i].damage / 2, monsters[i].damage) - (player.armour * 0.25);
          if (monster_dmg <= 0) {
            printf("The %s lunges at you and misses.\n", monsters[i].name);
          }
            printf("The %s hits you for %d damage.\n", monsters[i].name, monster_dmg);
            player.health -= monster_dmg;
            monster_atk--;
        }
        if (player.health <= 1 || monsters[i].health <= 1) {
          break;
        }
      }
      /* monster rolls initiative */
      else {
        if (monster_atk > 0) {
          monster_dmg = randomize(monsters[i].damage / 2, monsters[i].damage) - (player.armour * 0.25);
          if (monster_dmg <= 0) {
            printf("The %s charges at you and misses.\n", monsters[i].name);
            monster_atk--;
          }
          else {
            printf("\nThe %s hits you for %d damage.\n", monsters[i].name, monster_dmg);
            player.health -= monster_dmg;
            monster_atk--;
          }
        }
        if (player_atk > 0) {
          player_dmg = randomize(player.damage / 2, player.damage) - (monsters[i].armour * 0.25);
          if (player.damage <= 0) {
            printf("You swing wildly at the %s and miss.\n", monsters[i].name);
            player_atk--;
          }
          else {
            printf("\nYou bash %s for %d damage.\n", monsters[i].name, player_dmg);
            monsters[i].health -= player_dmg;
            player_atk--;
          }
        }
        if (player.health <= 1 || monsters[i].health <= 1) {
          break;
        }
      }
      show_prompt();
    } while (player_atk > 0 || monster_atk > 0);

  } while (monsters[i].health > 0 && player.health > 0);
  
  printf("**combat off**\n");
  show_prompt();

  pthread_exit(&combat);  // exit the thread when complete

  return NULL;
}

void execute_attack(const char *noun)
{
  char monster_name[15];

  strcpy(monster_name, noun);

  for (int i = 0; i < number_of_monsters; i++) {
    if (strcasecmp(monsters[i].name, noun) == 0 && 
        player.location == monsters[i].location) {

        pthread_create(&combat, NULL, combat_on, monster_name);
        return;
    }
  }
  printf("There is no %s to attack here.\n", noun);

  return;
}