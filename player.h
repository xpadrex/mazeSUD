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

#ifndef __CHARACTER__
#define __CHARACTER__
typedef struct {
  char *name;
  char *description;
  float damage;
  int energy;
} attack;

typedef struct {
  char *name;
  char *combat_class;
  int level;
  int xp;
  int armour;
  int health;
  int max_health;
  int energy;
  int damage;
  int str;
  int intel;
  int dex;
  int fort;
  object *hands;
  object *body;
  int location;
  int gold;
  int points;
} character;

extern character player;
extern attack fighter[];
extern attack caster[];
#endif