#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
#define NUM_SIZE 4
void error_handling(char *message);
int calculate(int opnum, int opnds[], char oprator);

int main(int argc, char *argv[])
{
	int serv_sock, clnt_sock;
	char data[BUF_SIZE];
	int num_cnt=0;
	int answer=0;
	int recv_len=0,recv_cnt;
	char message[BUF_SIZE];
	
	struct sockaddr_in serv_adr, clnt_adr;
	socklen_t clnt_adr_sz;
	
	
	if(argc!=2){
		printf("Usage : %s <port>\n",argv[0]);
		exit(1);
	}
	
	serv_sock=socket(PF_INET, SOCK_STREAM, 0);
	if(serv_sock==-1)
		error_handling("socket() error");
	
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_adr.sin_port=htons(atoi(argv[1]));
	
	if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
		error_handling("bind() error!");
	
	if(listen(serv_sock, 5)==-1)
		error_handling("listen() error");
	clnt_adr_sz=sizeof(clnt_adr);
	//~~~~~~~~~~~~~~~~~여기까지는 base code라 문제x.
	
	clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_adr,&clnt_adr_sz);
	read(clnt_sock, &num_cnt, 1); // 연산할 숫자의 개수를 전달받아 num_cnt에 저장.
	
		
	
	while(num_cnt*NUM_SIZE+1 > recv_len) // (연산할 숫자 개수 * 숫자 하나 크기 + 연산자 하나 크기) > recv_len. , 

		// number과 operator를 구분하는 게 아니라 하나의 데이터 블럭에 연속적으로 전달되는 형태에서 read를 끊어서 받는 구조!!
		
	{
		recv_cnt=read(clnt_sock, &data[recv_len], BUF_SIZE-1); // read 함수는 호출성공하면 데이터의 크기(int : 4, char : 1)를 반환한다.
		if(recv_cnt==-1)
			error_handling("read() error");
		recv_len+=recv_cnt;
		
		answer=calculate(num_cnt, (int*)data, data[recv_len-1]);
		write(clnt_sock, (char*)&answer,sizeof(answer));
		close(clnt_sock);
	}
	
}
	

int calculate(int opnum, int opnds[], char op)
{
	int result=opnds[0],i;
	switch(op)
	{
		case '+':
			for(i=1;i<opnum;i++) result +=opnds[i];
			break;
		case '-':
			for(i=1;i<opnum;i++) result -=opnds[i];
			break;
		case '*':
			for(i=1;i<opnum;i++) result *=opnds[i];
			break;

	}
	return result;
}




	
void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n',stderr);
	exit(1);
}
	

	
	
	
	