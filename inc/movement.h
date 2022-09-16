#ifndef MOVEMENT_H
#define MOVEMENT_H

unsigned char reached_bottom(void);
void set_piece(unsigned char val);
void drop(void);
void move_left(void);
void move_right(void);
void rotate_tetronimo(unsigned char direction);
void fast_drop(void);
void disable_fast_drop(void);

#endif
