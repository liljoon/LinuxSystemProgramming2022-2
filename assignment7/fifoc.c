#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include "fifo.h"
#include <stdlib.h>
#include <string.h>

int main()
{
	char		fname[MAX_FIFO_NAME]; // client specific fifo file명
	int			fd, sfd, n;
	MsgType		msg;

	sprintf(fname, ".fifo%d", getpid()); // client specific fifo file 이름 지정
	if (mkfifo(fname, 0600) < 0)  { // client specific fifo file 생성
		perror("mkfifo");
		exit(1);
	}

	if ((fd = open(fname, O_RDWR)) < 0)  { // client specific fifo file open (답장을 위한)
		perror("open");
		exit(1);
	}
	if ((sfd = open(SERV_FIFO, O_RDWR)) < 0)  { // well-known fifo file open (서버에 전송을 위한)
		perror("open");
		exit(1);
	}

	strcpy(msg.returnFifo, fname); // msg에 복사
	sprintf(msg.data, "This is a request from %d.", getpid()); // data 저장
	write(sfd, &msg, sizeof(msg)); // Well-known fifo를 통해 전송
	printf("Sent a request.....");

	if ((n = read(fd, &msg, sizeof(msg)) < 0))  { // client fifo file 을 통한 답장 대기
		perror("read");
		exit(1);
	}

	printf("Received reply: %s\n", msg.data); // 답장 출력
	
	close(fd);
	close(sfd);

	/* Remove temporary FIFO */ // client fifo file 삭제
	if (remove(fname) < 0)  {
		perror("remove");
		exit(1);
	}
}
