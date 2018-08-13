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
character player  = {NULL,  // name
                     0,     // combat_class
                     1,     // level
                     0,     // xp
                     4,     // armour
                     18,    // health
                     18,    // max_health
                     100,   // energy
                     100,   // max_energy
                     4,     // damage
                     4,     // magic
                     8,     // defence
                     8,     // str
                     8,     // int
                     8,     // wis
                     8,     // dex
                     8,     // fort
                     NULL,  // hands
                     NULL,  // body
                     1,     // location
                     1,     // gold
                     15,    // skill points
                     0,     // user id
                     0};    // combat on/off flag

login player_list[100];

bonus bonuses = {0, 0, 0, 0};

int number_of_players;

/*
player_classes[] = {{{"Fighter"},
                   {{2, "Crushing Blow", "CB", "Land a crushing blow on the target for 150%% damage.", 1.5, 0, 25},
                    {3, "Leeching Blow", "LB", "A percise strike that damages the target for 50%% damage, and heals you for 50%% of the damage caused." , 0.5, 0.5, 20}}},
                    {{"Spellcaster"}, 
                    {{2, "Magic Missle", "MM", "Fire a magical missle at the target for 200%% damage.", 2.0, 0, 30},
                     {3, "Drain Life", "DL", "Suck the life from your enemy causing 50%% damage, and healing you for 50%% of the damage caused.", 0.5, 0.5, 20}}}}
*/

char *classes[] = {
  "Fighter",
  "Spellcaster"
};

spell spells[] = {
  {"Fighter",
  {{2, "Crushing Blow", "CB", "Land a crushing blow on the target for 150% damage.", 1.5, 0, 25},
  {3, "Leeching Blow", "LB", "A percise strike that damages the target for 50% damage, and heals you for 50% of the damage caused." , 0.5, 0.5, 20}}},
  {"Spellcaster",
  {{1, "Magic Missle", "MM", "Fire a magical missle at the target for 200% damage.", 2.0, 0, 30},
  {3, "Drain Life", "DL", "Suck the life from your enemy causing 50% damage, and healing you for 50% of the damage caused.", 0.5, 0.5, 20}}}
};

/* legacy code - to be removed */
attack fighter[] = {
  {2, "Crushing Blow", "CB", "Land a crushing blow on the target for 150% damage.", 1.5, 0, 25},
  {3, "Leeching Blow", "LB", "A percise strike that damages the target for 50% damage, and heals you for 50% of the damage caused." , 0.5, 0.5, 20}
};

attack caster[] = {
  {2, "Magic Missle", "MM", "Fire a magical missle at the target for 200% damage.", 2.0, 0, 30},
  {3, "Drain Life", "DL", "Suck the life from your enemy causing 50% damage, and healing you for 50% of the damage caused.", 0.5, 0.5, 20}
};

int number_of_spells = sizeof(fighter) / sizeof(*fighter);
/* end of legacy code */

/* 
 * create_character() function - creates the player character when you first 
 * start the game 
 */
