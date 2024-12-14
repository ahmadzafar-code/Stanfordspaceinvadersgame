/* File: ps2_assign7.c
 * -------------------

 */
#include "gpio.h"
#include "gpio_extra.h"
#include "malloc.h"
#include "ps2.h"
#include "interrupts.h"
#include "gpio_interrupt.h"
#include "ringbuffer.h"
#include "timer.h"
#include "uart.h"
#include "printf.h"


 // A ps2_device is a structure that stores all of the state and information
 // needed for a PS2 device. The clock field stores the gpio id for the
 // clock pin, and the data field stores the gpio id for the data pin.
 // Read ps2_new for example code that sets and uses these fields.
 //
 // You may extend the ps2_device structure with additional fields as needed.
 // A pointer to the current ps2_device is passed into all ps2_ calls.
 // Storing state in this structure is preferable to using global variables:
 // it allows your driver to support multiple PS2 devices accessed concurrently
 // (e.g., a keyboard and a mouse).
 //
 // This definition fills out the structure declared in ps2.h.
 struct ps2_device {
     gpio_id_t clock;
     gpio_id_t data;

     uint8_t scancode;
     int bits_count; 
     int odds_count;       
     rb_t *scancode_rb; 
 };

 // Interrupt handler for PS/2 device
 void ps2_interrupt_handler(void *aux_data) {
     ps2_device_t *dev = (ps2_device_t *)aux_data;
     gpio_interrupt_clear(dev->clock);
     unsigned int bit = gpio_read(dev->data);

     // Start bit
     if (dev->bits_count == 0) {
         if (bit == 0) { 
             dev->bits_count++;
         }
     } 
     // Data bits
     else if (dev->bits_count < 9) {
         dev->scancode |= (bit << (dev->bits_count - 1));
         if (bit == 1) {
             dev->odds_count++;
         }
         dev->bits_count++;
     } 
     // Parity bit
     else if (dev->bits_count == 9) {
         unsigned char expected_parity = (dev->odds_count % 2 == 0) ? 1 : 0;
         if (bit != expected_parity) {
             dev->bits_count = 0;
             dev->scancode = 0;
             dev->odds_count = 0;
         } else {
             dev->bits_count++;
         }
     } 
     // Stop bit
     else if (dev->bits_count == 10) {
         if (bit == 1) { 
             rb_enqueue(dev->scancode_rb, dev->scancode);
         }
         // Reset for next scancode
         dev->bits_count = 0;
         dev->scancode = 0;
         dev->odds_count = 0;
     }
 }

 // Creates a new PS2 device connected to given clock and data pins,
 // The gpios are configured as input and set to use internal pull-up
 // (PS/2 protocol requires clock/data to be high default)
 ps2_device_t *ps2_new(gpio_id_t clock_gpio, gpio_id_t data_gpio) {
     ps2_device_t *dev = malloc(sizeof(*dev));
     dev->clock = clock_gpio;
     gpio_set_input(dev->clock);
     gpio_set_pullup(dev->clock);

     dev->data = data_gpio;
     gpio_set_input(dev->data);
     gpio_set_pullup(dev->data);

     dev->bits_count = 0;
     dev->odds_count = 0;
     dev->scancode = 0;
     dev->scancode_rb = rb_new();

     gpio_interrupt_init();
     gpio_interrupt_config(dev->clock, GPIO_INTERRUPT_NEGATIVE_EDGE, false);
     gpio_interrupt_register_handler(dev->clock, ps2_interrupt_handler, dev);
     gpio_interrupt_enable(dev->clock);
     interrupts_global_enable(); 

     return dev;
 }

 /*
  * Read a single PS2 scancode. Always returns a correctly received scancode.
  * If an error occurs (e.g., start bit not detected, parity is wrong), 
  * the function reads another scancode.
  */
 uint8_t ps2_read(ps2_device_t *dev) {
     int scancode = 0;
     while (!rb_dequeue(dev->scancode_rb, &scancode)) {

     }
     return (uint8_t)scancode;
 }
bool ps2_rb_is_empty(ps2_device_t *dev) {
    bool result = rb_empty(dev->scancode_rb);
    return result;
}

rb_t* ps2_get_scancode_buffer(ps2_device_t *dev) {
    return dev->scancode_rb;
}
