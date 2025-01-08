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
#define AUDIO_BASE			  0xFF203040

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

//audio constants
#define audioCap 32

//audio

/* Screen size. */
#define RESOLUTION_X 320
#define RESOLUTION_Y 240

/* Constants for animation */
#define FALSE 0
#define TRUE 1

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
//game vals
//board size
#define SIZE 4

//emptty
#define Empty 0

//global constant double array
struct tiles {
	int num;
	int x, y; //position
};

struct game {
	struct tiles tileInfo[SIZE][SIZE]; //[x],[y]
};

struct game gameBoard = {};
//////////////////////////////////////////////////////

// Begin part1.s for Lab 7

volatile int pixel_buffer_start; // global variable

// Interrupt Function Prototypes
void disable_A9_interrupts(void);
void enable_A9_interrupts(void);
void set_A9_IRQ_stack(void);
void config_interrupt(int N, int CPU_target);
void config_GIC(void);
void __attribute__ ((interrupt)) __cs3_isr_irq (void);
void config_PS2();
void config_KEYs();
void PS2_ISR();
void KEY_ISR();


// Drawing Function Prototypes
void clear_screen();
void draw_line(int x0, int y0, int x1, int y1, short int color);
void swap(int* a, int* b);
void plot_pixel(int x, int y, short int line_color);
void drawBackground();
void draw2(int x, int y);
void draw4(int x, int y);
void draw8(int x, int y);
void draw16(int x, int y);
void draw32(int x, int y);
void draw64(int x, int y);
void draw128(int x, int y);
void draw256(int x, int y);
void draw512(int x, int y);
void draw1024(int x, int y);
void draw2048(int x, int y);
void drawBlank(int x, int y);
void drawWin();
void drawLose();
void drawBoard();

//game logic
int randLoc();
int randVal();
void makeNewTile();
bool noChange(const struct game tempBoard);
void begin();
void moveUp(struct game* board, bool again);
void moveLeft(struct game* board, bool again);
void moveRight(struct game* board, bool again);
void moveDown(struct game* board, bool again);
bool trymove();
bool checkFinish();
void setLoc();


int main(void)
{
	disable_A9_interrupts();
	set_A9_IRQ_stack();
	config_GIC();
	config_PS2();
	config_KEYs();
	enable_A9_interrupts();
	
    volatile int * pixel_ctrl_ptr = (int *)0xFF203020;
    /* Read location of the pixel buffer from the pixel buffer controller */
    pixel_buffer_start = *pixel_ctrl_ptr;

	srand(time(0));

	begin();
	setLoc();
    clear_screen();
	drawBackground();
	drawBoard();
	
	while(1){}
	
	return 0;
}


//game logic
void setLoc(){
	for (int i = 0; i < SIZE; i++){
		for (int j = 0; j <SIZE; j++){
			gameBoard.tileInfo[i][j].x = 48 + i * 58;
			gameBoard.tileInfo[i][j].y = 8 + j * 58;
		}
	}
}

//generate randome 2 or 4 tiles
//new game should initialize 2 tiles 
int randLoc(){
	int loc = rand() % SIZE;
	return loc;
}

//chance of 4 appearing is much less than 2
int randVal(){
	int val = 2;

	if (rand() % 10 == 0) val = 4;
	else val = 2;

	return val;
}

//pop new tile after each move
void makeNewTile(){
	int i,j;
	while (true) {
		i = randLoc();
		j = randLoc();
		if (gameBoard.tileInfo[i][j].num == 0) {
			gameBoard.tileInfo[i][j].num = randVal();
			return;
		}
	}
}

//compare two boards, return ture if same
bool noChange(const struct game tempBoard){
	bool same = true;
	for (int i = 0; i < SIZE; i++){
		for (int j = 0; j < SIZE; j++){
			if (tempBoard.tileInfo[i][j].num != gameBoard.tileInfo[i][j].num) same = false;
		}
	}
	return same;
}

//initialize a game
void begin(){
	struct game temp = {};
	gameBoard = temp;
	makeNewTile();
	makeNewTile();
}

