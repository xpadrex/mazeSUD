/*
 * player.h - header file for player.c
 */
#include "objects.h"

extern void look_self();
extern void create_character();

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
  int level;
  int xp;
  int armour;
  int health;
  int max_health;
  int energy;
  object *hands;
  object *body;
  int location;
} character;

extern character player;
extern attack warrior[];
extern attack mage[];
#endif