#include <stdio.h>

typedef struct {
	int p_idx; 
	int p_offset;
	int p_len;
} p_req;

typedef struct {
	int p_idx;
	int p_offset;
	char * piece;
} Piece;

typedef struct {
	int m_len; // message length 
	uint8_t m_type;
	
	union {
		char * bitmap;
		int have; // index of piece that i have
		Piece piece; // for when we send a piece to peer
		p_req req;
		p_req cancel;
		int port; // for DHT
	}payload;
} msg;

	

