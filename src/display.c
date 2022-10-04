#include "tetris.h"
#include "display.h"
#include "display_driver.h"

/*
 * RETURN_SET()
 *
 * Return whether x is greater than 0.
 * Useful for when converting a char array to bitform.
 * Value inside char array may be greater than 2, which will mess up the display.
*/
#define RETURN_SET(x) (x ? 1:0)

/**
 * update_display()
 *
 * Update the display according to the 'board' variable.
 * Note that because the board starts displaying from row 2. Rows 0-1 are used
 * to as a starting place to spawn a tetris piece.
 *
 * Return: void
 */
void update_display(void)
{
    // Update top led matrix
    unsigned char bit_form = 0x00;
    for(unsigned char row=DISP_START_ROW; row<DISP_TOP_END; ++row) {
	for(unsigned char i=DISP_END_COL-1, j=0; i>=DISP_START_COL; --i, ++j) { // convert to bitform
	    bit_form |= (RETURN_SET(board[row][i]) << j);
	}
	display.spi_send_cmd_top(row-DISP_START_ROW+1, bit_form); // +1 since 0 is no-op
	bit_form = 0x00;
    }
    // Update bottom led matrix
    for(unsigned char row=DISP_TOP_END; row<DISP_BOT_END; ++row) {
	for(unsigned char i=DISP_END_COL-1, j=0; i>=DISP_START_COL; --i, ++j) { // convert to bitform
	    bit_form |= (RETURN_SET(board[row][i]) << j);
	}
	display.spi_send_cmd_bot(row-DISP_TOP_END+1, bit_form); // +1 since 0 is no-op
	bit_form = 0x00;
    }
}
