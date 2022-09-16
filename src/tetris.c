#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#include "audio_driver.h"
#include "bt_driver.h"
#include "display_driver.h"

#include "tetris.h"
#include "movement.h"
#include "display.h"
#include "button_handler.h"
#include "common.h"

static unsigned char time_till_drop = NORMAL_DROP;
Tetronimo tetronimo = {0};
unsigned char board[ROWS][COLUMNS] = {0};
unsigned char time_till_drop_time = NORMAL_DROP; // so it can be controlled

static void init_tetronimo();
ISR(TIMER1_COMPA_vect)
{
    // Audio functionality, switch to a different note based on tetris_melody
    if(tetris_melody[note]) {
	audio.change_note(tetris_melody[note]);
    }

    ++note;
    if(note >= tetris_melody_length) {
	note = 0;
    }

    // Drop a piece every 500 ms
    if(!(--time_till_drop)) {
	time_till_drop = time_till_drop_time;
	if(reached_bottom()) {
	    set_piece(FILLED);
	    init_tetronimo();
	} else {
	    drop();
	}
	update_display();
    }
}

/**
 * init_adc()
 *
 * Initialize ADC for random tetronimo generation.
 *
 * Return: void
 */
void init_adc(void)
{
    ADMUX  |= (1 << 6);
    ADCSRA |= (1 << 7);
}

/**
 * gen_rand_tetronimo()
 *
 * Generate a random tetronimo piece using ADC
 *
 * Return: void
 */
static unsigned char gen_rand_tetronimo(void)
{
    ADCSRA |= BIT6; // start conversion

    while(ADCSRA & BIT6);
    srand(ADC);
    return rand()%7;
}

static void init_board(void)
{
    unsigned char val = 0;
    for(unsigned char row=0; row < ROWS; ++row) {
	if(row == ROWS-1) {
	    val = 2;
	}
	for(unsigned char col = 0; col < COLUMNS; ++col) {
	    if(col >= DISP_START_COL && col < DISP_END_COL) {
		board[row][col] = val;
	    } else {
		board[row][col] = 2; // create a border around the display
	    }
	}
    }
}

/**
 * init_tetris()
 *
 * Initialize all the different peripherals.
 *
 * Return: void
 */
void init_tetris(void)
{
    // initialization of drivers
    display.init();
    audio.init();
    button.init();
    bluetooth.init();

    // initializations in this module
    init_adc();

    init_board();
    init_tetronimo();
    update_display();
    audio.play();
}

/**
 * _set_tetronimo_start_pos()
 *
 * Set the starting position of the tetronimo on the board.
 * Note that cell 3 will be set as the center.
 *
 * Return: void
 */
static void _set_tetronimo_start_pos(unsigned char c1_row, unsigned char c1_col,
				     unsigned char c2_row, unsigned char c2_col,
				     unsigned char c3_row, unsigned char c3_col,
				     unsigned char c4_row, unsigned char c4_col)
{

    tetronimo.c1 = (Cell){.col=c1_col, .row=c1_row};
    tetronimo.c2 = (Cell){.col=c2_col, .row=c2_row};
    tetronimo.c3 = (Cell){.col=c3_col, .row=c3_row};
    tetronimo.c4 = (Cell){.col=c4_col, .row=c4_row};

    board[c1_row][c1_col] = 1;
    board[c2_row][c2_col] = 1;
    board[c3_row][c3_col] = 1;
    board[c4_row][c4_col] = 1;
}

/**
 * init_tetronimo()
 *
 * Create a new tetronimo piece and initialize its values.
 *
 * Return: void
 */
static void init_tetronimo()
{
#warning change back
    //tetronimo.type = gen_rand_tetronimo();
    tetronimo.type = I_PIECE;
    tetronimo.rotation = ROT_0_DEG;

    // Set starting coordinates of center piece depending on tetronimo type
    switch(tetronimo.type) {
    case I_PIECE:
	_set_tetronimo_start_pos(1,4, 1,5, 1,6, 1,7);
	break;
    case O_PIECE:
	_set_tetronimo_start_pos(3,2, 4,2, 3,3, 4,3);
	break;
    case T_PIECE:
	_set_tetronimo_start_pos(2,3, 3,2, 3,3, 4,3);
	break;
    case Z_PIECE:
	_set_tetronimo_start_pos(2,2, 3,2, 3,3, 4,3);
	break;
    case S_PIECE:
	break;
    case L_PIECE:
	break;
    case J_PIECE:
	break;
    }
}
