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
    timer_init();
   // gpio_init();
   // interrupts_init();
 //   keyboard_init(GPIO_PIN_PS2_CLK, GPIO_PIN_PS2_DATA);
    
    // Initialize game
    Game game;
    game_init(&game);
    
    // Game loop
    while (1) {
        game_update(&game);
        game_render(&game);
        timer_delay_ms(16);  // ~60 FPS
    }
}
