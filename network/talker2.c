#include <arpa/inet.h>
#include <printf.h>
#include <stdlib.h>
#include <strings.h>

#define PORT 4950
#define DEST_IP   "10.11.255.255"

int main(int a, char **v) {
	printf("yo\n");
	int sockfd; // Socket
	struct sockaddr_in my_addr; //My addr
	struct sockaddr_in dest_addr;   /* Contiendra l'adresse de destination */
	printf("init\n");

	my_addr.sin_family = AF_INET;
	my_addr.sin_port = 0;
	my_addr.sin_addr.s_addr = INADDR_ANY;
	printf("my_addr\n");

	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(PORT);
	dest_addr.sin_addr.s_addr = inet_addr(DEST_IP);
	bzero(&(dest_addr.sin_zero), 8);
	printf("dest\n");

	sockfd = socket(AF_INET, SOCK_STREAM, 0); /* Contrôle d'erreur! */
	printf("sockfd\n");
	if (sockfd == -1) {
		perror("socket");
		exit(1);
	} else printf("%d\n", sockfd);
	printf("connect\n");
	if (connect(sockfd, (struct sockaddr *) &dest_addr,
				sizeof(struct sockaddr)) == -1) {
		perror("connect");
		exit(1);
	}
	printf("sendto\n");
	if (sendto(sockfd, "Yo", 2, 0, (struct sockaddr *) &dest_addr, sizeof(struct sockaddr))) {
		perror("connect");
		exit(1);
	}

	printf("> %d %d\n", my_addr.sin_port, sockfd);
}