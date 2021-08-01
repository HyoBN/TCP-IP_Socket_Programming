# 쓰레드의 이론적 이해.

### 멀티프로세스 기반의 단점.
1. 프로세스 생성이라는 부담스러운 작업과정을 거친다.
2. 두 프로세스 사이 데이터 교환을 위해서는 별도의 IPC 기법을 적용해야한다.
3. 초당 수천 번까지 일어나는 컨텍스트 스위칭에 따른 부담이 크다.

### 멀티쓰레드의 장점.
1. 쓰레드의 생성 및 컨텍스트 스위칭은 프로세스의 생성 및 컨텍스트 스위칭보다 빠르다.
2. 쓰레드 사이 데이터 교환에는 특별한 기법이 필요하지 않다.
- 즉, 멀티프로세스의 여러 단점을 최소화시킬 수 있다.

프로세스 : 운영체제 관점에서 별도의 실행흐름을 구성하는 단위.  
쓰레드 : 프로세스 관점에서 별도의 실행흐름을 구성하는 단위.  

즉, 한 프로세스 내에서 여러 쓰레드가 실행가능하며, 이들은 동일한 데이터, 힙 영역을 가지기 때문에  
별도의 방법을 사용하지 않고 이를 통해서 쓰레드 간 통신이 가능하다.  


- 쓰레드 생성 함수(pthread_create()) 예제) thread1.c 
	- gcc thread1.c -o tr1 -lpthread 로 실행하여야 한다.
	- -lpthread 옵션을 추가하여 쓰레드 라이브러리의 링크를 별도로 지시해야 pthread.h에 선언된 함수들을 호출할 수 있다.

##### 쓰레드의 실행 흐름을 조절하는 함수.

```c
#include <pthread.h>

int pthread_join(pthread_t thread, void** status);
// 성공 시 0, 실패 시 0 이외의 값 반환.

```

- thread : 이 매개변수에 전달되는 ID의 쓰레드가 종료될 때까지 함수는 반환하지 않는다.
- status : 쓰레드의 main 함수가 반환하는 값이 저장될 포인터 변수의 주소 값을 전달한다.

즉, pthread_join 함수는 첫 번째 인자로 전달되는 ID의 쓰레드가 종료될 때까지 이 함수를 호출한 프로세스 혹은 쓰레드를 대기 상태에 둔다.  
또한, 쓰레드의 main 함수가 반환하는 값까지 얻을 수 있다.(status 변수)  

------------

- 여러 개의 쓰레드를 생성하여 동시에 호출하면 문제를 일으키는 함수가 존재한다.  
- 임계영역(Critical Section) : 둘 이상의 쓰레드가 동시에 실행하면 문제를 일으키는 코드블록.
- 쓰레드에 안전한 함수(Thread-safe function) : 다수의 쓰레드에 의해 동시 호출 및 실행되어도 문제를 일으키지 않는 함수. 반대: Thread-unsafe function.

일반적으로 쓰레드에 안전한 형태로 재 구현된 함수의 이름 뒤에는 _ r이 붙는다.  
또한 헤어파일 선언 이전에 매크로 _ REENTRANT를 정의하면 자동으로 _ r을 붙여주는 효과를 얻을 수 있다.  
컴파일 시, -D_REENTRANT 옵션을 추가하는 방식으로도 매크로롤 정의할 수 있다. ex) gcc -D_REENTRANT thread2.c -o -lpthread  


## 쓰레드의 문제점과 임계영역(Critical Section).

동기화(Synchronization) : 여러 쓰레드가 한 공유 자원에 접근하려고 할 때 공유하는 자원의 일관성을 유지시키는 것.  


임계영역(Critical Section) : 함수 내에 둘 이상의 쓰레드가 동시에 실행하면 문제를 일으키는 하나 이상의 문장으로 묶여있는 코드블럭.  


## 쓰레드 동기화(Synchronization).

