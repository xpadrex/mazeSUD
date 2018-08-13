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



pthread_t combat, rest, monster_combat[number_of_monsters];    
int monster_loc[number_of_monsters];                   // local variable to be used in threads

/* combat_on(void * target) - function that is run in its own thread for player autoattacks and  */
void *combat_on(void *target)
{ 
  int i = *(int *)target;         // store the argument passed to thread in pointer 'i'

  if (player.in_combat == 0) {
    printf(YEL "\n**combat on**" RESET);
    player.in_combat = 1;              // set to 1 because player entered combat
  }

  int player_atk = (player.dex / 20) + 1;         // players attacks/round

  do {
    while (player_atk > 0 && monsters[i].health > 0) {
      sleep(2 / (player.dex / 20 + 1));
      player_attack(i);
      player_atk--;
    } 
    
    if (monsters[i].health <= 0) {
      printf(RED "\nYou have killed the %s.  You gain %d xp", monsters[i].name, monsters[i].xp);

      respawn_monster(i);           // call function to start respawn timer

      if (monsters[i].gold > 0) {
        monsters[i].gold = randomize(1, monsters[i].gold);
        printf(" and %d gold", monsters[i].gold);
      }
      printf(".\n" RESET);
      
      monsters[i].health = 0;
      player.xp += monsters[i].xp;
      player.gold += monsters[i].gold;
      if (monsters[i].hands != NULL) {
        printf("The %s dropped a %s.\n", monsters[i].name, monsters[i].hands->description);
        monsters[i].hands->location = locations[monsters[i].location].room_id;
        monsters[i].hands = NULL;
      }
    }
    
    if (monsters[i].health > 0 && player.health > 0) {
      show_prompt();
      player_atk = (player.dex / 20) + 1;
      sleep(2 / (player.dex / 20 + 1));
    }
  } while (player.health > 0 && monsters[i].health > 0);

  
  player.in_combat = 0;          // out of combat, set to 0
  printf(YEL "**combat off**\n" RESET);
  show_prompt();

  pthread_exit(NULL);  // exit the thread when complete
  return NULL;
}

/* monster_aggroed() thread - causes the monsters in a room to start auto attacking player */
void *monster_aggroed(void *id)
{
  int i = *(int *)id;

  int monster_atk = (monsters[i].dex / 20) + 1;   // monster attacks/round

  if (monsters[i].in_combat > 0) {
    sleep (3);
  }

  sleep(1); 
  do {
    while (monster_atk > 0 && player.health > 0 && monsters[i].health > 0) {
      monster_attack(i);
      monster_atk--;
    } 

    if (player.health < 1) {
      int rez_fee;
      //combat_off();           // dead you leave combat
      //cancel_aggro();         // dead, monsters stop attacking
      printf(RED "\nYou have been killed by the %s.\n" RESET, monsters[i].name);
      rez_fee = player.gold * 0.1;
      printf("Your inventory has been left with your corpse, and you had to pay %d gold\n"
            "to the temple for your resurection.\n" RESET, rez_fee);
      for (int i = 0; i < _number_of_objects; i++) {
        if (objects[i].location == player.id && objects[i].equipped == 0) {
          objects[i].location = player.location;
        }
      }
      player.health = 0;
      player.location = 2;
      player.gold -= rez_fee;
      show_prompt();

      pthread_exit(NULL);
      return NULL;
    }

    show_prompt();
    if (player.health > 0 && monsters[i].health > 0) {
      monster_atk = (monsters[i].dex / 20) + 1;
      sleep(4);
    }
  } while (player.health > 0 && monsters[i].health > 0);

  pthread_exit(NULL);

  return NULL;
}


/* aggro_monster() function - causes monsters to start attacking when you are in their range */
void aggro_monster(int i)
{
  if (monsters[i].in_combat > 0) {
    monster_loc[i] = i;
    pthread_create(&monster_combat[i], NULL, monster_aggroed, &monster_loc[i]);
  }

  return;
}

/* execute_attack() function - checks to see if there is a valid target in range, 
 * if so creates the combat thread and enters combat */
