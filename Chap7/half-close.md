# TCP 기반의 Half-close

## 일방적인 연결종료의 문제점.

> 리눅스의 close 함수호출은 완전종료를 의미한다.


완전종료란, 데이터 전송 뿐만 아니라 수신하는 것 조차 더 이상 불가능한 상황을 의미한다.
따라서, 한쪽에서의 일방적인 close는 경우에 따라 부적절할 수 있다.(책에서는 이를 '우아하지 못하다'고 표현하였다.)

close 함수를 호출하게 되면 송신, 수신 둘 다 불가능해지기 때문에  
A,B 두 소켓이 통신을 할 때,  
A 소켓에서 B 소켓으로 데이터를 송신하고 close함수를 호출한다면 그 이후로 B가 전송하는 데이터를 수신하지 못한다.  
따라서 B가 A에게 전송한 데이터는 소멸되고 만다.  

- 이러한 문제를 해결하기 위해 데이터의 송수신에 사용되는 스트림의 일부만 종료하는 방법이 제공되고 있다.
- 이를 **Half-close**라 하는데, 말 그대로 스트림의 반만 닫아서 송신&수신 중 하나만 가능하게 하는 방법이다.

-----------

## 소켓과 스트림(Stream)

### 스트림이 형성된 상태.
 : 소켓을 통해 두 호스트가 연결되어 상호간에 데이터 송수신이 가능한 상태.
 - 스트림은 일종의 연결통로라 할 수 있다.
 - 스트림은 한쪽 방향으로만 데이터 이동이 가능하다.
 - 따라서 양방향 통신을 위해서는 두 개의 스트림(송신, 수신)이 필요하다.
 - A 소켓의 입력버퍼와 B 소켓의 출력버퍼가 하나의 스트림으로 연결된다.
 - 소켓의 우아한 연결종료란, 이 스트림 중 하나만 끊는 것이다.

## 우아한 종료를 위한 shutdown 함수

```c
#include <sys/socket.h>
int shutdown(int sock, int howto);
```
- sock : 종료할 소켓의 파일 디스크립터 전달.
- howto : 종료방법에 대한 정보 전달.
	* howto 매개변수의 인자 종류.
	 1. SHUT_RD : 입력 스트림 종료.
	 2. SHUT_WR : 출력 스트림 종료.
	 3. SHUT_RDWR 입출력 스트림 종료.
	* SHUT_WR의 경우, 더이상 데이터 전송이 불가능해지지만,
	  출력 버퍼에 아직 전송되지 못한 상태로 남아있는 데이터가 존재하면 해당 데이터는 목적지로 전송된다.
- 성공시 0, 실패시 -1을 반환.


## Half-close가 필요한 이유.

> 충분히 연결을 유지했다가 연결을 종료하면 Half-close가 필요없다!!

서버가 클라이언트에 데이터를 전송하다가 연결 종료 직전에 클라이언트가 서버에게 전송해야 할 데이터가 존재하는 상황에서,  
서버는 계속 파일을 전송하기만 하면 되지만, 클라이언트는 파일의 끝이 어딘지 모르기 때문에  
즉, 언제까지 데이터를 수신해야 할지 알 수 없기 때문에 함부로 연결을 종료할 수 없다.  
그렇다고 해서 계속해서 입력함수를 호출할 수도 없다. 블로킹 상태(호출된 함수가 반환하지 않는 상태)에 빠질 수 있기 때문이다.  

> 서버와 클라이언트 사이에 파일의 끝을 의미하는 문자 하나를 약속하면 된다!!
그 문자와 일치하는 데이터가 파일에 존재할 수 있기 때문에 불가능하다.  

- 클라이언트는 EOF의 수신을 함수의 반환 값을 통해 확인 가능하기 때문에 서버가 EOF를 전달하기만 하면 문제가 해결된다!
- read 함수는 EOF를 만나면 0을 반환한다!! 이를 활용!

- 출력 스트림을 종료하면 상대 호스트로 EOF가 전송된다.
- close 함수호출을 통해 입출력 스트림을 모두 종료해줘도 EOF가 전송되지만, 더이상 데이터를 수신할 수 없게 된다.
### shutdown 함수를 통해 서버의 출력 스트림만 Half-close 한다면  EOF도 전송되고 입력스트림은 살아있어서 데이터 수신도 가능하다.




