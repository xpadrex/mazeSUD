/* 
 * objects.h - declares external functions for the LOOK, GO, MOVE and to list 
 * objects
 */

extern void execute_look(const char *noun);
extern void execute_go(const char *noun);
extern void move_player(const char *direction);
extern void list_objects(const char *here);
extern void list_inventory();
extern void execute_get(const char *noun);
extern void execute_drop(const char *noun);
extern void execute_equip(const char *noun);
extern void execute_unequip(const char *noun);

#ifndef __OBJECT__
#define __OBJECT__
typedef struct {
    char *description;
    char *tag;
    char *location;
    int value;
    int damage;
    int armour;
} object;

//extern object objects[];
#endif