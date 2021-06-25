#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

void read_childproc(int sig)
{
	int status;
	pid_t id=waitpid(-1, &status, WNOHANG); // waitpid 함수 호출로 인해 자식 프로세스는 좀비가 되지않고 소멸됨.
	if(WIFEXITED(status))
	{
		printf("Removed proc id : %d \n",id);
		printf("Child send: %d \n", WEXITSTATUS(status));
	}
	
}

int main()
{
	pid_t pid;
	struct sigaction act;
	act.sa_handler=read_childproc; // 자식 프로세스가 종료되면 read_childproc 함수 호출.
	sigemptyset(&act.sa_mask);
	act.sa_flags=0;
	sigaction(SIGCHLD,&act, 0);
	
	pid=fork();
	if(pid==0)
	{
		puts("Hi! I'm child process!");
		sleep(10);
		return 12;
	}
	else
	{
		printf("Child proc id: %d \n",pid);
		pid=fork(); //자식 프로세스 하나 더 생성.
		if(pid==0)
		{
			puts("Hi! I'm child process");
			sleep(10);
			exit(24);
		}
		else
		{
			int i;
			printf("Child proc id : %d \n",pid);
			for(i=0;i<5;i++)
			{
				puts("wait...");
				sleep(5);
			}
		}
	}
}



