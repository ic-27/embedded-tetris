#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <stdlib.h>

#include "audio_driver.h"
#include "bt_driver.h"
#include "display_driver.h"
#include "power.h"

#include "tetris.h"
#include "movement.h"
#include "display.h"
#include "button_handler.h"
#include "common.h"

unsigned char lock = 0;
volatile unsigned char damage = 0;
volatile unsigned char next_move = WAIT_NEXT_MOVE;
static unsigned char time_till_drop = NORMAL_DROP; // can change depending on time_till_drop_time
unsigned char time_till_drop_time = NORMAL_DROP;
Tetronimo tetronimo = {0};
unsigned char board[ROWS][COLUMNS] = {0};


void reinit(void);
void deinit(void);

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

/**
 * adc_set_rand()
 *
 * Set the seed for random number from floating ADC voltage.
 *
 * Return: void
 */
static void adc_set_rand(void)
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
    return rand()%NUM_TETRIS_TYPES;
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
    tetronimo.type = gen_rand_tetronimo();
    tetronimo.rotation = ROT_0_DEG;

    // Set starting coordinates of center piece depending on tetronimo type
    switch(tetronimo.type) {
    case I_PIECE:
	_set_tetronimo_start_pos(1,5, 1,6, 1,7, 1,8);
	break;
    case O_PIECE:
	_set_tetronimo_start_pos(0,6, 0,7, 1,6, 1,7);
	break;
    case T_PIECE:
	_set_tetronimo_start_pos(1,5, 0,6, 1,6, 1,7);
	break;
    case Z_PIECE:
	_set_tetronimo_start_pos(0,5, 0,6, 1,6, 1,7);
	break;
    case S_PIECE:
	_set_tetronimo_start_pos(1,5, 1,6, 0,6, 0,7);
	break;
    case L_PIECE:
	_set_tetronimo_start_pos(1,5, 1,7, 1,6, 0,7);
	break;
    case J_PIECE:
	_set_tetronimo_start_pos(0,5, 1,5, 1,6, 1,7);
	break;
    default:
	// Debugging
	/* DDRD |= (1 << PD2); */
	/* PORTD ^= (1 << PD2); */
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
	next_move = NEXT_MOVE_READY;
    }
}

/**
 * topmost_filled_row()
 *
 * Return the topmost row that is filled.
 * Note that the higher the row, the smaller the row number.
 *
 * Return: void
 */
static unsigned char topmost_filled_row(void)
{
    for(unsigned char row = DISP_START_ROW; row < DISP_BOT_END; ++row) {
	for(unsigned char col = DISP_START_COL; col < DISP_END_COL; ++col) {
	    if(FILLED == board[row][col]) {
		return row;
	    }
	}
    }
    return DISP_BOT_END; // no filled rows
}

/**
 * gen_rand_column()
 *
 * Generate a random column.
 *
 * Return: A random column number within display limits
 */
static unsigned char gen_rand_column(void)
{
    return (rand()%NUM_COLUMNS_DISPLAYED)+DISP_START_COL;
}

/**
 * add_damage()
 * @tp_filled_row: topmost row number w/ a column filled
 * @damage: Damage that was sent from other player
 *
 * Add damage to the board. If the row is out of bounds, then game over.
 *
 * Return: Returns bool of whether it was successful or not
 */
static unsigned char add_damage(unsigned char tp_filled_row, unsigned char damage)
{
    // Shift everything up by 'damage'
    for(unsigned char row = tp_filled_row; row < DISP_BOT_END; ++row) {
	for(unsigned char col = DISP_START_COL; col < DISP_END_COL; ++col) {
	    if(row-damage > DISP_BOT_END) { //catch case of trying to assign row less than 0
#warning make sure that this is correct...
		return 0;
	    }
	    board[row-damage][col] = board[row][col];
	}
    }
    //since we shifted everything up by 'damage', fill the last 'damage' rows
    unsigned char empty_col = gen_rand_column();
    for(unsigned char row = DISP_BOT_END-1; row >= DISP_BOT_END-damage; --row) {
	for(unsigned char col = DISP_START_COL; col < DISP_END_COL; ++col) {
	    if(col != empty_col) {
		board[row][col] = FILLED;
	    } else {
		board[row][col] = EMPTY;
	    }
	}
    }
    return 1;
}

/**
 * check_overlay()
 *
 * Check if the current tetris piece was overlayed on top of after damage
 * shifted all the rows up.
 *
 * Return: bool if current tetris piece was overlayed
 */
static unsigned char check_overlay(void)
{
    if(FILLED == board[tetronimo.c1.row][tetronimo.c1.col] ||
       FILLED == board[tetronimo.c2.row][tetronimo.c2.col] ||
       FILLED == board[tetronimo.c3.row][tetronimo.c3.col] ||
       FILLED == board[tetronimo.c4.row][tetronimo.c4.col]) {
	return 1;
    }
    return 0;
}

