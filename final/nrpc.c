#include "tcp.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


int	Sockfd;

void
CloseServer()
{
	close(Sockfd);
	printf("\nTCP Server exit.....\n");

	exit(0);
}

void main()
{
	int					newSockfd, cliAddrLen, n;
	struct sockaddr_in	cliAddr, servAddr;
	int	data;

	signal(SIGINT, CloseServer);

	if ((Sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)  {
		perror("socket");
		exit(1);
	}
	bzero((char *)&servAddr, sizeof(servAddr));
	servAddr.sin_family = PF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(SERV_TCP_PORT);

	if (bind(Sockfd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)  {
		perror("bind");
		exit(1);
	}

	listen(Sockfd, 5);
	printf("TCP Server started.....\n");
	while (1)  {
		newSockfd = accept(Sockfd, (struct sockaddr *) &cliAddr, &cliAddrLen);
		if (newSockfd < 0)  {
			perror("accept");
			exit(1);
		}
		
		if ((n = read(newSockfd, &data, sizeof(int))) < 0)  {
			perror("read");
			exit(1);
		}
	
		data = ntohl(data);
		int sum = 0;
		for (int i=0;i <= data;i++)
		{
			sum+=i;
		}
		data = htonl(sum);
		printf("Calc = %d\n", sum);

		if (write(newSockfd, &data, sizeof(int)) < 0)  {
			perror("write");
			exit(1);
		}

		usleep(10000);
		close(newSockfd);
	}

}
