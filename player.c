/*
 * player.c - player struct and player related program functions
 */

#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include "objects.h"
#include "player.h"
#include "misc.h"

/* declaring the player variable (character type) */
//character player;
character player = {NULL, NULL, 1, 0, 0, 15, 15, 100, 5, NULL, NULL, 1, 10};

attack fighter[] = {
  {"Swing", "Swing your weapon at the target for 100%% of your attack power.", 1.0, 0},
  {"Crushing Blow", "Land a crushing blow on the target for 200%% of your attack power.", 2.0, 25},
  {"Leeching Blow", "A percise strike that damages the target for 80%% of your attack power, and heals you for 50%% of the damage caused.", 0.5, 20}
};

attack caster[] = {
  {"Bolt", "Fire a bolt of energy at the target for 100%% of your attack power", 1.0, 0},
  {"Magic Missle", "Fire a magical missle at the target for 200%% of your attack power", 2.0, 25},
  {"Drain Life", "Suck the life from your enemy causing 80%% of your attack power as damage, and healing you for 50%% of the damage caused", 0.5, 20}
};

/* 
 * create_character() function - creates the player character when you first 
 * start the game 
 */
void create_character()
{
  
  int done;                                 // check for do/while loop
  static char input_name[20];             // variable for input of player name
  static char input_class[20];             // variable for input of player name

  do {
    printf("By what name shall you be called? ");
    while (fgets(input_name, sizeof(input_name), stdin) == NULL);
    remove_newline(input_name);
    // converts the first letter of the name to uppercase if it isn't already
    input_name[0] = toupper(input_name[0]);
    // converts the rest of the name to lower case
    for (int i = 1; i != strlen(input_name); i++) {
      input_name[i] = tolower(input_name[i]);
    }
    printf("%s, is that the name you would like to use? ", input_name);

    done = yes_or_no();
  } while (done < 1);

  player.name = input_name;
 
  done = 0; 
  do {
    printf("\nIn mazeSUD we have 2 types of player classes,\n");
    printf("close combat melee fighters and ranged spellcasters.\n");
    printf("What would you like to be, a Fighter or a Spellcaster? ");
    while (fgets(input_class, sizeof(input_class), stdin) == NULL);
    remove_newline(input_class);
    if (strcasecmp(input_class, "SPELLCASTER") == 0) {
      player.combat_class = "Spellcaster";
      done = 1;
    }
    else if (strcasecmp(input_class, "FIGHTER") == 0) {
      player.combat_class = "Fighter";
      done = 1;
    }
    else {
      printf("Sorry, %s is not a valid option.\n", input_class);
    }
  } while (done < 1);

  allocate_stats(10);

  printf("Hello %s the %s, welcome to mazeSUD.\n", player.name, player.combat_class);
  wait_for_keypress();

  save_player(player.name);
  return;
}

/*
 * look_self() function - looks at the player and shows all relevant player 
 * information 
 */
void look_self() 
{
  printf("\n    Name : %-15s", player.name);
  printf("   Class : %s\n", player.combat_class);
  printf("   Level : %-15d", player.level);
  printf("     Exp : %d\n", player.xp);
  printf("  Armour : %-15d", player.armour);
  printf(" Atk Pwr : %d\n", player.damage);
  printf("  Health : %d/%d\n", player.health, player.max_health);
  printf("  Energy : %d/100\n", player.energy);
  if (player.hands == NULL) {
    printf("\nYou have nothing in your hands.\n");
  }
  else {
    printf("\nYou have a %s in your main hand.\n", player.hands->description);
  }
  if (player.body == NULL) {
    printf("You aren't wearing any armour.\n");
  }
  else {
    printf("You are wearing a %s.\n", player.body->description);
  }
  
  return;
}

/* player_stats() function - lets the player allocate stats towards attack power or health */
void allocate_stats(int points)
{
  char i[10];
  int s;
  int done = 0;

  do {
    look_self();
    printf("\nYou have %d stat points to allocate towards health or attack power,\n", points);
    printf("how would you like to allocate them?\nHEALTH, ATTACK or DONE when complete: ");
    while (fgets(i, sizeof(i), stdin) == NULL);
    remove_newline(i);
    if (strcasecmp(i, "HEALTH") == 0) {
      printf("How many would you like to allocate to Health? [%d]", points);
      while (fgets(i, sizeof(i), stdin) == NULL);
      sscanf(i, "%d", &s);
      if (s == points) {
        player.health = player.health + points;
        player.max_health = player.max_health + points;
        points = 0;
      }
      else if (s < points) {
        player.health = player.health + s;
        player.max_health = player.max_health + s;
        points = points - s;
      }
      else if (s > points) {
        printf("Please enter a valid number.\n");
      }
      else {
        player.health = player.health + points;
        points = 0;
      }
    }
    else if (strcasecmp(i, "ATTACK") == 0) {
      printf("How many would you like to allocate to Attack? [%d]", points);
      while (fgets(i, sizeof(i), stdin) == NULL);
      sscanf(i, "%d", &s);
      if (s == points) {
        player.damage = player.damage + points;
        points = 0;
      }
      else if (s < points) {
        player.damage = player.damage + s;
        points = points - s;
      }
      else if (s > points) {
        printf("Please enter a valid number.\n");
      }
      else {
        player.damage = player.damage + points;
        points = 0;
      }      
    }
    else if (strcasecmp(i, "DONE") == 0) {

      return;
    }
    if (points == 0) {

      return;
    }
  } while (points > 0);

  return;
}