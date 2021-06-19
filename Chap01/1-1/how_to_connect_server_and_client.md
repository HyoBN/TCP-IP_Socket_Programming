
## 리눅스 기반에서 실행하기
터미널 창에서,


```
gcc hello_server.c -o hserver 
```
hello_server.c 파일을 컴파일해서 hserver라는 이름의 실행파일을 만드는 문장이다.


```
./hserver 9190
```
현재 디렉토리에 있는 hserver라는 이름의 파일을 9190번 포트로 연결하여 실행시키라는 의미이다.


```
gcc hello_client.c -o hclient
./hclient 127.0.0.1 9190
```
서버와 클라이언트가 각각 다른 컴퓨터에서 실행된다면, 위의 127.0.0.1 대신 **서버의 IP주소**를 입력해야 한다.
