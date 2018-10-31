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

	std::cout << "Connection ready ! " << std::endl;

}

void Network::connect_socket() {
	std::regex regex(R"(^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$)");
	std::smatch match;
	struct hostent *he;
	struct sockaddr_in dest;
	int sock = socket(AF_INET, SOCK_STREAM, 0);

	fcntl(sock, F_SETFL, fcntl(sock, F_GETFL, 0) | O_NONBLOCK);

	std::string buffer;
	std::getline(std::cin, buffer);
	std::string const buff_regex = buffer;

	dest.sin_family = AF_INET;
	dest.sin_port = htons(PORT_LISTEN);

	if (std::regex_search(buff_regex.begin(), buff_regex.end(), match, regex)) {
		dest.sin_addr.s_addr = inet_addr(buffer.c_str());
	} else if ((he = gethostbyname(buffer.c_str())) != NULL)
		dest.sin_addr = *((struct in_addr *) he->h_addr);

	bzero(&(dest.sin_zero), 8);

	std::cout << "Trying to connect " << inet_ntoa(dest.sin_addr)
			  << std::endl;

	if (connect(sock, reinterpret_cast<struct sockaddr *>(&dest),
				addr_len) != -1) {
		std::cout << "Connect Good !" << std::endl;
	} else {
		std::cout << strerror(errno) << " " << inet_ntoa(dest.sin_addr)
				  << std::endl;

	}
}

void Network::accept_socket() {
	struct sockaddr_in new_element;
	int fildes;
	std::cout << "Trying to accept" << std::endl;
	fildes = accept(my_network.sock_fd,
					reinterpret_cast<struct sockaddr *>(&new_element),
					&addr_len);
	if (fildes == -1) {
		std::cout << strerror(errno) << " " << inet_ntoa(new_element.sin_addr)
				  << std::endl;
	} else {
		fds.push_back(fildes);
		std::cout << "Accept successfully : "
				  << inet_ntoa(new_element.sin_addr) << std::endl;
	}

}

void Network::send_all_socket() {
	std::string s;
	std::getline(std::cin, s);
	for (int index = 0; index < fds.size(); ++index) {
		send(fds[index], s.c_str(), s.size(), 0);
	}
}

void Network::recv_socket() {
	char buff[MAX_BUFF_LEN];
	int numbytes = recv(my_network.sock_fd, buff, MAX_BUFF_LEN, 0);
	if (numbytes == -1) {
		std::cout << "Nothing recv from " << my_network.sock_fd << std::endl;
	} else {
		std::cout << "recv from " << my_network.sock_fd << " : " << buff
				  << std::endl;
	}
}

void Network::send_socket() {
	for (int i = 0; i < fds.size(); ++i) {
	}
}

Network::network Network::get_network() {
	network net;
	if ((net.sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
	}
	fcntl(net.sock_fd, F_SETFL, fcntl(net.sock_fd, F_GETFL, 0) | O_NONBLOCK);

	net.address.sin_family = AF_INET;
	net.address.sin_port = 0;
	net.address.sin_addr.s_addr = 0;

	bzero(&(net.address.sin_zero), 8);

	if (bind(net.sock_fd,
			 reinterpret_cast<struct sockaddr const *>(&net.address),
			 sizeof(struct sockaddr)) == -1)
		perror("bind");
	std::cout << "New network ready ! " << std::endl;
	return net;
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
	dest_addr.sin_port = htons(PORT_LISTEN);
	if (std::regex_search(buff_regex.begin(), buff_regex.end(), match, regex)) {
		std::cout << "Match Regex" << std::endl;
		dest_addr.sin_addr.s_addr = inet_addr(buffer.c_str());
	} else {
		if ((he = gethostbyname(buffer.c_str())) != NULL) {
			std::cout << "Match host" << std::endl;
			dest_addr.sin_addr = *((struct in_addr *) he->h_addr);
		} else {
			std::cout << "No match" << std::endl;
			return;
		}
	}
	std::getline(std::cin, buffer);
	int numbytes = sendto(my_network.sock_fd, buffer.c_str(), buffer.size(), 0,
						  reinterpret_cast<struct sockaddr const *>(&dest_addr),
						  addr_len);
	std::cout << "Send [" << numbytes << "] to "
			  << inet_ntoa(dest_addr.sin_addr) << std::endl;
//	for (int index = 1; index < address.size(); ++index) {
//		sendto(my_network.sock_fd, buffer.c_str(), buffer.size(), 0,
//			   reinterpret_cast<struct sockaddr const *>(&(address[index])),
//			   addr_len);
//	}
}

void Network::recvfrom_socket() {
	char buff[MAX_BUFF_LEN];
	struct sockaddr_in new_element;
	size_t bytes_recv;
// int recvfrom(int sockfd, void *buf, int len, unsigned int flags
//                 struct sockaddr *from, int *fromlen);
	bytes_recv = recvfrom(my_network.sock_fd, buff, MAX_BUFF_LEN, 0,
						  reinterpret_cast<struct sockaddr *>(&new_element),
						  &addr_len);
	if (bytes_recv != -1) {
		buff[bytes_recv] = '\0';
		std::cout << buff << std::endl;
	}
}
