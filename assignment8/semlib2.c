#include <stdio.h>
#include <pthread.h>
#include "semlib2.h"


int
sem_init(sem_t *sem, int pshared, int value) // semaphore 초기화
{
	if (pshared)  { // pshared 지원안함.
		fprintf(stderr, "Function not supported\n");
		return -1;
	}

	if (pthread_mutex_init(&sem->mutex, NULL) < 0) // mutex lock 초기화
		return -1;

	if (pthread_cond_init(&sem->cond, NULL) < 0) // condition variable 초기화
		return -1;

	sem->sval = value; // sval 을 value로 초기화

	return 0;
}

int
sem_wait(sem_t *sem) // wait, -1 하는 함수.
{
	if (pthread_mutex_lock(&sem->mutex) < 0) // critical section(sem)으로 mutex lock
		return -1;

	if (sem->sval == 0)  { // 만약 semaphore가 0이라면
		while (sem->sval == 0)  {
			if (pthread_cond_wait(&sem->cond, &sem->mutex) < 0)  { // lock 임시해제 후 wait로 바꿈
				if (pthread_mutex_unlock(&sem->mutex) < 0) // 에러면 unlock
					return -1;
				return -1;
			}
		}
		sem->sval--; // 깨어나면 -1
	}
	else  {
		sem->sval--; // 0이 아니라면 -1
	}

	if (pthread_mutex_unlock(&sem->mutex) < 0) // critical section 종료
		return -1;

	return 0;
}

int
sem_trywait(sem_t *sem) //trywait : wait했을 때 0이라면 -1 반환
{
	if (pthread_mutex_lock(&sem->mutex) < 0) // critical section 보호
		return -1;

	if (sem->sval == 0)  { //만약 0이라면 unlock후 -1 반환
		if (pthread_mutex_unlock(&sem->mutex) < 0)
			return -1;
		return -1;
	}
	else  {
		sem->sval--; // 0이 아니였다면 -1
	}

	if (pthread_mutex_unlock(&sem->mutex) < 0) // critical section 종료
		return -1;

	return 0;
}

int
sem_post(sem_t *sem) // signal, +1
{
	if (pthread_mutex_lock(&sem->mutex) < 0) // critical section 보호
		return -1;

	if (sem->sval == 0)  { // 만약 0이었다면
		if (pthread_cond_signal(&sem->cond) < 0)  { // con_signal로 대기중이던 thread 깨움.
			if (pthread_mutex_unlock(&sem->mutex) < 0) // error시 unlock 후 -1 반환
				return -1;
			return -1;
		}
	}

	sem->sval++; // sval +1

	if (pthread_mutex_unlock(&sem->mutex) < 0) // critical section 종료
		return -1;

	return 0;
}

int
sem_getvalue(sem_t *sem, int *sval) // sval 반환
{
	*sval = sem->sval;

	return 0;
}

int
sem_destroy(sem_t *sem)
{
	if (pthread_mutex_destroy(&sem->mutex) < 0) // mutex lock destroy
		return -1;

	if (pthread_cond_destroy(&sem->cond) < 0) // condition variable destroy
		return -1;

	return 0;
}
