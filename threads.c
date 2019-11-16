#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>

int sum = 0;

void *printHello(void *num) {
	sum = sum + *((int*)num);
	sleep(1);
	pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
	int NUM_THREADS = argc - 1;
	pthread_t threads[NUM_THREADS];
	int rc;
	long t;

	for (t = 0; t < NUM_THREADS; t++) {
		printf("In main: creating thread %ld\n", t);
		int num = atoi(argv[t + 1]);
		printf("%d\n", num);
		rc = pthread_create(&threads[t], NULL, printHello, (void *)(&num));
		if (rc) {
			printf("ERROR: %d in thread_create.\n", rc);
			exit(1);
		}
		pthread_join(threads[t], NULL);
		printf("sum: %d\n", sum);
		printf("exiting main thread.\n");
	}
	pthread_exit(NULL);
}