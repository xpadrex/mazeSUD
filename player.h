/*
 * player.h - header file for player.c
 */

extern void look_self();
extern void create_character();

typedef struct {
  char *name;
  int armour;
  int health;
  int energy;
  char *hands;
  char *body;
} character;

extern character player;