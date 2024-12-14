#include <stdint.h>

#include "uart.h"
#include "gg.h"
#include "printf.h"
#include "gl.h"
#include "timer.h"
#include "ps2.h"
#include "interrupts.h"
#include "timer.h"
#include "keyboard.h"
#include "ringbuffer.h"
#include "strings.h"
#include "audio.h"






#include <stdint.h>


bool move_shooter_left_flag = false;
bool move_shooter_right_flag = false;
bool fire_bullet_flag = false;



//remaining time implementation for the game mode
void update_timescore(Score *score) {

   	char time_str[32];
    
    	snprintf(time_str, sizeof(time_str), "Time: %d", score->time_remaining);
    	gl_draw_rect(score->x_timer, score->y_timer, 150, 30, COLOR_BACKGROUND);

    	score->time_remaining--;
    
    	snprintf(time_str, sizeof(time_str), "Time: %d", score->time_remaining);
    	gl_draw_string(score->x_timer, score->y_timer, time_str, score->timer_color);
    	gl_swap_buffer();
    
    	gl_draw_rect(score->x_timer, score->y_timer, 150, 30, COLOR_BACKGROUND);
    	gl_draw_string(score->x_timer, score->y_timer, time_str, score->timer_color);
    	gl_swap_buffer();

}

//instead of redrawing the graphics of bugs everytime, we move the shooter up to create the same effect but with less rendering overload
void update_shooter(Shooter *shooter) {


   	 shooter->prev_y = shooter->y;
    	int new_y = shooter->y - 10;
    	shooter->y = (new_y > 0) ? new_y : 0; // Don't go above screen
    
    	gl_draw_rect(shooter->x, shooter->prev_y, 65, 65, COLOR_BACKGROUND);
    	draw_shooter(shooter);
    	gl_swap_buffer();
    
    	gl_draw_rect(shooter->x, shooter->prev_y, 65, 65, COLOR_BACKGROUND);
    	draw_shooter(shooter);
    	gl_swap_buffer();


}

//initialization of the graphical elements of the game
void initialize_game(Shooter *shooter, Score *score) {
	
   	
	 shooter->x = SCREEN_WIDTH / 2 - PLAYER_WIDTH / 2;
   	 shooter->y = SCREEN_HEIGHT - PLAYER_HEIGHT - 20;
   	 shooter->prev_x = SCREEN_WIDTH / 2 - PLAYER_WIDTH / 2;
   	 shooter->prev_y = SCREEN_HEIGHT - PLAYER_HEIGHT - 20;

         init_enemy_grid(100, 50, 50, 50);
         init_score(score);
         init_bullets();
         init_bug_bullets();
     
}

//using interrupts in the main while loop to call this and store states of movement

void handle_key_press(int scancode) {

	switch(scancode) {
       		case 0x23:  // D key
            		move_shooter_right_flag = true;
            		break;
        	case 0x1C:  // A key
            		move_shooter_left_flag = true;
            		break;
        	case 0x32:  // B key
           		 fire_bullet_flag = true;
            		 break;
	    
    }
}



//Graphics Rendering function draws everything on both buffers at the start of the game but draws only required changes after that

void render_playing_mode(Shooter *shooter, Score *score, bool full_redraw) {

	if (full_redraw) {

        	draw_background();
        	draw_enemy_grid();
        	draw_shooter(shooter);
		draw_stanford_score(score);
		draw_cal_score(score);
		draw_timer_score(score);
		
		gl_swap_buffer(); 
        	draw_background();
        	draw_enemy_grid();
        	draw_shooter(shooter);
        	draw_stanford_score(score);
        	draw_cal_score(score);
        	draw_timer_score(score);
 
    	}

	
  	if (shooter->x != shooter->prev_x) {
       		gl_draw_rect(shooter->prev_x, shooter->prev_y - 10,50,60,COLOR_BACKGROUND );
        	draw_shooter(shooter);

        	gl_swap_buffer();
        	gl_draw_rect(shooter->prev_x, shooter->prev_y - 10, 50, 60, COLOR_BACKGROUND);
        	draw_shooter(shooter);
        	gl_swap_buffer();
        
	}
 
		gl_swap_buffer();


}








