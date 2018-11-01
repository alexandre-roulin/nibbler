//
// Created by Alexandre ROULIN on 10/29/18.
//
#include <iostream>
#include <fcntl.h>
#include <regex>
#include <netdb.h>
#include "Network.hpp"

//SOCK_STREAM Listen sur un port
//SOCK_DGRAM recvfrom et sendto
Network::Network() {
	addr_len = sizeof(struct sockaddr_in);

	//Create socket descriptor !
	//SOCK_DGRAM

	if ((my_network.sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
	}

	//Flag O_NONBLOCK then accept isnot blocking

	fcntl(my_network.sock_fd, F_SETFL,
		  fcntl(my_network.sock_fd, F_GETFL, 0) | O_NONBLOCK);

	//Create own struct addr

	my_network.address.sin_family = AF_INET;
	my_network.address.sin_port = htons(PORT_LISTEN);
	my_network.address.sin_addr.s_addr = INADDR_ANY;

	bzero(&(my_network.address.sin_zero), 8);
	//Bind socket to a addr
	if (bind(my_network.sock_fd,
			 reinterpret_cast<struct sockaddr const *>(&my_network.address),
			 sizeof(struct sockaddr)) == -1)
		perror("bind");
	if (listen(my_network.sock_fd, BACK_LOG) == -1)
		perror("listen");
	else
		std::cout << "Listen successfully" << std::endl;
	write_fds.push_back(my_network);
	std::cout << "Connection ready ! " << std::endl;

}

void Network::connect_socket() {
	std::regex regex(R"(^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$)");
	std::smatch match;
	struct hostent *he;
	network network1;
	network1.sock_fd = socket(AF_INET, SOCK_STREAM, 0);

	fcntl(network1.sock_fd, F_SETFL,
		  fcntl(network1.sock_fd, F_GETFL, 0) | O_NONBLOCK);

	std::string buffer;
	std::getline(std::cin, buffer);
	std::string const buff_regex = buffer;

	network1.address.sin_family = AF_INET;
	network1.address.sin_port = htons(PORT_LISTEN);

	if (std::regex_search(buff_regex.begin(), buff_regex.end(), match, regex)) {
		network1.address.sin_addr.s_addr = inet_addr(buffer.c_str());
	} else if ((he = gethostbyname(buffer.c_str())) != NULL)
		network1.address.sin_addr = *((struct in_addr *) he->h_addr);

	bzero(&(network1.address.sin_zero), 8);

	std::cout << "Trying to connect " << inet_ntoa(network1.address.sin_addr)
			  << std::endl;

	if (connect(network1.sock_fd,
				reinterpret_cast<struct sockaddr *>(&network1.address),
				addr_len) != -1) {
		std::cout << "Connect Good !" << std::endl;
	} else {
		write_fds.push_back(network1);
		std::cout << strerror(errno) << " "
				  << inet_ntoa(network1.address.sin_addr)
				  << std::endl;

	}
}

void Network::accept_socket() {
	network network1;
	std::cout << "Trying to accept" << std::endl;
	network1.sock_fd = accept(my_network.sock_fd,
							  reinterpret_cast<struct sockaddr *>(&network1.address),
							  &addr_len);
	if (network1.sock_fd == -1) {
		std::cout << strerror(errno) << " "
				  << inet_ntoa(network1.address.sin_addr)
				  << std::endl;
	} else {
		read_fds.push_back(network1);
		std::cout << "Accept successfully : "
				  << inet_ntoa(network1.address.sin_addr) << std::endl;
	}

}

void Network::send_socket() {
	std::string s;
	std::cout << "write_fds : ";
	std::getline(std::cin, s);
	for (int index = 0; index < write_fds.size(); ++index) {
		send(write_fds[index].sock_fd, s.c_str(), s.size(), 0);
	}
}

void Network::recv_socket() {
	char buff[MAX_BUFF_LEN];
	std::cout << "read_fds : ";
	for (int index = 0; index < read_fds.size(); ++index) {
		int read = recv(read_fds[index].sock_fd, buff, MAX_BUFF_LEN, 0);
		if (read != -1) {
			buff[read] = '\0';
			std::cout << buff << std::endl;
		}
	}
}
