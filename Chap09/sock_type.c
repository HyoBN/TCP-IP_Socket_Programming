#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>

void error_handling(char *message);

int main(int argc, char *argv[])
{
	int tcp_sock, udp_sock;
	int sock_type;
	socklen_t optlen;
	int state;
	
	optlen=sizeof(sock_type);
	tcp_sock=socket(PF_INET, SOCK_STREAM, 0);
	udp_sock=socket(PF_INET, SOCK_DGRAM, 0);
	printf("SOCK_STREAM : %d \n",SOCK_STREAM); ; // 소켓 생성시 전달하는 인자(SOCK_STREA,SOCK_DGRAM)의 상수 값 출력.
	printf("SOCK_DGRAM : %d \n",SOCK_DGRAM); // 소켓 생성시 전달하는 인자(SOCK_STREA,SOCK_DGRAM)의 상수 값 출력.
	
	state=getsockopt(tcp_sock, SOL_SOCKET, SO_TYPE, (void*)&sock_type, &optlen); //getsockopt 함수 호출을 통해 확인한 소켓의 확인결과를 4번째 매개변수인 sock_type에 저장함.
	if(state)
		error_handling("getsockopt() error!");
	printf("Socket type one : %d\n", sock_type); // 소켓의 타입정보를 출력. tcp_sock이 TCP 소켓이었으므로 getsockopt함수호출을 통해 sock_type에 SOCK_STREAM의 상수 값인 1을 출력.
	
	state=getsockopt(udp_sock,SOL_SOCKET,SO_TYPE,(void*)&sock_type, &optlen); //위와 동일.
	if(state)
		error_handling("getsockopt() error!");
	printf("Socket type two : %d \n",sock_type); //UDP 소켓이므로 SOCK_DGRAM의 상수 값인 2 출력.
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n',stderr);
	exit(1);
}