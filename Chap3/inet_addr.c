#include <stdio.h>
#include <arpa/inet.h>

int main(int argc, char* argv[])
{
	char *addr1="1.2.3.4";
	char *addr2="1.2.3.256"; //1byte당 표현할 수 있는 최대 크기의 정수는 255이므로 이는 잘못된 IP주소이다.
	
	unsigned long conv_addr=inet_addr(addr1);
	if(conv_addr == INADDR_NONE)
		printf("Error occured! \n");
	else
		printf("Network ordered integer addr : %#lx \n",conv_addr);
	
	conv_addr = inet_addr(addr2);
	if(conv_addr == INADDR_NONE)
		printf("Error occureded \n");
	else
		printf("Network ordered integer addr : %#lx \n",conv_addr);
	return 0;
	
	
}