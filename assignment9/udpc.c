#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "udp.h"
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void
main(int argc, char *argv[])
{
	int					sockfd, n, peerAddrLen;
	struct sockaddr_in	servAddr, peerAddr;
	MsgType				msg;

	// socket 생성, ipv4, upd
	if ((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) == -1)  {
		perror("socket");
		exit(1);
	}

	bzero((char *)&servAddr, sizeof(servAddr)); // server 주소 지정 초기화
	servAddr.sin_family = PF_INET; // ipv4
	servAddr.sin_addr.s_addr = inet_addr(SERV_HOST_ADDR); // server 주소 string에서 network byte order로 지정
	servAddr.sin_port = htons(SERV_UDP_PORT); // port networkbyte order로 지정

	msg.type = MSG_REQUEST; // Flag 지정
	sprintf(msg.data, "This is a request from %d.", getpid()); // 요청 msg 저장
	// sendto 를 이용해서 바로 전송, msg와 서버주소 지정, option  0, 자동으로 bind
	if (sendto(sockfd, &msg, sizeof(msg),0, (struct sockaddr *)&servAddr, sizeof(servAddr)) == -1)  {
		perror("sendto");
		exit(1);
	}
	printf("Sent a request.....");
		
	peerAddrLen = sizeof(peerAddr);
	// recvfrom으로 답장 받음. 
	if (recvfrom(sockfd, &msg, sizeof(msg), 0, (struct sockaddr *)&peerAddr, &peerAddrLen) == -1)  {
		perror("recvfrom");
		exit(1);
	}
	printf("Received reply: %s\n", msg.data); // 답장 출력

	close(sockfd);
}
