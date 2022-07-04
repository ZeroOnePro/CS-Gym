# 01. principles of application layer

<!--ts-->

- [01. principles of application layer](#01-principles-of-application-layer)
  - [1. Application layer](#1-application-layer)
  - [2. Application layer architecture](#2-application-layer-architecture)
  - [3. Processes communicating](#3-processes-communicating)
  - [4. Addressing processes](#4-addressing-processes)
  - [5. Application layer protocol defines](#5-application-layer-protocol-defines)
  - [6. Characteristics of Applications](#6-characteristics-of-applications)
  - [7. Internet apps: application, transport protocols](#7-internet-apps-application-transport-protocols)

<!-- Created by https://github.com/ekalinin/github-markdown-toc -->
<!-- Added by: sungminyou, at: 2022년 7월  4일 월요일 19시 22분 17초 KST -->

<!--te-->

## 1. Application layer

- 서로 다른 end systems들 간에 상호작용을 목적으로 한다
- network application 프로토콜들의 개념적, 구현적 관점
  - client - server 패러다임
  - peer - to - peer 패러다임
  - transport layer service model
- application layer의 대표적인 프로토콜들
  - HTTP
  - SMTP / POP3 / IMAP
  - DNS

## 2. Application layer architecture

![1](https://user-images.githubusercontent.com/48282185/177135752-1792e86c-f456-457f-9a90-90302191cc26.png)

- client - server
  - server
    - 항상 켜져있어야함
    - 고정된 IP 주소
    - 스케일링을 위한 데이터 센터를 구축한다
  - client
    - 주로 간헐적인 커넥션
    - 동적 IP 주소들을 갖는다
    - 서로 클라이언트들과 직접적 상호작용을 하지 않는다
- peer - to - peer
  - 서버로부터 컨텐츠를 제공받은 사용자가 다시 제공자가 되는 구조
  - 서버로 몰리는 부하를 분산시키는 장점이 있다
  - 서버가 항상 켜져있지 않아도 됨
  - 분산된 end system들 간에 상호작용이 일어남
  - self scalability
    - 새로운 peer가 새롭게 서비스 제공자가 되는 것
  - Peer들은 간헐적으로 연결되고 IP주소도 동적으로 변한다, 이것이 P2P에서 제일 크게 고려되어야할 점

## 3. Processes communicating

- 같은 host내에서 inter-process commnuncation을 하는 경우
- 프로세스들이 다른 host에 있는 경우 메시지를 교환
  - client process가 communication을 초기화한다
  - server process는 contact를 대기한다
- P2P 구조에서는 application이 client process와 server process를 동시에 갖고있다

## 4. Addressing processes

- 메시지를 받기위해서는 반드시 프로세스는 identifier를 가지고 있어야한다
  - Host → IP주소
- ip주소만으로는 프로세스들을 식별할 수 없다
- identifier = (IP address, port number)
  - 대표적인 port number들
    - HTTP 서버: 80
    - Mail 서버: 25
  - IP주소는 host를 Port number는 host 내의 프로세스의 identifier이다

## 5. Application layer protocol defines

- 교환되는 메시지의 유형
  - 요청(Request)
  - 응답(Response)
- 메시지 문법
  - 메시지의 필드 내에는 무슨 내용이 들어가는지, 내용을 어떻게 구분되는지
- 메시지 의미
  - 필드내에 있는 정보의 의미
- 규칙
  - 언제 어떻게 프로세스들이 메시지를 보냈을 때, 받았을 때 취해야 하는 행동을 규칙으로 정의
- 개방된 프로토콜
  - RFC에 명시되어있다, 표준으로 사용됨
  - HTTP, SMTP등이 있다
- 폐쇄된 프로토콜
  - Skype같은 것들은 같은 프로그램을 사용하는 사용자끼리만 같은 프로토콜을 가지고 통신함

## 6. Characteristics of Applications

- application의 요구 사항은 모두 다르다
- 전달되는 과정에서의 요구사항이라 application layer가 해결할 수 있는 문제가 아님 → transport layer의 역할
- Data integrity
  - 데이터가 온전히 오는 것이 중요
  - file 교환이라던지, 인터넷 뱅킹 등, web transactions
  - 비디오 같은 것들은 일정 손실에 대해 무관함
- Timing
  - 데이터를 주고받음에 있어 타이밍이 제일 중요
  - 인터넷 전화, 게임
- Throughput
  - 단위 시간 당 많이 처리
  - 다운로드 플랫폼, 빠른 다운로드가 중요
- Security
  - 보안이 중요한 경우
  - 인터넷 뱅킹 등

## 7. Internet apps: application, transport protocols

![2](https://user-images.githubusercontent.com/48282185/177135723-e53392fd-fe65-400a-88fb-9a035559bc12.png)
