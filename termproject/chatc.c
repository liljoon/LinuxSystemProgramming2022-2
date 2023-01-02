#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>
#include <stdlib.h>
#include <ctype.h>
#include "chat.h"

#define	MAX_BUF		256

int		Sockfd;
pthread_t tid1, tid2;

void ProcessStdin(void *arg)
{
	char	buf[MAX_BUF];
	
	while(1)
	{
		fgets(buf, MAX_BUF, stdin);
		if (send(Sockfd, buf, strlen(buf)+1, 0) < 0)  {
			perror("send");
			exit(1);
		}
	}
}

void ProcessUdp(void *arg)
{
	char	buf[MAX_BUF];
	int		n;

	while(1)
	{
		if ((n = recv(Sockfd, buf, MAX_BUF, 0)) < 0)  {
			perror("recv");
			exit(1);	
		}
		if (n == 0)  {
			fprintf(stderr, "Server terminated.....\n");
			close(Sockfd);
			exit(1);
		}
		printf("%s", buf);
	}
}

void
ChatClient(void)
{
	char	buf[MAX_BUF];
	int		count, n;
	fd_set	fdset;

	printf("Enter ID: ");
	fflush(stdout);
	fgets(buf, MAX_BUF, stdin);
	*strchr(buf, '\n') = '\0';
	if (send(Sockfd, buf, strlen(buf)+1, 0) < 0)  {
		perror("send");
		exit(1);
	}
	printf("Press ^C to exit\n");

	pthread_create(&tid1, 0, (void *)ProcessStdin, 0);
	pthread_create(&tid2, 0, (void *)ProcessUdp, 0);
	pthread_join(tid1, 0);
	pthread_join(tid2, 0);
}
	

void
CloseClient(int signo)
{
	close(Sockfd);
	printf("\nChat client terminated.....\n");
	pthread_cancel(tid1);
	pthread_cancel(tid2);
	exit(0);
}

void
main(int argc, char *argv[])
{
	struct sockaddr_in	servAddr;
	struct hostent		*hp;

	if (argc != 2)  {
		fprintf(stderr, "Usage: %s ServerIPaddress\n", argv[0]);
		exit(1);
	}

	if ((Sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)  {
		perror("socket");
		exit(1);
	}

	bzero((char *)&servAddr, sizeof(servAddr));
	servAddr.sin_family = PF_INET;
	servAddr.sin_port = htons(SERV_TCP_PORT);

	if (isdigit(argv[1][0]))  {
		servAddr.sin_addr.s_addr = inet_addr(argv[1]);
	}
	else  {
		if ((hp = gethostbyname(argv[1])) == NULL)  {
			fprintf(stderr, "Unknown host: %s\n", argv[1]);
			exit(1);
		}
		memcpy(&servAddr.sin_addr, hp->h_addr, hp->h_length);
	}

	if (connect(Sockfd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)  {
		perror("connect");
		exit(1);
	}

	signal(SIGINT, CloseClient);

	ChatClient();
}
