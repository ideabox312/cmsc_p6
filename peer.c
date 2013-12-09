#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "queue.h"
#include "peer.h"
#include <assert.h>

struct Peer * g_pl;

int init_peer_lst(){
	InitDQ(g_pl, struct Peer);
	assert(g_pl);
	return (g_pl != 0x0);

}


int add_peer ( char * ip, char *peer_id, unsigned short port){
	struct Peer * n_p = ( struct Peer * ) getmem(sizeof (struct Peer));
	inet_aton(ip, &n_p->ip); 	//set ip address
	//compute shah1 of ip addr and port.
	// or have it pre computed
	memcpy(n_p->id, peer_id, ID_SIZE);
	n_p->port = port;
	n_p->am_choking = 1;  
	n_p->am_instd = 0;
	n_p->peer_instd = 0;
	n_p->peer_choking = 1;

		
	InsertDQ(g_pl, n_p);
	return (n_p != 0x0);
}

struct Peer * find_peer(char * peer_id){
	struct Peer *fp;
		
	for( fp = g_pl->next; fp != g_pl; fp = fp->next){
		if( fp->id == peer_id)
			break;

	}
	if( fp->id == peer_id){
		return fp;
	}
	else {
		return 0x0;
	}
 
}


int remove_peer(char * peer_id){
	struct Peer * rp = find_peer(peer_id);
	
	if( rp->id == peer_id){
		DelDQ(rp);
		free(rp);
		return 0;
	}	
	else{
		return -1;
	}
}

void print_peer(struct Peer *p){

	
	assert(p); 
	// print 
	printf("Peer: %s \n Ip addr: %s Port: %d\n",
	 p->id, inet_ntoa(p->ip), p->port);  
	return;
}	

void print_pl(){
	struct Peer * p;
	for ( p = g_pl->next; p != g_pl; p = p->next){
	
		print_peer(p);
	}
}

//void get_id(char * ip, unsigned short port, char *id){
//	cha
