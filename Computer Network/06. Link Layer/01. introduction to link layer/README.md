# 1. introduction to link layer

<!--ts-->

- [1. introduction to link layer](#1-introduction-to-link-layer)
  - [1. Link layer](#1-link-layer)
  - [2. Context](#2-context)
  - [3. Link layer services](#3-link-layer-services)
  - [4. Two types of links](#4-two-types-of-links)
  - [5. Two sublayers of link layer](#5-two-sublayers-of-link-layer)
  - [6. link layer의 구현체는 어디에 있는가?](#6-link-layer의-구현체는-어디에-있는가)
  - [7. Adaptors communicating](#7-adaptors-communicating)

<!-- Created by https://github.com/ekalinin/github-markdown-toc -->
<!-- Added by: sungminyou, at: 2022년 7월 29일 금요일 13시 50분 45초 KST -->

<!--te-->

## 1. Link layer

- datagram을 물리적인 링크로 연결된 인접 노드에 전송하는 역할을 담당
- LAN은 router의 개입 없이 link layer 프로토콜 만으로 소통 가능한 네트워크
- link layer에서 주고 받는 데이터을 frame이라고 부름

## 2. Context

- datagram은 서로 다른 링크들 위에서 서로 다른 프로토콜을 통해 전송된다
  - 이더넷, wifi, frame relay 등등
- 각각의 link protocol이 제공하는 서비스(network layer에 제공)는 다르다
  - 어떤 protocol은 신뢰성 전송을 제공할 수도 있고 다른 것은 제공안 할 수도 있다

## 3. Link layer services

![1](https://user-images.githubusercontent.com/48282185/181684982-6c2554fe-b355-4c2e-8686-896a8da052e2.png)

- datagram에 header, trailer를 붙여서 frame으로 캡슐화한다
- MAC주소는 하나의 단말이 여러 단말들과 연결되어있을 때, 송신 주체와 수신 주체를 식별하기 위해서 존재한다(링크 하나가 공유되는 경우가 대부분임, wifi에 여러 명이 접속하는 것이 예시)
- 채널(= 링크)이 공유되기 때문에 어떤 frame을 먼저 보낼 것인가에 대한 문제는 성능과 직결됨
- 데이터를 의미 단위로 나눠보면 위상이 왜 저렇게 됐는지에 대한 이유가 있다
  - 왜 dest address가 제일 앞에 위치하는지
    - 물리적인 레벨에서 나에게 들어온 datagram인지 빠른 판단을 하기 위함
  - 다른 계층의 프로토콜들은 오류 검사를 헤더에 붙이는데, link layer에서는 왜 trailer에 붙이는지
- Reliable delivery between adjacent nodes
  - fiber나 twisted pair처럼 신호의 간섭이 적은 유선 링크에서는 드물게 사용함
  - 무선 링크에서는 간섭이나 손실이 굉장히 많기 때문에 많이 사용함
- flow control
  - 인접 노드 사이의 통신이기에 흐름에만 신경쓰면 된다
  - sending, receiving node사이에서 pace를 맞춰서 잘 전달될 수 있도록 한다
- error detection
  - 멀리 갈수록 신호가 약해지고, noise로 인해 신호가 망가져 error가 발생한다
  - receiver가 오류인지 판단할 수 있어야함
- error correction
  - 보내는데 시간이 오래 걸리는 링크(인공 위성 같은 것)에서는 다시 보내는 비용이 크기 때문에, 오류 교정을 할 수 있도록 코드를 실어서 보낸다
  - 데이터는 커지지만 다시 보내는 것보다는 비용이 더 클 때 사용
- half-duplex and full-duplex
  - half duplex는 어떤 시각 t에 한 단말만 단방향 전송할 수 있음
  - full duplex는 어떤 시각 t에 두 단말이 양방향 전송할 수 있음

## 4. Two types of links

![2](https://user-images.githubusercontent.com/48282185/181684979-28ca66ff-c089-4092-932d-a719dfa002d4.png)

- 단말들이 연결된 형태는 크게 두 가지가 있음
- point-to-point
  - 장비들을 연결하기 위한 링크나 네트워크 장치 수가 증가, 구성 비용 증가
  - 공유되는 환경이 아니라 단말을 구분할 필요가 없어짐, frame크기 감소
  - 링크를 공유하지 않아 두 단말 사이에서 데이터를 보낼 때 기다릴 필요가 없음
- broadcast link
  - 장비들이 네트워크와 연결되기 위해 broadcast링크와 연결하는 네트워크카드 하나만 있으면 된다
  - 공유되는 환경이라 단말을 구분해야함, frame크기 증가
  - 서로 동시에 데이터를 보내면 데이터가 섞여 error가 날 수 있다, 누가 링크 점유하고 있으면 기다려야함

## 5. Two sublayers of link layer

- link layer는 두 개의 layer로 다시 구분된다
- data link control sublayer
  - 신뢰성 전송 기능 등을 처리하는 레이어
  - broadcast나 point-to-point 형태에 모두 필요
- media access control sublayer
  - 링크 상에서 단말을 식별하는 기능을 처리하는 레이어
  - 링크 상에서 데이터의 충돌이 없도록 하기 위해 보내는 순서를 제어
  - point-to-point 형태에서는 필요 없음

## 6. link layer의 구현체는 어디에 있는가?

![3](https://user-images.githubusercontent.com/48282185/181684976-87f92e76-d574-48c0-a936-5c782dad3b5b.png)

- 하드웨어(네트워크 카드), 소프트웨어(네트워크 카드 디바이스 드라이버), 펌웨어(네트워크 카드 디바이스 컨트롤러)을 하나로 합쳐서 link layer를 구현한다

## 7. Adaptors communicating

![4](https://user-images.githubusercontent.com/48282185/181684964-9ad30b84-bb64-4816-b8f5-3a11f282354a.png)

- 어댑터 사이의 통신이 link layer 통신임
- sending side
  - datagram을 frame으로 캡슐화
  - bits error를 확인하기 위한 코드 삽입, flow control, 신뢰성 전송 등을 수행
- receiving side
  - bits error가 있는지 확인, 신뢰성 전송, flow control 등을 수행
  - frame에서 datagram을 추출하여 network layer로 보내는 역할
