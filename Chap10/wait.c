#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
	int status;
	pid_t pid=fork();
	
	if(pid==0)
		return 3; // 앞서 fork를 통해 만들어진 자식 프로세스가 return 을 만나 종료하게 된다.
	
	else
	{
		printf("Child PID: %d \n",pid);
		pid=fork();
		
		if(pid==0)
			exit(7); //위에서 생성된 자식 프로세스가 exit 를 만나 종료하게 된다.
		else
		{
			printf("Child PID: %d \n",pid);
			wait(&status); //종료된 프로세스 관련 정보가 저장되고 해당 프로세스는 완전 소멸.

			if(WIFEXITED(status)) // 자식 프로세스가 정상 종료되었으면 true.
				printf("Child send one : %d \n",WEXITSTATUS(status));

			wait(&status);
			if(WIFEXITED(status))
			printf("Child send two : %d \n",WEXITSTATUS(status));
			sleep(20);
		}
	}
	return 0;
}