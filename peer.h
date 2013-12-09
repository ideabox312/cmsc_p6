#include <netinet/in.h>
#include <arpa/inet.h>
#define ID_SIZE 20 
struct Peer
{
	struct in_addr ip;
	char id[20];
	unsigned short port;
	char* bit_map;
	int am_instd;
	int am_choking;
	int peer_instd;
	int peer_choking;
	struct Peer *next;
	struct Peer *prev;
	// might need to add sockets.

};

//extern Peer * peer_list;

int init_peer_lst();
int add_peer( char * ip, char *peer_id, unsigned short port);
int remove_peer(char * peer_id); 
struct Peer * find_peer(char * peer_id);
void print_peer();
void print_pl();
