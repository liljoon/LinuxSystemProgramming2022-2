#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include "fifo.h"
#include <stdlib.h>

void
SigIntHandler(int signo) // 프로그램 종료 신호 시 파일 지우는 핸들러
{
	if (remove(SERV_FIFO) < 0)  {
		perror("remove");
		exit(1);
	}

	exit(0);
}

void
main()
{
	int			fd, cfd, n;
	MsgType		msg;

	if (signal(SIGINT, SigIntHandler) == SIG_ERR)  { // handler 등록
		perror("signal");
		exit(1);
	}

	if (mkfifo(SERV_FIFO, 0600) < 0)  { // fifo파일 생성, 권한 0600
		if (errno != EEXIST)  { // 이미 존재하면 에러
			perror("mkfifo");
			exit(1);
		}
	}

	if ((fd = open(SERV_FIFO, O_RDWR)) < 0)  { // fifo파일 open
		perror("open");
		exit(1);
	}

	while (1)  {
		if ((n = read(fd, &msg, sizeof(msg))) < 0)  { //msg 도착 기다림(wait상태)
			if (errno == EINTR)  {
				continue;
			}
			else  {
				perror("read");
				exit(1);
			}
		}
		printf("Received request: %s.....", msg.data); //data 출력

		if ((cfd = open(msg.returnFifo, O_RDWR)) < 0)  { // 클라이언트 별 답장 파일 open
			perror("open");
			exit(1);
		}
		sprintf(msg.data, "This is a reply from %d.", getpid()); // data에 답장 저장
		write(cfd, &msg, sizeof(msg)); // 답장 파일 작성
		close(cfd);
		printf("Replied.\n"); // 답장완료
	}
}
