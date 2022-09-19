#include "tetris.h"
#include "display.h"

/**
 * reached_bottom()
 *
 * Check if the tetronimo has reached the bottom yet
 *
 * Return: void
 */
unsigned char reached_bottom(void)
{
    if(board[tetronimo.c1.row+1][tetronimo.c1.col] == FILLED ||
       board[tetronimo.c2.row+1][tetronimo.c2.col] == FILLED ||
       board[tetronimo.c3.row+1][tetronimo.c3.col] == FILLED ||
       board[tetronimo.c4.row+1][tetronimo.c4.col] == FILLED) {
	return 1;
    }
    return 0;
}

/**
 * set_piece()
 *
 * Set the piece to clear the board, display a piece, or set it as filled.
 *
 * Return: void
 */
void set_piece(unsigned char val)
{
    board[tetronimo.c1.row][tetronimo.c1.col] = val;
    board[tetronimo.c2.row][tetronimo.c2.col] = val;
    board[tetronimo.c3.row][tetronimo.c3.col] = val;
    board[tetronimo.c4.row][tetronimo.c4.col] = val;
}

/**
 * drop()
 *
 * Drop the tetronimo one cell down.
 *
 * Return: void
 */
void drop(void)
{
    set_piece(EMPTY);

    board[++tetronimo.c1.row][tetronimo.c1.col] = PIECE;
    board[++tetronimo.c2.row][tetronimo.c2.col] = PIECE;
    board[++tetronimo.c3.row][tetronimo.c3.col] = PIECE;
    board[++tetronimo.c4.row][tetronimo.c4.col] = PIECE;
}

/**
 * move_left()
 *
 * Move tetronimo left.
 *
 * Return: void
 */
void move_left(void)
{
    if(board[tetronimo.c1.row][tetronimo.c1.col-1] == FILLED ||
       board[tetronimo.c2.row][tetronimo.c2.col-1] == FILLED ||
       board[tetronimo.c3.row][tetronimo.c3.col-1] == FILLED ||
       board[tetronimo.c4.row][tetronimo.c4.col-1] == FILLED) {
	return;
    }
    set_piece(EMPTY);

    board[tetronimo.c1.row][--tetronimo.c1.col] = PIECE;
    board[tetronimo.c2.row][--tetronimo.c2.col] = PIECE;
    board[tetronimo.c3.row][--tetronimo.c3.col] = PIECE;
    board[tetronimo.c4.row][--tetronimo.c4.col] = PIECE;

    update_display();
}

/**
 * move_right()
 *
 * Move tetronimo right.
 *
 * Return: void
 */
void move_right(void)
{
    if(board[tetronimo.c1.row][tetronimo.c1.col+1] == FILLED ||
       board[tetronimo.c2.row][tetronimo.c2.col+1] == FILLED ||
       board[tetronimo.c3.row][tetronimo.c3.col+1] == FILLED ||
       board[tetronimo.c4.row][tetronimo.c4.col+1] == FILLED) {
	return;
    }
    set_piece(EMPTY);

    board[tetronimo.c1.row][++tetronimo.c1.col] = PIECE;
    board[tetronimo.c2.row][++tetronimo.c2.col] = PIECE;
    board[tetronimo.c3.row][++tetronimo.c3.col] = PIECE;
    board[tetronimo.c4.row][++tetronimo.c4.col] = PIECE;

    update_display();
}

/**
 * fast_drop()
 *
 * Drop the tetronimo faster.
 *
 * Return: void
 */
void fast_drop(void)
{
    time_till_drop_time = FAST_DROP;
}

void disable_fast_drop(void)
{
    time_till_drop_time = NORMAL_DROP;
}

/**
 * valid_rotation()
 *
 * Check a tetris piece has a valid rotation.
 *
 * Return: void
 */
unsigned char valid_rotation(Tetronimo * const t_copy)
{
    if(board[t_copy->c1.row][t_copy->c1.col] == FILLED ||
       board[t_copy->c2.row][t_copy->c2.col] == FILLED ||
       board[t_copy->c3.row][t_copy->c3.col] == FILLED ||
       board[t_copy->c4.row][t_copy->c4.col] == FILLED) {
	return 0;
    }
    return 1;
}

