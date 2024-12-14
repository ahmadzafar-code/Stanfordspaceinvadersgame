#ifndef GG_H
#define GG_H

#include <stdbool.h>

#include "gl.h"
#include "ps2.h"

// Game Constants
#define SCREEN_WIDTH    1024
#define SCREEN_HEIGHT   768
#define PLAYER_WIDTH    48
#define PLAYER_HEIGHT   32
#define ENEMY_WIDTH     40
#define ENEMY_HEIGHT    32
#define MAX_ROWS        3
#define MAX_COLS        9

#define MAX_BULLETS 100
#define BULLET_SPEED 15
#define BULLET_WIDTH 10
#define BULLET_HEIGHT 10

#define MAX_BUG_BULLETS 500
#define BUG_BULLET_WIDTH 20
#define BUG_BULLET_HEIGHT 20
#define BUG_BULLET_SPEED 10

// Colors
#define COLOR_BACKGROUND  0xFF002676     
#define COLOR_PLAYER     gl_color(255, 215, 0)   
#define COLOR_ENEMY      0xFDB515  
#define COLOR_TEXT       gl_color(255, 255, 255) 
#define COLOR_RED 0xFF8C1515

// Movement Directions for bug (which we didn't end up implementing in this game mode but can potentially extend)
typedef enum {
    LEFT,
    RIGHT,
    DOWN
} MovementDirection;


// bug/bear Structure
typedef struct {
    int x;        
    int y;        
    int prev_x;   
    int prev_y;   
    int alive;    
    bool updated;
} Enemy;


typedef struct {
    int x; 
    int y; 
    int prev_x;   
    int prev_y;  
} Shooter;

//shooter's bullet
typedef struct {
    int x;
    int y;
    int prev_x;   
    int prev_y;
    bool active;
} Bullet;

//Bug bullet is a letter which we previously implemented as different grades falling
typedef enum {
    GRADE_B,
    GRADE_C,
    GRADE_D,
    GRADE_F
} GradeType;

typedef struct {
    int x;
    int y;
    bool active;
    GradeType grade;
    int prev_x;    
    int prev_y;
} BugBullet;


typedef struct {
    int stanford_score;    // Score when shooter hits bugs
    int cal_score;        // Score when bugs hit shooter
    int time_remaining;   // Timer in seconds
    int x_stanford;       // X position for Stanford score
    int y_stanford;       // Y position for Stanford score
    int x_cal;           // X position for Cal score
    int y_cal;           // Y position for Cal score
    int x_timer;         // X position for timer
    int y_timer;         // Y position for timer
    color_t stanford_color;
    color_t cal_color;
    color_t timer_color;
} Score;


typedef enum {
    GAME_START,
    GAME_PLAYING,
    GAME_OVER,
    GAME_WON
} GameState;



// Shooter Functions
void draw_shooter(Shooter *shooter);
void move_shooter_left(Shooter *shooter);
void move_shooter_right(Shooter *shooter);

// Enemies/bears

void draw_enemy(int x, int y, color_t color);
void init_enemy_grid(int start_x, int start_y, int spacing_x, int spacing_y);
void draw_enemy_grid(void);
void move_enemies_full_pattern(MovementDirection* direction, int step_size);
void gl_draw_sprite(int x, int y, int width, int height, const uint32_t sprite[height][width]);
void draw_bug(int x, int y);


// Background
void draw_background(void);

// Game Screens
void draw_menu_screen(void);
void draw_gameover_screen(void);
void draw_win_screen(void);
void update_enemy_grid(void);

void init_bullets(void);
void fire_bullet(Shooter *shooter);
void update_bullets(Score *score);
void draw_bullet(int x, int y);

bool check_collision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);
void handle_bullet_collision(Bullet *bullet, Enemy *enemy, Score *score);


void init_bug_bullets(void);
void create_bug_bullet(void);
void update_bug_bullets(Shooter *shooter, Score *score);
bool get_front_enemy(int col, int *front_row);
void handle_bug_bullet_collision(BugBullet *bullet, Shooter *shooter, Score *score); 

void init_score(Score *score);
void draw_stanford_score(Score *score);
void draw_cal_score(Score *score);
void draw_timer_score(Score *score);

void draw_start_screen(void);
void draw_game_over_screen(void);
void draw_win_screen(void);







#endif
