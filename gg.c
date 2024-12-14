 /*
  * This file contains all the garphics, animation, and game logic
  * helping functions.
 */

#include "gg.h"
#include "printf.h"
#include "strings.h"
#include "interrupts.h"
#include "gl.h"
#include "shooter.h"
#include "bug_sprite.h"
#include "timer.h"
#include "audio.h"


// Enemy grid global variable
Enemy enemies[MAX_ROWS][MAX_COLS];
#define MAX(a, b) ((a) > (b) ? (a) : (b)) // these two marcos where used for intial optimization
#define MIN(a, b) ((a) < (b) ? (a) : (b)) // but currently not used


// Bullet initialization
Bullet bullets[MAX_BULLETS];
BugBullet bug_bullets[MAX_BUG_BULLETS];
int score = 10; 


// Draw an enemy (intial implemenation)
void draw_enemy(int x, int y, color_t color) {
    //gl_draw_rect(x, y, ENEMY_WIDTH, ENEMY_HEIGHT, color);
}


// Draw the background
void draw_background(void) {
    gl_clear(COLOR_BACKGROUND);
}

// Set up the scoring (two scores, one for Stanford and one for Cal)
void init_score(Score *score) {
    score->stanford_score = 0;
    score->cal_score = 0;
    score->time_remaining = 30;  // 30 secs countdown
    
    score->x_stanford = SCREEN_WIDTH - 200;
    score->y_stanford = 10;
    
    score->x_cal = 70;
    score->y_cal = 10;
    
    score->x_timer = SCREEN_WIDTH / 2 - 70;
    score->y_timer = 10;
    
    score->stanford_color = gl_color(140, 21, 21);  
    score->cal_color = COLOR_ENEMY;      
    score->timer_color = gl_color(255, 255, 255); 
}

// Display Stanford score
void draw_stanford_score(Score *score) {
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "Stanford: %d", score->stanford_score);
    gl_draw_string(score->x_stanford, score->y_stanford, buffer, score->stanford_color);
}
// Display cal score
void draw_cal_score(Score *score) {
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "Cal: %d", score->cal_score);
    gl_draw_string(score->x_cal, score->y_cal, buffer, score->cal_color);
}
// Display timer
void draw_timer_score(Score *score) {
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "Time: %d", score->time_remaining);
    gl_draw_string(score->x_timer, score->y_timer, buffer, score->timer_color);
}

// Initialize the enemy grid with max rows and max cols
void init_enemy_grid(int start_x, int start_y, int spacing_x, int spacing_y) {
    int current_x, current_y = start_y;
    for (int row = 0; row < MAX_ROWS; row++) {
        current_x = start_x;
        for (int col = 0; col < MAX_COLS; col++) {
            enemies[row][col] = (Enemy){.x = current_x, .y = current_y, .alive = 1};
            current_x += ENEMY_WIDTH + spacing_x;
        }
        current_y += ENEMY_HEIGHT + spacing_y;
    }
}

// Helper function to draw a filled horizontal line (currently not used)
void gl_draw_horizontal_line(int x_start, int x_end, int y, color_t color) {
    for (int x = x_start; x <= x_end; x++) {
        gl_draw_pixel(x, y, color);
    }
}

// Draw a filled circle (currently not used)
void gl_draw_circle(int x0, int y0, int radius, color_t color) {
    int x = radius, y = 0, err = 1 - radius;
    while (x >= y) {
        gl_draw_horizontal_line(x0 - y, x0 + y, y0 + x, color);
        gl_draw_horizontal_line(x0 - x, x0 + x, y0 + y, color);
        gl_draw_horizontal_line(x0 - x, x0 + x, y0 - y, color);
        gl_draw_horizontal_line(x0 - y, x0 + y, y0 - x, color);
        y++;
        err += (err <= 0) ? 2 * y + 1 : 2 * (y - x--) + 1;
    }
}

/*
 * This function used draw_pixel to draw a pre-computed sprite of a picture, 
 * NOTE: the sprites were computed using a Python script found online, will cite - Hadil
*/
void gl_draw_sprite(int x, int y, int width, int height, const uint32_t sprite[height][width]) {
    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            int pixel_x = x + col;
            int pixel_y = y + row;
            if (pixel_x >= 0 && pixel_x < gl_get_width() && pixel_y >= 0 && pixel_y < gl_get_height()) {
                uint32_t color = sprite[row][col];
                
                // extract individual color values
                uint8_t alpha = (color >> 24) & 0xFF;
                uint8_t red = (color >> 16) & 0xFF;
                uint8_t green = (color >> 8) & 0xFF;
                uint8_t blue = color & 0xFF;
                
                // Only draw if is not transparent and not the background color (some graphics manipulation)
                if (alpha != 0 && color != 0xFF1D2FA3) {
                    color_t fixed_color = gl_color(red, green, blue);
                    gl_draw_pixel(pixel_x, pixel_y, fixed_color);
                }
            }
        }
    }
}



