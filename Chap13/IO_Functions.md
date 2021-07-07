# 다양한 입출력 함수들

## send & recv 입출력 함수.

```c 
#include <sys/socket.h>

ssize_t send(int sockfd, const void* buf, size_t nbytes, int flags);
// 성공 시 전송된 바이트 수, 실패 시 -1 반환.

```

- send 함수의 매개변수들.
	- sockfd : 데이터 전송 대상과의 연결을 의미하는 소켓의 파일 디스크립터 전달.
	- buf : 전송할 데이터를 저장하고 있는 버퍼의 주소 값 전달.
	- nbytes: 전송할 바이트 수 전달.
	- flags : 데이터 전송 시 적용할 다양한 옵션 정보 전달.

```c 
#include <sys/socket.h>

ssize_t recv(int sockfd, const void* buf, size_t nbytes, int flags);
// 성공 시 수신한 바이트 수(EOF 전송 시 0), 실패 시 -1 반환.

```

- recv 함수의 매개변수들.
	- sockfd : 데이터 수신 대상과의 연결을 의미하는 소켓의 파일 디스크립터 전달.
	- buf : 수신된 데이터를 저장하고 있는 버퍼의 주소 값 전달.
	- nbytes: 수신할 수 있는 최대 바이트 수 전달.
	- flags : 데이터 수신 시 적용할 다양한 옵션 정보 전달.

#### flags 변수로 전달하는 옵션의 종류와 의미.
1. MSG_OOB : 긴급 데이터(Out-of-band data)의 전송을 위한 옵션.
	- 긴급으로 전송해야 할 메시지가 있을 때 메시지의 전송방법 및 경로를 달리할 때 사용.
	- 사실 MSG_OOB 옵션을 추가한다고 해서 데이터가 빨리 전송되는 게 아니라 '전혀 다른 통신 경로로 전송되는 데이터'라는 의미이다.
	- TCP에서는 별도의 통신 경로를 제공하지 않고 Urgent mode라는 것을 이용하여 데이터를 전송할 뿐이다.
		: Urgent mode : URG Pointer를 통해 패킷에서 긴급 메시지의 위치를 상대에게 전달한다.
2. MSG_PEEK : 입력 버퍼에 수신된 데이터의 존재유무 확인을 위한 옵션. 
3. MSG_DONTROUTE : 데이터 전송과정에서 라우팅 테이블을 참조하지 않을 것을 요구하는 옵션. 로컬 네트워크에서 사용됨.
4. MSG_DONTWAIT : 입출력 함수 호출과정에서 블로킹 되지 않을 것을 요규하는 옵션.
5. MSG_WAITALL : 요청한 바이트 수에 해당하는 데이터가 전부 수신될 때까지 호출된 함수가 반환되는 것을 막기 위한 옵션.


## readv & writev 입출력 함수.

 : 데이터를 모아서 전송하고, 모아서 수신하는 기능의 함수.

### writev 함수.

```c
#include <sys/uio.h>

ssize_t writev(int filedes, const struct iovec* iov, int iovcnt);
// 성공시 전송된 바이트 수, 실패 시 -1 반환
```

- writev의 매개변수들.
	- filedes : 데이터 전송의 목적지를 나타내는 소켓의 파일 디스크립터 전달.
	- iov : 구조체 iovec 배열의 주소 값 전달
	- iovcnt : 두 번째 인자로 전달된 주소 값이 가리키는 배열의 길이정보 전달.

 - iovec 구조체의 정의.
```c
struct iovec
{
	void* iov_base; // 버퍼의 주소 정보
	size_t iov_len; // 버퍼의 크기 정보
}
```

### readv 함수

```c
#include <sys/uio.h>

ssize_t readv(int filedes, const struct iovec* iov, int iovcnt);
// 성공시 수신된 바이트 수, 실패 시 -1 반환
```

- writev의 매개변수들.
	- filedes : 데이터 수신할 파일 혹은 소켓의 파일 디스크립터 전달.
	- iov : 데이터를 저장할 위치와 크기 정보를 담고 있는 구조체 iovec 배열의 주소 값 전달
	- iovcnt : 두 번째 인자로 전달된 주소 값이 가리키는 배열의 길이정보 전달.


### readv & writev 함수의 적절한 사용.

- 예를 들어, 전송할 데이터가 여러 개의 버퍼에 나뉘어 있는 경우, write 함수를 여러 번 호출하는 것보다 writev 함수를 딱 한 번 호출하는 것이 효율적임.

- 한 번에 모든 데이터를 출력버퍼로 밀어 넣기 때문에 하나의 퍀심나 생성되어 전송될 확률이 높다.