//move up, givn the current board
void moveUp(struct game* board, bool again){
	//fill empty
	for (int i = 0; i < SIZE; i++){
		for (int j = 1; j < SIZE; j++){
			if (board->tileInfo[i][j].num != 0){
				for (int temp = j; temp > 0; temp--){
					if (board->tileInfo[i][temp - 1].num == Empty){
						board->tileInfo[i][temp - 1].num = board->tileInfo[i][temp].num;
						board->tileInfo[i][temp].num = Empty;
					}
				}
			}
		}
	}

	//merge
	if (!again){
		for (int i = 0; i < SIZE; i++){
			for (int j = 0; j < SIZE - 1; j++){
				if (board->tileInfo[i][j].num == board->tileInfo[i][j + 1].num){
					board->tileInfo[i][j].num += board->tileInfo[i][j + 1].num;
					board->tileInfo[i][j + 1].num = Empty;
				}
			}
		}
	}

	if (!again) moveUp(board, true);

	if (again) return; 
}

//move left
void moveLeft(struct game* board, bool again){
	//fill empty
	for (int j = 0; j < SIZE; j++){
		for (int i = 1; i < SIZE; i++){
			if (board->tileInfo[i][j].num != 0){
				for (int temp = i; temp > 0; temp--){
					if (board->tileInfo[temp - 1][j].num == Empty){
						board->tileInfo[temp - 1][j].num = board->tileInfo[temp][j].num;
						board->tileInfo[temp][j].num = Empty;
					}
				}
			}
		}
	}

	//merge
	if (!again){
		for (int j = 0; j < SIZE; j++){
			for (int i = 0; i < SIZE - 1; i++){
				if (board->tileInfo[i][j].num == board->tileInfo[i + 1][j].num){
					board->tileInfo[i][j].num += board->tileInfo[i + 1][j].num;
					board->tileInfo[i + 1][j].num = Empty;
				}
			}
		}
	}

	if (!again) moveLeft(board, true);

	if (again) return;
}

//move right
void moveRight(struct game* board, bool again){
	//fill empty
	for (int j = 0; j < SIZE; j++){
		for (int i = SIZE - 2; i >= 0; i--){
			if (board->tileInfo[i][j].num != 0){
				for (int temp = i; temp < SIZE - 1; temp++){
					if (board->tileInfo[temp + 1][j].num == Empty){
						board->tileInfo[temp + 1][j].num = board->tileInfo[temp][j].num;
						board->tileInfo[temp][j].num = Empty;
					}
				}
			}
		}
	}

	//merge
	if (!again){
		for (int j = 0; j < SIZE; j++){
			for (int i = SIZE - 1; i > 0; i--){
				if (board->tileInfo[i][j].num == board->tileInfo[i - 1][j].num){
					board->tileInfo[i][j].num += board->tileInfo[i - 1][j].num;
					board->tileInfo[i - 1][j].num = Empty;
				}
			}
		}
	}

	if (!again) moveRight(board, true);

	if (again) return;
}

//move down
void moveDown(struct game* board, bool again){
	//fill empty
	for (int i = 0; i < SIZE; i++){
		for (int j = SIZE - 2; j >= 0; j--){
			if (board->tileInfo[i][j].num != 0){
				for (int temp = j; temp < SIZE - 1; temp++){
					if (board->tileInfo[i][temp + 1].num == Empty){
						board->tileInfo[i][temp + 1].num = board->tileInfo[i][temp].num;
						board->tileInfo[i][temp].num = Empty;
					}
				}
			}
		}
	}

	//merge
	if (!again){
		for (int i = 0; i < SIZE; i++){
			for (int j = SIZE - 1; j > 0; j--){
				if (board->tileInfo[i][j].num == board->tileInfo[i][j - 1].num){
					board->tileInfo[i][j].num += board->tileInfo[i][j - 1].num;
					board->tileInfo[i][j - 1].num = Empty;
				}
			}
		}
	}

	if (!again) moveDown(board, true);

	if (again) return;
}

