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
#define MAX_BUFF_LEN 100
#define BACK_LOG 20
#define TAG_CHAT "_CHAT_"
#define TAG_INPUT "_INPUT_"
#define TAG_READY "_READY_"


#include <vector>

class Network {
	struct network {
		int sock_fd;
		struct sockaddr_in address;
	};
	network my_network;

	socklen_t addr_len;
	std::vector<network> read_fds;
	std::vector<network> write_fds;
public:
	Network();

	void accept_socket();

	void connect_socket();

	void send_socket();

	void recv_socket();
};