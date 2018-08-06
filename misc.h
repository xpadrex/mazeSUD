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
extern int check_password(const char *name, const char *password);

/* define the ascii codes for colours in the terminal */
#define RED   "\x1B[31m"
#define LRED  "\x1B[91m"
#define GRN   "\x1B[32m"
#define LGRN  "\x1B[92m"
#define YEL   "\x1B[33m"
#define LYEL  "\x1B[93m"
#define BLU   "\x1B[34m"
#define LBLU  "\x1B[94m"
#define MAG   "\x1B[35m"
#define LMAG  "\x1B[95m"
#define CYN   "\x1B[36m"
#define LCYN  "\x1B[96m"
#define WHT   "\x1B[37m"
#define LWHT  "\x1B[97m"
#define RESET "\x1B[0m"