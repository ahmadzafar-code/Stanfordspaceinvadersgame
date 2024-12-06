#include "uart.h"
#include "mymodule.h"
#include "gpio.h"
#include "gpio_extra.h"
#include "malloc.h"
#include "ps2.h"
#include "gl.h"
#include "fb.h"
#include "timer.h"
#include "keyboard.h"
#include "interrupts.h"

void main(void) {
    // Initialize hardware
 interrupts_init();
gpio_init();
  uart_init(); 
    timer_init();
   keyboard_init(GPIO_PG12, GPIO_PB7);
    
    // Initialize game
    Game game;
    game_init(&game);
    interrupts_global_enable();
  game_render(&game);
       
    
    // Game loop
    while (1) {

        handle_input(&game);
      //  game_update(&game);
        game_render(&game);
        timer_delay_ms(16);
    }
}
