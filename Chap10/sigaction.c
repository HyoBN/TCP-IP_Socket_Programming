#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void timeout(int sig)
{
	if(sig==SIGALRM)
		puts("Time out!");
	alarm(2);
	
}

int main()
{
	int i;
	struct sigaction act;
	act.sa_handler=timeout; // 시그널 발생시 timeout 함수를 호출하라!
	sigemptyset(&act.sa_mask);
	act.sa_flags=0;
	sigaction(SIGALRM, &act, 0);
	
	alarm(2); // 2초 후 시그널 SIGALRM 발생하라!
	
	for(i=0;i<3;i++)
	{
		puts("wait...");
		sleep(100);
	}
	
	return 0;
}