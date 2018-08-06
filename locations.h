/* locations.h - external fuctions related to locations and location struct */


extern void execute_look(const char *noun);
extern void execute_go(const char *noun);
extern void move_player(int direction);
extern int load_locations();
void print_exits(int loc);
extern int number_of_locations;

#ifndef __LOCATION__
#define __LOCATION__
typedef struct {
  int room_id;
  char *description;
  char *tag;
  int north;
  int south;
  int east;
  int west;
  int up;
  int down;
} location;

extern location locations[];
#endif