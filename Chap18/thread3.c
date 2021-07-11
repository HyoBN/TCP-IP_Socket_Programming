#include <stdio.h>
#include <pthread.h>
void * thread_summation(void * arg); 

int sum=0;

int main()
{
	pthread_t id_t1, id_t2;
	int range1[]={1, 5};
	int range2[]={6, 10};
	
	pthread_create(&id_t1, NULL, thread_summation, (void *)range1);
	pthread_create(&id_t2, NULL, thread_summation, (void *)range2);

	pthread_join(id_t1, NULL); //id_t1 쓰레드가 종료될 때까지 프로세스 종료하지 않음
	pthread_join(id_t2, NULL);
	printf("result: %d \n", sum); //쓰레드1과 2가 전역변수 sum에 접근하기 때문에 문제의 소지가 있다.
	return 0;
}

void * thread_summation(void * arg) 
{
	int start=((int*)arg)[0];
	int end=((int*)arg)[1];

	while(start<=end)
	{
		sum+=start;
		start++;
	}
	return NULL;
}