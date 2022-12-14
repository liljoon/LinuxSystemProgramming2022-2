#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/time.h>

#define	NUM_MEN		5
#define	NLOOPS		5

#define	LEFT(i)		((i+NUM_MEN-1)%NUM_MEN) // 왼쪽 philosopher
#define	RIGHT(i)	((i+1)%NUM_MEN) // 오른쪽 philosopher

enum  { THINKING, EATING, HUNGRY }; // 상태 enum type

sem_t	Philosopher[NUM_MEN]; // philosopher를 semaphore
sem_t	Mutex; // pickup, putdown시 state가 shared data라 critical section 보호
int		State[NUM_MEN]; // philosopher 상태 저장

void
ThreadUsleep(int usecs) // Thread 각각 usleep
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
Thinking(int id) // random한 시간동안 thinking
{
	printf("Philosopher%d: Thinking.....\n", id);
	ThreadUsleep((rand()%200)*10000);
	printf("Philosopher%d: Want to eat.....\n", id);
}

void
Eating(int id) // random한 시간동안 eating
{
	printf("Philosopher%d: Eating.....\n", id);
	ThreadUsleep((rand()%100)*10000);
}

void
Test(int id)
{
	if (State[id] == HUNGRY && State[LEFT(id)] != EATING 
			&& State[RIGHT(id)] != EATING)  { // 좌우 philosopher 안먹어서 먹을 수 있는 상태
		State[id] = EATING; // 상태 변경 Test함수를 호출하는 함수가 mutexlock걸었으므로 보호
		
		// 먹어야하는 상황만 signal 로 +1이 됨.
		if (sem_post(&Philosopher[id]) != 0)  { // 해당 philosopher 먹을 수 있다고 signal
			perror("sem_post");
			pthread_exit(NULL);
		}
	}
}

void
PickUp(int id) // 식사 시작
{
	if (sem_wait(&Mutex) != 0)  { // critical section 보호
		perror("sem_wait");
		pthread_exit(NULL);
	}

	State[id] = HUNGRY; // 상태 변경

	Test(id); // 먹을 수 있는지 체크

	if (sem_post(&Mutex) != 0)  { // critical section 보호
		perror("sem_post");
		pthread_exit(NULL);
	}

	//philosopher wait, 만약 Test에서 못먹는 상태일 경우 여기서 waiting, 양 옆에서 다 먹었다면 signal해줌
	if (sem_wait(&Philosopher[id]) != 0)  {
		perror("sem_wait");
		pthread_exit(NULL);
	}
}

void
PutDown(int id)
{
	if (sem_wait(&Mutex) != 0)  { // critical section 보호
		perror("sem_wait");
		pthread_exit(NULL);
	}

	State[id] = THINKING; // 다 먹었으니 다시 THINKING

	Test(LEFT(id)); // 왼쪽 Philosopher가 만약 HUNGRY인지 확인 후 signal
	Test(RIGHT(id)); // 마찬가지로 오른쪽 확인

	if (sem_post(&Mutex) != 0)  { // critical section 보호
		perror("sem_post");
		pthread_exit(NULL);
	}
}

void
DiningPhilosopher(int *pId)
{
	int		i;
	int		id = *pId; // 몇번째 philosopher인지

	for (i = 0 ; i < NLOOPS ; i++)  { // NLOOPS 만큼 반복

		Thinking(id);

		PickUp(id);

		Eating(id);

		PutDown(id);
	}
	// 총 몇번 먹었는지 확인
	printf("Philosopher%d: thinking & eating %d times.....\n", id, i);

	pthread_exit(NULL);
}

void
main()
{
	pthread_t	tid[NUM_MEN];
	int			i, id[NUM_MEN];

	srand(0x8888);

	for (i = 0 ; i < NUM_MEN ; i++)  {
		if (sem_init(&Philosopher[i], 0, 0) != 0)  { // semaphore 0으로 초기화(아직 못먹는 상황)
			perror("sem_init");
			exit(1);
		}
		id[i] = i;
	}
	if (sem_init(&Mutex, 0, 1) != 0)  { // Mutex : binary semaphore  1로 초기화
		perror("sem_init");
		exit(1);
	}

	// philosopher 별로 thread 생성
	for (i = 0 ; i < NUM_MEN ; i++)  {
		if (pthread_create(&tid[i], NULL, (void *)DiningPhilosopher, 
				(void *)&id[i]) < 0)  {
			perror("pthread_create");
			exit(1);
		}
	}
	// 모든 Philosopher 다 먹을 때까지 기다림
	for (i = 0 ; i < NUM_MEN ; i++)  {
		if (pthread_join(tid[i], NULL) < 0)  {
			perror("pthread_join");
			exit(1);
		}
	}
	// semaphore delete
	for (i = 0 ; i < NUM_MEN ; i++)  {
		if (sem_destroy(&Philosopher[i]) != 0)  {
			perror("sem_destroy");
		}
	}

	if (sem_destroy(&Mutex) != 0)  {
		perror("sem_destroy");
		exit(1);
	}

}
