# 01. principles of transport layer and reliable data tranfer

<!--ts-->

- [01. principles of transport layer and reliable data tranfer](#01-principles-of-transport-layer-and-reliable-data-tranfer)
  - [1. Transport services and protocols](#1-transport-services-and-protocols)
  - [2. TCP / IP protocol stack messages](#2-tcp--ip-protocol-stack-messages)
  - [3. Internet transport layer protocols](#3-internet-transport-layer-protocols)
  - [4. TCP / IP protocol suite and implementation](#4-tcp--ip-protocol-suite-and-implementation)
  - [5. Multiplexing / Demultiplexing](#5-multiplexing--demultiplexing)
  - [6. How demultiplexing works](#6-how-demultiplexing-works)
  - [7. Reliable data transfer](#7-reliable-data-transfer)
  - [8. FSM Reliable data transfer](#8-fsm-reliable-data-transfer)
  - [9. Stop-and-Wait operation](#9-stop-and-wait-operation)

<!-- Created by https://github.com/ekalinin/github-markdown-toc -->
<!-- Added by: sungminyou, at: 2022년 7월 18일 월요일 17시 37분 22초 KST -->

<!--te-->

## 1. Transport services and protocols

- 서로 다른 end system에서 돌아가는 application process 사이에 논리적 소통을 담당함
- application layer에서 만든 메시지를 `전송` 해야하는 책임을 가지고 있음
- 논리적 이기에 서로 다른 end system이 바로 연결되어있다고 간주되고, end system이 연결되는 경로에 대해서는 신경 안 쓴다 즉, 서로 다른 프로세스간 소통을 위해 필요한 작업만을 수행함
- application layer가 만든 메시지를 패킷 스위칭하기 위해 패킷 단위로 분할하는 역할을 맡는다
- 분할된 패킷들을 받아서 다시 올바르게 합쳐서 application layer로 전달해주는 역할을 맡는다

## 2. TCP / IP protocol stack messages

![1](https://user-images.githubusercontent.com/48282185/179474574-333e7d89-2957-40c5-8292-f01817f0021c.png)

- link layer의 frame에만 trailer가 붙는이유 → error detection
  - transport layer에도 오류 교정이 있지만 header에 포함돼있음

## 3. Internet transport layer protocols

- reliable, in-order delivery: TCP
  - 모든 message를 순서에 맞게 보내주는 역할
  - congestion control
  - flow control
  - connection setup
  - congestion control이나 flow control을 transport에서 하는 이유는 결국에는 네트워크가 원할해야 손실이 적고, 빠른 전송이 가능하기 때문에 network의 혼잡도에 대해서도 고려를 해야한다 그냥 무작정 보내면 안됨
- unreliable, unordered delivery: UDP
  - ip가 전송을 담당하기 위해 꼭 필요한 요소들만 있음
  - 오류난 패킷은 재전송 하지 않고 버림
- transport layer에서 할 수 없는 책임
  - delay 보장 → nodal delay는 network에서 일어남
  - bandwidth 보장(단위 시간 당 보낼 수 있는 데이터의 최대량) → network의 책임

## 4. TCP / IP protocol suite and implementation

![2](https://user-images.githubusercontent.com/48282185/179474572-199cf7d8-1145-4cf4-a92c-7e744ac2e607.png)

- tracert는 경로 추적하는 application인데, transport layer를 거치지 않고 바로 network layer와 소통한다
- layer 간의 소통은 socket을 통해 이루어지고, application layer를 제외한 나머지 네 개의 layer는 OS에 의해 컨트롤 되고, socket을 사용하려면 system call을 통해 운영체제에 요청해야함

## 5. Multiplexing / Demultiplexing

- multiplexing
  - 여러개의 input을 모아서 한 곳으로 내보내는 것
  - 한 host 내에서 돌아가는 여러 개의 network application들이 하나의 transport layer의 구현체로 서비스를 요청하는 것
- demultiplexing
  - 한 곳으로 들어온 input을 여러 곳으로 적절하게 분배하는 것
  - 한 host 내에서 돌아가는 여러 개의 network application들에 들어오는 input들을 하나의 transport layer의 구현체가 적절한 process로 데이터를 넘겨준다
  - 누가 받아야 하느냐는 문제가 생김

## 6. How demultiplexing works

- host receives IP datagrams
  - 각 datagram은 source와 destination의 IP주소를 가지고 있다 → 서로 다른 end system 구분
  - 각 datagram은 transport layer의 segment를 운반하는 책임을 갖는다
  - 각 segment는 source와 destination의 port 번호를 가지고 있는다 → 서로 다른 application 구분
- 그럼 헷갈릴 수 있는게 서로 다른 application들이 같은 application layer protocol을 사용하는 경우는 없는가? 예를 들어 HTTP를 사용하는 프로그램이 여러개일 수 있지 않는가?
  - HTTP의 경우 포트 번호가 80으로 정해져있는데 이는 HTTP요청을 받아서 처리하는 서버 어플리케이션의 포트 번호이지 HTTP요청을 하는 프로그램의 번호가 아니다
  - HTTP 서버를 한 호스트 내에서 두 대를 동시에 띄우면 포트 번호가 중복돼서 실행이 안되는 것임
- TCP / UDP segment format

  ![3](https://user-images.githubusercontent.com/48282185/179474570-5e07f032-b692-4247-93de-5c0007e0706c.png)

  - source port의 경우 재전송 할 때 필요하고, 여러 개의 application에서 한 서버로 요청을 보낼 수 있는데 그 application을 식별하여 알맞는 처리를 하기 위해서 필요

## 7. Reliable data transfer

- network layer는 데이터의 신뢰성을 책임지지 않는다
- 신뢰성 통신을 구현하기 위해 다음과 같은 상황을 가정
  - 통신 채널에서 패킷의 데이터가 온전하지 않을 수 있다
    - checksum을 이용하여 bit error들을 검출한다
    - checksum으로 보낸 데이터가 망가질 가능성도 있으나 data의 크기에 비하면 checksum의 크기가 훨씬 작기때문에 확률적으로 안망가진다고 가정은하나 checksum이 망가진다면 재전송해서 비효율이 발생하지만, 완전성을 위해서는 이 비효율도 감당할 수 있다
    - recovery 방법은 대표적으로 재전송하는 방법이 있는데, 재전송을 위해서 receiver로 부터 잘 받았다는 feedback을 필요로 한다 ACK은 성공적 전송이 이루어졌을 때 응답, NAK은 데이터가 깨졌을 때 응답
    - ACK, NAK도 전송 중 손실되거나 망가지는 경우가 있으므로 sequence number를 사용하여 어떤 응답이 손실되거나 망가졌는지 확인한다
  - 통신 채널에서 패킷(data, ACK)이 손실될 수 있다
  - checksum, sequence number, ACKs로 재전송을 구현하기는 하나 이것만으로는 충분하지 않다
- sender는 일정 시간 ACK이 응답오기를 기다리고 그 시간이 지나도록 ACK이 오지 않는다면, 재전송하는 것이 일반적이다
- 패킷이 딜레이로 인해 늦는 경우
  - 재전송된건지 새로온 건지 구분을 하기 위해 순서 번호가 필요
  - 전송된 패킷들 중 어느 것이 손실되거나 망가졌는지 확인하기 위해 순서 번호가 필요
  - 수신자 입장에서 받은 패킷들을 재조립할 때도 순서 번호가 필요
  - ACK에도 몇 번 패킷에 대한 응답인지 순서 번호를 붙인다
- 재전송 타이머를 둔다

## 8. FSM Reliable data transfer

- $event\ occur \over handler$
- $\Lambda$는 무시한다는 의미
- 해당 FSM에서는 NAK이 없고, ACK을 이용하여 corrupt를 표현하고 있음
- sender

  ![4](https://user-images.githubusercontent.com/48282185/179474564-573fa45e-52cd-4254-b830-dda0998050ea.png)

- receiver

  ![5](https://user-images.githubusercontent.com/48282185/179474555-517bb7df-f6db-4c50-ab64-0231b77fa4ca.png)

- 데이터가 도중에 손실되거나 ACK이 손실되는 경우 둘 다 sender에게 ACK이 안가므로 timer expired가 되고 두 경우 모두 재전송된다
- ACK이 delay되는 경우도 마찬가지로 time expired되고 재전송이 되는데, sender는 delay된 ACK을 받아서 정상적으로 전송이 됬다고 처리하고 재전송된 데이터에 대한 ACK이 이후에 sender로 오면 무시하면 된다

## 9. Stop-and-Wait operation

- 지금까지의 경우를 보면 한 패킷을 보내고, 응답을 받은 후 다음 패킷을 보내는 방식으로 이루어졌는데, 이를 응답이 오기 전까지 작업을 멈추고 기다린다는 의미에서 stop-and-wait라고 부름
- 네트워크의 utilization을 살펴보면 아래와 같다

  ![7](https://user-images.githubusercontent.com/48282185/179474540-77ca2a0c-4115-40a2-9699-ca617bdef948.png)

  - 이 방식은 너무 네트워크의 idle time이 길어서 비효율적이다
  - idle time을 줄이고 한 번에 많은 양을 보내는 쪽으로 방향을 바꿔줘야함
