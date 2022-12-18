#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include "tcp.h"
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int	Sockfd; // socket close 사용을 위해 전역변수 

void
CloseServer() // SIGINT 발생시 sighandler function
{
	close(Sockfd); // Sockfd close
	printf("\nTCP Server exit.....\n");

	exit(0);
}

void main(int argc, char *argv[])
{
	int					newSockfd, cliAddrLen, n, pid;
	struct sockaddr_in	cliAddr, servAddr; // server, client 주소 구조체 생성
	MsgType				msg; // write, read할 구조체정보

	signal(SIGINT, CloseServer); // 종료시 signal 등록

	if ((Sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)  { // ipv4, tcp로 socket 생성하여 desciptor 반환
		perror("socket");
		exit(1);
	}

	bzero((char *)&servAddr, sizeof(servAddr)); // servAddr 구조체 0으로 초기화. server주소와 관련된 구조체
	servAddr.sin_family = PF_INET; // ipv4 사용
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY); // 서버 주소 INADDR_ANY(0.0.0.0) 다 받음, hosttonetwork로 정수 변환
	servAddr.sin_port = htons(SERV_TCP_PORT); // port 지정하여 socket address 전부 지정. port도 little-endian으로 변환

	if (bind(Sockfd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)  { // bind 하여 Sockfd에 서버 주소 연결
		perror("bind");
		exit(1);
	}

	listen(Sockfd, 5); // listen , 동시에 접속 몇개까지 허용할지, 통상적으로 5개

	printf("TCP Server started.....\n");

	cliAddrLen = sizeof(cliAddr); // 길이 저장
	while (1)  {
		// tcp connection 기다리고 연결되면 새로운 sockfd 반환하여 그걸로 통신.
		newSockfd = accept(Sockfd, (struct sockaddr *) &cliAddr, &cliAddrLen);
		if (newSockfd < 0)  {
			perror("accept");
			exit(1);
		}

		//fork
		if ((pid = fork()) ==  0) // child
		{
			if ((n = read(newSockfd, (char *)&msg, sizeof(msg))) < 0)  { // 새로 맺어진 fd로 read
				perror("read");
				exit(1);
			}
			printf("Received request: %s.....", msg.data); // read한 data출력

			msg.type = MSG_REPLY; // 답장이라고 flag 붙임
			sprintf(msg.data, "This is a reply from %d.", getpid()); // data에 답장 저장.
			if (write(newSockfd, (char *)&msg, sizeof(msg)) < 0)  { // write하여 전송
				perror("write");
				exit(1);
			}
			printf("Replied.\n");

			usleep(10000); // 답장 처리까지 잠시 기다림
			close(newSockfd); // 새로 만들어진 newsocketfd close하여 연결 종료
			exit(0); // child process 종료
		}
		//parent 는 그냥 다시 반복문
	}
}

