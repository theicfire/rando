/*
        demo-udp-03: udp-recv: a simple udp server
	receive udp messages

        usage:  udp-recv

        Paul Krzyzanowski
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "port.h"

#define BUFSIZE 2048

int
main(int argc, char **argv)
{
	struct sockaddr_in myaddr;	/* our address */
	struct sockaddr_in remaddr;	/* remote address */
	socklen_t addrlen = sizeof(remaddr);		/* length of addresses */
	int recvlen;			/* # bytes received */
	int fd;				/* our socket */
	unsigned char buf[BUFSIZE];	/* receive buffer */


	/* create a UDP socket */

	if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("cannot create socket\n");
		return 0;
	}

	int sockbufsize = 250000/2;
	socklen_t optlen = sizeof(sockbufsize);


	setsockopt(fd, SOL_SOCKET, SO_RCVBUF,
        (char *)&sockbufsize,  (int)sizeof(sockbufsize));
	sockbufsize = 1;
	int ret = getsockopt(fd, SOL_SOCKET, SO_RCVBUF, 
        (char*)&sockbufsize, &optlen);

	printf("ret %d size %d paramsize %d\n", ret, sockbufsize, optlen);

	/* bind the socket to any valid IP address and a specific port */

	memset((char *)&myaddr, 0, sizeof(myaddr));
	myaddr.sin_family = AF_INET;
	myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	myaddr.sin_port = htons(SERVICE_PORT);

	if (bind(fd, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0) {
		perror("bind failed");
		return 0;
	}

	/* now loop, receiving data and printing what we received */
	int count = 0;
	for (;;) {
			recvlen = recvfrom(fd, buf, BUFSIZE, 0, (struct sockaddr *)&remaddr, &addrlen);
			if (recvlen > 0) {
					printf("rec %d\n", count);
					count++;
					//buf[recvlen] = 0;
					//printf("received message: \"%s\"\n", buf);
			}
	}
	/* never exits */
}
