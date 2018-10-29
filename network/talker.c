#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MYPORT 4242    /* Le port de connection */

int main(int argc, char *argv[]) {
	int sockfd;
	struct sockaddr_in their_addr; /* adresse du connecté */
	struct hostent *he;
	int numbytes;

	if (argc != 3) {
		exit(1);
	}

	if ((he = gethostbyname(argv[1])) == NULL) {
		herror("gethostbyname");
		exit(1);
	}

	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}

	their_addr.sin_family = AF_INET;
	their_addr.sin_port = htons(MYPORT);
	their_addr.sin_addr = *((struct in_addr *) he->h_addr);
	bzero(&(their_addr.sin_zero), 8);

	if ((numbytes = (int) sendto(sockfd, argv[2], strlen(argv[2]), 0,
								 (struct sockaddr *) &their_addr,
								 sizeof(struct sockaddr))) == -1) {
		perror("sendto");
		exit(1);
	}

	printf("Send %d to %s\n", numbytes, inet_ntoa(their_addr.sin_addr));

	close(sockfd);

	return 0;
}