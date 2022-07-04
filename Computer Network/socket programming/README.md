# Soket Programming

소켓을 이용한 간단한 메시지 핑퐁 프로그램

## Contents

- client

  - 메시지를 받는 프로그램

- server

  - 메시지를 받고 처리하는 프로그램

- 메시지 핑퐁을 한다는 목적성에서 서버 - 클라이언트 구조가 이상하기는 하지만 소켓 프로그래밍 연습 삼아 만들어 봤음

## 실행

- server가 client보다 먼저 실행돼서 연결을 받을 준비를 해야함

```bash

$ gcc -o server server.c

$ gcc -o client client.c

$ ./server

$ ./client


```
