#pragma once

#define PORT 4242
#define MAX_BUFF_LEN 100


class Network {
    int _sock_fd;
public:
    Network();
    std::vector<struct sockaddr_in> ;
};