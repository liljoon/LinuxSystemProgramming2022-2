#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "tcp.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void main(int argc, char *argv[])
{
	int					sockfd, n;
	struct sockaddr_in	servAddr;
	MsgType				msg;

	// socket 생성, ipv4, tcp
	if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1)  {
		perror("socket");
		exit(1);
	}

	bzero((char *)&servAddr, sizeof(servAddr)); // servAddr : 서버 주소 정보 초기화
	servAddr.sin_family = PF_INET; // ipv4 set
	servAddr.sin_addr.s_addr = inet_addr(SERV_HOST_ADDR); // server주소 입력, dotted string을 정수+ newtwork byte 로 변환
	servAddr.sin_port = htons(SERV_TCP_PORT); // port 지정, network byte(little-endian) 으로 변환

	//connect 를 통해 socket을 server와 연결, 
	if (connect(sockfd, (struct sockaddr *)&servAddr, sizeof(servAddr)) == -1)  {
		perror("connect");
		exit(1);
	}
	
	msg.type = MSG_REQUEST; // 요청 flag 삽입
	sprintf(msg.data, "This is a request from %d.", getpid()); //msg.data에 메시지 작성
	if (write(sockfd, (char *)&msg, sizeof(msg)) == -1)  { //write로 msg 전송
		perror("write");
		exit(1);
	}
	printf("Sent a request.....");
		
	if (n = read(sockfd, (char *)&msg, sizeof(msg)) == -1)  { // 서버로 부터의 답장 read
		perror("read");
		exit(1);
	}
	printf("Received reply: %s\n", msg.data); // 답장 출력

	close(sockfd); // socket close하여 연결 종료
}
