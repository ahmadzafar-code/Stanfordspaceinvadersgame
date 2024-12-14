#ifndef AUDIO_H
#define AUDIO_H

/*
 * Header file for the audio implemnation used in the game
*/
#include <stdbool.h>
#include <stdint.h>
#include "gpio.h"


// Delay function
void usleep(unsigned int usec);
// Setup
void setup_buzzer();
// Test
void play_buzzer(int frequency);
// Gameover/loss sound
void play_gameover_sound();
// Win sound
void play_winning_sound();
// Shooter's bullet collide with enemy 
void play_bullet_sound();
// Enemy's bullet collide with shooter
void play_proj_sound();

#endif