#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "tcp.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

// 원하는 ip 또는 도메인에 전송하는 프로그램
void main(int argc, char *argv[])
{
	int					sockfd, n;
	struct sockaddr_in	servAddr;
	MsgType				msg;
	struct hostent		*hp; // gethostbyname 반환 구조체

	if (argc != 2)  { // argument로 ipaddress도 받아야함
		fprintf(stderr, "Usage: %s IPaddress\n", argv[0]);
		exit(1);
	}
	
	// tcp로 socket 생성
	if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1)  {
		perror("socket");
		exit(1);
	}

	bzero((char *)&servAddr, sizeof(servAddr)); // server 주소 구조체 초기화
	servAddr.sin_family = PF_INET; // ipv4 지정
	servAddr.sin_port = htons(SERV_TCP_PORT); // port를 network byte로 지정
	
	// argument에 따라서 server ip 지정
	if (isdigit(argv[1][0]))  {
		servAddr.sin_addr.s_addr = inet_addr(argv[1]); // 만약 ip 첫 character가 숫자라면, string 을 network byte order로 변환하여 지정
	}
	else  {
		// 숫자가 아니라면 gethostbyname으로 hostent로 정보 반환
		if ((hp = gethostbyname(argv[1])) == NULL)  {
			fprintf(stderr, "Unknown host: %s\n", argv[1]);
			exit(1);
		}
		memcpy(&servAddr.sin_addr, hp->h_addr, hp->h_length); // hp->h_addr은첫번째 주소 network byte order로 이미 되어있어서 copy, h_length : 주소 길이
	}

	if (connect(sockfd, (struct sockaddr *)&servAddr, sizeof(servAddr)) == -1)  { //connect로 서버와 연결
		perror("connect");
		exit(1);
	}

	msg.type = MSG_REQUEST; // flag
	sprintf(msg.data, "This is a request from %d.", getpid());// data 저장
	if ((n = write(sockfd, &msg, sizeof(msg))) == -1)  { // data 전송
		perror("write");
		exit(1);
	}
	printf("Sent a request.....");
		
	// 답장 read, 출력
	if (read(sockfd, &msg, sizeof(msg)) == -1)  {
		perror("read");
		exit(1);
	}
	printf("Received reply: %s\n", msg.data);

	close(sockfd);
}
