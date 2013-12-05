#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "sha1.h"
#include "geek_hdo.h"
#include "bencode.h"

void print_peers(char* peers_info);
void DieWithError(const char *errorMessage){
	printf("%s\n", errorMessage);
	exit(1);
}


static char *read_file( const char *file, int *length){
	struct stat st;
	FILE *fp;
	char * ret_buf = NULL;

	if (stat(file, &st))
	{
		
	}
	*length = st.st_size;
	if (!(fp = fopen(file, "r")))
	{
		DieWithError("file open failed");
	}
	if (!(ret_buf = malloc(*length)))
	{
		DieWithError("failed to malloc");
	}
	fread(ret_buf,1, *length, fp);
	fclose(fp);

	return ret_buf;
}

static char* get_url(char* str){
	// printf("%s\n",str );
	char* temp = malloc(strlen(str)+1);
	strcpy(temp, str);
	char* pch = strrchr(temp, ':'); 
	if (pch == NULL)
	{	
		pch = strrchr(temp, '/');
		if (pch != NULL)
		{
			int idx;
		for (idx = pch - temp ; idx < strlen(temp); idx++)
		{
			temp[idx] = '\0';
		}
		}
	}else
	{
		int idx;
		for (idx = pch - temp ; idx < strlen(temp); idx++)
		{
			temp[idx] = '\0';
		}
	}
	
	return temp;
}
int main(int argc, char *argv[]) {

	if (argc != 2)
	{
		DieWithError("Usage: get_peers <torrent file>");

	}
	char *buffer;
	char *temp;
	int len;
	int ben_ret;
	benc_val_t val; 
	char *url;
	// int url_len;
	int inf_hs_len;
	sha1_byte_t info_hash[21];
//	memset(info_hash, 0, sizeof(info_hash));	
	char * info_hash_encoded;
	sha1_state_s inf_hs;
	char msg_buff[500];
	char recv_buff[1000];
	const char* peer_id = "ABCDEFGHIJKLMNOPQRST";
	//char* peer_id_encoded = url_encode((char*) peer_id);
	memset(msg_buff, 0, 500);	
	
	// struct sockaddr_in 
	// memset(&servAddr, 0, sizeof(servAddr));
	// servAddr.sin_family = AF_INET;
	// servAddr.sin_addrs.s_addr = 
	int sock;


	buffer = read_file( argv[1], &len);
	ben_ret = _tr_bencLoad(buffer, len, &val, NULL);

	url = tr_bencStealStr(tr_bencDictFind(&val, "announce"));

	temp = tr_bencSaveMalloc(tr_bencDictFind(&val, "info"), &inf_hs_len);

	sha1_init(&inf_hs);
	sha1_update(&inf_hs, (unsigned char *) temp, inf_hs_len);
	sha1_finish(&inf_hs, info_hash);
	info_hash[20] = '\0';
	printf("hash length %d\n", strlen((char*)info_hash) );

	// to do free info
	info_hash_encoded = url_encode((char *)info_hash);
	//printf("%s\n",info_hash_encoded);
	//setting up server struct
	char* url_temp = get_url(url+7);
	printf("%s\n",url);

	struct addrinfo hints; 
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC; //IPv4 address family. this might be an issue so keep in mind
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	struct addrinfo *servAddr, *step;  
	printf("%s\n", url_temp);

	int err = getaddrinfo(url_temp, "6969", &hints, &servAddr);
	if (err!= 0) 
	{
		
		printf("%s\n",url_temp);
		printf("getaddrinfo failed %s\n", gai_strerror(err));
	}
	struct in_addr addr;
	addr.s_addr = ((struct sockaddr_in *)(servAddr->ai_addr))->sin_addr.s_addr;
	//printf("ip address : %s\n", inet_ntoa(addr));
	// if ((sock = (socket(servAddr->ai_family, servAddr->ai_socktype, servAddr->ai_protocol))) < 0)
	// {
	// 	DieWithError("create socket failed");
	// }

	// //establish the connection to the echo server
	for (step = servAddr; step != NULL; step = step->ai_next){
		sock = socket(step->ai_family, step->ai_socktype, step->ai_protocol);
		if (sock < 0)
		{
			continue;
		}
		//printf("%x\n",step->ai_addr);
		if (connect(sock, step->ai_addr, step->ai_addrlen) == 0)
			break;

		close(sock);
		sock = -1;
	}

	if (sock == -1)
	{
		DieWithError("connect failed");
	}

	freeaddrinfo(servAddr);

	// if (connect(sock, (struct sockaddr *) servAddr, sizeof(servAddr)) != 0 )
	// {
	// 	DieWithError("connect failed");
	// }



	//create url get request

 sprintf(msg_buff,"Get /announce?info_hash=%s&peer_id=%s&port=6996&uploaded=0&downloaded=0&left=0&compact=1&no_peer_id=1&event=started&numwant=50 HTTP/1.1\r\n Host: %s\r\n \r\n \r\n",
 	info_hash_encoded,peer_id, url_temp);

	// sprintf(msg_buff,"%sinfo_hash=%s&peer_id=%s&port=6881&uploaded=0&downloaded=0&left=0&compact=1&event=started\r\n \r\n \r\n",
	// 	url,info_hash_encoded,peer_id_encoded);
	
	int num_byte = send(sock, msg_buff, strlen(msg_buff), MSG_WAITALL);

		printf("%s\n", msg_buff);
	if (num_byte <= 0){
		DieWithError("send failed");
	} 

	num_byte =  recv(sock, recv_buff, 1000, MSG_WAITALL);

	if (num_byte < 0)
	{
		DieWithError("recv failed");
	}else if (num_byte == 0)
	{
	//	num_byte =  recv(sock, recv_buff + num_byte, 1000 , 0);

	}
	recv_buff[num_byte] = '\0';
	printf("%s\n",recv_buff );
		//printf("%d\n",num_byte );
	benc_val_t new_val;
	_tr_bencLoad(recv_buff + 19, num_byte -19 , &new_val, NULL);
	char *peers = tr_bencStealStr(tr_bencDictFind(&new_val, "peers"));
	print_peers(peers);

	close(sock);
	free(buffer);



  return 0;
}

