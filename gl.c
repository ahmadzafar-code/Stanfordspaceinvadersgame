/* File: gl.c
 * ----------
 * ***** TODO: add your file header comment here *****
 */
#include "gl.h"
#include "font.h"

void gl_init(int width, int height, gl_mode_t mode) {
    fb_init(width, height, mode);
}

int gl_get_width(void) {
    return fb_get_width();
}

int gl_get_height(void) {
    return fb_get_height();
}

color_t gl_color(uint8_t r, uint8_t g, uint8_t b) {    //dividing the colors into the BGRA format

	color_t alpha = 0xFF << 24;
        color_t red = r << 16;
        color_t green = g << 8;

    return alpha | red | green | b;


}


void gl_swap_buffer(void) {
	fb_swap_buffer(); 
}

void* gl_get_back(void) {
    return fb_get_back_buffer();
}


void gl_clear(color_t c) {						//clearing all the pixels in the buffer to whatver color is assigned
	unsigned int * buffer = (unsigned int *)fb_get_draw_buffer();
    	int width = fb_get_width();
    	int height = fb_get_height();
    	int pixels = width * height; 

	for (int i = 0; i < pixels; i++) {
		buffer[i] = c;
	}

}

static int in_bounds(int x, int y) {

   	 if (x < 0 || x >= fb_get_width()) {
        	return 0;
    	}
    
    	if (y < 0 || y >= fb_get_height()) {
        	return 0;
   	 }
    
    	return 1;
}

void gl_draw_pixel(int x, int y, color_t c) {

    if (!in_bounds(x, y)) {
        return;
    }

    color_t (*buffer)[fb_get_width()];
    buffer = (color_t (*)[fb_get_width()])fb_get_draw_buffer();
    buffer[y][x] = c;


}

color_t gl_read_pixel(int x, int y) {

    if (!in_bounds(x, y)) {
        return 0;
    }

    color_t (*buffer)[fb_get_width()];
    buffer = (color_t (*)[fb_get_width()])fb_get_draw_buffer();
   	return buffer[y][x]; 


}

void gl_draw_rect(int x, int y, int w, int h, color_t c) {
	for (int y_point = y; y_point < y + h; y_point++) {
		for (int x_point = x; x_point < x + w; x_point++) {
			gl_draw_pixel(x_point, y_point, c); 
		}
	} 


}

void gl_draw_char(int x, int y, char ch, color_t c) {

	int pixels = font_get_glyph_size();
	unsigned char glyph[pixels];

    	if (!font_get_glyph(ch, glyph, pixels)) {
        	return; 
    	}

	int ch_width = gl_get_char_width();
	int ch_height = gl_get_char_height();

   	 unsigned char (*glyph_array)[ch_width] = (unsigned char (*)[ch_width])glyph;


    	for (int y_point = 0; y_point < ch_height; y_point++) { 
       		 for (int x_point = 0; x_point < ch_width; x_point++) {
            		if (glyph_array[y_point][x_point] == 0xFF) { 
                		gl_draw_pixel((x + x_point), (y + y_point), c);             
        		}
   		 }

	}
}

void gl_draw_string(int x, int y, const char* str, color_t c) {

	int ch_width = gl_get_char_width();

	int i = 0;
	while (str[i] != '\0') {
		gl_draw_char(x + (i * ch_width), y, str[i], c);
		i++;
	}

}

int gl_get_char_height(void) {
    return font_get_glyph_height();;
}

int gl_get_char_width(void) {
   	return font_get_glyph_width();
}


