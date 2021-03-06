# Multicast and Broadcast

## Multicast. 
- 멀티캐스트 방식의 데이터 전송은 UDP를 기반으로 한다.
- 단 한번의 데이터 전송으로 다수의 호스트에게 데이터를 전송할 수 있다.

### 멀티캐스트 데이터 전송특성.

1. 멀티캐스트 서버는 특정 멀티캐스트 그룹에게 데이터를 딱 한번 전송한다.
2. 딱 한번 전송하더라도 그룹에 속하는 클라이언트는 모두 데이터를 수신한다.
3. 멀티캐스트 그룹의 수는 IP주소 범위 내에서 얼마든지 추가 가능하다.
4. 특정 멀티캐스트 그룹으로 전송되는 데이터를 수신하려면 해당 그룹에 가입하면 된다.

### 라우팅과 TTL, 그룹 가입방법.

TTL(Time to Live) : 패킷을 얼마나 멀리 전달할 것인가를 결정하는 요소.
 - 라우터를 하나 거칠 때마다 1씩 감소. 0되면 패킷 소멸.
 - 소켓의 옵션설정을 통해 TTL 설정.
 - TTL을 64로 설정할 때의 코드)

```c
int send_sock;
int time_live= 64;

sned_sock=socket(PIF_INET, SOCK_DGRAM,0);
setsockopt(send_sock, IPPROTO_IP, IPMULTICAST_TTL, (void*)&time_live, sizeof(time_live));
```

---------

멀티캐스트 그룹 가입과 관련된 프로토콜의 레벨 : IPPROTO_IP, 옵션 명 : IP_ADD_MEMBERSHIP.
- 그룹 가입 코드)

```c
int recv_sock;
struct ip_mreq join_adr;

recv_sock=socket(PF_INET, SOCK_DGRAM,0);

join_adr.imr_multiaddrd.s_addr="멀티캐스트 그룹의 주소정보";
join_adr.imr_interface.s_addr="그룹에 가입할 호스트 주소정보";

setsockopt(recv_sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (void*)&join_adr, sizeof(join_adr));
```

- ip_mreq 구조체 정의

```c
struct ip_mreq
{
	struct in_addr imr_multiaddr; // 가입할 그룹의 IP주소.
	struct in_addr imr_interface; // 그룹에 가입하는 소켓이 속한 호스트의 IP주소 명시. INADDR_ANY도 가능.
}
```


-------------
 
## 브로드캐스트(Broadcast)

: 동일한 네트워크에 연결되어 있는 모든 호스트에게 동시에 데이터를 전송하기 위한 방법.

- 멀티캐스트와 마찬가지로 UDP 기반.

#### 데이터 전송 시 사용되는 IP주소의 형태에 따라 두 가지로 구분.
1. Directed 브로드캐스트
	- IP주소의 호스트 주소 부분을 전부 1로 설정하여 브로드캐스트. ex) 26.48.1.255
2. Local 브로드캐스트
	- IP주소 255.255.255.255로 데이터를 전송하면 송신한 호스트의 네트워크 영역의 모든 호스트에게 데이터 전송.

- 사실상, 브로드캐스트와 일반 UDP 전송과 큰 차이가 없다.


setsockopt 함수호출을 통해 SO_BROADCAST의 옵션 정보를 1로 변경해야 해당 소켓의 브로드캐스트 기반의 데이터 전송이 가능하다.

```c 
int bcast=1;
send_sock=socket(PF_INET, SOCK_DGRAM, 0);
setsockopt(send_sock, SOL_SOCKET, SO_BROADCAST, (void*)&bcast, sizeof(bcast));
```








