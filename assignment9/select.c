#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>
#include "select.h"
#include <stdlib.h>
#include <pthread.h>

int	TcpSockfd;
int	UdpSockfd;
// 전역변수로 설정
void
CloseServer() // SIGINT 종료 signal handler
{
	close(TcpSockfd);
	close(UdpSockfd);

	printf("\nServer daemon exit.....\n");

	exit(0);
}

void
MakeTcpSocket()
{
	struct sockaddr_in	servAddr;
	
	// Tcp Socket 생성
	if ((TcpSockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)  {
		perror("socket");
		exit(1);
	}
	
	// server address 주소 세팅
	bzero((char *)&servAddr, sizeof(servAddr));
	servAddr.sin_family = PF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(SERV_TCP_PORT); // port udp 와 다름.
	
	// bind 하여 socket 에 주소 연결
	if (bind(TcpSockfd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)  {
		perror("bind");
		exit(1);
	}

	listen(TcpSockfd, 5); // 최대 개수 지정
}

void
MakeUdpSocket()
{
	struct sockaddr_in	servAddr;

	//udp socket 생성
	if ((UdpSockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0)  {
		perror("socket");
		exit(1);
	}
	
	// server address 지정
	bzero((char *)&servAddr, sizeof(servAddr));
	servAddr.sin_family = PF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(SERV_UDP_PORT); // 전용 port

	// server 주소와 socket 연결
	if (bind(UdpSockfd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)  {
		perror("bind");
		exit(1);
	}

}


void
ProcessTcpRequest()
{
	int					newSockfd, cliAddrLen, n;
	struct sockaddr_in	cliAddr;
	MsgType				msg;

	cliAddrLen = sizeof(cliAddr);
	// accpet로 client 와 tcp connection 맺음, 새로운 socket 반환
	newSockfd = accept(TcpSockfd, (struct sockaddr *) &cliAddr, &cliAddrLen);
	if (newSockfd < 0)  {
		perror("accept");
		exit(1);
	}
		
	// data read
	if ((n = read(newSockfd, (char *)&msg, sizeof(msg))) < 0)  {
		perror("read");
		exit(1);
	}
	printf("Received TCP request: %s.....", msg.data);

	// data write
	msg.type = MSG_REPLY;
	sprintf(msg.data, "This is a reply from %lu.", pthread_self()); // thread id
	if (write(newSockfd, (char *)&msg, sizeof(msg)) < 0)  {
		perror("write");
		exit(1);
	}
	usleep(10000); // error 방지. client 가 read전 끊어버리면 netstat에 남음
	printf("Replied.\n");
	close(newSockfd);
}

void
ProcessUdpRequest()
{
	int					cliAddrLen, n;
	struct sockaddr_in	cliAddr;
	MsgType				msg;

	cliAddrLen = sizeof(cliAddr);
	// recvfrom으로 바로 udp를 통해 데이터 받음
	if ((n = recvfrom(UdpSockfd, (char *)&msg, sizeof(msg), 
				0, (struct sockaddr *)&cliAddr, &cliAddrLen)) < 0)  {
		perror("recvfrom");
		exit(1);
	}
	printf("Received UDP request: %s.....", msg.data);
	
	// sendto 로 바로 udp 보냄
	msg.type = MSG_REPLY;
	sprintf(msg.data, "This is a reply from %lu.", pthread_self()); // thread id
	if (sendto(UdpSockfd, (char *)&msg, sizeof(msg),
				0, (struct sockaddr *)&cliAddr, cliAddrLen) < 0)  {
		perror("sendto");
		exit(1);
	}
	printf("Replied.\n");
}

void
main(int argc, char *argv[])
{
	fd_set	fdvar; // descriptor 여러개 control
	int		count;
	pthread_t		tid1, tid2;

	signal(SIGINT, CloseServer); // signal handler 등록

	MakeTcpSocket(); // tcp socket 생성
	MakeUdpSocket(); // upd socket 생성

	printf("Server daemon started.....\n");

	while (1)  {
		FD_ZERO(&fdvar); // fd_set 초기화
		FD_SET(TcpSockfd, &fdvar); // Tcp socket 등록 -> 해당 fd를 검사하겠다.
		FD_SET(UdpSockfd, &fdvar); // Udp socket 등록
		// 동시에 기다리고 있는 socket 개수 반환, 무한히 기다림, 최대 몇까지 확인할지 대충10넣음
		if ((count = select(10, &fdvar, 0, 0, 0)) == -1)  {
			perror("select");
			exit(1);
		}
		while (count--)  { // 기다리는 수 만큼 반복
			if (FD_ISSET(TcpSockfd, &fdvar))  { // TcpSockfd가 setting되었다면 (읽을 데이터가 있다.)
				pthread_create(&tid1, 0, (void *)ProcessTcpRequest, 0); // tcp 처리 , thread로
			}
			else if (FD_ISSET(UdpSockfd, &fdvar))  { // 마찬가지로 udp도 확인
				pthread_create(&tid2, 0, (void *)ProcessUdpRequest, 0);
			}
		}
	}
}
