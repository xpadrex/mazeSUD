/* 
 * location.h - declares external functions for the 'LOOK' and 'GO' commands
 */

extern void execute_look(const char *noun);
extern void execute_go(const char *noun);
extern void move_player(const int *direction);
