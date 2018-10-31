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
	if ((_sock_fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		perror("socket");
	}

	//Flag O_NONBLOCK then accept isnot blocking

	int flags = fcntl(_sock_fd, F_GETFL, 0);
	fcntl(_sock_fd, F_SETFL, flags | O_NONBLOCK);

	//Create own struct addr
	struct sockaddr_in my_addr;

	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(PORT);
	my_addr.sin_addr.s_addr = INADDR_ANY;

	bzero(&(my_addr.sin_zero), 8);
	address.resize(1);
	address[0] = my_addr;
	//Bind socket to a addr
	if (bind(_sock_fd,
			 reinterpret_cast<struct sockaddr const *>(&my_addr),
			 sizeof(struct sockaddr)) == -1)
		perror("bind");
	std::cout << my_addr.sin_addr.s_addr << std::endl;
	std::cout << inet_ntoa(my_addr.sin_addr) << std::endl;

}

void Network::listen_socket() {

	//listen to specify socket
	if (listen(_sock_fd, BACK_LOG) == -1)
		perror("listen");
}

void Network::accept_socket() {
	struct sockaddr_in new_element;
	socklen_t len;
	int fildes;
	for (; (fildes = accept(_sock_fd,
							reinterpret_cast<struct sockaddr *>(&new_element),
							&len)) != -1;) {
		std::cout << fildes;
		std::cout << new_element.sin_addr.s_addr;

	}
}

void Network::recvfrom_socket() {
	char buff[MAX_BUFF_LEN];
	struct sockaddr_in new_element;
	size_t bytes_recv;
// int recvfrom(int sockfd, void *buf, int len, unsigned int flags
//                 struct sockaddr *from, int *fromlen);
	bytes_recv = recvfrom(_sock_fd, buff, MAX_BUFF_LEN, 0,
						  reinterpret_cast<struct sockaddr *>(&new_element),
						  &addr_len);
	if (bytes_recv != -1) {
		buff[bytes_recv] = '\0';
		std::cout << buff << std::endl;
		address.resize(address.size() + 1);
		address[address.size() - 1] = new_element;
	}
}

void Network::sendto_socket() {
//    int sendto(int sockfd, const void *msg, int len, unsigned int flags,
//               const struct sockaddr *to, int tolen);
	std::regex regex(R"(^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$)");
	std::smatch match;
	struct sockaddr_in dest_addr;
	struct hostent *he;

	std::string buffer;
	std::getline(std::cin, buffer);
	std::string const buff_regex = buffer;
	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(PORT);
	if (std::regex_search(buff_regex.begin(), buff_regex.end(), match, regex)) {
		std::cout << "Match Regex" << std::endl;
		dest_addr.sin_addr.s_addr = inet_addr(buffer.c_str());
	} else {
		std::cout << "Match host" << std::endl;
		he = gethostbyname(buffer.c_str());
		dest_addr.sin_addr = *((struct in_addr *) he->h_addr);
	}
	std::getline(std::cin, buffer);
	sendto(_sock_fd, buffer.c_str(), buffer.size(), 0, reinterpret_cast<struct sockaddr const *>(&dest_addr), addr_len);
//	for (int index = 1; index < address.size(); ++index) {
//		sendto(_sock_fd, buffer.c_str(), buffer.size(), 0,
//			   reinterpret_cast<struct sockaddr const *>(&(address[index])),
//			   addr_len);
//	}
}
