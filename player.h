/*
 * player.h - header file for player.c
 */
#include "objects.h"

extern void look_self();
extern void create_character();
extern void allocate_stats(int points);

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
  object *hands;
  object *body;
  int location;
  int gold;
} character;

extern character player;
extern attack warrior[];
extern attack mage[];
#endif