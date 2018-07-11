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
  int armour;
  int health;
  int energy;
  object *hands;
  object *body;
} character;

extern character player;
#endif