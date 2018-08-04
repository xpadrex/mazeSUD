/* misc.h - header file for misc function declarations */

extern void clear_screen();
extern void wait_for_keypress();
extern int yes_or_no();
extern int scan_objects(const char *search);
extern int randomize(int min, int max);
extern void remove_newline(char *str);
extern int save_player(const char *name);
extern int load_player(const char *name);
extern void load_objects();
extern void save_objects();
extern int _number_of_objects;
extern void show_prompt();
extern int load_player_list();
extern void save_player_list(const char *password);