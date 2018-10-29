#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MYPORT 4950    /* Le port de connection pour l'utilisateur */

#define MAXBUFLEN 100

main() {
	int sock_fd;
	struct sockaddr_in my_addr;    /* mon adresse */
	struct sockaddr_in their_addr; /* Adresse du connecté */
	int addr_len, numbytes;
	char buf[MAXBUFLEN];
	printf("yo");

	if ((sock_fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}
	printf("init");

	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(MYPORT);
	my_addr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(my_addr.sin_zero), 8);

	printf("bind");
	if (bind(sock_fd, (const struct sockaddr *) &my_addr,
			 sizeof(struct sockaddr)) == -1) {
		perror("bind");
		exit(1);
	}

	printf("recv");
	addr_len = sizeof(struct sockaddr);
	if ((numbytes = (int) recv(sock_fd, buf, MAXBUFLEN, 0)) == -1) {
		perror("recvfrom");
		exit(1);
	}
	system("say \"Call Krystal\"");
	printf(" [%d] bytes \n", numbytes);
	buf[numbytes] = '\0';
	printf("Packet is [%s]\n", buf);

	close(sock_fd);
}