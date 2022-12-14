#include <stdio.h>
#include <pthread.h>

//start 부터 +49까지 더하는 함수
void sum50(int *start)
{
	int ret = 0;
	for(int i = 0;i<50;i++)
	{
		ret += (i + *start);
	}
	pthread_exit((void *)ret); // 쓰레드 종료시 더한 값 반한
}

int main()
{
	int a,b;
	int start1 = 1;
	int start2 = 51;

	pthread_t tid1, tid2;
	pthread_create(&tid1, NULL,(void *)sum50, &start1); //1~50
	pthread_create(&tid2, NULL,(void *)sum50, &start2); // 51~100
	pthread_join(tid1, (void **)&a); // a로 1~50 까지 더한 값 반환 기다림
	pthread_join(tid2, (void **)&b); // bfh 51~100 까지 더한 값 반환 기다림
	printf("%d\n", a + b); // 최종 값 출력
}
