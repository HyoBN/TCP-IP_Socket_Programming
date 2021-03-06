
## 소켓의 생성.

```c
int socket(int domain, int type, int protocol);
```
-	 **domain** : 소켓이 사용할 프로토콜 체계(Protocol Family) 정보를 전달. 
-	 **type** : 소켓의 데이터 전송방식에 대한 정보 전달.
-	 **protocol** : 두 컴퓨터 간 통신에 사용되는 프로토콜 정보 전달.

------

## domain : 프로토콜 체계.

### 프로토콜 체계의 종류.

- PF_INET : IPv4 인터넷 프로토콜 체계.
- PF_INET6 : IPv6 인터넷 프로토콜 체계.
- PF_LOCAL :로컬 통신을 위한 UNIX 프로토콜 체계.
- PF_PACKET Low Level 소켓을 위한 프로토콜 체계.
- PF_IPX : IPX 노벨 프로토콜 체계.

**주로 PF_INET(IPv4 인터넷  프로토콜 체계)를 사용.**



## type : 소켓의 타입.



### 1. 연결지향형 소켓(SOCK_STREAM) - TCP.

> socket 함수의 두 번째 인자로 SOCK_STREAM을 전달하면 연결지향형 소켓이 생성된다.


**연결지향형 소켓의 데이터 송수신 방식 특징**
1. 중간에 데이터가 소멸되지 않고 목적지로 전송된다. 즉, 에러나 데이터의 손실 없이 전달.
2. 전송하는 순서대로 데이터가 수신된다.
3. 전송되는 데이터의 경계(Boundary)가 존재하지 않는다. - 데이터를 전송할 때 데이터를 몇 번 write하더라도 한 번의 read로 전부 읽어들일 수 있다. 이를 경계가 존재하지 않는다고 표현한다.
> 이를 공장의 컨베이어 벨트에 비유할 수 있다.



### 2. 비 연결지향형 소켓(SOCK_DGRAM) - UDP.
> socket 함수의 두 번째 인자로 SOCK_DGRAM을 전달하면 비 연결지향형 소켓이 생성된다.


**비 연결지향형 소켓의 데이터 송수신 방식 특징**
1. 전송된 순서에 상관없이 가장 빠른 전송을 지향한다.
2. 전송된 데이터는 손실의 우려가 있고 파손의 우려가 있다.
3. 전송되는 데이터의 경계(Boundary)가 존재한다. - 데이터를 전송할 때 write 함수가 2번 호출되면 데이터를 수신할 때에도 read 함수가 두 번 호출되어야 한다는 것이다.
4. 한 번에 전송할 수 있는 데이터의 크기가 제한된다.
> 이를 퀵 배달 서비스에 비유할 수 있다.



## protocol : 프로토콜 선택.
> domain, type 두 매개변수 만으로도 소켓 생성하는데 충분하지만 하나의 프로토콜 체계 안에 데이터의 전송방식이 동일한 프로토콜이 둘 이상 존재하는 경우, 
> 그 안에서 프로토콜이 나뉘는 상황에서 프로토콜을 구체화하기 위해 필요하다.
