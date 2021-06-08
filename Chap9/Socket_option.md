# 소켓의 다양한 옵션

## 소켓의 옵션과 입출력 버퍼의 크기

### 소켓의 다양한 옵션
지금까지는 소켓을 생성하여 기본으로 설정된 특성을 바탕으로 데이터 송수신을 했다.  
하지만 소켓의 특성을 변경시켜야만 하는 경우도 흔히 발생하기 때문에 소켓의 다양한 옵션을 알아야 한다.  

먼저, 소켓의 옵션은 계층별로 분류된다.
1. IPPROTO_IP 레벨 : IP 프로토콜에 관련된 사항들이며,
2. IPPROTO_TCP 레벨 : TCP 프로토콜에 관련된 사항들이고,
3. SOL_SOCKET : 소켓에 대한 가장 일반적인 옵션들이다.

#### 소켓의 옵션을 확인, 변경할 때 호출하는 함수.

##### < 소켓의 옵션을 확인할 때 호출하는 함수, getsockopt >
```c
#include <sys/socket.h>

int getsockopt(int sock, int level, int optname, void *optval, socklen_t *optlen);
//성공 시 0, 실패 시 -1 반환.
```
- sock : 옵션 확인을 위한 소켓의 파일 디스크립터 전달.
- level : 확인할 옵션의 프로토콜 레벨 전달.
- optname : 확인할 옵션의 이름 전달.
- optval : 확인 결과의 저장을 위한 버퍼의 주소 값 전달.
- optlen : optval로 전달된 주소 값의 버퍼 크리를 담고 있는 변수의 주소 값 전달. 

소켓의 타입정보 확인을 위한 옵션 SO_TYPE은 확인만 가능하고 변경이 불가능한 대표적인 옵션이다.  

즉,  
###### '소켓의 타입은 소켓 생성시 한 번 결정되면 변경이 불가능하다!'

> 예제 : 'sock_type.c' 파일  

##### < 소켓의 옵션을 변경할 때 호출하는 함수, setsockopt >

```c
#include <sys/socket.h>

int setsockopt(int sock, int level, int optname, const void *optval, socklen_t optlen);
// 성공 시 0, 실패 시 -1 반환.
```

- sock : 옵션변경을 위한 소켓의 파일 디스크립터 전달.
- level : 변경할 옵션의 프로토콜 레벨 전달.
- optname : 변경할 옵션의 이름 전달.
- optval : 변경할 옵션정보를 저장한 버퍼의 주소 값 전달.
- optlen : optval로 전달된 옵션정보의 바이트 단위 크기 전달.


#### SO_SNDBUF & SO_RCVBUF

입출력버퍼와 관련 있는 소켓 옵션이다.
SO_SNDBUF는 출력버퍼의 크기와 관련된 옵션이고,  
SO_RCVBUF는 입력버퍼의 크기와 관련된 옵션이다.  
즉, 이 두 옵션을 이용해서 입출력 버퍼의 크기를 참조할 수 있을 뿐만 아니라, 변경도 가능하다.








