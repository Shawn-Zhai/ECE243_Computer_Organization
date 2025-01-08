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

// Begin part3.c code for Lab 7

volatile int pixel_buffer_start; // global variable


int main(void)
{
    volatile int * pixel_ctrl_ptr = (int *)0xFF203020;
    // declare other variables(not shown)
    // initialize location and direction of rectangles(not shown)
	int box1x = rand() % 319;
	int box1y = rand() % 239;
	int box1dx = (rand() % 2) * 2 - 1;
	int box1dy = (rand() % 2) * 2 - 1;
	int box1c = rand() % 65536;
	
	int box2x = rand() % 319;
	int box2y = rand() % 239;
	int box2dx = (rand() % 2) * 2 - 1;
	int box2dy = (rand() % 2) * 2 - 1;
	int box2c = rand() % 65536;
	
	int box3x = rand() % 319;
	int box3y = rand() % 239;
	int box3dx = (rand() % 2) * 2 - 1;
	int box3dy = (rand() % 2) * 2 - 1;
	int box3c = rand() % 65536;
	
	int box4x = rand() % 319;
	int box4y = rand() % 239;
	int box4dx = (rand() % 2) * 2 - 1;
	int box4dy = (rand() % 2) * 2 - 1;
	int box4c = rand() % 65536;
	
	int box5x = rand() % 319;
	int box5y = rand() % 239;
	int box5dx = (rand() % 2) * 2 - 1;
	int box5dy = (rand() % 2) * 2 - 1;
	int box5c = rand() % 65536;
	
	int box6x = rand() % 319;
	int box6y = rand() % 239;
	int box6dx = (rand() % 2) * 2 - 1;
	int box6dy = (rand() % 2) * 2 - 1;
	int box6c = rand() % 65536;
	
	int box7x = rand() % 319;
	int box7y = rand() % 239;
	int box7dx = (rand() % 2) * 2 - 1;
	int box7dy = (rand() % 2) * 2 - 1;
	int box7c = rand() % 65536;
	
	int box8x = rand() % 319;
	int box8y = rand() % 239;
	int box8dx = (rand() % 2) * 2 - 1;
	int box8dy = (rand() % 2) * 2 - 1;
	int box8c = rand() % 65536;
	
    /* set front pixel buffer to start of FPGA On-chip memory */
    *(pixel_ctrl_ptr + 1) = 0xC8000000; // first store the address in the 
                                        // back buffer
    /* now, swap the front/back buffers, to set the front buffer location */
    wait_for_vsync();
    /* initialize a pointer to the pixel buffer, used by drawing functions */
    pixel_buffer_start = *pixel_ctrl_ptr;
    clear_screen(); // pixel_buffer_start points to the pixel buffer
    /* set back pixel buffer to start of SDRAM memory */
    *(pixel_ctrl_ptr + 1) = 0xC0000000;
    pixel_buffer_start = *(pixel_ctrl_ptr + 1); // we draw on the back buffer
    clear_screen(); // pixel_buffer_start points to the pixel buffer

    while (1)
    {
        /* Erase any boxes and lines that were drawn in the last iteration */
        clear_screen();

        // code for drawing the boxes and lines (not shown)
        // code for updating the locations of boxes (not shown)
		// Boxes
		draw_box(box1x, box1y, box1c);
		draw_box(box2x, box2y, box2c);
		draw_box(box3x, box3y, box3c);
		draw_box(box4x, box4y, box4c);
		draw_box(box5x, box5y, box5c);
		draw_box(box6x, box6y, box6c);
		draw_box(box7x, box7y, box7c);
		draw_box(box8x, box8y, box8c);

		// Lines
		draw_line(box1x, box1y, box2x, box2y, box1c);
		draw_line(box2x, box2y, box3x, box3y, box2c);
		draw_line(box3x, box3y, box4x, box4y, box3c);
		draw_line(box4x, box4y, box5x, box5y, box4c);
		draw_line(box5x, box5y, box6x, box6y, box5c);
		draw_line(box6x, box6y, box7x, box7y, box6c);
		draw_line(box7x, box7y, box8x, box8y, box7c);
		draw_line(box8x, box8y, box1x, box1y, box8c);

		// Check Direction
		if(box1x == 0 || box1x == 318)
			box1dx = -box1dx;
		if(box1y == 0 || box1y == 238)
			box1dy = -box1dy;

		if(box2x == 0 || box2x == 318)
			box2dx = -box2dx;
		if(box2y == 0 || box2y == 238)
			box2dy = -box2dy;

		if(box3x == 0 || box3x == 318)
			box3dx = -box3dx;
		if(box3y == 0 || box3y == 238)
			box3dy = -box3dy;

		if(box4x == 0 || box4x == 318)
			box4dx = -box4dx;
		if(box4y == 0 || box4y == 238)
			box4dy = -box4dy;

		if(box5x == 0 || box5x == 318)
			box5dx = -box5dx;
		if(box5y == 0 || box5y == 238)
			box5dy = -box5dy;

		if(box6x == 0 || box6x == 318)
			box6dx = -box6dx;
		if(box6y == 0 || box6y == 238)
			box6dy = -box6dy;

		if(box7x == 0 || box7x == 318)
			box7dx = -box7dx;
		if(box7y == 0 || box7y == 238)
			box7dy = -box7dy;

		if(box8x == 0 || box8x == 318)
			box8dx = -box8dx;
		if(box8y == 0 || box8y == 238)
			box8dy = -box8dy;
		
		box1x = box1x + box1dx;
		box1y = box1y + box1dy;
		
		box2x = box2x + box2dx;
		box2y = box2y + box2dy;
		
		box3x = box3x + box3dx;
		box3y = box3y + box3dy;
		
		box4x = box4x + box4dx;
		box4y = box4y + box4dy;
		
		box5x = box5x + box5dx;
		box5y = box5y + box5dy;
		
		box6x = box6x + box6dx;
		box6y = box6y + box6dy;
		
		box7x = box7x + box7dx;
		box7y = box7y + box7dy;
		
		box8x = box8x + box8dx;
		box8y = box8y + box8dy;
		
        wait_for_vsync(); // swap front and back buffers on VGA vertical sync
        pixel_buffer_start = *(pixel_ctrl_ptr + 1); // new back buffer
    }
}

// code for subroutines (not shown)
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

void draw_box(int x, int y, short int color){
    plot_pixel(x, y, color);
    plot_pixel(x + 1, y, color);
    plot_pixel(x, y + 1, color);
    plot_pixel(x + 1, y + 1, color);
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