/**
 * rotate_I()
 * @t_copy: Pointer to a tetronimo struct used to check if rotation is valid
 * @new_rotation: New rotation to pivot to
 *
 * Rotate an 'I' piece. Specifically for an 'I' piece, we need to know which
 * direction we are rotating in since the pivot point is in an awkward spot.
 *
 * Return: void
 */
void rotate_I(Tetronimo *t_copy, unsigned char new_rotation)
{
    switch(new_rotation) {
    case ROT_0_DEG:; // Can't declare something after a label, interesting!
	Cell pivot = (Cell){.row = t_copy->c3.row-1, .col = t_copy->c3.col}; // covers ROT_90_DEG
	if(tetronimo.rotation == ROT_270_DEG) {
	    pivot = (Cell){.row = t_copy->c3.row, .col = t_copy->c3.col+1};
	}

	t_copy->c1 = (Cell){.row = pivot.row, .col = pivot.col-2};
	t_copy->c2 = (Cell){.row = pivot.row, .col = pivot.col-1};
	t_copy->c3 = pivot;
	t_copy->c4 = (Cell){.row = pivot.row, .col = pivot.col+1};
	break;
    case ROT_90_DEG:
	if(tetronimo.rotation == ROT_0_DEG) {
	    pivot = (Cell){.row = t_copy->c3.row+1, .col = t_copy->c3.col};
	} else if(tetronimo.rotation == ROT_180_DEG) {
	    pivot = (Cell){.row = t_copy->c3.row, .col = t_copy->c3.col+1};
	}

	t_copy->c1 = (Cell){.row = pivot.row-2, .col = pivot.col};
	t_copy->c2 = (Cell){.row = pivot.row-1, .col = pivot.col};
	t_copy->c3 = pivot;
	t_copy->c4 = (Cell){.row = pivot.row+1, .col = pivot.col};
	break;
    case ROT_180_DEG:
	if(tetronimo.rotation == ROT_90_DEG) {
	    pivot = (Cell){.row = t_copy->c3.row, .col = t_copy->c3.col-1};
	} else if(tetronimo.rotation == ROT_270_DEG) {
	    pivot = (Cell){.row = t_copy->c3.row+1, .col = t_copy->c3.col};
	}

	t_copy->c1 = (Cell){.row = pivot.row, .col = pivot.col+2};
	t_copy->c2 = (Cell){.row = pivot.row, .col = pivot.col+1};
	t_copy->c3 = pivot;
	t_copy->c4 = (Cell){.row = pivot.row, .col = pivot.col-1};
	break;
    case ROT_270_DEG:
	if(tetronimo.rotation == ROT_0_DEG) {
	    pivot = (Cell){.row = t_copy->c3.row, .col = t_copy->c3.col-1};
	} else if(tetronimo.rotation == ROT_180_DEG) {
	    pivot = (Cell){.row = t_copy->c3.row-1, .col = t_copy->c3.col};
	}

	t_copy->c1 = (Cell){.row = pivot.row+2, .col = pivot.col};
	t_copy->c2 = (Cell){.row = pivot.row+1, .col = pivot.col};
	t_copy->c3 = pivot;
	t_copy->c4 = (Cell){.row = pivot.row-1, .col = pivot.col};
	break;
    }
}
/**
 * rotate_T()
 * @t_copy: Pointer to a tetronimo struct used to check if rotation is valid
 * @new_rotation: New rotation to pivot to
 *
 * Rotate a 'T' piece.
 *
 * Return: void
 */
void rotate_T(Tetronimo *t_copy, unsigned char new_rotation)
{
    Cell pivot = (Cell){.row = t_copy->c3.row, .col = t_copy->c3.col};
    switch(new_rotation) {
    case ROT_0_DEG:
	t_copy->c1 = (Cell){.row = pivot.row, .col = pivot.col-1};
	t_copy->c2 = (Cell){.row = pivot.row-1, .col = pivot.col};
	t_copy->c4 = (Cell){.row = pivot.row, .col = pivot.col+1};
	break;
    case ROT_90_DEG:
	t_copy->c1 = (Cell){.row = pivot.row-1, .col = pivot.col};
	t_copy->c2 = (Cell){.row = pivot.row, .col = pivot.col+1};
	t_copy->c4 = (Cell){.row = pivot.row+1, .col = pivot.col};
	break;
    case ROT_180_DEG:
	t_copy->c1 = (Cell){.row = pivot.row, .col = pivot.col-1};
	t_copy->c2 = (Cell){.row = pivot.row+1, .col = pivot.col};
	t_copy->c4 = (Cell){.row = pivot.row, .col = pivot.col+1};
	break;
    case ROT_270_DEG:
	t_copy->c1 = (Cell){.row = pivot.row+1, .col = pivot.col};
	t_copy->c2 = (Cell){.row = pivot.row, .col = pivot.col-1};
	t_copy->c4 = (Cell){.row = pivot.row-1, .col = pivot.col};
	break;
    }
}