void execute_attack(const char *noun)
{
  if (noun == NULL) {
    for (int i = 0; i < number_of_monsters; i++) {
      if (monsters[i].location == player.location && monsters[i].health > 0) {
        if (player.in_combat != 0) {
          combat_off();
        }
        printf(YEL "\n**combat on**" RESET);
        player.in_combat = 1;

        monster_loc[i] = i;          // set monster_loc to the location in the monsters array
      
        pthread_create(&combat, NULL, combat_on, &monster_loc[i]);   // start player attack thread
      
        if (monsters[monster_loc[i]].in_combat == 0) {        // if monster isn't already set to attack, start attacking
          pthread_create(&monster_combat[i], NULL, monster_aggroed, &monster_loc[i]);
        }
      
        return;
      }
    }
    printf("What do you want to attack?\n");

    return;
  }

  for (int i = 0; i < number_of_monsters; i++) {
    if (strcasecmp(monsters[i].name, noun) == 0 && 
        player.location == monsters[i].location && monsters[i].health > 0) {
      if (player.in_combat != 0) {
        combat_off();
      }
      printf(YEL "\n**combat on**" RESET);
      player.in_combat = 1;

      monster_loc[i] = i;          // set monster_loc to the location in the monsters array
      
      pthread_create(&combat, NULL, combat_on, &monster_loc[i]);   // start player attack thread
      
      if (monsters[monster_loc[i]].in_combat == 0) {        // if monster isn't already set to attack, start attacking
        pthread_create(&monster_combat[i], NULL, monster_aggroed, &monster_loc[i]);
      }
      
      return;
    }
  }
  printf("There is no %s here.\n", noun);

  return;
}

/* player_attack() function */
void player_attack(int i)
{
  int player_dmg;
  
  if (randomize(1,50) > monsters[i].dex) {
    player_dmg = randomize((player.damage + bonuses.damage) / 2, player.damage + bonuses.damage) - monsters[i].armour * 0.25;
    if (player_dmg <= 0) {
      printf(LCYN "\nYou charge the %s." RESET, monsters[i].name);
    }
    else {
      if (player.combat_class == 1) {
        printf(LRED "\nYou thwap the %s for %d damage." RESET, monsters[i].name, player_dmg);
      }
      else {
        printf(LRED "\nYou hack the %s for %d damage." RESET, monsters[i].name, player_dmg);
      }
      monsters[i].health -= player_dmg;
    }
  }
  else {
    printf(LCYN "\nThe %s avoids your attack." RESET, monsters[i].name);
  }
  fflush(stdout);

  return;
}

/* monster_attack() function */
void monster_attack(int i)
{
  int monster_dmg;

  if (randomize(1,50) > player.dex) {
    monster_dmg = randomize(monsters[i].damage / 2, monsters[i].damage) - ((player.armour + bonuses.armour) * 0.25);
    if (monster_dmg <= 0) {
      printf(CYN "\nThe %s lunges at you." RESET, monsters[i].name);
    }
    else {
      printf(RED "\nThe %s hits you for %d damage." RESET, monsters[i].name, monster_dmg);
      player.health -= monster_dmg;
    }
  }
  else {
    printf(CYN "\nYou skillfully dodge the %s attack." RESET, monsters[i].name);
  }
  fflush(stdout);

  return;
}

/* combat_off() function - stops combat completely by calling a cancel on the thread */
int combat_off()
{
  int success;            // flag to pass for success of thread closing

  if (player.in_combat != 0) {                     // if in combat, exit combat
    success = pthread_cancel(combat);       
    printf(YEL "**combat off**\n" RESET);    
    player.in_combat = 0;                          // leaving combat, set to 0 
  }
  
  return success;
}

/* cancel_aggro() function - gets monsters to stop attacking if you move locations */
int cancel_aggro()
{
  for (int i = 0; i < number_of_monsters; i++) {
    if (monsters[i].location == player.location) {
      pthread_cancel(monster_combat[i]);
    }
  }

  return 0;
}

