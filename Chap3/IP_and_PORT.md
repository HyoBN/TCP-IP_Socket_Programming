## 소켓에 할당되는 IP주소와 PORT번호

> **IP주소** : Internet Protocol의 약자로, 인터넷상에서 데이터를 송수신할 목적으로 PC에 부여하는 값.
> **PORT번호** : PC가 아닌 프로그램상에서 생성되는 소켓을 구분하기 위해 소켓에 부여되는 번호.

PORT 번호는 하나의 운영체제 내에서 소켓을 구분하는 목적으로 사용되기 때문에 하나의 운영체제 내에서 동일한 PORT번호를 둘 이상의 소켓에 할당할 수 없다.
PORT 번호는 16비트로 표현된다. -> 범위 : 0 ~ 65535 / 0~1023번은 잘 알려진 PORT(Well known PORT)라 하여 이미 예약된 번호로 사용이 불가능하다.
TCP소켓과 UDP소켓은 PORT번호를 공유하지 않기 때문에 중복되어도 상관없다.


### 바이트 순서(Order)와 네트워크 바이트 순서.

CPU가 데이터를 메모리에 저장하는 방식은 다음 두 가지로나뉜다.

* 빅 엔디안(Big Endian) : 상위 바이트의 값을 작은 번지수에 저장하는 방식.
* 리틀 엔디안(Little Endian) : 상위 바이트의 값을 큰 번지수에 저장하는 방식.

즉, CPU에 따라서 데이터가 저장되는 순서(호스트 바이트 순서)가 다를 수 있다. 따라서 호스트 바이트 순서가 다른 CPU가 데이터를 주고받을 때 문제가 발생하게 된다.

ex) PC_A에서 0x1234를 전송하였는데 PC_B에서 0x1234로 수신하였지만 0x3412로 해석하는 경우가 발생할 수 있다.

이러한 문제점을 해결하기 위해 네트워크를 통해서 데이터를 전송할 때에는 통일된 기준으로 데이터를 전송하기로 약속하였으며, 이 약속을 네트워크 바이트 순서(Network Byte Order)라고 한다.
이 약속은 한 마디로 '빅 엔디안 방식으로 통일하자!' 이다. 즉, 네트워크상으로 데이터를 전송할 때에는 데이터의 배열을 빅 엔디안 기준으로 변경하여 송수신하기로 약속한다.

#### 바이트 순서의 변환(Endian Conversions)
바이트 순서의 변환을 돕는 함수.
```c
unsigned short htons(unsigned short);
unsigned short ntohs(unsigned short);
unsigned long htonl(unsigned long);
unsigned long ntohl(unsigned long);
```
htons에서 h : host 바이트 순서, n : network 바이트 순서, s : short(자료형) 을 의미한다!
해석하면, short형 데이터를 호스트 바이트 순서에서 네트워크 바이트 순서로 변환하라! 이다.

htonl에서 l은 long(자료형) 의미.  * 리눅스에서 long형은 4byte(window에서는 8byte)

> 인텔, AMD 계열의 CPU는 모두 리틀 엔디안을 기준으로 정렬한다!

### 인터넷 주소의 초기화!

인터넷 주소정보의 초기화 방법.

```c
struct sockaddr_in addr; 
char *serv_ip = "211.217.168.13";  // IP주소 문자열 선언.
char *serv_port="9190";  // PORT번호 문자열 선언.
memset(&addr, 0, sizeof(addr));  // 구조체 변수 addr의 모든 멤버 0으로 초기화.	
addr.sin_family=AF_INET;  //주소체계 지정.
addr.sin_addr.s_addr=inet_addr(serv_ip); // 문자열 기반의 IP주소 초기화. 
addr.sin_port=htons(atoi(serv_port));  // 문자열 기반의 PORT번호 초기화.
```
> **IP주소를 초기화할 때, INADDR_ANY 상수 활용 가능.**

>> 소켓이 동작하는 컴퓨터의 IP주소가 자동으로 할당되기 때문에 IP주소를 직접 입력해줄 필요가 없고,

>> 할당 받은 여러 개의 IP중 어떤 주소를 통해서 데이터가 들어오던 PORT번호만 일치하면 수신할 수 있다.

>>> 따라서 서버프로그램의 구현에서 많이 선호되는 방법이다.




