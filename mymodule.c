/* File: mymodule.c
 * -------------------
 * Implementation of Stanford GPA Space Invaders game
 */
#include "mymodule.h"
#include "gpio.h"
#include "gpio_extra.h"
#include "malloc.h"
#include "ps2.h"
#include "gl.h"
#include "fb.h"
#include "timer.h"
#include "strings.h"
#include "printf.h"
#include "keyboard.h"
#include "uart.h" 


// Colors
#define COLOR_BACKGROUND gl_color(0, 0, 40)    // Dark blue
#define COLOR_PLAYER     gl_color(255, 215, 0)  // Stanford gold
#define COLOR_ENEMY      gl_color(140, 21, 21)  // Stanford cardinal
#define COLOR_TEXT       gl_color(255, 255, 255) // White

void game_init(Game* game) {
    // Initialize graphics
    gl_init(SCREEN_WIDTH, SCREEN_HEIGHT, GL_DOUBLEBUFFER);
    
    // Initialize keyboard
  //  game->keyboard = ps2_new(GPIO_PS2_CLK, GPIO_PS2_DATA);
    
    // Initialize player
    game->player.x = SCREEN_WIDTH / 2 - PLAYER_WIDTH / 2;
    game->player.y = SCREEN_HEIGHT - PLAYER_HEIGHT - 20;
    game->player.width = PLAYER_WIDTH;
    game->player.height = PLAYER_HEIGHT;
    
    // Initialize game state
    game->state = MENU;
    game->gpa = 4.0f;

}

void draw_player(Game* game) {
	 gl_draw_rect(game->player.x, game->player.y,
                        PLAYER_WIDTH, PLAYER_HEIGHT, COLOR_PLAYER);

}


void game_update(Game* game) {
    // Will add more game logic here later
}

void game_render(Game* game) {
    gl_clear(COLOR_BACKGROUND);
    
    switch(game->state) {
        case MENU:
            gl_draw_string(SCREEN_WIDTH/2 - 100, SCREEN_HEIGHT/2,
                         "Stanford GPA Invaders", COLOR_TEXT);
            gl_draw_string(SCREEN_WIDTH/2 - 80, SCREEN_HEIGHT/2 + 40,
                         "Press ENTER to start", COLOR_TEXT);

   gl_draw_rect(game->player.x, game->player.y,
                        PLAYER_WIDTH, PLAYER_HEIGHT, COLOR_PLAYER);



           // break;
            
       case PLAYING:
            // Draw player
            gl_draw_rect(game->player.x, game->player.y,
                        PLAYER_WIDTH, PLAYER_HEIGHT, COLOR_PLAYER);
            
            // Draw GPA
            char buffer[32];
            snprintf(buffer, sizeof(buffer), "GPA: %.2f", game->gpa);
            gl_draw_string(10, 10, buffer, COLOR_TEXT);
            break;
            
        case GAMEOVER:
            gl_draw_string(SCREEN_WIDTH/2 - 100, SCREEN_HEIGHT/2,
                         "GAME OVER - Academic Probation!", COLOR_TEXT);
            break;
            
        case WIN:
            gl_draw_string(SCREEN_WIDTH/2 - 100, SCREEN_HEIGHT/2,
                         "Congratulations! You Graduate!", COLOR_TEXT);
            break;
    }
    
    gl_swap_buffer();
}
