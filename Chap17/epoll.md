# select보다 나은 epoll

IO 멀티플렉싱을 구현하는 전통적인 방법에 select와 poll이 있고 이들의 단점을 보완하기 위해 epoll 등이 등장하였다.

### select기반의 IO 멀티플렉싱이 느린 이유.
1. select 함수 호출 이후, 항상 모든 파일 디스크립터를 대상으로 반복문을 수행해야 한다.
2. select 함수를 호출할 때마다 관찰대상에 대한 정보들을 매번 전달해야 한다.
 - 관찰대상에 대한 정보를 매번 운영체제에게 전달하는 것이 프로그램에 큰 부담을 준다.

##### select 함수의 가장 큰 단점은 절대적으로 운영체제에 의해 기능이 완성되는 함수라는 것이다. 

## select 함수를 쓸 만한 상황.
1. 서버의 접속자 수가 많지 않은 경우.
2. 다양한 운영체제에서 운영이 가능해야 하는 경우.
	- epoll 방식은 리눅스에서만 지원되는 방식인 반면, select는 대부분의 운영체제에서 지원한다.


## epoll 구현에 필요한 함수와 구조체.

### 함수 종류.
1. epoll_create : epoll 파일 디스크립터 저장소 생성.
2. epoll_ctl : 저장소에 파일 디스크립터 등록 및 삭제.
3. epoll_wait : select 함수와 마찬가지로 파일 디스크립터의 변화 대기.


epoll_event 구조체를 기반으로 상태변화(이벤트)가 발생한 파일 디스크립터가 별도로 묶인다.

```c
struct epoll_event
{
	__uint32_t events;
	epoll_data_t data;
}
	typedef union epoll_data
	{
		void *ptr;
		int fd;
		__uint32_t u32;
		__uint64_t u64;
	}epoll_data_t;
	
```

### epoll_create 함수.

```c
#include <sys/epoll.h>

int epoll_create(int size); // size : epoll 인스턴스의 크기정보.
// 성공 시 epoll 파일 디스크립터, 실패 시 -1 반환.
```

- epoll 인스턴스 : epoll_create 함수호출 시 생성되는 파일 디스크립터의 저장소.
- 리눅스 커널 2.6.8 이후부터는 커널 내에서 상황에 맞게 알아서 조정하기 때문에 매개변수 size를 완전히 무시한다. 

### epoll_ctl 함수.

```c 
#include <sys/epoll.h>

int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
// 성공 시 0, 실패 시 -1 반환.
```

- epfd : 관찰대상을 등록할 epoll 인스턴스의 파일 디스크립터.
- op : 관찰대상의 추가, 삭제 또는 변경 여부 지정.
	- EPOLL_CTL_ADD : 파일 디스크립터를 epoll 인스턴스에 등록한다.
	- EPOLL_CTL_DEL : 파일 디스크립터를 epoll 인스턴스에서 삭제한다.
	- EPOLL_CTL_MOD : 등록된 파일 디스크립터의 이벤트 발생상황을 변경한다.
	
- fd : 등록할 관찰대상의 파일 디스크립터.
- event : 관찰대상의 관찰 이벤트 유형.
	- EPOLLIN : 수신할 데이터가 존재하는 상황.
	- EPOLLOUT : 출력버퍼가 비워져서 당장 데이터를 전송할 수 있는 상황.
	- EPOLLPRI : OOB 데이터가 수신된 상황.
	- EPOLLRDHUP : 연결 종료 or Half-close가 진행된 상황.
	- EPOLLERR : 에러가 발생한 상황.
	- EPOLLLET : 이벤트의 감지를 엣지 트리거 방식으로 동작시킨다.
	- EPOLLONESHOT : 이벤트가 한번 감지되면, 해당 FD는 더 이상 이벤트를 발생시키지 않는다.

### epoll_wait 함수.

```c
#include <sys/epoll.h>

int epoll_wait(int epfd, struct epoll_event* events, int maxevents, int timeout);
// 성공 시 이벤트가 발생한 FD의 수, 실패 시 -1 반환.
```

- epfd : 이벤트 발생의 관찰영역인 epoll 인스턴스의 파일 디스크립터.
- events : 이벤트가 발생한 파일 디스크립터가 채워질 버퍼의 주소 값.
- maxevents : 두 번째 인자로 전달된 주소 값의 버퍼에 등록 가능한 최대 이벤트 수.
- timeout : 1/1000초 단위의 대기시간, -1 전달 시 이벤트가 발생할 때까지 무한 대기.


## 레벨 트리거(Level Trigger)와 엣지 트리거(Edge Trigger).

시스템에서 특정 이벤트를 감지하기 위해 트리거(Trigger)라는 개념을 사용한다.  
 - 논리 회로에서 배웠던 Timing Diagram을 통해 쉽게 이해할 수 있다.
 
 - 레벨 트리거 : 입력버퍼에 데이터가 남아있는 동안 계속해서 이벤트가 등록된다.
 	- 설명 사진)  
	![lt](https://user-images.githubusercontent.com/50162252/125166525-04d1ab00-e1d7-11eb-957e-4f13e259de59.PNG)  
	- 1인 구간에서는 레벨 트리거를 몇 번이고 수행해도 그때 마다 이벤트가 발생한다.
	
 - 엣지 트리거 : 입력버퍼로 데이터가 수신된 상황에서 딱 한 번만 이벤트가 등록된다.
 	- 설명 사진)
	![et](https://user-images.githubusercontent.com/50162252/125166618-74479a80-e1d7-11eb-9062-0139182c1ecb.PNG)  
	- 0에서 1로 바뀌는 순간(상승 에지 트리거)에만 이벤트를 발생시킨다.
※ select 모델은 레벨 트리거 방식으로 동작한다.

##### 레벨 트리거와 다르게 엣지 트리거는 데이터의 수신과 데이터가 처리되는 시점을 분리할 수 있다.

