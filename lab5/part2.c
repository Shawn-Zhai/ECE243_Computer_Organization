/* This files provides address values that exist in the system */

#define SDRAM_BASE            0xC0000000
#define FPGA_ONCHIP_BASE      0xC8000000
#define FPGA_CHAR_BASE        0xC9000000

/* Cyclone V FPGA devices */
#define LEDR_BASE             0xFF200000
#define HEX3_HEX0_BASE        0xFF200020
#define HEX5_HEX4_BASE        0xFF200030
#define SW_BASE               0xFF200040
#define KEY_BASE              0xFF200050
#define TIMER_BASE            0xFF202000
#define PIXEL_BUF_CTRL_BASE   0xFF203020
#define CHAR_BUF_CTRL_BASE    0xFF203030

/* VGA colors */
#define WHITE 0xFFFF
#define YELLOW 0xFFE0
#define RED 0xF800
#define GREEN 0x07E0
#define BLUE 0x001F
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define GREY 0xC618
#define PINK 0xFC18
#define ORANGE 0xFC00

#define ABS(x) (((x) > 0) ? (x) : -(x))

/* Screen size. */
#define RESOLUTION_X 320
#define RESOLUTION_Y 240

/* Constants for animation */
#define BOX_LEN 2
#define NUM_BOXES 8

#define FALSE 0
#define TRUE 1

#include <stdlib.h>
#include <stdio.h>

// Begin part1.s for Lab 7

volatile int pixel_buffer_start; // global variable

int main(void)
{
    volatile int * pixel_ctrl_ptr = (int *)0xFF203020;
    /* Read location of the pixel buffer from the pixel buffer controller */
    pixel_buffer_start = *pixel_ctrl_ptr;

    clear_screen();
    int x0 = 0;
	int y0 = 120;
	int x1 = 319;
	int y1 = 120;
	int deltay = -1;
		
	while(1){
		draw_line(x0, y0, x1, y1, 0xC618);
		wait_for_vsync();
		draw_line(x0, y0, x1, y1, 0x0);
		y0 = y0 + deltay;
		y1 = y1 + deltay;
		if(y0 == 0 || y0 == 239)
			deltay = -deltay;
	}
}

// code not shown for clear_screen() and draw_line() subroutines
void wait_for_vsync(){
	volatile int * pixel_ctrl_ptr = 0xFF203020;
	register int status;
	
	*pixel_ctrl_ptr = 1;
	
	status = *(pixel_ctrl_ptr + 3);
	while((status & 0x01) != 0)
		status = *(pixel_ctrl_ptr + 3);
}

void clear_screen(){
	int row;
	int column;
	for(row = 0; row < 241; row++){
		for(column = 0; column < 321; column++){
			plot_pixel(column, row, 0x0);
		}
	}
}

void draw_line(int x0, int y0, int x1, int y1, short int color){
    int is_steep = ABS(y1 - y0) > ABS(x1 - x0) ? 1 : 0;
    if (is_steep){
        swap(&x0, &y0);
        swap(&x1, &y1);
    }
    if (x0 > x1){
        swap(&x0, &x1);
        swap(&y0, &y1);
    }

    int deltax = x1 - x0;
    int deltay = ABS(y1 - y0);
    int error = -(deltax / 2);
    int y = y0;
    int y_step;

    if (y0 < y1)
		y_step = 1;
    else 
		y_step = -1;
	
	int x;
    for (x = x0, y = y0; x < (x1+1); x++){
        if (is_steep) 
			plot_pixel(y, x, color);
        else 
			plot_pixel(x, y, color);

        error = error + deltay;

        if (error > 0){
            y = y + y_step;
            error = error - deltax;
        }
    }
}

void swap(int* a, int* b){
    int temp = *a;
    *a = *b;
    *b = temp;
}

void plot_pixel(int x, int y, short int line_color)
{
    *(short int *)(pixel_buffer_start + (y << 10) + (x << 1)) = line_color;
}
