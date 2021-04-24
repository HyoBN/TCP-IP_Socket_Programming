#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>

int main(void)
{
	int fd1, fd2, fd3;
	fd1=socket(PF_INET, SOCK_STREAM, 0);
	fd2=open("test.dat",O_CREAT|O_WRONLY|O_TRUNC);
	fd3=socket(PF_INET, SOCK_DGRAM, 0);
	
	printf("file descriptor 1 : %d\n",fd1);
	printf("file descriptor 2 : %d\n",fd2);	
	printf("file descriptor 3 : %d\n",fd3);	

	//Desciptor가 3부서 시작하는 이유는 0,1,2는 이미 표준 입출력에 할당되었기 때문.
}