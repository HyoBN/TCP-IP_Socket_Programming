#include <stdio.h>
#include <arpa/inet.h>

int main(int argc, char* argv[])
{
	unsigned short host_port=0x1234;
	unsigned short net_port;
	unsigned long host_addr=0x12345678;
	unsigned long net_addr;
	
	net_port=htons(host_port);
	net_addr=htonl(host_addr);
	
	printf("Host ordered port : %#x \n",host_port);
	printf("Network ordered port : %#x \n",net_port);
	printf("Host ordered address : %#lx \n",host_addr);
	printf("Network ordered address : %#lx \n", net_addr);
	return 0;
}


/*
실행결과.

Host ordered port : 0x1234
Network ordered port : 0x3412
Host ordered address : 0x12345678
Network ordered address : 0x78563412

실행 결과, 이 CPU는 리틀 엔디안을 기준으로 정렬하는 것을 알 수 있다!
*/