void print_peers(char* peers_info){
	char peers[6];
	int i = 0; 
	// for ( i = 0; i < strlen (peers_info); i+= 6)
	// {
		// peers[0] = peers_info[i+0];
		// peers[1] = peers_info[i+1];		
		// peers[2] = peers_info[i+2];		
		// peers[3] = peers_info[i+3];		
		// peers[4] = peers_info[i+4];
		// peers[5] = peers_info[i+5];





		struct sockaddr_in a;
		a.sin_addr.s_addr.s_addr = htonl((peers[0+i] << 24) + (peers[1+i] << 16) +
                        (peers[2+i] << 8) + (peers[3+i]));
		a.sin_port = htons((peers[4+i] << 8) + peers[5+i]);
		printf("%s\n", inet_ntoa(&a.sin_addr));
		printf("ports: %u\n",ntohs(a.sin_port));
	// }
	
}

// void append_file(char * write_buff, int num_bytes){
// 	FILE *fp1;
// 	fp1 = fopen("append", "a");
// 	if (!fp1){
// 		DieWithError("file open failed");
// 	}
// 	//int num_bytes_written = 
// 	fwrite ((const char *) write_buff, sizeof(write_buff[0]), num_bytes, fp1);
// 	int length;
// 	struct stat st;
// 	char * buf = NULL;
// 	fstat(fp1, &st);
// 	length = st.st_size;
// 	// if (!(fp1 = fopen(file, "r")))
// 	// {
// 	// 	DieWithError("file open failed");
// 	// }
// 	if (!(buf = malloc(length)))
// 	{
// 		DieWithError("failed to malloc");
// 	}
// 	fread(buf,1, length, fp1);
// 	fclose(fp1);
// 	benc_val_t new_val;
// 	_tr_bencLoad(buff, length, &new_val, NULL);

// }
