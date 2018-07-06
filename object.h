/*
 * object.h - define the object structs for use in the maze-rpg game
 */

typedef struct {
    const char *description;
    const char *tag;
    struct location *location;
} object;