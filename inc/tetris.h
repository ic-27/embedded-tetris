#ifndef TETRIS_H
#define TETRIS_H

// Board
#define ROWS    19		// 16 rows displayed, 2 extra above and 1 below for buffer
#define COLUMNS 14		// 8 columns displayed, 4 extra on sides to easily check if valid rotation

#define NUM_COLUMNS_DISPLAYED	8

// Tetris types
#define NUM_TETRIS_TYPES	6
#define I_PIECE			0
#define O_PIECE			1
#define T_PIECE			2
#define Z_PIECE			3
#define S_PIECE			4
#define L_PIECE			5
#define J_PIECE			6

// Tetris rotations
#define ROT_UNDER	((unsigned char)~0)
#define ROT_0_DEG	0
#define ROT_90_DEG	1
#define ROT_180_DEG	2
#define ROT_270_DEG	3
#define ROT_OVER	4

// Tetris starting row
#define DISP_START_ROW	2	// Rows 0-3 are where to initially place tetris piece
#define DISP_TOP_END	10
#define DISP_BOT_END	18
#define DISP_START_COL	3
#define DISP_END_COL	11

// Tetris board array elements
#define EMPTY	0
#define PIECE	1
#define FILLED	2

// Tetris drop control macros
#define FAST_DROP	1
#define NORMAL_DROP	2

// Tetris next move macros
#define WAIT_NEXT_MOVE  0
#define NEXT_MOVE_READY 1

typedef struct cell { // 2 bytes
    unsigned char row;
    unsigned char col;
} Cell;

typedef struct tetronimo { // 10 bytes
    unsigned char type : 3; // tetronimo type
    unsigned char rotation : 2;
    Cell c1; // store location of each 'cell' of tetronimo
    Cell c2;
    Cell c3; // the pivot point of the tetronimo, for easy rotation
    Cell c4;
} Tetronimo;
extern Tetronimo tetronimo;

// Not memory efficient since we only use need 2 bits to store, but good enough.
// We have plenty of RAM!
extern unsigned char board[ROWS][COLUMNS];
extern unsigned char time_till_drop_time;
extern unsigned char lock;
extern volatile unsigned char next_move;
extern volatile unsigned char damage; // how much damage opponent sent to you
void start_tetris(void);
void next_move_logic(void);
void check_power_switch(void);
void reinit(void);
void deinit(void);
#endif

