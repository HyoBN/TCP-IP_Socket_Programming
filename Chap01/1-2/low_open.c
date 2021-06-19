#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
void error_handling(char* message);

int main(void)
{
	int fd;
	char buf[] = "Let's go!!\n";
	
	fd=open("data.txt",O_CREAT|O_WRONLY|O_TRUNC); // 파일 오픈 모드가 O_CREAT O_WRONLY O_TRUNC이므로 새로운 파일이 생성되어 쓰기만 가능하며, data.txt라는 이름의 파일이 존재한다면 이 파일의 모든 데이터는 지워진다.
	if(fd==-1)
		error_handling("open() error!");
	printf("file descriptor: %d \n",fd);
	
	if(write(fd, buf, sizeof(buf))==-1) //fd에 저장된 파일 디스크립터에 해당하는 파일에 buf에 저장된 데이터를 전송한다.
		error_handling("write() error!");
	close(fd);
	return 0;
}


void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n',stderr);
	exit(1);
}
