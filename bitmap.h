#ifndef _BITMAP_H
#define _BITMAP_H
 
void init_bf( int num_pieces);
unsigned char* get_bm();
void set( int piece);
int is_set( int piece);
int get_num_set();
void print_bm();

extern unsigned char* bit_map;
extern int bit_map_length;
extern int number_pieces;

#endif