//run fall functions with a temp board to check if a fall can happen
bool trymove(){
	bool same;
	struct game tempBoard = gameBoard;

	moveDown(&tempBoard, false);
	same = noChange(tempBoard);
	if (!same) return same;

	moveLeft(&tempBoard, false);
	same = noChange(tempBoard);
	if (!same) return same;

	moveRight(&tempBoard, false);
	same = noChange(tempBoard);
	if (!same) return same;

	moveUp(&tempBoard, false);
	same = noChange(tempBoard);
	if (!same) return same;

	return same;
}

//check game finish, first check 2048, then movability.
bool checkFinish(){
	bool finish = false;

	for (int i = 0; i < SIZE; i++){
		for (int j = 0; j < SIZE; j++){
			if (gameBoard.tileInfo[i][j].num == 2048) finish = true;
		}
	}

	if (finish) {
		drawWin();
		return finish;
	}

	finish = trymove();

	if (finish) {
		drawLose();
		return finish;
	}

	return finish;
}

// Set Stack
void set_A9_IRQ_stack(void){
	int stack, mode;
	stack = 0xFFFFFFFF - 7;// top of A9 on-chip memory, aligned to 8 bytes 
 
	/* change processor to IRQ mode with interrupts disabled */
	mode = 0b11010010;
	asm("msr cpsr, %[ps]" : : [ps] "r" (mode));
	/* set banked stack pointer */
	asm("mov sp, %[ps]" : : [ps] "r" (stack));

	/* go back to SVC mode before executing subroutine return! */
	mode = 0b11010011;
	asm("msr cpsr, %[ps]" : : [ps] "r" (mode));

	//printf("Done setting up stack\n");
}

// Configure PS/2
void config_PS2(){
	volatile int * PS2_Control = (int *) 0xFF200104;
	*PS2_Control = 0x00000001;  //enable interrupts for PS2
}

// Configure Keys
void config_KEYs(){
	volatile int * KEY_ptr = (int *) 0xFF200050; // KEY base address
	*(KEY_ptr + 2) = 0xF; // enable interrupts for all four KEYs
}


// Configure Interrupt
void config_interrupt (int N, int CPU_target){
	int reg_offset, index, value, address;

	/* Configure the Interrupt Set-Enable Registers (ICDISERn). 
	 * reg_offset = (integer_div(N / 32) * 4
	 * value = 1 << (N mod 32) */

	reg_offset = (N >> 3) & 0xFFFFFFFC;
	index = N & 0x1F;
	value = 0x1 << index;
	address = 0xFFFED100 + reg_offset;

	/* Now that we know the register address and value, set the appropriate bit */ 
	*(int *)address |= value;

	/* Configure the Interrupt Processor Targets Register (ICDIPTRn) 
	 * reg_offset = integer_div(N / 4) * 4
	 * index = N mod 4 */

	reg_offset = (N & 0xFFFFFFFC);
	index = N & 0x3;
	address = 0xFFFED800 + reg_offset + index;

	/* Now that we know the register address and value, write to (only) the appropriate byte */ 
	*(char *)address = (char) CPU_target;
}

// Configure GIC
void config_GIC(void){
	config_interrupt (73, 1); // configure the KEYs parallel port (Interrupt ID = 73)
	config_interrupt (79, 1); // configure the PS/2 port (Interrupt ID = 79)

	// Set Interrupt Priority Mask Register (ICCPMR). Enable interrupts of all priorities
	*((int *) 0xFFFEC104) = 0xFFFF;

	// Set CPU Interface Control Register (ICCICR). Enable signaling of interrupts
	*((int *) 0xFFFEC100) = 1; // enable = 1

	// Configure the Distributor Control Register (ICDDCR) to send pending interrupts to CPUs
	*((int *) 0xFFFED000) = 1; // enable = 1
}

