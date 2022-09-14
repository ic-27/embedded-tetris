#ifndef TETRIS_H

// Board
#define ROWS    20 // Only 16 rows actually displayed
#define COLUMNS 8

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
#define ROT_0_DEG   0
#define ROT_90_DEG  1
#define ROT_180_DEG 2
#define ROT_270_DEG 3

// Tetris starting row
#define DISP_START_ROW 4 // Rows 0-3 are where to initially place tetris piece
#define DISP_TOP_END 12
#define DISP_BOT_END 20

typedef struct cell { // 2 bytes
    unsigned char x;
    unsigned char y;
} Cell;

typedef struct tetronimo { // 10 bytes
    unsigned char type : 3; // tetronimo type
    unsigned char rotation : 2;
    unsigned char cent_x_coord : 5; // keep track for easy rotation
    unsigned char cent_y_coord : 5;
    Cell c1; // store location of each 'cell' of tetronimo
    Cell c2;
    Cell c3;
    Cell c4;
} Tetronimo;

// Not memory efficient since we only use 2 bits in each element, but good enough.
extern unsigned char board[ROWS][COLUMNS];

void init_tetris(void);
void update_board(void);

#endif
