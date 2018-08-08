/*
 * player.h - header file for player.c
 */
#include "objects.h"

extern void look_self();
extern void create_character();
extern void allocate_stats(int points);
void fortitude_to_health(int points);
void intellect_to_damage(int points);
void strength_to_damage(int points);
extern int number_of_players;
extern void execute_training(const char *noun);

#define xp_to_level 400

#ifndef __CHARACTER__
#define __CHARACTER__

typedef struct {
  int level;
  char *name;
  char *description;
  float damage;
  int energy;
} attack;

typedef struct {
  char *name;           // player name
  char *combat_class;   // class (SPELLCASTER/FIGHTER)
  int level;            // experience level
  int xp;               // experience points
  int armour;           
  int health;
  int max_health;
  int energy;           // energy for special abilities
  int damage;           // maximum damage
  int str;              // strength
  int intel;            // intellect
  int dex;              // dexterity
  int fort;             // fortitude
  object *hands;
  object *body;
  int location;         // location number in the locations array
  int gold;             // gold coins (currency)
  int points;           // skill points 
  int id;               // unique id used to identify what items are on the character
  int in_combat;        // flag for combat
} character;

typedef struct {
  char *name;
  char *password;
  int id;
} login;

extern login player_list[];
extern character player;
extern attack fighter[];
extern attack caster[];
#endif