void create_character()
{ 
  int new_player_id = number_of_players + 1;
  
  int done;                               // check for do/while loop
  static char input_name[20];             // variable for input of player name
  static char input_password[20];         
  static char input_password_verify[20];
  static char input_class[20];            // variable for input of player name

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
  
  do {
    printf("Please enter a password: ");
    while (fgets(input_password, sizeof(input_password), stdin) == NULL);
    remove_newline(input_password);
    printf("Please re-enter your password: ");
    while (fgets(input_password_verify, sizeof(input_password_verify), stdin) == NULL);
    remove_newline(input_password_verify);
  } while (strcmp(input_password, input_password_verify) != 0);

  player.name = input_name;
  player_list[new_player_id].name = player.name;
  player_list[new_player_id].password = input_password;
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
      player.combat_class = 1;
      player.health = 18;
      player.max_health = 18;
      player.str = 4;
      player.intel = 8;
      player.wis = 6;
      player.damage = 2;
      player.magic = 4;
      player.defence = 6;
      player.dex = 6;
      player.armour = 0;
      player.fort = 6;
      done = 1;
      /* place starting player equipment in start zone */
      memcpy(&objects[_number_of_objects], &objects[2], sizeof(objects[2]));
      objects[_number_of_objects].location = 1;
      _number_of_objects++;
      memcpy(&objects[_number_of_objects], &objects[3], sizeof(objects[3]));
      objects[_number_of_objects].location = 1;
      _number_of_objects++;
    }
    else if (strcasecmp(input_class, "FIGHTER") == 0) {
      player.combat_class = 0;
      player.health = 20;
      player.max_health = 20;
      player.str = 8;
      player.intel = 4;
      player.wis = 6;
      player.damage = 4;
      player.magic = 2;
      player.defence = 8;
      player.dex = 8;
      player.armour = 2;
      player.fort = 8;
      done = 1;
      /* place starting player equipment in start zone */
      memcpy(&objects[_number_of_objects], &objects[0], sizeof(objects[0]));
      objects[_number_of_objects].location = 1;
      _number_of_objects++;
      memcpy(&objects[_number_of_objects], &objects[1], sizeof(objects[1]));
      objects[_number_of_objects].location = 1;
      _number_of_objects++;
    }
    else {
      printf("Sorry, %s is not a valid option.\n", input_class);
    }
  } while (done < 1);

  allocate_stats(player.points);
  printf("\nHello %s the %s, welcome to mazeSUD.\n", player.name, classes[player.combat_class]);
  save_player(player.name);
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
  printf("   Class : %s\n", classes[player.combat_class]);
  printf("   Level : %-15d", player.level);
  printf("     Exp : %d\n", player.xp);
  printf("  Armour : %-15d", player.armour + bonuses.armour);
  printf(" Defense : %d\n", player.defence + bonuses.defence);
  printf("  Damage : %-15d", player.damage + bonuses.damage);
  printf("   Magic : %d\n", player.magic + bonuses.magic);
  printf("     Str : %-15d", player.str);
  printf("     Int : %d\n", player.intel);
  printf("     Wis : %-15d", player.wis);
  printf("     Dex : %d\n", player.dex);
  printf("    Fort : %d\n", player.fort);
  printf("  Health : %d/%d\n", player.health, player.max_health);
  printf("  Energy : %d/%d\n", player.energy, player.max_energy);
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
    printf("STR, INT, WIS, DEX, FORT, DONE when complete or HELP: ");
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
        dexterity_to_defence(points);
        points = 0;
      }
      else if (s < points) {
        player.dex = player.dex + s;
        dexterity_to_defence(s);
        points = points - s;
      }
      else if (s > points) {
        printf("Please enter a valid number.\n");
      }
      else {
        player.dex = player.dex + points;
        dexterity_to_defence(points);
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
    else if (strcasecmp(i, "WIS") == 0) {
      printf("How much would you like to allocate to Wisdom? [%d]", points);
      while (fgets(i, sizeof(i), stdin) == NULL);
      sscanf(i, "%d", &s);
      if (s == points) {
        player.wis = player.wis + points;
        wisdom_to_energy(s);
        points = 0;
      }
      else if (s < points) {
        player.wis = player.wis + s;
        wisdom_to_energy(s);
        points = points - s;
      }
      else if (s > points) {
        printf("Please enter a valid number.\n");
      }
      else {
        player.wis = player.wis + points;
        wisdom_to_energy(points);
        points = 0;
      }
    }
    else if (strcasecmp(i, "DONE") == 0) {
      player.points = points;
      return;
    }
    else if (strcasecmp(i, "HELP") == 0) {
      printf("\n\nThe 5 main stats in mazeSUD have various affects on your character:\n");
      printf("  STRENGTH  - Increases damage of melee attacks.\n");
      printf("  INTELLECT - Increases damage of magic and ranged attacks.\n");
      printf("  WISDOM    - Increases maximum energy and spell/special abilitys you can learn\n");
      printf("  DEXTERITY - Increases defense and attack speed.\n");
      printf("  FORTITUDE - Increases your health and survival.\n\n");
      printf("Once your stats reach the base level (40) 2 training points are required to\n"
             "raise it one point.  You can save your training points for later or use them\n"
             "all up now.   Return to the Hall of Training any time to TRAIN STATS\n");
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


/* function to convert dexterity to defense */
void dexterity_to_defence(int points)
{
  player.defence = player.dex * 1;
  return;
}

/* function to convert wisdom to energy */
void wisdom_to_energy(int points)
{
  player.max_energy = player.max_energy + (points * 5);
  player.energy = player.max_energy;

  return;
}

/* function to convert added fortitude to health */
void fortitude_to_health(int points)
{
  player.max_health = player.max_health + points;
  player.health = player.max_health;

  return;
}

/* function to convert added intellect to damage if applicable */
void intellect_to_damage(int points)
{
  player.magic = player.intel / 2;
  
  return;
}

/* function to convert added strength to damage if applicable */
void strength_to_damage(int points)
{
  player.damage = player.str / 2;
  
  return;
}

/* execute_training() function - trains character to level up or add unused stat points */
void execute_training(const char *noun)
{
  int next_level = player.level * (player.level * xp_to_level);
  
  if (player.location != 4) {
    printf("You must be at the Hall of Training to train.\n");

    return;
  }

  printf(LCYN "%s Trainer:\n" RESET, classes[player.combat_class]);
  if (noun == NULL) {
    if (player.xp >= next_level) {
      player.level++;
      player.points += 4;
      player.max_health += 4;
      player.health = player.max_health;
      printf(WHT"  With your training and hard work, you have reached level %d.\n", player.level);
      printf("  You gain 4 health and 4 training points. Type ""TRAIN STATS"" to allocate your points.\n" RESET);
      for (int i = 0; spells[player.combat_class].attacks[i].name != NULL; i++) {
        if (player.level == spells[player.combat_class].attacks[i].level) {
            printf(WHT "  You have learned a new spell: %s\n" RESET, spells[player.combat_class].attacks[i].name);

            return;
        }
      }
    }
    else {
      printf(WHT "  You still need %d xp before you can reach the next level.\n" RESET, next_level - player.xp);
    }
  }
  else if (strcasecmp(noun, "STATS") == 0) {
    if (player.points > 0) {
      allocate_stats(player.points);
    }
    else {
      printf(WHT "  You don't have any training points.\n" RESET);
    }
  }
  else {
    printf(WHT "  I'm a busy man, stop wasting my time with this %s of yours!\n" RESET, noun);
  }

  return;
}

/* execute_list_spells() function - lists the spells known to the player */
void execute_list_spells()
{
  int i = 0;
  int spells_available = 0;

  while (spells[player.combat_class].attacks[i].name != NULL) {
    if (spells[player.combat_class].attacks[i].level <= player.level) {
      spells_available++;
      printf(LCYN "Name: %-18s Short: %s               Energy cost: %d\n" RESET, spells[player.combat_class].attacks[i].name,
                                                                                spells[player.combat_class].attacks[i].tag,
                                                                                spells[player.combat_class].attacks[i].energy);
      printf(WHT "  %s\n" RESET, spells[player.combat_class].attacks[i].description);
    }
    i++;
  }
  if (spells_available == 0) {
    printf("You don't have any spells/specials yet.\n");
  }

  return;
}