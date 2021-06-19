#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	pid_t pid=fork(); // 자식 프로세스 생성.
	
	if(pid==0) // 자식 프로세스이면.
		puts("Hi, I am a child process!!");
	
	else
	{
		printf("Child Process ID: %d \n",pid); // 
		sleep(20); //20초간 지연.
	}
	if(pid==0)
		puts("End child process");
	else
		puts("End parent process");
	
	return 0;
	
}