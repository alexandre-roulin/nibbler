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


#define PORT_LISTEN 4242
#define PORT_CONNECT 4243
#define MAX_BUFF_LEN 100
#define BACK_LOG 20
#include <vector>

class Network {
	struct network {
		int sock_fd;
		struct sockaddr_in	 address;

	};
	network my_network;

	socklen_t addr_len;
	std::vector<int> read_fds;
	std::vector<int> write_fds;
public:
	Network();

	void accept_socket();
	void connect_socket();
	void recvfrom_socket();
	void sendto_socket();
	void send_all_socket();
	void recv_socket();
	void send_socket();
	network get_network();
};