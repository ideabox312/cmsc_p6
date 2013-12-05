#include <netinet/in.h>
#include <arpa/inet.h>


typedef struct Peer
{
	struct in_addr peer_ip;
	char* peer_id;
	int port;
	char* bit_map;
	int interested;
	int choked;
};

