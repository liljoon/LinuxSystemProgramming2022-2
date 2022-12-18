#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <stdlib.h>
#include "udp.h"
#include <string.h>
#include <unistd.h>

int	Sockfd;

void
CloseServer() // SIGINT 종료 handler function
{
	close(Sockfd);
	printf("\nUDP Server exit.....\n");

	exit(0);
}


void main(int argc, char *argv[])
{
	int					cliAddrLen, n;
	struct sockaddr_in	cliAddr, servAddr; // client(답장을 위해), server(서버 주소 지정) address struct
	MsgType				msg; // write,read할 data struct

	signal(SIGINT, CloseServer); // SIGINT signal 등록

	// socket 생성, ipv4, udp
	if ((Sockfd = socket(PF_INET, SOCK_DGRAM, 0)) == -1)  {
		perror("socket");
		exit(1);
	}

	bzero((char *)&servAddr, sizeof(servAddr)); // server 주소 구조체 초기화
	servAddr.sin_family = PF_INET; // ipv4 set
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY); // INADDR_ANY(0.0.0.0) network byte로 지정
	servAddr.sin_port = htons(SERV_UDP_PORT); // port를 network byte order로 지정

	// bind하여 Sockfd 에다가 서버 주소 연결
	if (bind(Sockfd, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0)  {
		perror("bind");
		exit(1);
	}
	//udp라서 listen 필요 없음
	printf("UDP Server started.....\n");
	
	cliAddrLen = sizeof(cliAddr);
	while (1)  {
		// udp라서 연결 없이 read,write없음
		// 바로 recvfrom 으로 msg 받음. option = 0, client struct 받음 (답장)
		if ((n = recvfrom(Sockfd, &msg, sizeof(msg), 0, (struct sockaddr *)&cliAddr, &cliAddrLen)) == -1)  {
			perror("recvfrom");
			exit(1);
		}
		printf("Received request: %s.....", msg.data); // 받은 msg 출력

		msg.type = MSG_REPLY; // 답장 flag 지정
		sprintf(msg.data, "This is a reply from %d.", getpid()); // 답장 msg에 저장
		// sendto 를 통해 msg 전송, option = 0, client 주소 지정
		if (sendto(Sockfd, &msg, sizeof(msg), 0, (struct sockaddr *)&cliAddr, cliAddrLen) == -1)  {
			perror("sendto");
			exit(1);
		}
		printf("Replied.\n");
	}
}
