# DNS(Domain Name System)

## DNS 서버

- 우리는 인터넷 사이트에 접속할 때 IP주소가 아닌 도메인 이름을 입력한다.  
- 도메인 이름은 실제 주소가 아닌 가상의 주소이기에 가상의 주소를 실제주소로 변환하는 과정이 필요하다.  
- 이러한 변환을 담당하는 것이 DNS 서버이다.  
- 모든 컴퓨터에는 디폴트 DNS 서버 주소가 등록되어 있다. 이를 통해 해당 주소로 접근할 수 있는 것이다.  
- 디폴트 DNS 서버가 모든 도메인의 IP주소를 알고 있지는 않지만 더 상위 DNS 서버에 요청하여 알아낸다.  


### Domain name이 필요한 이유.

- 12자리나 되는 IP주소를 외워서 입력하는 것보다 영어로 된 주소를 입력하는 것이 사용자가 사용하기에 더 편리한 이유도 있지만,  

- 시스템적인 이유로 IP주소 변경은 언제든지 발생할 수 있다. 반면, 도메인 이름은 한 번 등록하고 나면 평생 유지가 가능하다.  
- 도메인 이름을 사용하면 도메인 이름을 근거로 IP주소를 얻어온 다음에 서버에 접속하므로 클라이언트는 IP주소로부터 자유롭다.  

##### 따라서 도메인 이름을 사용하는 것이 프로그램상으로도 유리하다.

-----------

### - 도메인 이름을 이용해서 IP주소 얻어오기.

```c
#include <netdb.h>

struct hostent * gethostbyname(const char* hostname);

// 호출 성공 시 hostent 구조체 변수의 주소 값, 실패 시 NULL 포인터 반환.  
```

### - IP주소를 이용해서 도메인 정보 얻어오기.

```c
#include <netdb.h>
struct hostent* gethostbyaddr(const char* addr, socklen_t len, int family);
// 호출 성공 시 hostent 구조체 변수의 주소 값, 실패시 NULL 포인터 반환.
```
- addr : IP주소를 지니는 in_addr 구조체 변수의 포인터 전달,  IPv4 이외의 다양한 정보를 전달받을 수 있도록 일반화하기 위해 매개변수를 char형 포인터로 선언.
- len : 첫 번째 인자로 전달된 주소정보의 길이 전달.
- family : 주소체계 정보 전달.

### - hostent 구조체
```c
struct hostent
{
	char* h_name;
	char** h_aliases;
	int h_addrtype;
	int h_length;
	char** h_addr_list;
}
```

##### 구조체의 멤버 변수
- h_name : 공식 도메인 이름(Official domain name)이 저장됨. 해당 홈페이지를 대표하는 도메인 이름이다.
- h_aliases : 공식 도메인 이름 이외에 접속할 수 있는 또 다른 도메인 이름이 저장됨.
- h_addrtype : IP주소의 주소체계에 대한 정보를 저장. IPv4의 경우 AF_INET이 저장됨.
- h_length : 함수호출의 결과로 반환된 IP주소의 크기 정보가 담긴다. IPv4의 경우 4바이트이므로 4가 저장됨.
- h_addr_list : 가장 중요한 변수인데, 이 변수를  통해 도메인 이름에 대한 IP주소가 정수의 형태로 반환된다.