// initial implementation of clearing a sprite (not used)
void gl_draw_sprite_clear(int x, int y, int width, int height, const uint32_t sprite[height][width]) {
    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            int pixel_x = x + col;
            int pixel_y = y + row;
            if (pixel_x >= 0 && pixel_x < gl_get_width() && pixel_y >= 0 && pixel_y < gl_get_height()) {
                color_t color = sprite[row][col];
                if (color >> 24 != 0 && color != 0xFF1D2FA3) { 
                    //no
                }
                gl_draw_pixel(pixel_x, pixel_y, gl_color(0, 0, 40));
                
            }
        }
    }
   //gl_draw_rect(x,y,width,height,gl_color(0, 0, 40));
}


// Draw the shooter using a pre-computed shooter sprite
void draw_shooter(Shooter *shooter) {
    gl_draw_sprite(shooter->x, shooter->y, 50, 50, shooter_sprite);
}

/*
 * Draw a bug (enemy)
 * NOTE: this is currently drawing a cal bear but this was initial implementation
*/ 
void draw_bug(int x, int y) {
    gl_draw_sprite(x, y, 50, 50, bug_sprite);
}
// not used now
void clear_bug(int x, int y) {
    gl_draw_sprite_clear(x, y, 50, 50, bug_sprite);
}

// Draws the full enemy grid for the first time
void draw_enemy_grid(void) {
    for (int row = 0; row < MAX_ROWS; row++) {
        for (int col = 0; col < MAX_COLS; col++) {

            if (enemies[row][col].alive) {
               // clear_bug(enemies[row][col].prev_x, enemies[row][col].prev_y);
                draw_bug(enemies[row][col].x, enemies[row][col].y);
            }
            
        }
    }
}


// Enemies Animation
// NOTE: this was implemented for space invaders but isn't currently used as enemies stay in place for the current game

// Move the grid to the left
void move_left(int step_size) {
    for (int row = 0; row < MAX_ROWS; row++) {
        for (int col = 0; col < MAX_COLS; col++) {
            if (enemies[row][col].alive) {
                enemies[row][col].prev_x = enemies[row][col].x+10;
                enemies[row][col].prev_y = enemies[row][col].y;
                enemies[row][col].x = enemies[row][col].x - step_size;
            }
        }
    }
}
// Move the grid to the right
void move_right(int step_size) {
    for (int row = 0; row < MAX_ROWS; row++) {
        for (int col = 0; col < MAX_COLS; col++) {
            if (enemies[row][col].alive) {
                enemies[row][col].prev_x = enemies[row][col].x-10;
                enemies[row][col].prev_y = enemies[row][col].y;

                enemies[row][col].x = MIN(enemies[row][col].x + step_size, SCREEN_WIDTH - ENEMY_WIDTH);
            }
        }
    }
}
// Move the grid down
void move_down(int step_size) {
    for (int row = 0; row < MAX_ROWS; row++) {
        for (int col = 0; col < MAX_COLS; col++) {
            if (enemies[row][col].alive) {
                // Update previous position
                enemies[row][col].prev_x = enemies[row][col].x;
                enemies[row][col].prev_y = enemies[row][col].y-10;

                // Update current position
                enemies[row][col].y = MIN(enemies[row][col].y + step_size, SCREEN_HEIGHT - ENEMY_HEIGHT);
            }
        }
    }
}



