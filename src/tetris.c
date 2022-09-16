#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#include "audio_driver.h"
#include "bt_driver.h"
#include "display_driver.h"

#include "tetris.h"
#include "display.h"
#include "button_handler.h"
#include "common.h"

unsigned char board[ROWS][COLUMNS] = {0};
unsigned char time_till_drop = 2;
Tetronimo tetronimo = {0};

static void init_tetronimo();
static void drop(void);
static void set_piece(unsigned char val);
static unsigned char reached_bottom(void);
static void shift_tetronimo_cw(void);
static void rotate_tetronimo(unsigned char direction);
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
	time_till_drop = 2;
	if(reached_bottom()) {
	    set_piece(2);
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
    rotate_tetronimo(0);
    /* rotate_tetronimo(0); */
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

void rotate_I(unsigned char new_rotation, unsigned char direction);
static void rotate_tetronimo(unsigned char direction)
{
    // find new rotation
    unsigned char rotation = tetronimo.rotation;
    if(direction) {
	++rotation;
    } else {
	--rotation;
    }

    //check if valid rotation state, since only 2 bits
    if(rotation == ROT_OVER) {
	rotation = ROT_0_DEG;
    } else if(rotation == ROT_UNDER) {
	rotation = ROT_270_DEG;
    }

    switch(tetronimo.type) {
    case I_PIECE:
	rotate_I(rotation, direction);
	break;
    case O_PIECE:
	break;
    case T_PIECE:
	break;
    case Z_PIECE:
	break;
    case S_PIECE:
	break;
    case L_PIECE:
	break;
    case J_PIECE:
	break;
    }
}

/**
 * drop()
 *
 * Drop the tetronimo one cell down.
 *
 * Return: void
 */
static void drop(void)
{
    set_piece(0);

    board[++tetronimo.c1.row][tetronimo.c1.col] = 1;
    board[++tetronimo.c2.row][tetronimo.c2.col] = 1;
    board[++tetronimo.c3.row][tetronimo.c3.col] = 1;
    board[++tetronimo.c4.row][tetronimo.c4.col] = 1;
}

/**
 * set_piece()
 *
 * Set the piece if it has reached the bottom.
 *
 * Return: void
 */
static void set_piece(unsigned char val)
{
    board[tetronimo.c1.row][tetronimo.c1.col] = val;
    board[tetronimo.c2.row][tetronimo.c2.col] = val;
    board[tetronimo.c3.row][tetronimo.c3.col] = val;
    board[tetronimo.c4.row][tetronimo.c4.col] = val;
}

static unsigned char reached_bottom(void)
{
    if(board[tetronimo.c1.row+1][tetronimo.c1.col] == 2 ||
       board[tetronimo.c2.row+1][tetronimo.c2.col] == 2 ||
       board[tetronimo.c3.row+1][tetronimo.c3.col] == 2 ||
       board[tetronimo.c4.row+1][tetronimo.c4.col] == 2) {
	return 1;
    }
    return 0;
}

static unsigned char valid_rotation(Tetronimo * const t_copy);
/**
 * rotate_I()
 * @new_rotation: New rotation to pivot to
 * @direction: Direction we are rotating in 
 *
 * Rotate an 'I' piece. Specifically for an 'I' piece, we need to know which
 * direction we are rotating in since the pivot point is in an awkward spot.
 *
 * Return: void
 */
void rotate_I(unsigned char new_rotation, unsigned char direction)
{
    Tetronimo t_copy = tetronimo;
    switch(new_rotation) {
    case ROT_0_DEG:; // Can't declare something after a label, interesting!
	Cell pivot = (Cell){.row = t_copy.c3.row-1, .col = t_copy.c3.col}; // covers ROT_90_DEG
	if(tetronimo.rotation == ROT_270_DEG) {
	    pivot = (Cell){.row = t_copy.c3.row, .col = t_copy.c3.col+1};
	}

	t_copy.c1 = (Cell){.row = pivot.row, .col = pivot.col-2};
	t_copy.c2 = (Cell){.row = pivot.row, .col = pivot.col-1};
	t_copy.c3 = pivot;
	t_copy.c4 = (Cell){.row = pivot.row, .col = pivot.col+1};
	break;
    case ROT_90_DEG:
	if(tetronimo.rotation == ROT_0_DEG) {
	    pivot = (Cell){.row = t_copy.c3.row+1, .col = t_copy.c3.col};
	} else if(tetronimo.rotation == ROT_180_DEG) {
	    pivot = (Cell){.row = t_copy.c3.row, .col = t_copy.c3.col+1};
	}

	t_copy.c1 = (Cell){.row = pivot.row-2, .col = pivot.col};
	t_copy.c2 = (Cell){.row = pivot.row-1, .col = pivot.col};
	t_copy.c3 = pivot;
	t_copy.c4 = (Cell){.row = pivot.row+1, .col = pivot.col};
	break;
    case ROT_180_DEG:
	if(tetronimo.rotation == ROT_90_DEG) {
	    pivot = (Cell){.row = t_copy.c3.row, .col = t_copy.c3.col-1};
	} else if(tetronimo.rotation == ROT_270_DEG) {
	    pivot = (Cell){.row = t_copy.c3.row+1, .col = t_copy.c3.col};
	}

	t_copy.c1 = (Cell){.row = pivot.row, .col = pivot.col+2};
	t_copy.c2 = (Cell){.row = pivot.row, .col = pivot.col+1};
	t_copy.c3 = pivot;
	t_copy.c4 = (Cell){.row = pivot.row, .col = pivot.col-1};
	break;
    case ROT_270_DEG:
	if(tetronimo.rotation == ROT_0_DEG) {
	    pivot = (Cell){.row = t_copy.c3.row, .col = t_copy.c3.col-1};
	} else if(tetronimo.rotation == ROT_180_DEG) {
	    pivot = (Cell){.row = t_copy.c3.row-1, .col = t_copy.c3.col};
	}

	t_copy.c1 = (Cell){.row = pivot.row+2, .col = pivot.col};
	t_copy.c2 = (Cell){.row = pivot.row+1, .col = pivot.col};
	t_copy.c3 = pivot;
	t_copy.c4 = (Cell){.row = pivot.row-1, .col = pivot.col};
	break;
    }

    if(valid_rotation(&t_copy)) {
	set_piece(0);
	tetronimo = t_copy;
	tetronimo.rotation = new_rotation;
	set_piece(1);
	update_display();
    }
}

static unsigned char valid_rotation(Tetronimo * const t_copy)
{
    if(board[t_copy->c1.row][t_copy->c1.col] == 2 ||
       board[t_copy->c2.row][t_copy->c2.col] == 2 ||
       board[t_copy->c3.row][t_copy->c3.col] == 2 ||
       board[t_copy->c4.row][t_copy->c4.col] == 2) {
	return 0;
	}
    return 1;
}
