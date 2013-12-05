#include "bitmap.h"
#include <stdio.h>
#include <stdlib.h>


int main(int argc, const char* argv[]){

	init_bf(40);
	set(2);
	set(9);
	set(39);
	set(1);
	printf("num of should be 4 it is %d", get_num_set());
	print_bm();

	return 0;
}

	