// Move enemies in a full pattern in a S-like shap, as well as draws them in their new positions
void move_enemies_full_pattern(MovementDirection *direction, int step_size) {
    // Find the leftmost and rightmost active enemies for boundary-checking
    int leftmost_x = SCREEN_WIDTH;
    int rightmost_x = 0;
    
    for (int row = 0; row < MAX_ROWS; row++) {
        for (int col = 0; col < MAX_COLS; col++) {
            if (enemies[row][col].alive) {
                leftmost_x = MIN(leftmost_x, enemies[row][col].x);
                rightmost_x = MAX(rightmost_x, enemies[row][col].x);
            }
        }
    }
    
    bool should_change_direction = false;
    
    if (*direction == RIGHT && rightmost_x + step_size + ENEMY_WIDTH >= SCREEN_WIDTH) {
        should_change_direction = true;
    } else if (*direction == LEFT && leftmost_x - step_size <= 0) {
        should_change_direction = true;
    }
    
    for (int row = 0; row < MAX_ROWS; row++) {
        for (int col = 0; col < MAX_COLS; col++) {
            if (enemies[row][col].alive) {
                enemies[row][col].prev_x = enemies[row][col].x;
                enemies[row][col].prev_y = enemies[row][col].y;
                
                if (should_change_direction) {
                    enemies[row][col].y += step_size; 
                } else if (*direction == RIGHT) {
                    enemies[row][col].x += step_size;
                } else if (*direction == LEFT) {
                    enemies[row][col].x -= step_size;
                }
            }
        }
    }
    

    for (int row = 0; row < MAX_ROWS; row++) {
        for (int col = 0; col < MAX_COLS; col++) {
            if (enemies[row][col].alive) {
                gl_draw_rect(enemies[row][col].prev_x - 15, 
                           enemies[row][col].prev_y - 15, 
                           80, 65, COLOR_BACKGROUND);
            }
        }
    }
    
    for (int row = 0; row < MAX_ROWS; row++) {
        for (int col = 0; col < MAX_COLS; col++) {
            if (enemies[row][col].alive) {
                draw_bug(enemies[row][col].x, enemies[row][col].y);
            }
        }
    }
    
    if (should_change_direction) {
        *direction = (*direction == RIGHT) ? LEFT : RIGHT;
    }
}


// Shooter Animation

// Move the shooter to the left
void move_shooter_left(Shooter *shooter) {
    shooter->prev_x = shooter->x;
    if (shooter->x - 10 >= 0) {
        shooter->x -= 10;
    }
}

// Move the shooter to the right
void move_shooter_right(Shooter *shooter) {
    shooter->prev_x = shooter->x;
    if (shooter->x + PLAYER_WIDTH + 10 <= SCREEN_WIDTH) {
        shooter->x += 10;
    }
}


// Initialize the shootter bullets
void init_bullets(void) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        bullets[i].active = false;
    }
}

// Shoots a bullets when user hits associated button on the keyboard
void fire_bullet(Shooter *shooter) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!bullets[i].active) {
            bullets[i].x = shooter->x + (PLAYER_WIDTH / 2) - (BULLET_WIDTH / 2);
            bullets[i].y = shooter->y;
            bullets[i].prev_x = bullets[i].x;  
            bullets[i].prev_y = bullets[i].y;
            bullets[i].active = true;
            break;
        }
    }
}
// Display Bullet

void draw_bullet(int x, int y) {
    color_t bullet_color = COLOR_RED;
    
    gl_draw_rect(x, y + (BULLET_HEIGHT/2) - 1, BULLET_WIDTH, 2, bullet_color);
    gl_draw_rect(x + (BULLET_WIDTH/2) - 1, y, 2, BULLET_HEIGHT, bullet_color);
}

// Game Logic 

// Helper function to check if two objects
bool check_collision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2) {
    return (x1 < x2 + w2 &&
            x1 + w1 > x2 &&
            y1 < y2 + h2 &&
            y1 + h1 > y2);
}

//  Handles the shooter's bullets hitting the enemies, both graphics and score 
void handle_bullet_collision(Bullet *bullet, Enemy *enemy, Score *score) {

	score->stanford_score += 1;

 	gl_draw_rect(enemy->x-20, enemy->y-20, 100, 100, COLOR_BACKGROUND);
    gl_draw_rect(bullet->x, bullet->y, BULLET_WIDTH, BULLET_HEIGHT, COLOR_BACKGROUND);
    gl_draw_rect(score->x_stanford, score->y_stanford, 150, 20, COLOR_BACKGROUND);
	draw_stanford_score(score);
	gl_swap_buffer();

 	gl_draw_rect(enemy->x-20, enemy->y-20, 100, 100, COLOR_BACKGROUND);
    gl_draw_rect(bullet->x, bullet->y, BULLET_WIDTH, BULLET_HEIGHT, COLOR_BACKGROUND);
   	gl_draw_rect(score->x_stanford, score->y_stanford, 150, 20, COLOR_BACKGROUND);
	draw_stanford_score(score);
	gl_swap_buffer();                  
 
    bullet->active = false;
    enemy->alive = false;
	play_bullet_sound(); 
}

