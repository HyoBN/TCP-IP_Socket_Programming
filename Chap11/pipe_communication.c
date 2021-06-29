#include <stdio.h>
#include <unistd.h>
#define BUF_SIZE 30

int main()
{
	int fds[2];
	char str[]="Hello, HB!";
	char buf[BUF_SIZE];
	pid_t pid;
	
	pipe(fds); // 파이프 생성.
	pid=fork(); //자식 프로세스 생성, 이 자식 프로세스는 부모의 파일 디스크립터도 복사한다. (파이프가 복사된 것은 아니다!!).
	if(pid==0)
		write(fds[1],str,sizeof(str));
	
	else
	{
		read(fds[0], buf, BUF_SIZE);
		puts(buf);
	}
	return 0;
}