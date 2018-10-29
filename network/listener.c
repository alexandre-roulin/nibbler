#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <zconf.h>
#include <arpa/inet.h>

#define MYPORT 4950    /* Le port de connection pour l'utilisateur */

#define MAXBUFLEN 100

main()
{
    int sockfd;
    struct sockaddr_in my_addr;    /* mon adresse */
    struct sockaddr_in their_addr; /* Adresse du connecté */
    int addr_len, numbytes;
    char buf[MAXBUFLEN];

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    my_addr.sin_family = AF_INET;         /* host byte order */
    my_addr.sin_port = htons(MYPORT);     /* short, network byte order */
    my_addr.sin_addr.s_addr = INADDR_ANY; /* auto-fill with my IP */
    bzero(&(my_addr.sin_zero), 8);        /* zero pour le reste de struct */

    if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) \
                                                                       == -1) {
        perror("bind");
        exit(1);
    }

    addr_len = sizeof(struct sockaddr);

    if ((numbytes=recvfrom(sockfd, buf, MAXBUFLEN, 0, \
                           (struct sockaddr *)&their_addr, &addr_len)) == -1) {
        perror("recvfrom");
        exit(1);
    }

    printf("reçu un paquet de %s\n",inet_ntoa(their_addr.sin_addr));
    printf("le paquet fait %d octets de long\n",numbytes);
    buf[numbytes] = '\0';
    printf("Le paquet contient \"%s\"\n",buf);

    close(sockfd);
}