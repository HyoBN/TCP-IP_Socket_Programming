# 입력 스트림과 출력 스트림의 분리.

### 입출력 스트림 분리 2가지 방법.
1. TCP의 입출력 루틴 분할.
 - fork 함수호출을 통해 파일 디스크립터를 하나 복사하여 입력과 출력에 사용되는 파일 디스크립터 구분.

2. 두 번의 fdopen 함수호출을 통해 읽기모드의 FILE포인터와 쓰기모드의 FILE 포인터를 생성하여 분리.

### 스트림 분리 이후의 EOF에 대한 문제점.
 - 위의 2번째 방법으로 스트림을 분리한 경우, 스트림 종료 시 Half-close를 하지 못한다.
 - 대신 소켓이 완전히 연결 종료되며 이는 "하나의 파일 디스크립터"를 통해 얻은 FILE 구조체 포인터들이기 때문이다.
 
 - FILE 포인터를 생성하기에 앞서 파일 디스크립터를 미리 복사하면 Half-close가 가능하다. 
 
 
### 파일 디스크립터의 복사.
 - 위에서 말한 파일 디스크립터의 복사는 fork 함수를 통해 진행되는 복사와 차이가 있다. 프로세스의 생성을 동반하지 않고 원본과 복사본이 하나의 프로세스 내에 존재하는 형태의 복사이다.
 - 동일한 파일 또는 소켓의 접근을 위한 또 다른 파일 디스크립터 생성이라고 할 수 있다.
 - 복사에 사용되는 함수)

```c
#include <unistd.h>

int dup(int fildes);
int dup2(int fildes, int fildes2);
// 성공 시 복사된 파일 디스크립터, 실패 시 -1 반환.
```

- fildes : 복사할 파일 디스크립터 전달.
- fildes2 : 명시적으로 지정할 파일 디스크립터의 정수 값 전달.

##### 파일 디스크립터 복사 후 shutdown 함수가 호출되면 복사된 파일 디스크립터의 수에 상관없이 Half-close가 진행되며 그 과정에서 EOF도 전달된다.