### 쓰레드의 두 가지 측면.
1. 동일한 메모리 영역으로의 동시접근이 발생하는 상황.
2. 동일한 메모리 영역에 접근하는 쓰레드의 실행순서를 지정해야 하는 상황.


### 뮤텍스(Mutex).
 : Mutual Exclusion의 줄임말로써 상호배제 즉, 쓰레드의 동시접근을 허용하지 않는다는 의미이다.

#### 뮤텍스 생성 및 소멸 함수.
```c
#include <pthread.h>

int pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr);
int pthread_mutex_destroy(pthread_mutex_t *mutex);
// 성공 시 0, 실패 시 0 이외의 값 반환.
```

 - mutex : 뮤텍스 (생성, 소멸)시, ( , 소멸하고자 하는) 뮤텍스의 참조 값 저장을 위한 변수의 주소 값 전달.
 - attr : 생성하는 뮤텍스의 특성정보를 담고 있는 변수의 주소 값 전달.
 
 
#### 임계영역에 접근 가능 여부 설정하는 함수.

```c
#include <pthread.h>

int pthread_mutex_lock(pthread_mutex_t *mutex);
int pthread_mutex_unlock(pthread_mutex_t *mutex);
// 성공 시 0, 실패 시 0 이외의 값 반환.
```

- 사용 방법)

```c
pthread_mutex_lock(&mutex);
// 임계영역
pthread_mutex_unlock(&mutex);
```

데드락(Dead-lock) 상태 : 임계영역을 빠져나가는 이전 쓰레드가 unlock 함수를 호출하지 않아서  
다음 쓰레드가 임계영역으로 진입하기 위한 pthread_mutex_lock 함수가 블로킹 상태에서 빠져나가지 못하게 되는 상태.  

### 세마포어(Semaphore)

#### 세마포어 생성 및 소멸에 관한 함수.

```c
#include <semaphore.h>

int sem_init(sem_t *sem, int pshared, unsigned int value);
int sem_destroy(sem_t *sem);
// 성공 시 0, 실패 시 0 이외의 값 반환.
```

- sem : 세마포어 (생성 ,소멸) 시에 (, 소멸하고자 하는) 세마포어의 참조 값 저장 변수의 주소 값 전달.
- pshared : 0 이외의 값 전달 시, 둘 이상의 프로세스에 의해 접근 가능한 세마포어 생성. 0 전달 시, 하나의 프로세스 내에서만 접근 가능한 세마포어 생성.
- value : 생성되는 세마포어의 초기 값 지정.

#### lock, unlock에 해당하는 세마포어 관련 함수.
```c
#include <semaphore.h>

int sem_post(sem_t *sem);
int sem_wait(sem_t *sem);
// 성공 시 0, 실패 시 0 이외의 값 반환.
```

 - sem : 세마포어의 참조 값을 저장하고 있는 변수의 주소 값 전달.
 	- sem_post에 전달되면 세마포어의 값 1 증가, sem_wait에 전달되면 1 감소.

세마포어 값은 0보다 작을 수 없기 때문에 현재 0인 상태에서 sem_wait가 호출되면 블로킹 상태에 놓이게 된다.  
다른 쓰레드가 sem_post 함수를 호출하면 세마포어의 값이 1이 되고 이를 0으로 감소시키며 블로킹 상태를 빠져나간다.  
이러한 특징을 이용하여 임계영역을 동기화시킨다.


##### 쓰레드의 소멸과 멀티쓰레드 기반의 다중접속 서버 구현.
 - chat_server.c 파일과 chat_clnt.c 파일. 
 - 클라이언트 간 멀티 채팅.
 - gcc chat_serv.c -D_REENTRANT -o cserv -lpthread 
 - ./cserv 9190 
 - 입력하여 서버 실행.
 - gcc chat_clnt.c -D_REENTRANT -o cclnt -lpthread 
 - ./cclnt 127.0.0.1 9190 yhb(이름)
 - 입력하여 클라이언트 실행.