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

unsigned char lock = 0;
static unsigned char next_state = 0;
static unsigned char time_till_drop = NORMAL_DROP; // can change depending on time_till_drop_time
Tetronimo tetronimo = {0};
unsigned char time_till_drop_time = NORMAL_DROP;
unsigned char board[ROWS][COLUMNS] = {0};

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

    set_piece(PIECE);
}

static void set_rand_seed(void)
{
    ADCSRA |= BIT6; // start conversion

    while(ADCSRA & BIT6);
    srand(ADC);
}

/**
 * gen_rand_tetronimo()
 *
 * Generate a random tetronimo piece using ADC as seed.
 *
 * Return: void
 */
static unsigned char gen_rand_tetronimo(void)
{
    return rand()%7;
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
    //tetronimo.type = gen_rand_tetronimo();
    tetronimo.type = J_PIECE;
    tetronimo.rotation = ROT_0_DEG;

    // Set starting coordinates of center piece depending on tetronimo type
    switch(tetronimo.type) {
    case I_PIECE:
	_set_tetronimo_start_pos(1,4, 1,5, 1,6, 1,7);
	break;
    case O_PIECE:
	_set_tetronimo_start_pos(0,5, 0,6, 1,5, 1,6);
	break;
    case T_PIECE:
	_set_tetronimo_start_pos(1,4, 0,5, 1,5, 1,6);
	break;
    case Z_PIECE:
	_set_tetronimo_start_pos(0,4, 0,5, 1,5, 1,6);
	break;
    case S_PIECE:
	_set_tetronimo_start_pos(1,4, 1,5, 0,5, 0,6);
	break;
    case L_PIECE:
	_set_tetronimo_start_pos(1,4, 1,6, 1,5, 0,6);
	break;
    case J_PIECE:
	_set_tetronimo_start_pos(0,4, 1,4, 1,5, 1,6);
	break;
    }
}

static void init_board(void);
void clear_lines(void);
unsigned char check_game_over(void);
ISR(TIMER1_COMPA_vect)
{
    // Audio functionality, switch to a different note based on tetris_melody
    if(tetris_melody[note]) {
	audio.change_note(tetris_melody[note]);
    }

    if(++note >= tetris_melody_length) {
	note = 0;
    }

    // Drop a piece every 500 ms
    if(!(--time_till_drop)) {
	time_till_drop = time_till_drop_time;
	next_state = 1;
    }
}

/**
 * next_state_logic()
 *
 * Called every time TIMER1 interrupt 1 sets the next_state var.
 * The ISR is what sets the pace of the game.
 */
void next_state_logic(void)
{
    if(next_state) {
	if(reached_bottom()) {
	    lock = 1;
	    set_piece(FILLED);
	    clear_lines();
	    if(check_game_over())
		{
		    init_board();
		}
	    init_tetronimo();
	    lock = 0;
	} else {
	    drop();
	}
	update_display();
	next_state = 0;
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
 * init_board()
 *
 * Initialize the board to be empty.
 *
 * Return: void
 */
static void init_board(void)
{
    unsigned char val = 0;
    for(unsigned char row=0; row < ROWS; ++row) {
	for(unsigned char col = 0; col < COLUMNS; ++col) {
	    if(row == ROWS-1) {
		board[row][col] = FILLED;
	    } else if(col >= DISP_START_COL && col < DISP_END_COL) {
		board[row][col] = EMPTY;;
	    } else {
		board[row][col] = FILLED; // create a border around the display
	    }
	}
    }
}

static void set_rand_seed(void);
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
    set_rand_seed();

    init_board();
    init_tetronimo();
    update_display();
    audio.play();
}

/**
 * check_row_filled()
 * @row: Row this function will be checking if full
 *
 * Return: 'bool' of whether it is filled or not
 */
unsigned char row_filled(unsigned char row)
{
    for(unsigned char col = DISP_START_COL; col < DISP_END_COL; ++col) {
	if(board[row][col] != FILLED) {
	    return 0;
	}
    }
    return 1;

}

/**
 * clear_row()
 * @row: Row this function will clear
 *
 * Clear all the columns in this row.
 *
 * Return: void
 */
void clear_row(unsigned char row)
{
    for(unsigned char col = DISP_START_COL; col < DISP_END_COL; ++col) {
	board[row][col] = EMPTY;
    }
}

/**
 * shift_row()
 * @row:
 *
 * Return: void
 */
void shift_row(unsigned char t_row)
{
    for(unsigned char row = t_row-1; row >= DISP_START_ROW; --row) {
	for(unsigned char col = DISP_START_COL; col < DISP_END_COL; ++col) {
	    if(board[row][col] == FILLED) {
		board[row][col] = EMPTY;
		board[row+1][col] = FILLED;
	    }
	}
    }
}

/**
 * clear_lines()
 *
 * If any lines are filled (same line 8 cols filled), clear them.
 * If none, do nothing.
 *
 * Return: void
 */
void clear_lines(void)
{

    for(unsigned char row = DISP_START_ROW; row < DISP_BOT_END; ++row) {
	if(row_filled(row)) {
	    clear_row(row);
	    shift_row(row);
	}
    }
}

/**
 * check_game_over()
 *
 * Return: 'bool' of whether game over or not
 */
unsigned char check_game_over(void)
{
    // If anything filled in the buffer, game over!
    for(unsigned char row = 0; row < DISP_START_ROW; ++row) {
	for(unsigned char col = DISP_START_COL; col < DISP_END_COL; ++col)
	    {
		if(FILLED == board[row][col]) {
		    return 1;
		}
	    }
    }
    return 0;
}
