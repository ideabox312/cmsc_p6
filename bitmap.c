#include "bitmap.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


unsigned char* bit_map;
int bit_map_length;
int number_pieces;

void init_bf( int num_pieces){
	number_pieces = num_pieces;
	bit_map_length = num_pieces / 8;
	if(	num_pieces % 8 ){
		bit_map_length ++;

	}
	bit_map = calloc(bit_map_length, sizeof( bit_map));
	memset(bit_map, 0 , bit_map_length);
}

unsigned char* get_bm(){
	return bit_map;
}

void set( int piece){
	int idx = piece/8;
//	char bit;
//	bit = (1<<(piece % 8 ));
	int bit[] = { 128, 64, 32, 16,8,4,2,1};
	bit_map[idx] |= bit[piece %8];

}

int is_set( int piece){
	int idx = piece/8;
//	char bit;
//	bit = (1<< (piece % 8));
        int bit[] = { 128, 64, 32, 16,8,4,2,1};

	return (bit_map[idx] & bit[piece %8]);

}

int get_num_set(){
	int num =0;
	int idx;
	for(idx =0; idx < number_pieces; idx++){
		if (is_set(idx))
		{
			num++;			
		}
	}

	return num;
}

void print_bm(){
	FILE * fp;
	fp = fopen("bit_map_log.txt","a" );
	int idx = 0;

	for ( idx = 0; idx < number_pieces; idx++){
		if (is_set(idx))
		{
			fprintf(fp, "piece %d: 1\n", idx);
		}else
		{
			fprintf(fp, "piece %d: 0\n", idx);
		}
	}
	fclose(fp);
	
}




