#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "tcp.h"
#include <unistd.h>
#include <stdlib.h>

int Calc(int number)
{
	
	int					sockfd, n;
	struct sockaddr_in	servAddr;
	int	data = htonl(number);

	if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)  {
		perror("socket");
		exit(1);
	}

	bzero((char *)&servAddr, sizeof(servAddr));
	servAddr.sin_family = PF_INET;
	servAddr.sin_addr.s_addr = inet_addr(SERV_HOST_ADDR);
	servAddr.sin_port = htons(SERV_TCP_PORT);

	if (connect(sockfd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)  {
		perror("connect");
		exit(1);
	}
	
	if (write(sockfd, &data, sizeof(int)) < 0)  {
		perror("write");
		exit(1);
	}
		
	if ((n = read(sockfd, &data, sizeof(int))) < 0)  {
		perror("read");
		exit(1);
	}
	data = ntohl(data);

	close(sockfd);
	return (data);
}

void main(int argc, char *argv[])
{
	printf("Calc = %d\n", Calc(atoi(argv[1])));
}
