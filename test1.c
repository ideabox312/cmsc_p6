#include "peer.h"
#include <stdio.h>
#include <stdlib.h>

int main(){

	init_peer_lst();
	add_peer("128.2.2.2", "asdfghjkjhgfdsasd\0", 6666);
	add_peer("12.2.1.2", "asdfghjkllkfdsafg\0", 3333);
	add_peer("128.2.2.2", "asdfghjkjhgfdsasd\0", 6666);
	add_peer("12.2.1.2", "asdfghjkllkfdsafg\0", 3333);
	print_pl();
	return 0;
}
