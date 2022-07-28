# 6. SDN control plane, SNMP

<!--ts-->

- [6. SDN control plane, SNMP](#6-sdn-control-plane-snmp)
  - [1. Traffic engineering](#1-traffic-engineering)
  - [2. Software Defined Networking(SDN)](#2-software-defined-networkingsdn)
  - [3. SDN perspective](#3-sdn-perspective)
  - [4. 네트워크 관리](#4-네트워크-관리)

<!-- Created by https://github.com/ekalinin/github-markdown-toc -->
<!-- Added by: sungminyou, at: 2022년 7월 28일 목요일 19시 30분 38초 KST -->

<!--te-->

학부 수준에서는 깊게 이해하기 어려운 내용이니 요새 네트워크 routing, forwarding이 이렇게 바뀌어 가고 있다라는 것과 기본적인 동작 원리 정도만 알고가자

## 1. Traffic engineering

![1](https://user-images.githubusercontent.com/48282185/181484771-63da20e9-f22f-42c2-950d-5073f6e638fc.png)

- router 각각에서 개별적으로 route를 결정하면 해결하기 어려운 문제
- u → z로 가는 traffic을 uvwz로 보내고 싶고, x → z로 가는 traffic을 xwyz로 보내고 싶다.
  - 전통적인 라우팅 프로토콜들은 shortest path를 기반으로 route를 구하기 때문에 관리자 마음대로 제어하기 어렵다
  - 저렇게 해주기 위해서는 link의 cost를 원하는 길이 되게 변경해주거나 새로운 routing algorithm을 도입해야한다
- u → z로 가는 traffic을 무조건 x로 보내지 않고, v로도 보내서 분산시켜주고싶다(load balancing)
  - 전통적인 라우팅 프로토콜로는 불가능

![2](https://user-images.githubusercontent.com/48282185/181484768-8d340d67-d943-4040-892c-76496b20d5cf.png)

- w에서 z로 보낼 때 u에서 들어온 패킷은 직접 보내고, x에서 들어온 패킷은 y를 통해서 보내고 싶다
  - 전통적인 라우팅 프로토콜로는 불가능
- 이러한 트래픽 공학 문제를 해결하기 위해 새로운 해결책이 필요하게 되었음

## 2. Software Defined Networking(SDN)

- 중앙에서 네트워크를 조작한다
- 새로운 routing algorithm을 적용할 때 중앙 컨트롤러에만 적용하면 됨
- 접근 제어도 가능
- load balance을 가능

## 3. SDN perspective

![3](https://user-images.githubusercontent.com/48282185/181484743-b4cde01c-a6b6-431a-9ff3-c681a4bfc575.png)

- Data plane switches
  - 전통적인 방식의 라우터가 이제는 중앙으로 부터 forwarding table을 받아 단순히 switching해주는 장치가 되었음
  - 빠르고, 간단해졌음
  - generalized data-plane forwarding을 수행(pattern matching - action)
  - controller와 통신하기 위한 프로토콜이 필요
- SDN controller
  - network의 상태 정보를 관리한다
  - network를 제어하기 위한 application으로 부터 제어 정보를 받고 스위치에 적용시키는 중간 역할을 수행한다
  - MVC모델의 컨트롤러와 같이 model과 view 사이에 데이터 흐름을 중간 컨트롤러가 받아서 매개시켜주는 역할
  - 네트워크 스위치와 통신하기 위해 사용하는 API를 southbound API라고 하고, 네트워크 제어 애플리케이션들과 통신하기 위해 사용하는 APIFmf northbound API라고 한다
  - 성능, 확장성, 견고성(문제가 발생하지 않도록 원천 차단), 장애 허용 시스템(장애가 나도 정상적 혹은 부분적으로 기능을 수행할 수 있는 시스템)을 특칭으로하는 분산 시스템으로 구현된다
- network control apps
  - SDN controller로 부터 제공되는 API와 low-level services들을 이용하여 제어 기능을 프로그램으로 구현하여 네트워크를 제어하게됨
  - 직접 개발하거나 따로 구매하여 SDN controller에 끼워맞춰진다

## 4. 네트워크 관리

- 너무 복잡한 시스템이라 사람이 추적하고 문제를 발견하기 어렵다
- 직접 사람이 관찰하는 것보다 네트워크 장치들이 주기적으로 정보를 전송하고, 문제 상황이 생기면 그 때 사람이 개입
- 네트워크 상태를 파악하기 위해 알아야하는 필수 정보들을 Management Information Base(MIB)라고 하고, 네트워크 장치는 이 정보들을 수집하고 managing entity(관리 주체)에게 보낸다
  - 이런 정보 획득과정과 관리 주체와 통신하는 일련의 작업들을 프로토콜로 정해둔 것이 SNMP이다
- SNMP(Simple Network Management Protocol)
  - MIB를 수집하는 두 가지 모드가 존재
  - request - response mode
    - managing entity가 주기적으로 agent들에게 문제 없는지 확인하는 방식
  - trap mode
    - agent에 문제 발생 시, managing entity로 문제 상황을 보고하는 방식
