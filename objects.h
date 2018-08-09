/* 
 * objects.h - declares external functions for the LOOK, GO, MOVE and to list 
 * objects
 */

extern void list_objects(int here);
extern void list_inventory();
extern void execute_get(const char *noun);
extern void execute_drop(const char *noun);
extern void execute_equip(const char *noun);
extern void execute_unequip(const char *noun);
extern void init_loot();
extern void load_equip(const char *item);
extern int scan_objects(const char *search);
extern int look_objects(const char *item);
extern void execute_list();
extern void execute_sell(const char *noun);
extern void execute_buy(const char *noun);

#ifndef __OBJECT__
#define __OBJECT__
typedef struct {
    char *description;
    char *tag;
    int location;
    char *look;
    int value;
    int damage;
    int armour;
    int equipped;
} object;

extern object objects[];
#endif