// Define the IRQ exception handler
void __attribute__ ((interrupt)) __cs3_isr_irq (void){

	// Read the ICCIAR from the processor interface
	int int_ID = *((int *) 0xFFFEC10C);
	if (int_ID == 79) // check if interrupt is from PS/2
		PS2_ISR();
	else if(int_ID == 73) // check if interrupt is from Keys
		KEY_ISR();
	else
		while (1){} // if unexpected, then stay here

	// Write to the End of Interrupt Register (ICCEOIR)
	*((int *) 0xFFFEC110) = int_ID;
	return;
}

// Define the remaining exception handlers
void __attribute__ ((interrupt)) __cs3_reset (void){
	while(1);
}
void __attribute__ ((interrupt)) __cs3_isr_undef (void){
	while(1);
}
void __attribute__ ((interrupt)) __cs3_isr_swi (void){
	while(1);
}
void __attribute__ ((interrupt)) __cs3_isr_pabort (void){
	while(1);
}
void __attribute__ ((interrupt)) __cs3_isr_dabort (void){
	while(1);
}
void __attribute__ ((interrupt)) __cs3_isr_fiq (void){
	while(1);
}

// Disable Processor Interrupts
void disable_A9_interrupts(void){
   int status = 0b11010011;
   asm("msr cpsr, %[ps]" : : [ps]"r"(status));
}

// Enable Processor Interrupts
void enable_A9_interrupts(void){
	int status = 0b01010011;
	asm("msr cpsr, %[ps]" : : [ps]"r"(status));
}

char byte1, byte2, byte3;

void PS2_ISR(){
	volatile int * PS2_Control = (int *) 0xFF200104;
	*PS2_Control = 1;  // clear RI bit

	volatile int * PS2_ptr = (int *) 0xFF200100; // PS/2 port address
	int PS2_data, RVALID;

	PS2_data = *(PS2_ptr); // read the Data register in the PS/2 port
	RVALID = PS2_data & 0x8000; // extract the RVALID field

	struct game tempBoard = gameBoard;//temp board

	if (RVALID){
		byte1 = byte2;
		byte2 = byte3;
		byte3 = PS2_data & 0xFF;
		if(byte2 == 0xF0){
			if(byte3 == 0x1c){
				moveLeft(&tempBoard, false);
				if (!noChange(tempBoard) && !checkFinish()){
					moveLeft(&gameBoard, false);
					makeNewTile();
				}
				drawBoard();
				checkFinish();
			}
			else if(byte3 == 0x23){
				moveRight(&tempBoard, false);
				if (!noChange(tempBoard) && !checkFinish()){
					moveRight(&gameBoard, false);
					makeNewTile();
				}
				drawBoard();
				checkFinish();
			}
			else if(byte3 == 0x1D){
				moveUp(&tempBoard, false);
				if (!noChange(tempBoard) && !checkFinish()){
					moveUp(&gameBoard, false);
					makeNewTile();
				}
				drawBoard();
				checkFinish();
			}
			else if(byte3 == 0x1B){
				moveDown(&tempBoard, false);
				if (!noChange(tempBoard) && !checkFinish()){
					moveDown(&gameBoard, false);
					makeNewTile();
				}
				drawBoard();
				checkFinish();
			}
		}
	}
	return;
}
			
void KEY_ISR(){
	/* KEY base address */
	volatile int *KEY_ptr = (int *) 0xFF200050;
	
	int press;
	press = *(KEY_ptr + 3); // read the pushbutton interrupt register
	*(KEY_ptr + 3) = press; // Clear the interrupt
	
	if (press & 0x1){ // KEY0
		begin();
		setLoc();
    	clear_screen();
		drawBackground();
		drawBoard();
	}
	return;
}


