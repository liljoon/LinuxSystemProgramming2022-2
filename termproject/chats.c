#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include "chat.h"
#include <stdlib.h> 
#include <string.h>
#include <unistd.h>

#define	DEBUG

#define	MAX_CLIENT	5
#define	MAX_ID		32
#define	MAX_BUF		256

typedef	struct  {
	int			sockfd;
	int			inUse;
	char		uid[MAX_ID];
}
	ClientType;

int				Sockfd;
pthread_mutex_t	Mutex;

ClientType		Client[MAX_CLIENT];


int
GetID()
{
	int	i;

	for (i = 0 ; i < MAX_CLIENT ; i++)  {
		if (! Client[i].inUse)  {
			Client[i].inUse = 1;
			return i;
		}
	}
}

void
SendToOtherClients(int id, char *buf)
{
	int		i;
	char	msg[MAX_BUF+MAX_ID];

	sprintf(msg, "%s> %s", Client[id].uid, buf);
#ifdef	DEBUG
	printf("%s", msg);
	fflush(stdout);
#endif

	pthread_mutex_lock(&Mutex);
	for (i = 0 ; i < MAX_CLIENT ; i++)  {
		if (Client[i].inUse && (i != id))  {
			if (send(Client[i].sockfd, msg, strlen(msg)+1, 0) < 0)  {
				perror("send");
				exit(1);
			}
		}
	}
	pthread_mutex_unlock(&Mutex);
}
	

void
ProcessClient(int id)
{
	char	buf[MAX_BUF];
	int		n;


	if ((n = recv(Client[id].sockfd, Client[id].uid, MAX_ID, 0)) < 0)  {
		perror("recv");
		exit(1);
	}
	printf("Client %d log-in(ID: %s).....\n", id, Client[id].uid);

	while (1)  {
		if ((n = recv(Client[id].sockfd, buf, MAX_BUF, 0)) < 0)  {
			perror("recv");
			exit(1);
		}
		if (n == 0)  { // 끊어짐 감지
			printf("Client %d log-out(ID: %s).....\n", id, Client[id].uid);

			pthread_mutex_lock(&Mutex);
			close(Client[id].sockfd);
			Client[id].inUse = 0;
			pthread_mutex_unlock(&Mutex);

			strcpy(buf, "log-out.....\n");
			SendToOtherClients(id, buf);
			return ;
		}

		SendToOtherClients(id, buf);
	}
}


void
CloseServer(int signo)
{
	int		i;

	close(Sockfd);

	for (i = 0 ; i < MAX_CLIENT ; i++)  {
		if (Client[i].inUse)  {
			close(Client[i].sockfd);
		}
	}
	if (pthread_mutex_destroy(&Mutex) < 0)  {
		perror("pthread_mutex_destroy");
		exit(1);
	}

	printf("\nChat server terminated.....\n");

	exit(0);
}

void
main(int argc, char *argv[])
{
	int					newSockfd, cliAddrLen, id, one = 1;
	struct sockaddr_in	cliAddr, servAddr;
	int					count;
	fd_set				fdvar;

	signal(SIGINT, CloseServer); // server 종료시 sighandler 등록
	if (pthread_mutex_init(&Mutex, NULL) < 0)  { // Mutex 초기화
		perror("pthread_mutex_init");
		exit(1);
	}

	if ((Sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)  { // tcp socket 생성
		perror("socket");
		exit(1);
	}

	if (setsockopt(Sockfd, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one)) < 0)  {
		perror("setsockopt");
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

	printf("Chat server started.....\n");

	cliAddrLen = sizeof(cliAddr);
	while (1)  {
		FD_ZERO(&fdvar);
		FD_SET(Sockfd, &fdvar);
		count = select(10, &fdvar, 0, 0, 0);
		
		while (count--)
		{
			newSockfd = accept(Sockfd, (struct sockaddr *) &cliAddr, &cliAddrLen);
			if (newSockfd < 0)  {
				perror("accept");
				exit(1);
			}

			id = GetID(); // 안쓰는 client index 반환
			Client[id].sockfd = newSockfd; // 새로 연결된 socketFd 지정
			ProcessClient(id);
		}
	}
}
