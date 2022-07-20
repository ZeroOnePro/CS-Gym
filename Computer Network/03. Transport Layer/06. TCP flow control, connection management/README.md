# 06. TCP flow control, connection management

<!--ts-->

- [06. TCP flow control, connection management](#06-tcp-flow-control-connection-management)
  - [1. TCP flow control](#1-tcp-flow-control)
  - [2. flow control example](#2-flow-control-example)
  - [3. TCP Persist Timer](#3-tcp-persist-timer)
  - [4. Agreeing to establish a connection](#4-agreeing-to-establish-a-connection)
  - [5. TCP 3-way handshaking](#5-tcp-3-way-handshaking)
  - [6. TCP closing a connection](#6-tcp-closing-a-connection)

<!-- Created by https://github.com/ekalinin/github-markdown-toc -->
<!-- Added by: sungminyou, at: 2022년 7월 20일 수요일 17시 57분 13초 KST -->

<!--te-->

## 1. TCP flow control

- application에서 system call을 통해 TCP buffer에 있는 데이터들을 읽어가야한다, 읽어가지 않는다면 buffer가 계속 쌓이다가 overflow가 나버림
- 이런 문제가 생기기에 receiver는 sender는 control할 필요가 있다
- receiver는 응답을 할 때 헤더에 rwnd 값을 실어서 보낸다

  ![1](https://user-images.githubusercontent.com/48282185/179941601-443217a7-a424-4b51-a0fa-434121432fef.png)

  - RcvBuffer는 socket option으로 제공해서 조절가능함(일반적으로 4096bytes), 기본적으로 OS가 설정하는 default 값을 따라 설정됨
  - sender는 unacked data의 양이 receiver가 보내는 rwnd값을 초과하면 안된다
  - 이렇게 함으로서 receiver buffer가 overflow가 나지 않게 보장할 수 있다
  - rwnd값은 free space로 할 수도 있겠지만, 그 동안에 application이 읽어갈 수 있기때문에 좀 더 크게 잡아도 상관은 없다, 구현하기 나름

## 2. flow control example

![2](https://user-images.githubusercontent.com/48282185/179941563-55e6371b-8b8d-4f43-9725-aa55c558e673.png)

- 공간이 났을 때 보내는 ACK이 손실되면, sender는 무한 대기에 빠진다

## 3. TCP Persist Timer

![3](https://user-images.githubusercontent.com/48282185/179941561-c977fe91-8a72-4cd6-a39b-56ee4f714558.png)

- sender의 무한 대기 상태를 막기 위한 방법
- persist timer가 expired되면, sender는 작은 크기의 packet(probe)을 만들어 여유 공간이 있는지 확인한다

## 4. Agreeing to establish a connection

- 2-way만으로는 부족한 이유

  - ghost client

    ![4](https://user-images.githubusercontent.com/48282185/179941559-f312c24f-9c98-4d7e-97ea-1741646c3438.png)
    ![5](https://user-images.githubusercontent.com/48282185/179941556-afdc1f8b-0101-4923-a315-948cba4f1149.png)

    - 재전송된 연결 요청 리퀘스트가 연결 종료 된 뒤에 도착하는 경우 서버는 무한 대기 상태에 빠질 수 있다

## 5. TCP 3-way handshaking

![6](https://user-images.githubusercontent.com/48282185/179941549-a1d15fd3-6d83-4035-bcc4-da2f8f55a39f.png)

- 3-way의 문제는 client가 연결 요청을 하면 완전히 무시하지 않고 ack올 것을 대기하는 일련의 준비 과정을 거친다 그래서 일정 시간 동안 ack이 오지 않으면 server는 block된다
  - 받을 준비를 안해놓으면 client의 데이터를 못 받기 때문에 준비는 무조건 해야함
- syn flooding attack
  - client가 계속 synbit를 1로 설정해서 무한히 요청시켜 서버를 다운시키는 공격방법
  - 연결을 위해 할당된 자원들이 모두 소비됨
  - TCP의 약점
  - 이러한 약점 때문에 새로 개발된 프로토콜들은 4-way handshaking을 쓴다
- 3-way도 server의 block을 완벽하게 막을 수는 없다

## 6. TCP closing a connection

![7](https://user-images.githubusercontent.com/48282185/179941524-42c2a253-62d1-4591-a764-e7d217f3b1c4.png)

- client와 server 어느 쪽에서든 연결을 끝낼 수 있음
  - TCP 헤더의 FIN flag를 1로 설정
- FIN과 ACK flag가 모두 1로 세팅되있으면 완전히 연결 종료
  - FIN flag가 1이 된 패킷을 받아도 내 할 일 이 안 끝났으면 플래그 설정 안하면 그만임
- client는 서버의 FIN을 받고 왜 2 \* max segment lifetime(네트워크에서 패킷이 살아있을 수 있는 최대 시간)만큼 종료되면 안 되는지?
  - client가 보낸 ack이 서버에 도착했는지 알 수 없기때문임, ack이 정상적으로 도착하지 않았다면 서버가 다시 FIN을 보내고 다시 ACK을 보내줘야함