// Display of the bullets
void update_bullets(Score *score) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].active) {
            gl_draw_rect(bullets[i].prev_x, bullets[i].prev_y, 
                        BULLET_WIDTH, BULLET_HEIGHT, COLOR_BACKGROUND);
            
            bullets[i].prev_x = bullets[i].x;
            bullets[i].prev_y = bullets[i].y;
            
            bullets[i].y -= BULLET_SPEED;
            
            bool collision_occurred = false;
            for (int row = 0; row < MAX_ROWS && !collision_occurred; row++) {
                for (int col = 0; col < MAX_COLS && !collision_occurred; col++) {
                    if (enemies[row][col].alive) {
                        if (check_collision(bullets[i].x, bullets[i].y,
                                         BULLET_WIDTH, BULLET_HEIGHT,
                                         enemies[row][col].x, enemies[row][col].y,
                                         ENEMY_WIDTH, ENEMY_HEIGHT)) {
                            handle_bullet_collision(&bullets[i], &enemies[row][col], score);
                            collision_occurred = true;
                        }
                    }
                }
            }
            
            // Only continue with bullet if no collision occurred
            if (!collision_occurred) {
                // Check if bullet is about to go out of screen
                if (bullets[i].y == SCREEN_HEIGHT) {
                    gl_draw_rect(bullets[i].prev_x, bullets[i].prev_y, 
                               BULLET_WIDTH, BULLET_HEIGHT, COLOR_BACKGROUND);
                    bullets[i].active = false;
                    continue;
                }
                
                draw_bullet(bullets[i].x, bullets[i].y);
            }
        }
    }
}

// Initialize the bullets coming down from the enemies (NOTE: this is currently cal bears)
void init_bug_bullets(void) {
    for (int i = 0; i < MAX_BUG_BULLETS; i++) {
        bug_bullets[i].active = false;
    }
}

// Helper function to get the front most alive enemy to drop bullets from
bool get_front_enemy(int col, int *front_row) {
    for (int row = MAX_ROWS - 1; row >= 0; row--) {
        if (enemies[row][col].alive) {
            *front_row = row;
            return true;
        }
    }
    return false;
}

// Create a new enemy bullet from a random frontline enemy
void create_bug_bullet(void) {
    for (int i = 0; i < MAX_BUG_BULLETS; i++) {
        if (!bug_bullets[i].active) {
            int col = timer_get_ticks() % MAX_COLS; // simple randomzing
            int front_row;
            
            if (get_front_enemy(col, &front_row)) {
                bug_bullets[i].x = enemies[front_row][col].x + (ENEMY_WIDTH / 2);
                bug_bullets[i].y = enemies[front_row][col].y + ENEMY_HEIGHT + 15;
                bug_bullets[i].prev_x = bug_bullets[i].x;
                bug_bullets[i].prev_y = bug_bullets[i].y;
                bug_bullets[i].grade = timer_get_ticks() % 4;  
                bug_bullets[i].active = true;
                return;
            }
            
            int original_col = col;
            do {
                col = (col + 1) % MAX_COLS;
                if (get_front_enemy(col, &front_row)) {
                    bug_bullets[i].x = enemies[front_row][col].x + (ENEMY_WIDTH / 2);
                    bug_bullets[i].y = enemies[front_row][col].y + ENEMY_HEIGHT;
                    bug_bullets[i].prev_x = bug_bullets[i].x;
                    bug_bullets[i].prev_y = bug_bullets[i].y;
                    bug_bullets[i].grade = timer_get_ticks() % 4;  
                    bug_bullets[i].active = true;
                    return;
                }
            } while (col != original_col);
            
            return;
        }
    }
}

// Handles when a enemy bullets hits the shooter
void handle_bug_bullet_collision(BugBullet *bullet, Shooter *shooter, Score *score) {

	score->cal_score += 1;
    gl_draw_rect(bullet->x, bullet->y-10, 20, 20, COLOR_BACKGROUND);  
    gl_draw_rect(score->x_cal, score->y_cal, 150, 20, COLOR_BACKGROUND);
    draw_cal_score(score);
	gl_swap_buffer();

    gl_draw_rect(bullet->x, bullet->y-10, 20, 20, COLOR_BACKGROUND); 
    gl_draw_rect(score->x_cal, score->y_cal, 150, 20, COLOR_BACKGROUND);
    draw_cal_score(score);
	gl_swap_buffer();

    bullet->active = false;
	play_proj_sound(); 
}


