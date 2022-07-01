# 1. What is the Internet

<!--ts-->

- [1. What is the Internet](#1-what-is-the-internet)
  - [1. What is the Network](#1-what-is-the-network)
  - [2. What is the Internet](#2-what-is-the-internet)
  - [3. what is a Protocol?](#3-what-is-a-protocol)

<!-- Created by https://github.com/ekalinin/github-markdown-toc -->
<!-- Added by: sungminyou, at: 2022년 7월  2일 토요일 01시 43분 58초 KST -->

<!--te-->

## 1. What is the Network

- 정의
  - **상호 연결된 모든 그룹 또는 시스템**
- 유형
  - 생물학적 네트워크
  - 비즈니스 네트워크
  - 컴퓨터 네트워크
  - 신경망
  - 소셜 네트워크
  - etc…

## 2. What is the Internet

- service view(사용자 관점)
  - 인터넷을 사용하여 할 수 있는 어떤 것
  - 다서서한응용응 서들스을제 제공한한다
  - **앱 프로그램을 인터넷에 연결할 수 있는 후크**
  - 서비스 옵션들을 제공
  - 웹, VoIP, email, games, e-commerce, social networks, …
- “Nuts and bolts” view(설계자 관점)
  - 인터넷을 구성하는 요소들
  - 스위치, 링크, 프로토콜
  - 수십억개의 컴퓨팅 장치들 간의 연결
    - host = end systems
    - running network apps
  - 물리적 연결장치
    - fiber, copper, radio, satellite
    - transmission rate: bandwidth, 단위 시간 당 전송할 수 있는 데이터 또는 신호의 양
  - packet(응답과 요청 데이터의 잘라진 단위) switches
    - forward packets(chunks of data)
    - routers and switches, 전달자
  - Network of networks
    - interconnected ISPs
  - protocols(인터넷의 소프트웨어 구성요소)
    - 메시지를 주고 받는 과정을 제어하는 약속
    - TCP, IP, HTTP, Skype, 802.11, etc,…
  - 인터넷 표준
    - 각 장치가 다르게 설계되었기 때문에 통신에 있어서 표준화된 규약이 존재해야함
    - 테스트를 통해 검증된 것만이 표준이 될 수 있음
    - 이미 체계화된 공식화된 절차만이 표준이 될 수 있음
    - IETF: 인터넷 표준을 만드는 단체
    - RFC: 표준 명세서

## 3. what is a Protocol?

- 네트워크 뿐만아니라 오래된 관습 또는 약속을 프로토콜이라 부른다.
- 두 개의 통신 개체가 성공적인 커뮤니케이션을 하기 위해서는
  - 무엇을 주고 받니?
  - 어떻게 주고 받니?
  - 언제 주고 받니?
    상호 동의된 규칙에 따른다.
- 네트워크 프로토콜은 두 통신 개체 사이의 메시지 주고 받음의 포멧, 순서와 메시지 전송, 수신할 때 취하는 액션을 정의한다.
  - 왜 이런 종류의 프로토콜이 필요한지?
  - 왜 이런 때에 이런 메시지를 주고받는지?
  - 위의 두가지 관점으로 프로토콜을 살펴보자
  - 프로토콜의 논리를 다른 새로운 기술의 논리에 적용할 수 있어야함
  - 일상생활과 프로토콜의 상황을 비교해보자