/**
 * rotate_Z()
 * @t_copy: Pointer to a tetronimo struct used to check if rotation is valid
 * @new_rotation: New rotation to pivot to
 *
 * Rotate a 'Z' piece.
 *
 * Return: void
 */
void rotate_Z(Tetronimo *t_copy, unsigned char new_rotation)
{
    Cell pivot = (Cell){.row = t_copy->c3.row, .col = t_copy->c3.col};
    switch(new_rotation) {
    case ROT_0_DEG:
	t_copy->c1 = (Cell){.row = pivot.row-1, .col = pivot.col-1};
	t_copy->c2 = (Cell){.row = pivot.row-1, .col = pivot.col};
	t_copy->c4 = (Cell){.row = pivot.row, .col = pivot.col+1};
	break;
    case ROT_90_DEG:
	t_copy->c1 = (Cell){.row = pivot.row-1, .col = pivot.col+1};
	t_copy->c2 = (Cell){.row = pivot.row, .col = pivot.col+1};
	t_copy->c4 = (Cell){.row = pivot.row+1, .col = pivot.col};
	break;
    case ROT_180_DEG:
	t_copy->c1 = (Cell){.row = pivot.row+1, .col = pivot.col+1};
	t_copy->c2 = (Cell){.row = pivot.row+1, .col = pivot.col};
	t_copy->c4 = (Cell){.row = pivot.row, .col = pivot.col-1};
	break;
    case ROT_270_DEG:
	t_copy->c1 = (Cell){.row = pivot.row+1, .col = pivot.col-1};
	t_copy->c2 = (Cell){.row = pivot.row, .col = pivot.col-1};
	t_copy->c4 = (Cell){.row = pivot.row-1, .col = pivot.col};
	break;
    }
}

/**
 * rotate_S()
 * @t_copy: Pointer to a tetronimo struct used to check if rotation is valid
 * @new_rotation: New rotation to pivot to
 *
 * Rotate a 'S' piece.
 *
 * Return: void
 */
void rotate_S(Tetronimo *t_copy, unsigned char new_rotation)
{
    Cell pivot = (Cell){.row = t_copy->c3.row, .col = t_copy->c3.col};
    switch(new_rotation) {
    case ROT_0_DEG:
	t_copy->c1 = (Cell){.row = pivot.row, .col = pivot.col-1};
	t_copy->c2 = (Cell){.row = pivot.row-1, .col = pivot.col};
	t_copy->c4 = (Cell){.row = pivot.row-1, .col = pivot.col+1};
	break;
    case ROT_90_DEG:
	t_copy->c1 = (Cell){.row = pivot.row-1, .col = pivot.col};
	t_copy->c2 = (Cell){.row = pivot.row, .col = pivot.col+1};
	t_copy->c4 = (Cell){.row = pivot.row+1, .col = pivot.col+1};
	break;
    case ROT_180_DEG:
	t_copy->c1 = (Cell){.row = pivot.row+1, .col = pivot.col-1};
	t_copy->c2 = (Cell){.row = pivot.row+1, .col = pivot.col};
	t_copy->c4 = (Cell){.row = pivot.row, .col = pivot.col+1};
	break;
    case ROT_270_DEG:
	t_copy->c1 = (Cell){.row = pivot.row-1, .col = pivot.col-1};
	t_copy->c2 = (Cell){.row = pivot.row, .col = pivot.col-1};
	t_copy->c4 = (Cell){.row = pivot.row+1, .col = pivot.col};
	break;
    }
}

/**
 * rotate_L()
 * @t_copy: Pointer to a tetronimo struct used to check if rotation is valid
 * @new_rotation: New rotation to pivot to
 *
 * Rotate a 'L' piece.
 *
 * Return: void
 */
