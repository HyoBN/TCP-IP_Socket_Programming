# 소켓의 옵션과 입출력 버퍼의 크기

## 소켓의 다양한 옵션
지금까지는 소켓을 생성하여 기본으로 설정된 특성을 바탕으로 데이터 송수신을 했다.  
하지만 소켓의 특성을 변경시켜야만 하는 경우도 흔히 발생하기 때문에 소켓의 다양한 옵션을 알아야 한다.  

먼저, 소켓의 옵션은 계층별로 분류된다.
1. IPPROTO_IP 레벨 : IP 프로토콜에 관련된 사항들이며,
2. IPPROTO_TCP 레벨 : TCP 프로토콜에 관련된 사항들이고,
3. SOL_SOCKET : 소켓에 대한 가장 일반적인 옵션들이다.

---------------

### 소켓의 옵션을 확인, 변경할 때 호출하는 함수.

#### < 소켓의 옵션을 확인할 때 호출하는 함수, getsockopt >
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
##### '소켓의 타입은 소켓 생성시 한 번 결정되면 변경이 불가능하다!'

> 예제 : 'sock_type.c' 파일  

#### < 소켓의 옵션을 변경할 때 호출하는 함수, setsockopt >

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

-------------

### SO_SNDBUF & SO_RCVBUF

입출력버퍼와 관련 있는 소켓 옵션이다.
SO_SNDBUF는 출력버퍼의 크기와 관련된 옵션이고,  
SO_RCVBUF는 입력버퍼의 크기와 관련된 옵션이다.  
즉, 이 두 옵션을 이용해서 입출력 버퍼의 크기를 참조할 수 있을 뿐만 아니라, 변경도 가능하다.

> 예제 : 'get_buf.c', 'set_buf.c'

#### get_buf.c 예제 실행 결과.

구름IDE에서 돌려본 결과,
input buffer size : 87380
output buffer size : 87380
이 나옴.

책 결과값으로는 87380, 16384가 나와야하는데 왜 input, output buffer 사이즈가 같게 나올까?  
단순 시스템 차이인지 구름ide에서 이유가 있어서 그렇게 설정한 것인지 또, 시스템별로 어느정도의 차이가 있는지 궁금해졌다.  

구글링을 해보니 터미널 창에서,  
> sysctl -a | grep mem  
정상적인 실행 결과 : https://rocksea.tistory.com/64  
입력하면 각 버퍼의 기존값을 볼 수 있다길래 실행시켜보니  
reading key "kernel.unprivileged_userns_apparmor_policy"  
reading key "net.ipv6.conf.all.stable_secret"  
이런식으로 뜨고 버퍼 크기를 알려주지 않음. 왜 알려주지 않는 이유에 대해서 구름에 문의 해놓았으니 답변오면 올려야지.

Goorm forum에 문의한 결과)

![cap](https://user-images.githubusercontent.com/50162252/122038455-920c2480-ce10-11eb-87a1-7b415c5ef963.PNG)

음.... 그럼 도커 컨테이너는 왜 버퍼 크기를 못 보게 하는 걸까  

웹 컴파일러에서 실행해본 결과는(두 가지 컴파일러로 돌려봄),  
input buffer size : 131072  
output buffer size : 16384  
가 나옴.  이 두 수는 모두 2의 배수이다. 87380은 어디서 나온 숫자일까?
이건 window에서 실행한 거라 책과는 다른 결과가 나온 것으로 생각됨.  
시스템마다 버퍼의 크기는 다르기 때문!



일단 구름IDE가 아닌 기존 리눅스 환경에서는 값이 책과 동일하게 나오는지 확인해보고,  
이 상관관계에 대해서 더 찾아볼 예정.

-------------

### SO_RUSEADDR

#### Time-wait란?

예제) reuseadr_eserver.c 파일을 Chap4의 echo_client.c 파일과 같이 실행.    

- 서버를 먼저 종료하고 재실행하면 bind error가 발생하고 약 3분 기다려야 정상적인 실행이 가능하다.  
- 호스트 간 연결 해제 과정인 Four-way handshaking 이후에 소켓이 바로 소멸되지 않고 Time-wait 상태를 일정시간 거치는 것!(먼저 연결 종료를 요청한 호스트만)
- Time-wait 상태에 있는 동안에는 해당 소켓의 PORT번호가 사용중인 상태가 된다  
※ 서버, 클라이언트 소켓 구분없이 모두 Time-wait를 거침. 하지만 클라이언트 소켓은 프로그램이 실행될 때마다 PORT번호가 유동적으로 할당되기 때문에 Time-wait를 신경 쓸 필요 없음.

##### Time-wait가 존재하는 이유
1. 호스트간 통신 중 호스트 A가 메시지 전송 후 소켓을 바로 소멸시켰는데 ACK 메시지가 B에게 전달되지 못하고 중간에 소멸되었다고 가정,    
2. 호스트 B는 A로부터 메시지를 전달받지 못하고 자신이 보낸 메시지도 A에게 전송되지 못했다고 생각하고 재 전송을 시도,  
3. 하지만 A의 소켓은 완전히 종료되었기 때문에 영원히 메시지를 보내지 못한다.  
- A의 소켓이 Time-wait 상태라면, B에게 전송 실패한 메시지를 재전송하고 B가 정상적으로 종료될 수 있게됨.

##### Time-wait의 단점.
- 서버가 갑작스럽게 종료된 경우, 재빨리 서버를 재 가동시켜야 하는데 Time-wait 상태가 된다면 몇 분을 기다려야 한다.  
 또한 Time-wait 상태는 상황에 따라 더 길어질 수 있다.
 호스트 간 통신 중 마지막 데이터가 손실되면, FIN 메시지를 재전송하게 되고 time-wait 타이머를 재가동하게 된다.  

<해결책>
- SO_REUSEADDR 옵션의 상태를 변경하면 된다!  
SO_REUSEADDR 의 디폴트 값은 0(FALSE)인데,  
이를 1(TRUE)로 변경하면 Time-wait 상태의 소켓의 PORT번호가 할당가능한 상태가 된다.
```c
optlen=sizeof(option);
option=TRUE:
setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, (void*)&option, optlen);
```