// Update positions of all active enemy bullets
void update_bug_bullets(Shooter *shooter, Score *score) {
    for (int i = 0; i < MAX_BUG_BULLETS; i++) {
        if (bug_bullets[i].active) {
            gl_draw_rect(bug_bullets[i].prev_x - 5, bug_bullets[i].prev_y - 5, 
                        20, 20, COLOR_BACKGROUND);
            
            bug_bullets[i].prev_x = bug_bullets[i].x;
            bug_bullets[i].prev_y = bug_bullets[i].y;
            bug_bullets[i].y += BUG_BULLET_SPEED;
            
            if (bug_bullets[i].y == SCREEN_HEIGHT) {
 		        gl_draw_rect(bug_bullets[i].prev_x - 5, bug_bullets[i].prev_y - 5,
                        20, 20, COLOR_BACKGROUND);
                bug_bullets[i].active = false;
                continue;
            }

            if (check_collision(bug_bullets[i].x, bug_bullets[i].y,
                              15, 15,  
                              shooter->x, shooter->y,
                              PLAYER_WIDTH, PLAYER_HEIGHT)) {
                handle_bug_bullet_collision(&bug_bullets[i], shooter, score);
                continue;  
            }

            char grade_char;					//we first implemented a theme where we had to escape bad grades and shoot A+'s but then
            switch(bug_bullets[i].grade) {			//we ended up modifying to just C's from the Cal bears
              //  case GRADE_B: grade_char = 'B'; break;	//but we kept this code so it can be repurposed for multiple modes of the game if needed
              //  case GRADE_C: grade_char = 'C'; break;
              //  case GRADE_D: grade_char = 'D'; break;
              //  case GRADE_F: 
                default: grade_char = 'C'; break;
            }
            
            char str[2] = {grade_char, '\0'};
            gl_draw_string(bug_bullets[i].x, bug_bullets[i].y, str, COLOR_ENEMY);
            
        }
    }
}

// Different state screens
void draw_start_screen(void) {
    gl_clear(COLOR_RED);
    const char *title = "It's that time of the year again...";
    const char *next = "STANFORD VS CAL... BIG GAME!  ";
    const char *prompt = "You are our only hope left...Good luck! Press ENTER to Start";
    
    int title_x = (SCREEN_WIDTH - strlen(title) * 8) / 2 - 85;  
    int prompt_x = (SCREEN_WIDTH - strlen(next) * 8) / 2 - 330;
	int next_x = (SCREEN_WIDTH - strlen(prompt) * 8) / 2;


    
    gl_draw_string(title_x, SCREEN_HEIGHT/2 - 40, title, COLOR_TEXT);
    gl_draw_string(next_x, SCREEN_HEIGHT/2, next, COLOR_ENEMY);
    gl_draw_string(prompt_x, SCREEN_HEIGHT/2 + 40, prompt, COLOR_TEXT);
}

void draw_game_over_screen(void) {
    gl_clear(COLOR_BACKGROUND);
    const char *message = "Yeah Cal won again lol..";
    const char *prompt = "go back to NERD NATION or press ENTER to start again";
    
    int message_x = (SCREEN_WIDTH - strlen(message) * 8) / 2 - 50;
    int prompt_x = (SCREEN_WIDTH - strlen(prompt) * 8) / 2 - 150;
    
    gl_draw_string(message_x, SCREEN_HEIGHT/2 - 20, message, COLOR_ENEMY);
    gl_draw_string(prompt_x, SCREEN_HEIGHT/2 + 20, prompt, COLOR_TEXT);
}

void draw_win_screen(void) {
    gl_clear(COLOR_RED);
    const char *message = "Stanford Finally WINS!";
    const char *prompt = "Press ENTER to Play Again (maybe?)";
    
    int message_x = (SCREEN_WIDTH - strlen(message) * 8) / 2 - 70;
    int prompt_x = (SCREEN_WIDTH - strlen(prompt) * 8) / 2 - 70;
    
    gl_draw_string(message_x, SCREEN_HEIGHT/2 - 20, message, COLOR_ENEMY);
    gl_draw_string(prompt_x, SCREEN_HEIGHT/2 + 20, prompt, COLOR_TEXT);
}