void rotate_L(Tetronimo *t_copy, unsigned char new_rotation)
{
    Cell pivot = (Cell){.row = t_copy->c3.row, .col = t_copy->c3.col};
    switch(new_rotation) {
    case ROT_0_DEG:
	t_copy->c1 = (Cell){.row = pivot.row, .col = pivot.col-1};
	t_copy->c2 = (Cell){.row = pivot.row, .col = pivot.col+1};
	t_copy->c4 = (Cell){.row = pivot.row-1, .col = pivot.col+1};
	break;
    case ROT_90_DEG:
	t_copy->c1 = (Cell){.row = pivot.row-1, .col = pivot.col};
	t_copy->c2 = (Cell){.row = pivot.row+1, .col = pivot.col};
	t_copy->c4 = (Cell){.row = pivot.row+1, .col = pivot.col+1};
	break;
    case ROT_180_DEG:
	t_copy->c1 = (Cell){.row = pivot.row, .col = pivot.col+1};
	t_copy->c2 = (Cell){.row = pivot.row, .col = pivot.col-1};
	t_copy->c4 = (Cell){.row = pivot.row+1, .col = pivot.col-1};
	break;
    case ROT_270_DEG:
	t_copy->c1 = (Cell){.row = pivot.row-1, .col = pivot.col-1};
	t_copy->c2 = (Cell){.row = pivot.row-1, .col = pivot.col};
	t_copy->c4 = (Cell){.row = pivot.row+1, .col = pivot.col};
	break;
    }
}

/**
 * rotate_J()
 * @t_copy: Pointer to a tetronimo struct used to check if rotation is valid
 * @new_rotation: New rotation to pivot to
 *
 * Rotate a 'J' piece.
 *
 * Return: void
 */
void rotate_J(Tetronimo *t_copy, unsigned char new_rotation)
{
    Cell pivot = (Cell){.row = t_copy->c3.row, .col = t_copy->c3.col};
    switch(new_rotation) {
    case ROT_0_DEG:
	t_copy->c1 = (Cell){.row = pivot.row-1, .col = pivot.col-1};
	t_copy->c2 = (Cell){.row = pivot.row, .col = pivot.col-1};
	t_copy->c4 = (Cell){.row = pivot.row, .col = pivot.col+1};
	break;
    case ROT_90_DEG:
	t_copy->c1 = (Cell){.row = pivot.row-1, .col = pivot.col+1};
	t_copy->c2 = (Cell){.row = pivot.row-1, .col = pivot.col};
	t_copy->c4 = (Cell){.row = pivot.row+1, .col = pivot.col};
	break;
    case ROT_180_DEG:
	t_copy->c1 = (Cell){.row = pivot.row, .col = pivot.col-1};
	t_copy->c2 = (Cell){.row = pivot.row, .col = pivot.col+1};
	t_copy->c4 = (Cell){.row = pivot.row+1, .col = pivot.col+1};
	break;
    case ROT_270_DEG:
	t_copy->c1 = (Cell){.row = pivot.row-1, .col = pivot.col};
	t_copy->c2 = (Cell){.row = pivot.row+1, .col = pivot.col};
	t_copy->c4 = (Cell){.row = pivot.row+1, .col = pivot.col-1};
	break;
    }
}

void rotate_tetronimo(unsigned char direction)
{
    // find new rotation
    unsigned char rotation = tetronimo.rotation;
    if(direction) {
	++rotation;
    } else {
	--rotation;
    }

    //check if valid rotation state, since it only uses 2 bits
    if(rotation == ROT_OVER) {
	rotation = ROT_0_DEG;
    } else if(rotation == ROT_UNDER) {
	rotation = ROT_270_DEG;
    }

    Tetronimo t_copy = tetronimo;
    switch(tetronimo.type) {
    case I_PIECE:
	rotate_I(&t_copy, rotation);
	break;
    case O_PIECE:
	break;
    case T_PIECE:
	rotate_T(&t_copy, rotation);
	break;
    case Z_PIECE:
	rotate_Z(&t_copy, rotation);
	break;
    case S_PIECE:
	rotate_S(&t_copy, rotation);
	break;
    case L_PIECE:
	rotate_L(&t_copy, rotation);
	break;
    case J_PIECE:
	rotate_J(&t_copy, rotation);
	break;
    }

    if(valid_rotation(&t_copy)) {
	set_piece(EMPTY);
	tetronimo = t_copy;
	tetronimo.rotation = rotation;
	set_piece(PIECE);
	update_display();
    }
}

