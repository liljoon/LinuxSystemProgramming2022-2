#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/time.h>

#define	NUM_MEN		5
#define	NLOOPS		5

sem_t	ChopStick[NUM_MEN]; // 젓가락 semaphore

void
ThreadUsleep(int usecs) // thread마다 각각 usleep해주는 함수
{
	pthread_cond_t		cond;
	pthread_mutex_t		mutex;
	struct timespec		ts;
	struct timeval		tv;

	if (pthread_cond_init(&cond, NULL) < 0)  {
		perror("pthread_cond_init");
		pthread_exit(NULL);
	}
	if (pthread_mutex_init(&mutex, NULL) < 0)  {
		perror("pthread_mutex_init");
		pthread_exit(NULL);
	}

	gettimeofday(&tv, NULL);
	ts.tv_sec = tv.tv_sec + usecs/1000000;
	ts.tv_nsec = (tv.tv_usec + (usecs%1000000)) * 1000;
	if (ts.tv_nsec >= 1000000000)  {
		ts.tv_nsec -= 1000000000;
		ts.tv_sec++;
	}

	if (pthread_mutex_lock(&mutex) < 0)  {
		perror("pthread_mutex_lock");
		pthread_exit(NULL);
	}
	if (pthread_cond_timedwait(&cond, &mutex, &ts) < 0)  {
		perror("pthread_cond_timedwait");
		pthread_exit(NULL);
	}

	if (pthread_cond_destroy(&cond) < 0)  {
		perror("pthread_cond_destroy");
		pthread_exit(NULL);
	}
	if (pthread_mutex_destroy(&mutex) < 0)  {
		perror("pthread_mutex_destroy");
		pthread_exit(NULL);
	}
}

void
Thinking(int id) // Thinking 후 random시간 sleep후 eat으로 넘어감
{
	printf("Philosopher%d: Thinking.....\n", id);
	ThreadUsleep((rand()%200)*10000);
	printf("Philosopher%d: Want to eat.....\n", id);
}

void
Eating(int id) // 먹는 상태 일정시간 유지 후 종료
{
	printf("Philosopher%d: Eating.....\n", id);
	ThreadUsleep((rand()%100)*10000);
}

void
DiningPhilosopher(int *pId)
{
	int		i;
	int		id = *pId; // 몇번째 philosopher인지

	for (i = 0 ; i < NLOOPS ; i++)  { // NLOOPS 만큼 thinking, eating 반복

		Thinking(id);

		if (sem_wait(&ChopStick[id]) != 0)  { // 왼쪽 젓가락 wait
			perror("sem_wait");
			pthread_exit(NULL);
		}
		//여기서 데드락 발생 가능성이 있음.
		if (sem_wait(&ChopStick[(id+1) % NUM_MEN]) != 0)  { // 오른쪽 젓가락 wait
			perror("sem_wait");
			pthread_exit(NULL);
		}

		Eating(id); // 둘 다 집었으니 식사

		if (sem_post(&ChopStick[id]) != 0)  { // 왼쪽 젓가락 signal
			perror("sem_post");
			pthread_exit(NULL);
		}
		if (sem_post(&ChopStick[(id+1) % NUM_MEN]) != 0)  { // 오른쪽 젓가락 signal
			perror("sem_post");
			pthread_exit(NULL);
		}
	}
	// Philosopher가 총 몇번 했는지 출력
	printf("Philosopher%d: thinking & eating %d times.....\n", id, i);
	
	pthread_exit(NULL);
}

void
main()
{
	pthread_t	tid[NUM_MEN];
	int			i, id[NUM_MEN];

	srand(0x8888);

	for (i = 0 ; i < NUM_MEN ; i++)  { // 모든 젓가락 semaphore 1로 초기화
		if (sem_init(&ChopStick[i], 0, 1) != 0)  {
			perror("sem_init");
			exit(1);
		}
		id[i] = i;
	}

	for (i = 0 ; i < NUM_MEN ; i++)  { // 각 philospher 별로 쓰레드 생성
		if (pthread_create(&tid[i], NULL, (void *)DiningPhilosopher, 
				(void *)&id[i]) < 0)  {
			perror("pthread_create");
			exit(1);
		}
	}

	for (i = 0 ; i < NUM_MEN ; i++)  { // 모든 쓰레드 종료까지 기다림
		if (pthread_join(tid[i], NULL) < 0)  {
			perror("pthread_join");
			exit(1);
		}
	}

	for (i = 0 ; i < NUM_MEN ; i++)  {
		if (sem_destroy(&ChopStick[i]) != 0)  { // 모든 semaphore 삭제
			perror("sem_destroy");
		}
	}
}
