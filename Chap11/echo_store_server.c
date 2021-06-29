#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 100

void error_handling(char *message);
void read_childproc(int sig);

int main(int argc, char *argv[])
{
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_adr, clnt_adr;
	int fds[2];
	
	pid_t pid;
	struct sigaction act; //sigaction 형 변수 act를 선언.
	socklen_t adr_sz;
	int str_len, state;
	char buf[BUF_SIZE];
	if(argc!=2)
	{
		printf("Usage : %s <port>\n",argv[0]);
		exit(1);
	}
	
	act.sa_handler=read_childproc; 
	sigemptyset(&act.sa_mask);
	act.sa_flags=0;
	state=sigaction(SIGCHLD, &act, 0); // sigaction 함수 실행. -> SIGCHLD 시그널 발생 시, 변수 act의 sa_handler에 저장되어 있는 함수
	
	serv_sock=socket(PF_INET, SOCK_STREAM, 0);
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_adr.sin_port=htons(atoi(argv[1]));
	
	if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
		error_handling("bind() error");
	if(listen(serv_sock, 5)==-1)
		error_handling("listen() error!");
	
	pipe(fds);
	pid=fork();
	if(pid==0)
	{
		FILE* fp=fopen("echomsg.txt","wt");
		char msgbuf[BUF_SIZE];
		int i, len;
		
		for(i=0;i<10;i++)
		{
			len=read(fds[0], msgbuf, BUF_SIZE);
			fwrite((void*)msgbuf, 1, len, fp);
		}
		fclose(fp);
		return 0;
	}
	
	while(1)
	{
		adr_sz=sizeof(clnt_adr);
		clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz); //클라이언트의 연결 요청을 수락.
		if(clnt_sock==-1)
			continue;
		else
			puts("new client connected...");
		pid=fork(); // 자식 프로세스 생성, 위의 accpeet함수를 통해 생성된 파일 디스크립터를 자식 프로세스도 가짐.
			
		
		if(pid==0) //자식 프로세스만 실행되는 부분.
		{
			close(serv_sock);
			while((str_len=read(clnt_sock,buf,BUF_SIZE))!=0)
			{
				write(clnt_sock, buf, str_len);
				write(fds[1],buf, str_len);
			}
			
			
			close(clnt_sock);
			puts("client disconnected...");
			return 0;
		}
		else
			close(clnt_sock); // 소켓에 존재하는 모든 파일 디스크립터가 종료(소멸)되어야 소켓이 소멸되기 때문에 이제 서로에게 상관없는 파일 디스크립터를 닫아야 한다.

	}
	close(serv_sock);
	return 0;
}

void read_childproc(int sig)
{
	pid_t pid;
	int status;
	pid=waitpid(-1, &status, WNOHANG);
	printf("removed proc id : %d \n",pid);
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n',stderr);
	exit(1);
}

