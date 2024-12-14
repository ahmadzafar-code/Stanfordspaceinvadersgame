/* File: fb.c
 * ----------
 * ***** TODO: add your file header comment here *****
 */
#include "fb.h"
#include "de.h"
#include "hdmi.h"
#include "malloc.h"
#include "strings.h"

// module-level variables, you may add/change this struct as you see fit
static struct {
    int width;             // count of horizontal pixels
    int height;            // count of vertical pixels
    int depth;             // num bytes per pixel
    void *framebufferF;     // address of framebuffer memory FRONT
    void *framebufferB;     // address of framebuffer memory BACK
    void *current_displayFB;  //currently displayed framebuffer
    fb_mode_t mode; 
} module;

void fb_init(int width, int height, fb_mode_t mode) {

	 hdmi_resolution_id_t id = hdmi_best_match(width, height);
        hdmi_init(id);
        de_init(width, height, hdmi_get_screen_width(), hdmi_get_screen_height());

	 module.width = width;
    	module.height = height;
    	module.depth = 4;
	module.mode = mode; 
    	int nbytes = module.width * module.height * module.depth;

    	module.framebufferF = malloc(nbytes);
    	memset(module.framebufferF, 0x0, nbytes);


    	if (mode == FB_DOUBLEBUFFER) {
    		module.framebufferB = malloc(nbytes);
    		memset(module.framebufferB, 0x0, nbytes);
	}

   	 module.current_displayFB = module.framebufferF; 
   	 de_set_active_framebuffer(module.framebufferF);
}

int fb_get_width(void) {
    	return module.width;
}

int fb_get_height(void) {
   	 return module.height;
}

int fb_get_depth(void) {
   	 return module.depth;
}

void* fb_get_draw_buffer(void){

    	if (module.mode == FB_SINGLEBUFFER) {
        	return module.framebufferF;
   	 }

	if (module.current_displayFB == module.framebufferF) {
		return module.framebufferB;
	} else {
		return module.framebufferF;
	} 

}

void fb_swap_buffer(void) {

	if (module.mode == FB_DOUBLEBUFFER) {

	module.current_displayFB = fb_get_draw_buffer(); 
	de_set_active_framebuffer(module.current_displayFB);

	}
}


void* fb_get_back_buffer(void) {
    if (module.mode == FB_SINGLEBUFFER) {
        return NULL;
    }
    
    // Return the buffer that's not currently being displayed
    return (module.current_displayFB == module.framebufferF) ? 
           module.framebufferB : module.framebufferF;
}