// code not shown for clear_screen() and draw_line() subroutines
void clear_screen(){
	int row;
	int column;
	for(row = 0; row < 240; row++){
		for(column = 0; column < 320; column++){
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

void drawBackground(){
	int row;
	int col;
	for(row = 0; row < 240; row++){
		for(col = 40; col < 280; col++){
			plot_pixel(col, row, 0xBD73);
		}
	}
}

void drawBoard(){
	for (int i = 0; i < SIZE; i++){
		for (int j = 0; j < SIZE; j++){
			if (gameBoard.tileInfo[i][j].num == 2) draw2(gameBoard.tileInfo[i][j].x, gameBoard.tileInfo[i][j].y);
			else if (gameBoard.tileInfo[i][j].num == 4) draw4(gameBoard.tileInfo[i][j].x, gameBoard.tileInfo[i][j].y);
			else if (gameBoard.tileInfo[i][j].num == 8) draw8(gameBoard.tileInfo[i][j].x, gameBoard.tileInfo[i][j].y);
			else if (gameBoard.tileInfo[i][j].num == 16) draw16(gameBoard.tileInfo[i][j].x, gameBoard.tileInfo[i][j].y);
			else if (gameBoard.tileInfo[i][j].num == 32) draw32(gameBoard.tileInfo[i][j].x, gameBoard.tileInfo[i][j].y);
			else if (gameBoard.tileInfo[i][j].num == 64) draw64(gameBoard.tileInfo[i][j].x, gameBoard.tileInfo[i][j].y);
			else if (gameBoard.tileInfo[i][j].num == 128) draw128(gameBoard.tileInfo[i][j].x, gameBoard.tileInfo[i][j].y);
			else if (gameBoard.tileInfo[i][j].num == 256) draw256(gameBoard.tileInfo[i][j].x, gameBoard.tileInfo[i][j].y);
			else if (gameBoard.tileInfo[i][j].num == 512) draw512(gameBoard.tileInfo[i][j].x, gameBoard.tileInfo[i][j].y);
			else if (gameBoard.tileInfo[i][j].num == 1024) draw1024(gameBoard.tileInfo[i][j].x, gameBoard.tileInfo[i][j].y);
			else if (gameBoard.tileInfo[i][j].num == 2048) draw2048(gameBoard.tileInfo[i][j].x, gameBoard.tileInfo[i][j].y);
			else if (gameBoard.tileInfo[i][j].num == 0) drawBlank(gameBoard.tileInfo[i][j].x, gameBoard.tileInfo[i][j].y);
		}
	}
}

void draw2(int x, int y){
	int row;
	int col;
	for(row = y; row < y + 50; row++){
		for(col = x; col < x + 50; col++){
			plot_pixel(col, row, WHITE);
		}
	}
	draw_line(x+20, y+15, x+30, y+15, 0x0);
	draw_line(x+20, y+25, x+30, y+25, 0x0);
	draw_line(x+20, y+35, x+30, y+35, 0x0);
	draw_line(x+30, y+15, x+30, y+25, 0x0);
	draw_line(x+20, y+25, x+20, y+35, 0x0);
}

void draw4(int x, int y){
	int row;
	int col;
	for(row = y; row < y + 50; row++){
		for(col = x; col < x + 50; col++){
			plot_pixel(col, row, 0xEF18);
		}
	}
	draw_line(x+20, y+15, x+20, y+25, 0x0);
	draw_line(x+30, y+15, x+30, y+35, 0x0);
	draw_line(x+20, y+25, x+30, y+25, 0x0);
}

void draw8(int x, int y){
	int row;
	int col;
	for(row = y; row < y + 50; row++){
		for(col = x; col < x + 50; col++){
			plot_pixel(col, row, 0xED8F);
		}
	}
	draw_line(x+20, y+15, x+30, y+15, WHITE);
	draw_line(x+20, y+25, x+30, y+25, WHITE);
	draw_line(x+20, y+35, x+30, y+35, WHITE);
	draw_line(x+20, y+15, x+20, y+35, WHITE);
	draw_line(x+30, y+15, x+30, y+35, WHITE);
}

void draw16(int x, int y){
	int row;
	int col;
	for(row = y; row < y + 50; row++){
		for(col = x; col < x + 50; col++){
			plot_pixel(col, row, 0xF4AC);
		}
	}
	draw_line(x+18, y+15, x+18, y+35, WHITE);
	
	draw_line(x+22, y+15, x+32, y+15, WHITE);
	draw_line(x+22, y+25, x+32, y+25, WHITE);
	draw_line(x+22, y+35, x+32, y+35, WHITE);
	draw_line(x+22, y+15, x+22, y+35, WHITE);
	draw_line(x+32, y+25, x+32, y+35, WHITE);
}

void draw32(int x, int y){
	int row;
	int col;
	for(row = y; row < y + 50; row++){
		for(col = x; col < x + 50; col++){
			plot_pixel(col, row, 0xF3EC);
		}
	}
	draw_line(x+13, y+15, x+23, y+15, WHITE);
	draw_line(x+13, y+25, x+23, y+25, WHITE);
	draw_line(x+13, y+35, x+23, y+35, WHITE);
	draw_line(x+23, y+15, x+23, y+35, WHITE);
	
	draw_line(x+27, y+15, x+37, y+15, WHITE);
	draw_line(x+27, y+25, x+37, y+25, WHITE);
	draw_line(x+27, y+35, x+37, y+35, WHITE);
	draw_line(x+37, y+15, x+37, y+25, WHITE);
	draw_line(x+27, y+25, x+27, y+35, WHITE);
}

void draw64(int x, int y){
	int row;
	int col;
	for(row = y; row < y + 50; row++){
		for(col = x; col < x + 50; col++){
			plot_pixel(col, row, 0xF2E7);
		}
	}
	draw_line(x+13, y+15, x+23, y+15, WHITE);
	draw_line(x+13, y+25, x+23, y+25, WHITE);
	draw_line(x+13, y+35, x+23, y+35, WHITE);
	draw_line(x+13, y+15, x+13, y+35, WHITE);
	draw_line(x+23, y+25, x+23, y+35, WHITE);
	
	draw_line(x+27, y+15, x+27, y+25, WHITE);
	draw_line(x+37, y+15, x+37, y+35, WHITE);
	draw_line(x+27, y+25, x+37, y+25, WHITE);
}

void draw128(int x, int y){
	int row;
	int col;
	for(row = y; row < y + 50; row++){
		for(col = x; col < x + 50; col++){
			plot_pixel(col, row, 0xEE6E);
		}
	}
	draw_line(x+13, y+17, x+13, y+33, WHITE);
	
	draw_line(x+17, y+17, x+25, y+17, WHITE);
	draw_line(x+17, y+25, x+25, y+25, WHITE);
	draw_line(x+17, y+33, x+25, y+33, WHITE);
	draw_line(x+25, y+17, x+25, y+25, WHITE);
	draw_line(x+17, y+25, x+17, y+33, WHITE);
	
	draw_line(x+29, y+17, x+37, y+17, WHITE);
	draw_line(x+29, y+25, x+37, y+25, WHITE);
	draw_line(x+29, y+33, x+37, y+33, WHITE);
	draw_line(x+29, y+17, x+29, y+33, WHITE);
	draw_line(x+37, y+17, x+37, y+33, WHITE);
}

void draw256(int x, int y){
	int row;
	int col;
	for(row = y; row < y + 50; row++){
		for(col = x; col < x + 50; col++){
			plot_pixel(col, row, 0xEE4C);
		}
	}
	draw_line(x+9, y+17, x+17, y+17, WHITE);
	draw_line(x+9, y+25, x+17, y+25, WHITE);
	draw_line(x+9, y+33, x+17, y+33, WHITE);
	draw_line(x+17, y+17, x+17, y+25, WHITE);
	draw_line(x+9, y+25, x+9, y+33, WHITE);
	
	draw_line(x+21, y+17, x+29, y+17, WHITE);
	draw_line(x+21, y+25, x+29, y+25, WHITE);
	draw_line(x+21, y+33, x+29, y+33, WHITE);
	draw_line(x+21, y+17, x+21, y+25, WHITE);
	draw_line(x+29, y+25, x+29, y+33, WHITE);
	
	draw_line(x+33, y+17, x+41, y+17, WHITE);
	draw_line(x+33, y+25, x+41, y+25, WHITE);
	draw_line(x+33, y+33, x+41, y+33, WHITE);
	draw_line(x+33, y+17, x+33, y+33, WHITE);
	draw_line(x+41, y+25, x+41, y+33, WHITE);
}

void draw512(int x, int y){
	int row;
	int col;
	for(row = y; row < y + 50; row++){
		for(col = x; col < x + 50; col++){
			plot_pixel(col, row, 0xEE2A);
		}
	}
	draw_line(x+13, y+17, x+21, y+17, WHITE);
	draw_line(x+13, y+25, x+21, y+25, WHITE);
	draw_line(x+13, y+33, x+21, y+33, WHITE);
	draw_line(x+13, y+17, x+13, y+25, WHITE);
	draw_line(x+21, y+25, x+21, y+33, WHITE);
	
	draw_line(x+25, y+17, x+25, y+33, WHITE);
	
	draw_line(x+29, y+17, x+37, y+17, WHITE);
	draw_line(x+29, y+25, x+37, y+25, WHITE);
	draw_line(x+29, y+33, x+37, y+33, WHITE);
	draw_line(x+37, y+17, x+37, y+25, WHITE);
	draw_line(x+29, y+25, x+29, y+33, WHITE);
}

void draw1024(int x, int y){
	int row;
	int col;
	for(row = y; row < y + 50; row++){
		for(col = x; col < x + 50; col++){
			plot_pixel(col, row, 0xEE08);
		}
	}
	draw_line(x+10, y+19, x+10, y+31, WHITE);
	
	draw_line(x+14, y+19, x+20, y+19, WHITE);
	draw_line(x+14, y+31, x+20, y+31, WHITE);
	draw_line(x+20, y+19, x+20, y+31, WHITE);
	draw_line(x+14, y+19, x+14, y+31, WHITE);
	
	draw_line(x+24, y+19, x+30, y+19, WHITE);
	draw_line(x+24, y+25, x+30, y+25, WHITE);
	draw_line(x+24, y+31, x+30, y+31, WHITE);
	draw_line(x+30, y+19, x+30, y+25, WHITE);
	draw_line(x+24, y+25, x+24, y+31, WHITE);
	
	draw_line(x+34, y+25, x+40, y+25, WHITE);
	draw_line(x+34, y+19, x+34, y+25, WHITE);
	draw_line(x+40, y+19, x+40, y+31, WHITE);
}

void draw2048(int x, int y){
	int row;
	int col;
	for(row = y; row < y + 50; row++){
		for(col = x; col < x + 50; col++){
			plot_pixel(col, row, 0xF5E6);
		}
	}
	draw_line(x+7, y+19, x+13, y+19, WHITE);
	draw_line(x+7, y+25, x+13, y+25, WHITE);
	draw_line(x+7, y+31, x+13, y+31, WHITE);
	draw_line(x+7, y+25, x+7, y+31, WHITE);
	draw_line(x+13, y+19, x+13, y+25, WHITE);
	
	draw_line(x+17, y+19, x+23, y+19, WHITE);
	draw_line(x+17, y+31, x+23, y+31, WHITE);
	draw_line(x+23, y+19, x+23, y+31, WHITE);
	draw_line(x+17, y+19, x+17, y+31, WHITE);
	
	draw_line(x+27, y+25, x+33, y+25, WHITE);
	draw_line(x+27, y+19, x+27, y+25, WHITE);
	draw_line(x+33, y+19, x+33, y+31, WHITE);
	
	draw_line(x+37, y+19, x+43, y+19, WHITE);
	draw_line(x+37, y+25, x+43, y+25, WHITE);
	draw_line(x+37, y+31, x+43, y+31, WHITE);
	draw_line(x+37, y+19, x+37, y+31, WHITE);
	draw_line(x+43, y+19, x+43, y+31, WHITE);
}

void drawBlank(int x, int y){
	int row;
	int col;
	for(row = y; row < y + 50; row++){
		for(col = x; col < x + 50; col++){
			plot_pixel(col, row, 0xCE16);
		}
	}
}

void drawWin(){
	draw_line(8, 70, 20, 82, WHITE);
	draw_line(20, 82, 32, 70, WHITE);
	draw_line(20, 82, 20, 98, WHITE);
	
	draw_line(8, 114, 8, 126, WHITE);
	draw_line(32, 114, 32, 126, WHITE);
	draw_line(16, 106, 24, 106, WHITE);
	draw_line(16, 134, 24, 134, WHITE);
	draw_line(8, 114, 16, 106, WHITE);
	draw_line(24, 106, 32, 114, WHITE);
	draw_line(8, 126, 16, 134, WHITE);
	draw_line(32, 126, 24, 134, WHITE);
	
	draw_line(8, 142, 8, 162, WHITE);
	draw_line(32, 142, 32, 162, WHITE);
	draw_line(16, 170, 24, 170, WHITE);
	draw_line(8, 162, 16, 170, WHITE);
	draw_line(32, 162, 24, 170, WHITE);
	
	draw_line(288, 70, 294, 98, WHITE);
	draw_line(294, 98, 300, 70, WHITE);
	draw_line(300, 70, 306, 98, WHITE);
	draw_line(306, 98, 312, 70, WHITE);
	
	draw_line(300, 106, 300, 134, WHITE);
	draw_line(295, 106, 305, 106, WHITE);
	draw_line(295, 134, 305, 134, WHITE);
	
	draw_line(288, 142, 288, 170, WHITE);
	draw_line(312, 142, 312, 170, WHITE);
	draw_line(288, 142, 312, 170, WHITE);
}

void drawLose(){
	draw_line(8, 70, 20, 82, WHITE);
	draw_line(20, 82, 32, 70, WHITE);
	draw_line(20, 82, 20, 98, WHITE);
	
	draw_line(8, 114, 8, 126, WHITE);
	draw_line(32, 114, 32, 126, WHITE);
	draw_line(16, 106, 24, 106, WHITE);
	draw_line(16, 134, 24, 134, WHITE);
	draw_line(8, 114, 16, 106, WHITE);
	draw_line(24, 106, 32, 114, WHITE);
	draw_line(8, 126, 16, 134, WHITE);
	draw_line(32, 126, 24, 134, WHITE);
	
	draw_line(8, 142, 8, 162, WHITE);
	draw_line(32, 142, 32, 162, WHITE);
	draw_line(16, 170, 24, 170, WHITE);
	draw_line(8, 162, 16, 170, WHITE);
	draw_line(32, 162, 24, 170, WHITE);
	
	draw_line(288, 52, 288, 80, WHITE);
	draw_line(288, 80, 312, 80, WHITE);
	
	draw_line(288, 98, 288, 108, WHITE);
	draw_line(312, 98, 312, 108, WHITE);
	draw_line(296, 88, 304, 88, WHITE);
	draw_line(296, 116, 304, 116, WHITE);
	draw_line(288, 98, 296, 88, WHITE);
	draw_line(312, 98, 304, 88, WHITE);
	draw_line(288, 108, 296, 116, WHITE);
	draw_line(312, 108, 304, 116, WHITE);
	
	draw_line(292, 124, 308, 124, WHITE);
	draw_line(288, 128, 288, 134, WHITE);
	draw_line(292, 138, 308, 138, WHITE);
	draw_line(312, 142, 312, 148, WHITE);
	draw_line(292, 152, 308, 152, WHITE);
	draw_line(308, 124, 312, 128, WHITE);
	draw_line(292, 124, 288, 128, WHITE);
	draw_line(288, 134, 292, 138, WHITE);
	draw_line(308, 138, 312, 142, WHITE);
	draw_line(312, 148, 308, 152, WHITE);
	draw_line(292, 152, 288, 148, WHITE);
	
	draw_line(288, 160, 288, 188, WHITE);
	draw_line(288, 160, 312, 160, WHITE);
	draw_line(288, 174, 312, 174, WHITE);
	draw_line(288, 188, 312, 188, WHITE);
}