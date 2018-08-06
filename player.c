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
character player  = {NULL, NULL, 1, 0, 4, 18, 18, 100,
                   4, 8, 8, 8, 8, NULL, NULL, 1, 1, 18, 0, 0};

login player_list[100];

int number_of_players;

attack fighter[] = {
  {1, "Swing", "Swing your weapon at the target for 100%% of your attack power.", 1.0, 0},
  {2, "Crushing Blow", "Land a crushing blow on the target for 200%% of your attack power.", 2.0, 25},
  {3, "Leeching Blow", "A percise strike that damages the target for 80%% of your attack power, and heals you for 50%% of the damage caused." , 0.8, 20}
};

attack caster[] = {
  {1, "Bolt", "Fire a bolt of energy at the target for 100%% of your attack power", 1.0, 0},
  {2, "Magic Missle", "Fire a magical missle at the target for 200%% of your attack power", 2.0, 25},
  {3, "Drain Life", "Suck the life from your enemy causing 80%% of your attack power as damage, and healing you for 50%% of the damage caused", 0.8, 20}
};

/* 
 * create_character() function - creates the player character when you first 
 * start the game 
 */
void create_character()
{ 
  int new_player_id = number_of_players + 1;
  
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
  player_list[new_player_id].name = player.name;
  player_list[new_player_id].password = "password";
  player.id = 1001 + (number_of_players);
  player_list[new_player_id].id = player.id;

  save_player_list(player_list[new_player_id].password);

 
  done = 0; 
  do {
    printf("\nIn mazeSUD we have 2 types of player classes, ");
    printf("close combat melee fighters and ranged spellcasters.\n");
    printf("What would you like to be, a Fighter or a Spellcaster? ");
    while (fgets(input_class, sizeof(input_class), stdin) == NULL);
    remove_newline(input_class);
    if (strcasecmp(input_class, "SPELLCASTER") == 0) {
      player.combat_class = "Spellcaster";
      player.health = 18;
      player.max_health = 18;
      player.str = 4;
      player.intel = 8;
      player.damage = 4;
      player.dex = 6;
      player.armour = 3;
      player.fort = 6;
      done = 1;
    }
    else if (strcasecmp(input_class, "FIGHTER") == 0) {
      player.combat_class = "Fighter";
      player.health = 20;
      player.max_health = 20;
      player.str = 8;
      player.intel = 4;
      player.damage = 4;
      player.dex = 6;
      player.armour = 3;
      player.fort = 6;
      done = 1;
    }
    else {
      printf("Sorry, %s is not a valid option.\n", input_class);
    }
  } while (done < 1);

  allocate_stats(player.points);
  printf("\nHello %s the %s, welcome to mazeSUD.\n", player.name, player.combat_class);
  wait_for_keypress();
  
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
  printf("  Damage : %d\n", player.damage);
  printf("     Str : %-15d", player.str);
  printf("     Int : %d\n", player.intel);
  printf("     Dex : %-15d", player.dex);
  printf("    Fort : %d\n", player.fort);
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
  char i[15];
  int s;
  int done = 0;

  do {
    look_self();
    printf("\nYou have %d stat points to allocate towards any of your 4 main stats.\n", points);
    printf("How would you like to allocate them?\n");
    printf("STR, INT, DEX, FORT, DONE when complete or HELP: ");
    while (fgets(i, sizeof(i), stdin) == NULL);
    remove_newline(i);
    if (strcasecmp(i, "STR") == 0) {
      printf("How much would you like to allocate to Strength? [%d]", points);
      while (fgets(i, sizeof(i), stdin) == NULL);
      sscanf(i, "%d", &s);
      if (s == points) {
        player.str = player.str + points;
        strength_to_damage(s);
        points = 0;
      }
      else if (s < points) {
        player.str = player.str + s;
        strength_to_damage(s);
        points = points - s;
      }
      else if (s > points) {
        printf("Please enter a valid number.\n");
      }
      else {
        player.str = player.str + points;
        strength_to_damage(points);
        points = 0;
      }      
    }
    else if (strcasecmp(i, "INT") == 0) {
      printf("How much would you like to allocate to Intellect? [%d]", points);
      while (fgets(i, sizeof(i), stdin) == NULL);
      sscanf(i, "%d", &s);
      if (s == points) {
        player.intel = player.intel + points;
        intellect_to_damage(s);
        points = 0;
      }
      else if (s < points) {
        player.intel = player.intel + s;
        intellect_to_damage(s);
        points = points - s;
      }
      else if (s > points) {
        printf("Please enter a valid number.\n");
      }
      else {
        player.intel = player.intel + points;
        intellect_to_damage(points);
        points = 0;
      }
    }
    else if (strcasecmp(i, "DEX") == 0) {
      printf("How much would you like to allocate to Dexterity? [%d]", points);
      while (fgets(i, sizeof(i), stdin) == NULL);
      sscanf(i, "%d", &s);
      if (s == points) {
        player.dex = player.dex + points;
        points = 0;
      }
      else if (s < points) {
        player.dex = player.dex + s;
        points = points - s;
      }
      else if (s > points) {
        printf("Please enter a valid number.\n");
      }
      else {
        player.dex = player.dex + points;
        points = 0;
      }
      player.armour = player.dex / 2;      
    }
    else if (strcasecmp(i, "FORT") == 0) {
      printf("How much would you like to allocate to Fortitude? [%d]", points);
      while (fgets(i, sizeof(i), stdin) == NULL);
      sscanf(i, "%d", &s);
      if (s == points) {
        player.fort = player.fort + points;
        fortitude_to_health(s);
        points = 0;
      }
      else if (s < points) {
        player.fort = player.fort + s;
        fortitude_to_health(s);
        points = points - s;
      }
      else if (s > points) {
        printf("Please enter a valid number.\n");
      }
      else {
        player.fort = player.fort + points;
        fortitude_to_health(points);
        points = 0;
      }
    }
    else if (strcasecmp(i, "DONE") == 0) {
      player.points = points;
      points = 0;
    }
    else if (strcasecmp(i, "HELP") == 0) {
      printf("\n\nThe 4 main stats in mazeSUD have various affects on your character:\n");
      printf("  STRENGTH  - Increases damage of melee attacks.\n");
      printf("  INTELLECT - Increases damage of magic and ranged attacks.\n");
      printf("  DEXTERITY - Increases armour, dodge and attack speed.\n");
      printf("  FORTITUDE - Increases your health and survival.\n\n");
      wait_for_keypress();
    }
    else {
      printf("Please enter a valid choice.\n");
    }
  } while (points > 0);
  player.points = 0;
  look_self();

  return;
}

/* function to convert added fortitude to health */
void fortitude_to_health(int points)
{
  player.max_health = player.max_health + (points / 2);
  player.health = player.max_health;

  return;
}

/* function to convert added intellect to damage if applicable */
void intellect_to_damage(int points)
{
  if (strcasecmp(player.combat_class, "SPELLCASTER") == 0) {
    player.damage = player.damage + (points / 2);
  }

  return;
}

/* function to convert added strength to damage if applicable */
void strength_to_damage(int points)
{
  if (strcasecmp(player.combat_class, "FIGHTER") == 0) {
    player.damage = player.damage + (points / 2);
  }

  return;
}