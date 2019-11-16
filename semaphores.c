#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<string.h>
#include<semaphore.h>

sem_t sem;
int counter;

void handler(void *ptr) {
	int x;
	x = *((int *)ptr);
	printf("Thread %d: waiting to enter critical region...\n", x);
	sem_wait(&sem);

	// critical section starts
	sleep(2);
	printf("critical section start. thread: %d\n", x);
	printf("thread %d: counter value: %d\n", x, counter);
	counter++;
	printf("thread %d: new counter value %d\n", x, counter);
	printf("thread %d: exiting.\n", x);

	sem_post(&sem);
	pthread_exit(0);
}

int main() {

	int i[2];
	pthread_t t1;
	pthread_t t2;
	i[0] = 0;
	i[1] = 1;

	sem_init(&sem, 0, 1);
	pthread_create(&t1, NULL, (void *)&handler, (void *)&i[0]);
	pthread_create(&t2, NULL, (void *)&handler, (void *)&i[1]);

	pthread_join(t1, NULL);
	pthread_join(t2, NULL);

	sem_destroy(&sem);
	return 0;
}