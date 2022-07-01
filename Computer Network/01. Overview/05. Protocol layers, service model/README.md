# 5. Protocol layers, service model

<!--ts-->

- [5. Protocol layers, service model](#5-protocol-layers-service-model)
  - [1. Internet protocol stack](#1-internet-protocol-stack)
  - [2. ISO / OSI reference model](#2-iso--osi-reference-model)
  - [3. layer 구조가 필요한 이유](#3-layer-구조가-필요한-이유)
  - [4. TCP in Linux](#4-tcp-in-linux)
  - [5. Encapsulation](#5-encapsulation)

<!-- Created by https://github.com/ekalinin/github-markdown-toc -->
<!-- Added by: sungminyou, at: 2022년 7월  2일 토요일 01시 46분 13초 KST -->

<!--te-->

## 1. Internet protocol stack

- layer는 protocol의 집합
- application layer
  - 네트워킹 애플리케이션들을 서포트하기 위해 필요
  - FTP, SMTP, HTTP등이 있다
- transport layer
  - process 간의 데이터 전송
  - TCP, UDP
- network layer
  - source에서 destination까지 datagram들을 라우팅해주는 역할
  - 컴퓨터 사이 간의 데이터 전송
  - IP, routing protocols
- link layer
  - 이웃 네트워크까지 데이터를 전송하는 역할
  - 이더넷, 802.11(wifi), PPP
- physical layer
  - 실제 전기 신호

## 2. ISO / OSI reference model

![1](https://user-images.githubusercontent.com/48282185/176937508-d5a12e3f-2172-4ab2-a99c-6ed541b6f2a1.png)

- 개념적 모델, 상용화 되지는 못함
- presentation layer
  - 장치들간의 표현형태(암호화, 압축 형태 등)가 다름으로 인해 발생하는 문제를 해결하기 위한 layer, 데이터의 제시 형태
- session layer
  - 두 end system 사이의 동기를 맞추거나 실패를 복원하거나 check pointing 하는 역할
- 실제 상용화된 internet protocol stack에서는 위의 두 레이어가 생략되었는데, 실제 구현에 있어서 까다로웠기 때문에 application, transport layer에 각각 흩어져서 구현되었다

## 3. layer 구조가 필요한 이유

- 복잡한 시스템을 다루기 위해 필요하다
- 복잡한 시스템 내에서 명시적인 구조를 통해 관계를 식별할 수 있다
- 각 layer의 기능을 독립적으로 개발 가능하다
- 네트워킹을 위해 여러 application에서 중복되는 기능들을 재사용 할 수 있게 만들어주고 유지 보수도 용이하게 만들어준다
- 무선에서는 유선에서 겪지 못했던 문제들을 맞딱뜨리면서 layer 구조가 오히려 heavy해졌다

## 4. TCP in Linux

![2](https://user-images.githubusercontent.com/48282185/176937485-4327dc3b-4b75-4348-9c4a-8b706769c637.png)

## 5. Encapsulation

- protocol stack을 거치면서 데이터를 캡슐화한다
- application layer에서 message를 전송하기 위해 message를 transport layer로 전달한다
- transport layer에서는 받는 message에 header를 붙이고(segment) network layer로 전달한다
- network layer에서는 받은 segment에 header를 붙이고(datagram) link layer로 전달한다
- link layer에서는 받은 datagram에 header를 붙이고 (frame) physical layer로 전달하게 된다
- switch vs router
  - switch는 단순히 데이터 전달역할만 한다, physical, link layer만을 가지고 데이터를 전송
  - router는 데이터 전달을 위해 함께 포워딩, 라우팅을 해준다, physical, link network layer를 가지고 데이터 전송
