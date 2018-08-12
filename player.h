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
void dexterity_to_defence(int points);
void wisdom_to_energy(int points);
extern int number_of_players;
extern int number_of_spells;
extern char *classes[];
extern void execute_training(const char *noun);
extern void execute_list_spells();

#define xp_to_level 400

#ifndef __CHARACTER__
#define __CHARACTER__


typedef struct {
  int level;
  char *name;
  char *tag;
  char *description;
  float damage;
  float healing;
  int energy;
} attack;

typedef struct {
  char *class_name;
  attack attacks[10];
} spell;

typedef struct {
  int damage;
  int magic;
  int defence;
  int armour;
} bonus;

typedef struct {
  char *name;           // player name
  int combat_class;   // class (number represents the element in the classes array)
  int level;            // experience level
  int xp;               // experience points
  int armour;           
  int health;
  int max_health;
  int energy;           // energy for special abilities
  int max_energy;       // maximum energy
  int damage;           // maximum damage
  int magic;            // max spell damage
  int defence;
  int str;              // strength
  int intel;            // intellect
  int wis;              // wisdom
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
extern bonus bonuses;
extern spell spells[];
extern attack fighter[];
extern attack caster[];
#endif