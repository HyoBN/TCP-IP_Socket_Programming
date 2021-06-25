#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
	int status;
	pid_t pid= fork();
	
	if(pid==0)
	{
		sleep(21); // 자식 프로세스의 종료를 늦추기 위해 21초간의 지연 발생.
		return 24;
	}
	else
	{
		while(!waitpid(-1, &status, WNOHANG)) //21초간의 지연이 있기 때문에 총 7번 출력됨. == waitpid 함수가 블로킹 되지 않았음을 증명!!
		{
			sleep(3);
			puts("sleep 3sec.."); 
		}
		
		if(WIFEXITED(status))
			printf("Child send %d \n", WEXITSTATUS(status));
	}
}