/* resting() thread - player rests and regains 10% health every second */
void *resting()
{
  int location = player.location;
  int counter = 0;
  int hps = player.health / 100;
  if (hps < 1) {
    hps = 1;
  }

  printf(BLU "(resting)" RESET);
  fflush(stdout);
  sleep(2);
  do {
    sleep(1);
    if (player.health < player.max_health) {
      player.health = player.health + hps;
      if (player.health > player.max_health) {
        player.health = player.max_health;
      }
    }

    if (player.energy < player.max_energy) {
      player.energy++;
      if (player.energy < player.max_energy) {
        player.energy++;
      }
    }

    counter++;

    if (player.location != location || player.in_combat != 0) {  // stop resting if you move or enter combat
      show_prompt();
      pthread_exit(NULL);

      return NULL;
    }

    if (counter > 20) {
      show_prompt();
      printf(BLU "(resting)" RESET);
      fflush(stdout);
      counter = 0;
    }
  } while (player.health < player.max_health || player.energy < 100);

  show_prompt();
  pthread_exit(NULL);

  return NULL;
}

/* execute_rest() function - heals the player by 1% every second */
void execute_rest()
{ 
  for (int i = 0; i < number_of_monsters; i++) {
    if (monsters[i].location == player.location && monsters[i].health > 0) {
      printf("You can't rest with enemys nearby.\n");

      return;
    }
  }
  if (player.in_combat == 0) {
    pthread_create(&rest, NULL, resting, NULL);

    return;
  }
  else {

    printf("You cannot rest right now.\n");

    return;
  }
}

/* execute_cast() function - casts a player special ability */
void execute_cast(const char *noun)
{
  if (noun == NULL) {
    printf("You must specify a spell to cast, type SPELLS to list them.\n");

    return;
  }

  char* tok = calloc(strlen(noun)+1, sizeof(char));
  strcpy(tok, noun);
  char* spell = strtok(tok, " ");
  char* target = strtok(NULL, "\n");
  

  
  if (target == NULL) {      // attack nearest enemy if no target is given
    for (int i = 0; i < number_of_monsters; i++) {
      if (monsters[i].location == player.location && monsters[i].health > 0) {
        if (player.in_combat == 0) {
          execute_attack(monsters[i].name);
        }
        if (player.combat_class == 1) {
          printf(LYEL "\ncasting...");
          sleep(1);
        }
        cast_spell(i, spell);
                       
        return;
      }
    }
    printf("There are no targets in range.\n");

    return;
  }
  

  for (int i = 0; i < number_of_monsters; i++) {    // attack specified target if available
    if (strcasecmp(monsters[i].name, target) == 0 && 
        player.location == monsters[i].location && monsters[i].health > 0) {
      if (player.in_combat == 0) {      // puts player in combat if they aren't already
        execute_attack(target);
      }
      if (player.combat_class == 1) {
        sleep(1);
        printf(LYEL "\ncasting...");
      }
      cast_spell(i, spell);
      
            
      return;
    }
  }
  printf("That is not a valid target.\n");

  return;
}


/* cast_spell() function - casts a spell for the spell caster class */
void cast_spell(int target, const char *spell)
{
  int d;
  int h;

  for (int i = 0; spells[player.combat_class].attacks[i].tag != NULL; i++) {
    if (strcasecmp(spell, spells[player.combat_class].attacks[i].tag) == 0 && 
                    player.level >= spells[player.combat_class].attacks[i].level) {
      if (spells[player.combat_class].attacks[i].energy > player.energy) {
        printf("\nYou don't have enough energy to cast that.");

        return;
      }

      d = spells[player.combat_class].attacks[i].damage * (randomize(player.damage / 2, player.damage));
      h = spells[player.combat_class].attacks[i].healing * d;
      player.energy -= spells[player.combat_class].attacks[i].energy;
      if (player.combat_class == 1) {
        printf(LYEL "\nYou cast ");
      }
      else {
        printf(LYEL "\nYou execute a ");
      }
      printf("%s on the %s for %d damage." RESET, spells[player.combat_class].attacks[i].name, monsters[target].name, d);
      monsters[target].health -= d;

      if (h > 0 && player.health < player.max_health) {
        player.health += h;
        if (player.health > player.max_health) {
          player.health = player.max_health;
        }
        printf(LBLU "\nYou are healed for %d hp." RESET, h);
      }

      return;
    }
  }
  printf("Not a valid spell.\n");

  return;
}