/**
 * shift_tetris_piece_up()
 *
 * Shift the tetris piece up. This is used when we have received damage, but the
 * current tetris piece will coincides if we have received the damage.
 *
 * Return: boolean of whether shift was successful or not.
 */
static unsigned char shift_tetris_piece_up(void)
{
    // Make sure we aren't shifting up into the buffer zone (row 0 & 1)
    while(tetronimo.c1.row-1 >= DISP_START_ROW && tetronimo.c2.row-1 >= DISP_START_ROW &&
	  tetronimo.c3.row-1 >= DISP_START_ROW && tetronimo.c4.row-1 >= DISP_START_ROW) {
	--tetronimo.c1.row; --tetronimo.c2.row; --tetronimo.c3.row; --tetronimo.c4.row;
	// Make sure the board is empty where we are shifting into
	if(board[tetronimo.c1.row][tetronimo.c1.col] == EMPTY &&
	   board[tetronimo.c2.row][tetronimo.c2.col] == EMPTY &&
	   board[tetronimo.c3.row][tetronimo.c3.col] == EMPTY &&
	   board[tetronimo.c4.row][tetronimo.c4.col] == EMPTY) {
	    set_piece(PIECE);
	    return 1;
	    break;
	}
    }
    set_piece(PIECE);
    return 0;
}

/**
 * next_move_logic()
 *
 * Called every time TIMER1 interrupt 1 sets the next_move var.
 * This function checks for game over conditions and handles logic for dropping
 * the piece and receiving damage.
 *
 * Return: void
 */
void next_move_logic(void)
{
    if(damage) { // check for any damage first if PVP
	unsigned char tp_filled_row = topmost_filled_row();
	if(!add_damage(tp_filled_row, damage)) {
	    init_board(); // game over, restart!
	    init_tetronimo();
	} else if(check_overlay()) {
	    if(!shift_tetris_piece_up()) {
		init_board(); // game over, restart!
		init_tetronimo();
	    }
	}
	damage = 0;
    }

    if(reached_bottom() == 1) {
	lock = 1;
	set_piece(FILLED);
	clear_lines();
	if(check_game_over()) {
	    init_board(); // game over, restart!
	}
	init_tetronimo();

	lock = 0;
    } else {
	drop();
    }
    update_display();
    next_move = WAIT_NEXT_MOVE;
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
    for(unsigned char row=0; row < ROWS; ++row) {
	for(unsigned char col = 0; col < COLUMNS; ++col) {
	    if(row == ROWS-1) {
		board[row][col] = FILLED;
	    } else if(col >= DISP_START_COL && col < DISP_END_COL) {
		board[row][col] = EMPTY;
	    } else {
		board[row][col] = FILLED; // create a border around the display
	    }
	}
    }
}

/**
 * power_disable_int() - Enable interrupt for power pin
 *
 * Return: void
 */
void power_disable_int()
{
    GICR &= ~(1 << INT2);
}

/**
 * External interrupt for detecting slide switch (power button)
 */
ISR(INT2_vect)
{
    if(!power_button_trig) {
	power_button_trig = 1;
	power.start_main_clock();
    }
}

/**
 * reinit()
 *
 * Initialize modules upon a power on.
 *
 * Return: void
 */
void reinit(void)
{
    // config slide switch to look for rising edge
    MCUCSR |= (1 << ISC2); 

    // restart audio (timer)
    audio.play();
    button.start_poll();
    
    // power on inits
    init_board();
    init_tetronimo();
    display.init();

    power.turn_on();
    power_button_trig = 0;
}

/**
 * deinit()
 *
 * De-init modules upon a power off.
 *
 * Return: void
 */
void deinit(void)
{
    // config slide switch to look for falling edge
    MCUCSR &= ~(1 << ISC2);

    // stop software timers/interrupts
    button.stop_poll();
    audio.stop();
    power.stop_main_clock();

    // turn off MAX7219 to save power
    display.off();

    // power down de-inits
    power_button_trig = 0;
    power.turn_off();
    /* power_enable_int(); */
    /* set_sleep_mode(SLEEP_MODE_PWR_DOWN); */
    /* sleep_mode(); */
}

void check_power_switch(void)
{
    // if power button triggered, check the state and turn on/off accordingly
    if(power_button_trig) {
	power_disable_int();
	if(power.check_switch_state()) {
	    reinit();
	} else {
	    deinit();
	}
	
    }
}

/**
 * start_tetris()
 *
 * Initialize all the different peripherals and start!.
 *
 * Return: void
 */
void start_tetris(void)
{
    power.init();
    //power_init(); // move later
	
    // initialization of drivers
    display.init();
    audio.init();
    button.init();
    bluetooth.init();
	
    // initializations in this module
    init_adc();
    adc_set_rand();
    init_board();
    init_tetronimo();

    // check power on/off
    if(power.check_switch_state()) { // on
	reinit();
	audio.play();
	power.start_main_clock();
    } else { // off
	deinit();
    }
}

/**
 * row_filled()
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
 * Shift row down
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
