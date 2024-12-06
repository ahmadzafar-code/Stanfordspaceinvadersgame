#ifndef MYMODULE_H
#define MYMODULE_H

#include "ps2.h" 

// Game Constants
#define SCREEN_WIDTH    1024
#define SCREEN_HEIGHT   768
#define PLAYER_WIDTH    48
#define PLAYER_HEIGHT   32
#define ENEMY_WIDTH     40
#define ENEMY_HEIGHT    32

// Game States
typedef enum {
    MENU,
    PLAYING,
    GAMEOVER,
    WIN
} GameState;

// Entity Structures
typedef struct {
    int x, y;
    int width, height;
    int dx;
    int speed;  
} Player;

typedef struct {
    int x, y;
    char* class_name;
} Enemy;

typedef struct {
    GameState state;
    Player player;
    float gpa;
    ps2_device_t* keyboard;
    bool key_left_pressed;
    bool key_right_pressed;
} Game;

// Function prototypes
void game_init(Game* game);
void draw_player(Game* game);
void moveplayer_right(Game* game); 
void moveplayer_left(Game* game);  
void game_update(Game* game);
void game_render(Game* game);
void handle_input(Game* game);

#endif
