#ifndef TETRIS_H

// Board
#define ROWS    19 // 16 rows displayed, 2 extra above and 1 below for buffer
#define COLUMNS 12  // 8 columns displayed, 4 extra on sides to easily check if valid rotation

// Tetris types
#define NULL_PIECE 0
#define I_PIECE    1
#define O_PIECE    2
#define T_PIECE    3
#define Z_PIECE    4
#define S_PIECE    5
#define L_PIECE    6
#define J_PIECE    7

// Tetris rotations
#define ROT_UNDER   ((unsigned char)~0)
#define ROT_0_DEG    0
#define ROT_90_DEG   1
#define ROT_180_DEG  2
#define ROT_270_DEG  3
#define ROT_OVER     4

// Tetris starting row
#define DISP_START_ROW 2 // Rows 0-3 are where to initially place tetris piece
#define DISP_TOP_END   10
#define DISP_BOT_END   18
#define DISP_START_COL 2
#define DISP_END_COL   10

// Tetris board array elements
#define EMPTY  0
#define PIECE  1
#define FILLED 2

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
extern unsigned char time_till_drop;
extern unsigned char board[ROWS][COLUMNS];

void init_tetris(void);
void update_board(void);

#endif
