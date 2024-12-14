/*
	This file contains teh audio implemenation used in the game
*/
#include "gpio.h"
#include "audio.h"
#include "timer.h"
#define BUZZER_PIN  GPIO_PG13 

/*
 * This function is a delay 
*/
void usleep(unsigned int usec) {
    timer_delay_us(usec);  
}

/*
	This function is the initial setup of the buzzer pin
*/
void setup_buzzer() {
    gpio_set_output(BUZZER_PIN); 
}

// test function
void play_buzzer(int frequency) {
    int period = 1000000 / frequency;
    
    for (int i = 0; i < 100; i++) {  
        gpio_write(BUZZER_PIN, 1);    
        usleep(period / 2);            
        gpio_write(BUZZER_PIN, 0);     
        usleep(period / 2);        
    }
}


/*
 * This function plays a sound when the game ends and user loses
*/
void play_gameover_sound() {
    int frequency = 1000;  
    int duration = 500000; 
    
    play_buzzer(frequency);  
    //usleep(duration);        
    gpio_write(BUZZER_PIN, 0);  
}

/*
 * This function plays a sound when the game ends and user wins
*/
void play_winning_sound() {
    int winning_freqs[] = {800, 1000, 1500, 2000, 2500, 2000, 1500, 1000, 800}; 
    int num_freqs = sizeof(winning_freqs) / sizeof(winning_freqs[0]);

    for (int i = 0; i < num_freqs; i++) {
        play_buzzer(winning_freqs[i]);
        usleep(300000);  
    }
    
    play_buzzer(3000);  
    usleep(500000);    
    gpio_write(BUZZER_PIN, 0);  
}

/*
 * This function plays a sound when a shooter's bullet hits an enemy
*/
void play_bullet_sound() {
    int frequency = 4000;  
    int duration = 15000000;
    
    play_buzzer(frequency);  
    gpio_write(BUZZER_PIN, 0);  
}

/*
 * This function plays a sound when a enemy's projictle hits the shooter
*/
void play_proj_sound() {
    int frequency = 500;  
    int duration = 150000;
    
    play_buzzer(frequency);  
    gpio_write(BUZZER_PIN, 0);  
}