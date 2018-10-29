#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h>


#define PORT 4242
#define MAX_BUFF_LEN 100
#define BACK_LOG 20
#include <vector>

class Network {
	int _sock_fd;
	socklen_t addr_len;
public:
	Network();

	std::vector<struct sockaddr_in> address;
	void listen_socket();
	void accept_socket();
	void recvfrom_socket();
	void sendto_socket();
};