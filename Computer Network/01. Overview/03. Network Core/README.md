# 3. Network core

<!--ts-->

- [3. Network core](#3-network-core)
  - [1. network core란?](#1-network-core란)
  - [2. Packet Switching - store and forward](#2-packet-switching---store-and-forward)
  - [3. Packet Switching - queueing delay, loss](#3-packet-switching---queueing-delay-loss)
  - [4. Two Key Network - Core functions](#4-two-key-network---core-functions)
  - [5. Circuit Switching](#5-circuit-switching)
  - [6. Packet switching vs Circuit switching](#6-packet-switching-vs-circuit-switching)

<!-- Created by https://github.com/ekalinin/github-markdown-toc -->
<!-- Added by: sungminyou, at: 2022년 7월  2일 토요일 01시 45분 03초 KST -->

<!--te-->

## 1. network core란?

![1](https://user-images.githubusercontent.com/48282185/176936969-5b34f3da-7608-4ddc-b724-c323996491fe.png)

- edge network 간의 상호 작용을 위한 연결다리 역할
- 네트워크 코어는 상호작용 하는 라우터들 간의 그물 형태로 구성돼있다
- 패킷 스위칭
  - 호스트는 packet단위로 application layer의 메시지를 잘게 쪼갠다
  - 패킷은 라우터들을 통해서 다음으로 나아간다
  - source에서 destination까지 경로는 여러 개의 링크에 걸쳐져있다
    - 각 패킷은 full link capacity에서 전송된다

## 2. Packet Switching - store and forward

![2](https://user-images.githubusercontent.com/48282185/176936965-0f1545b5-ac60-403f-8da9-8c113ddbcb50.png)

- store and forward
  - 패킷이 다음 링크로 전송되기 전에 전체 패킷이 라우터에 도착해야함, 패킷의 부분만 도착했다면 다음 링크로 전송 불가능
- one-hop transmission delay
  - L은 패킷의 크기(bits), real world에서는 보통 1KB
  - R은 링크의 단위 시간당 전송량(bps)
  - 한 링크를 타고 데이터가 전달되는 전달 지연 시간(one-hop transmission delay)은 $L \over R$초
- end-end delay
  - source에서 destination까지 총 지연 시간
  - propagation delay가 0이라고 가정했을 때 위의 예시에서 링크가 2개이므로 2 \* $L \over R$

## 3. Packet Switching - queueing delay, loss

![3](https://user-images.githubusercontent.com/48282185/176936948-934769c9-9711-45ee-a1d5-0adc2b5cb2b5.png)

- 도착 비율이 전송 비율을 초과할 경우 벌어지는 상황
  - 패킷들은 자신의 전송 차례가 될 때까지 큐에서 대기한다(큐잉 지연)
  - 패킷들은 라우터의 메모리(or 버퍼)가 꽉찰 경우 버려지게된다
- 그래서 패킷 손실과 큐잉 지연을 줄이기 위해 각 호스트에서 라우터로 내보내는 양을 조절하는 것이 중요해진다

## 4. Two Key Network - Core functions

![4](https://user-images.githubusercontent.com/48282185/176936909-f1efe176-3725-4fc4-9558-323c6b2dd6e6.png)

- forwarding
  - 들어온 패킷을 적절한 링크로 내보내는 작업을 말함
  - 패킷에 목적지에 대한 정보가 들어있어서 forwarding할 수 있다
- routing
  - 패킷의 route를 결정 짓는 작업
  - 어떤 목적지를 갖느냐에 따라 어떤 링크로 내보내야 최적일까에 대한 문제이고, 라우팅 알고리즘으로 이를 해결하고, 포워딩 테이블을 만든다
- 모든 네트워크 구조가 위와 같은 형태는 아니지만(SDN), 대다수의 네트워크가 위와 같이 동작한다

## 5. Circuit Switching

- 회로(circuit) 단위의 선로 변경(switching)
- 예전 전화망의 구성형태
- 자원을 독점한다(no sharing)
  - 자원을 독점하기에 성능이 보장됨
  - 전화를 걸어놓고 의미 없이 통화를 하지 않는다면 자원이 독점되고 있기에 낭비가 발생한다
  - a와 b가 전화하고 있으면 자원이 독점되어있기에 c는 a에게 전화를 걸 수 없음
- FDM vs TDM
  - FDM은 회로를 부여할 때 주파수 단위로 나눠서 하나의 신호로 합쳐서 공유하는 것이다
  - TDM은 회로를 부여할 때 시간 단위로 나눠서 공유하는 것이다

## 6. Packet switching vs Circuit switching

- 1Mbps의 링크에 각 사용자들이 있고, 이 사용자들이 active상태일 때 100Kbps로 데이터를 전송한다고 하자, 유저는 총 시간의 10%만 active상태가 된다
  - circuit switching의 경우 최대 10명의 유저밖에 수용할 수 없다(100Kbps \* 10 = 1Mbps)
  - packet switching의 경우 만약 35명의 유저가 사용한다고 하더라도 이 35명이 동시에 데이터를 보낼 확률이 0.0004밖에 안되기 때문에 데이터를 보내는데 크게 신경쓰지 않아도 무방하다
- 간간히 데이터를 보내는 상황에서 부족한 자원을 효율적으로 사용하는 방법은 packet switching이 더 적합하다
- 패킷 스위치도 만능은 아닌데, 앞서 봤듯이 하나의 라우터를 여러 사용자가 공유할 때 큐잉 지연과 패킷 손실이 발생하는 경우를 보았다. 그래서 프로토콜은 신뢰성(reliability)를 위해 transfer, congestion control이 필요하다
- 어떻게 circuit switching처럼 성능을 보장할까?
  - 오디오, 비디오 및 스트리밍 서비스들은 bandwidth가 보장되어야함
  - 여전히 풀리지 않은 문제이다