int main(void) {

	 gpio_init();
	 setup_buzzer(); 
    
    	 gl_init(SCREEN_WIDTH, SCREEN_HEIGHT, GL_DOUBLEBUFFER);
    
   	uart_init();
    	timer_init();
    	interrupts_init();
    	keyboard_init(KEYBOARD_CLOCK, KEYBOARD_DATA);
    	interrupts_global_enable();
    
    	GameState game_state = GAME_START;
    	rb_t *scancode_buffer = keyboard_get_scancode_buffer();

	//defining time variables that we are going to use to add intervals for graphics rendering in the loop

    	unsigned long last_bullet_update = timer_get_ticks();
    	const unsigned long BULLET_UPDATE_INTERVAL = 1600;  
	unsigned long last_bug_bullet_spawn = timer_get_ticks();
	const unsigned long BUG_BULLET_SPAWN_INTERVAL = 3000000;
	unsigned long last_enemy_move = timer_get_ticks();
	const unsigned long ENEMY_MOVE_INTERVAL = 500000;
	unsigned long last_shooter_move = timer_get_ticks();
	unsigned long SHOOTER_MOVE_INTERVAL = 50000000;
	unsigned long last_second = 0;
	const unsigned long SECOND_INTERVAL = 30000000;

 	MovementDirection direction = RIGHT;
    	const int step_size = 10;
    

 	bool full_redraw = true; // Full redraw on the first fram

	Shooter shooter; 
	Score score;
    
   	 while (1) {

        	int scancode;
       		 bool is_release = false;

		//this is where we use the ps2 module ringbuffer directly(the implementation of interrupts there)
		//we are using the handle_key_press function to deal with the interrupts to store flags so that we can use them later on
		//this helps in not making the graphics weird while using interrupts
		
        	while (rb_dequeue(scancode_buffer, &scancode)) {
              		  if (scancode == 0xE0) continue;
           		  if (scancode == 0xF0) {
                	  is_release = true;
                      		continue;
           	 	}

            	    	 if (!is_release) {

                		// Check for ENTER key (scancode 0x5A)
                		if (scancode == 0x5A) {
					if (game_state ==GAME_START || game_state == GAME_OVER || GAME_WON) {

						initialize_game(&shooter, &score); 
						render_playing_mode(&shooter, &score, full_redraw);
						game_state = GAME_PLAYING;
						full_redraw = false;

					} 
				 
				} else if (game_state == GAME_PLAYING) {
                    			handle_key_press(scancode);
                		}

           	        }
            		is_release = false;
        	}

            switch (game_state) {
            	case GAME_START:
               		draw_start_screen();
                	gl_swap_buffer();
                	draw_start_screen();
                	break;

            case GAME_PLAYING:
                	unsigned long current_time = timer_get_ticks();
                
                	// Handling movement flags stored by the handle key press after getting interrupted

                	if (move_shooter_left_flag) {
                   		 move_shooter_left(&shooter);
                    		move_shooter_left_flag = false;
                	}

              	       if (move_shooter_right_flag) {
                    		move_shooter_right(&shooter);
                    		move_shooter_right_flag = false;
                	}

                	if (fire_bullet_flag) {
                   		 fire_bullet(&shooter);
                   		 fire_bullet_flag = false;
               		}
			
                    	update_bullets(&score);

			//using the time variables we defined to render the bullets at an interval

               	      if (current_time - last_bug_bullet_spawn >= BUG_BULLET_SPAWN_INTERVAL) {
                    		create_bug_bullet();
                    		last_bug_bullet_spawn = current_time;
               	     
		       }

    		    update_bug_bullets(&shooter, &score);

		//timer score implementation
		    if (current_time - last_second >= SECOND_INTERVAL) {

			update_timescore(&score);
	       		last_second = current_time;
               
   				 if (score.time_remaining <= 0) {
       		 			if (score.stanford_score > score.cal_score) {
           					 game_state = GAME_WON;
       					 } else {
          					  game_state = GAME_OVER;
       					 }
    				}
	
		   }
    

 		unsigned long current_time_s = timer_get_ticks();
		if (current_time_s - last_shooter_move >= SHOOTER_MOVE_INTERVAL) {
  			update_shooter(&shooter);
   			last_shooter_move = current_time_s;

		}

                render_playing_mode(&shooter, &score, full_redraw);
                full_redraw = false;



                break;

            case GAME_OVER:
                	draw_game_over_screen();
                	gl_swap_buffer();
		 	play_gameover_sound();			//playing the specified sound on the GAME STATE change
                	draw_game_over_screen();
                	break;

            case GAME_WON:
                	draw_win_screen();
                	gl_swap_buffer();
        		play_winning_sound();
                	draw_win_screen();
                	break;
        }
    }
    
    return 0;
}










