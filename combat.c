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

/* define the ascii codes for colours in the terminal */
#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

pthread_t combat, rest;     // threads
int monster_loc;            // local variable to be used in threads
int in_combat = 0;          // 1 if player is in combat

/* combat_on(void * target) - function that is run in its own thread for player autoattacks and 
 *                            and mosnter attacks
 */
void *combat_on(void *target)
{ 
  int i = *(int *)target;         // store the argument passed to thread in pointer 'i'
  int in_combat = 1;              // set to 1 because player entered combat

  // printf("thread i value: %d\n", i);              // uncoment this line for debugging thread arguments
  int player_atk;         // players attacks per round
  int monster_atk;
  
  printf(YEL "\n**combat on**" RESET);
  
  do {
    /* player rolls initiative */
    if (randomize(1,20) >= randomize(1,20)) { 
      do {
        player_attack(i);
        player_atk--;
      } while (player_atk > 0 && monsters[i].health > 0);
      if (monsters[i].health > 0) {
        do {
          monster_attack(i);
          monster_atk--;
        } while (monster_atk > 0 && player.health > 0);
      }
    }
    /* monster rolls initiative */
    else {
      do {
        monster_attack(i);
        monster_atk--;
      } while (monster_atk > 0 && player.health > 0);
      if (player.health > 1) {
        do {
          player_attack(i);
          player_atk--;
        } while (player_atk > 0 && monsters[i].health > 0);
      }
    }
    if (monsters[i].health <= 0) {
      printf(RED "\nYou have killed the %s.  You gain %dxp", monsters[i].name, monsters[i].xp);
      if (monsters[i].gold > 0) {
        printf(" and %d gold.\n" RESET, monsters[i].gold);
      }
      else {
        printf(".\n" RESET);
      }
      monsters[i].health = 0;
      player.xp += monsters[i].xp;
      player.gold += monsters[i].gold;
      if (monsters[i].hands != NULL) {
        printf("There was a %s on the %s corpse.\n", monsters[i].hands->description, monsters[i].name);
        monsters[i].hands->location = locations[monsters[i].location].tag;
        monsters[i].hands = NULL;
      }
    }
    else if (player.health <= 1) {
      printf(RED "\nYou have died...\n" RESET);

    }

    show_prompt();
    sleep(4);

  } while (player.health >= 1 && monsters[i].health > 0);

  in_combat = 0;          // out of combat, set to 0

  printf(YEL "**combat off**\n" RESET);

  show_prompt();

  pthread_exit(&combat);  // exit the thread when complete
  return NULL;
}

/* execute_attack() function - checks to see if there is a valid target in range, 
 * if so creates the combat thread and enters combat */
void execute_attack(const char *noun)
{
  for (int i = 0; i < number_of_monsters; i++) {
    if (strcasecmp(monsters[i].name, noun) == 0 && 
        player.location == monsters[i].location) {
      
      combat_off();

      monster_loc = i;          // set monster_loc to the location in the monsters array
      
      pthread_create(&combat, NULL, combat_on, &monster_loc);
      
      return;
    }
  }
  printf("There is no %s to attack here.\n", noun);

  return;
}

/* player_attack() function */
void player_attack(int i)
{
  int player_dmg;

  player_dmg = randomize(player.damage / 2, player.damage) - (monsters[i].armour * 0.25);
  if (player_dmg <= 0) {
    printf("\nYou charge the %s and miss.", monsters[i].name);
  }
  else {
    printf("\nYou bash %s for %d damage.", monsters[i].name, player_dmg);
    monsters[i].health -= player_dmg;
  }

  return;
}

/* monster_attack() function */
void monster_attack(int i)
{
  int monster_dmg;

  monster_dmg = randomize(monsters[i].damage / 2, monsters[i].damage) - (player.armour * 0.25);
  if (monster_dmg <= 0) {
    printf("\nThe %s lunges at you and misses.", monsters[i].name);
  }
  else {
    printf("\nThe %s hits you for %d damage.", monsters[i].name, monster_dmg);
    player.health -= monster_dmg;
  }

  return;
}

/* combat_off() function - stops combat completely by calling a cancel on the thread */
int combat_off()
{
  int success = 0;

  if (in_combat != 0) {
    success = pthread_cancel(combat);
    printf(YEL "**combat off**\n" RESET);
  }

  return success;
}