void gl_draw_line (int x0, int y0, int x1, int y1, color_t color) {
	
	bool is_steep;  						//checking if rise is greater then run or not, so that we can decide what axis do we wanna check the difference towards
	if ((y1 - y0) < 0) {
		is_steep = (-1 * (y1 - y0)) > (x1 - x0); 
	} else {
		is_steep = (y1 - y0) > (x1 - x0); 
	} 

	if (is_steep) {						//swapping x and y values 
		int temp = x0; 
		x0 = y0;
		y0 = temp; 
		temp = x1; 
		x1 = y1;
		y1 = temp; 
	}
	
	if  (x0 > x1) {				//making sure that we draw from left to write, otherwise swap
		int temp = x0; 
		x0 = x1;
		x1 = temp; 
		temp = y0;
		y0 = y1;
		y1 = temp; 
	}

	int delta_x = x1 - x0;			//calculating the difference between points
	int delta_y = y1 - y0;

	if (delta_x == 0) {			//if there is no diference in x, it means it's a vertical line, so handle accordingly 
		if (y0 > y1) {
			int temp = y0;
			y0 = y1;
			y1 = temp;
		}
		for (int y = y0; y <= y1; y++) {
			if (is_steep) {
				gl_draw_pixel(y, x0, color); 
			} else {
				gl_draw_pixel(x0, y, color);
			}
		}
		return; 
	}
	if (delta_y == 0) {         		//horizontal line
		for (int x = x0; x <= x1; x++) {
			if (is_steep) {
				gl_draw_pixel(y0, x, color); 
			} else {
				gl_draw_pixel(x, y0, color); 
			}
		}
		return;
	}
	
	
	int y_fixed_p = y0 << 8;  			//i planned to use fixed point rather than floating, in which it's actually easier to do calculations
	int m = (delta_y << 8) / delta_x; 		// using it as 8.8, which means the left is integer and the right 8 are decimal points i.e fraction

	for (int x = x0; x <= x1; x++) {
		int y = y_fixed_p >> 8; 
		int fraction = y_fixed_p & 0xFF; 	

		if (is_steep) {
			gl_draw_pixel(y, x, (color * (255 - fraction))/ 255);	//255 - fraction, for the main pixel
			gl_draw_pixel(y + 1, x, (color * fraction)/ 255);	//fraction directly, for the adjacent ones
		} else {
        		gl_draw_pixel(x, y, (color * (255 - fraction))/ 255);
                        gl_draw_pixel(x, y + 1, (color * fraction)/ 255);
                }
		
		y_fixed_p += m; 
	}
}
void gl_draw_triangle (int x0, int y0, int x1, int y1, int x2, int y2, color_t color) {
   	
	gl_draw_line(x0, y0, x1, y1, color);			//drawing the anti-alaised lines
	gl_draw_line(x1, y1, x2, y2, color);
	gl_draw_line(x2, y2, x0, y0, color);

	if (y1 < y0) {			//making sure the vertices are sorted, y0 < y1 < y2
		int temp = x0;
        	x0 = x1;
        	x1 = temp;
        	temp = y0;
        	y0 = y1;
        	y1 = temp;
    	}
 	if (y2 < y0) {
        	int temp = x0;
       		x0 = x2;
        	x2 = temp;
        	temp = y0;
       		y0 = y2;
        	y2 = temp;
    }
   	if (y2 < y1) {
        	int temp = x1;
       		x1 = x2;
       	        x2 = temp;
       	        temp = y1;
        	y1 = y2;
        	y2 = temp;
    }
	//calcualting difference between x and y for edges
    	int delta_x_1 = x1 - x0;
   	int delta_x_2 = x2 - x0;
   	int delta_x_3 = x2 - x1;
   	int delta_y_1 = y1 - y0;
    	int delta_y_2 = y2 - y0;
    	int delta_y_3 = y2 - y1;
	
	//as we are approaching this in a way that we are dividing the triangle into upper and lower, because the slope is different at edges, so we deal with upper and lower separately

	if (delta_y_1) {		//upper
        for (int y = y0; y < y1; y++) {
		int x_start = x0 + delta_x_1 * (y - y0) / delta_y_1;
            	int x_end = x0 + delta_x_2 * (y - y0) / delta_y_2;
          
            	if (x_start > x_end) {
                	int temp = x_start;
                	x_start = x_end;
                	x_end = temp;
            }
            
            for (int x = x_start; x <= x_end; x++) {
                gl_draw_pixel(x, y, color);
            }
        }
    }

    if (delta_y_3) {		//lower
        for (int y = y1; y <= y2; y++) {
            int x_start = x1 + delta_x_3 * (y - y1) / delta_y_3;
            int x_end = x0 + delta_x_2 * (y - y0) / delta_y_2;
            
            if (x_start > x_end) {
                int temp = x_start;
                x_start = x_end;
                x_end = temp;
            }
            
            for (int x = x_start; x <= x_end; x++) {
                gl_draw_pixel(x, y, color);
            }
        }
    }
}		
