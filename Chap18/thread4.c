#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#define NUM_THREAD	100

void * thread_inc(void * arg);
void * thread_des(void * arg);
long long num=0;

int main() 
{
	pthread_t thread_id[NUM_THREAD];
	int i;

	printf("sizeof long long: %d \n", sizeof(long long));
	
	for(i=0; i<NUM_THREAD; i++)
	{
		if(i%2)
			pthread_create(&(thread_id[i]), NULL, thread_inc, NULL); 
		else
			pthread_create(&(thread_id[i]), NULL, thread_des, NULL);	
	}	
	for(i=0; i<NUM_THREAD; i++){
		pthread_join(thread_id[i], NULL);
		
	}
	printf("result: %lld \n", num); //1번, 2번 쓰레드가 동시에 실행되는 경우 때문에 원하는 값(0)이 나오지 않는다.
	return 0;
}

void * thread_inc(void * arg) 
{
	int i;
	for(i=0; i<5000; i++) // 기존 코드는 i<50000000이지만 컴퓨터 성능을 고려하여 5000으로 줄였음.
		num+=1;
	return NULL;
}
void * thread_des(void * arg)
{
	int i;
	for(i=0; i<5000; i++)
		num-=1;
	return NULL;
}