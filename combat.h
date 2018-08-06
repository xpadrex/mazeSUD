/*
 * combat.h - combat related functions
 */

extern void execute_attack(const char *noun);
void player_attack(int i);
void monster_attack(int i);
extern int combat_off();
extern void execute_rest();
extern void aggro_monster(int i);
extern int cancel_aggro();