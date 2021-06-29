#include <stdio.h>
#include <unistd.h>
#define BUF_SIZE 30

int main()
{
	int fds[2];
	char str1[]="Hello, HB!";
	char str2[]="Nice to meet you!";
	char buf[BUF_SIZE];
	pid_t pid;
	
	pipe(fds); // 파이프 생성.
	pid=fork(); //자식 프로세스 생성, 이 자식 프로세스는 부모의 파일 디스크립터도 복사한다. (파이프가 복사된 것은 아니다!!).
	if(pid==0)
	{
		write(fds[1],str1,sizeof(str1));
		sleep(2);
		read(fds[0],buf,BUF_SIZE); //데이터를 받아 buf에 저장.
		printf("Child proc output: %s \n",buf);
	}
	
	else
	{
		read(fds[0],buf, BUF_SIZE);
		printf("Parent proc output: %s \n",buf);
		write(fds[1],str2,sizeof(str2));
		//sleep(3); // 부모 프로세스가 먼저 종료되면 명령어 수행이 완료된 줄 알고 다음 명령어 입력라인이 뜬다. 이를 방지하기 위해 자식 프로세스가 끝나기까지 3초간 기다리는 것.
	}
	return 0;
}