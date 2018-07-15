/* locations.h - external fuctions for LOOK and GO, location struct */


extern void execute_look(const char *noun);
extern void execute_go(const char *noun);
extern void move_player(const char *direction);

#ifndef __LOCATION__
#define __LOCATION__
typedef struct {
  char *description;
  char *tag;
  char *north;
  char *south;
  char *east;
  char *west;
} location;

extern location locations[];
#endif