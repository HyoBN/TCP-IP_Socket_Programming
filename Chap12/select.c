#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/select.h>
#define BUF_SIZE 30

int main()
{
	fd_set reads, temps;
	int result, str_len;
	char buf[BUF_SIZE];
	struct timeval timeout;
	
	FD_ZERO(&reads); //fd_set형 변수 reads를 0으로 초기화.
	FD_SET(0,&reads); // reads의 0번째 인자를 set(1로 변경).

	/*
	timeout.tv_sec=5;
	timeout.tv_usec=5000;
	*/
	
	while(1)
	{
		temps=reads; //reads에 저장된 원본을 유지하기 위해. select함수호출이 끝나면 변화가 생긴 파일디스크립터의 위치를 제외한 나머지 비트들은 0으로 초기화되기 때문.
		timeout.tv_sec=5;
		timeout.tv_usec=0;
		result=select(1, &temps, 0, 0, &timeout);
		if(result==-1)
		{
			puts("select() error!");
			break;
		}
		else if(result==0)
			puts("Time out !");
		else{
			if(FD_ISSET(0, &temps));
			{
				str_len=read(0, buf, BUF_SIZE);
				buf[str_len]=0;
				printf("message from console: %s", buf);
			}
		}
	}





}