# 5. ethernet

<!--ts-->

- [5. ethernet](#5-ethernet)
  - [1. Ethernet](#1-ethernet)
  - [2. Ethernet physical topology](#2-ethernet-physical-topology)
  - [3. Ethernet frame structure](#3-ethernet-frame-structure)
  - [4. Ethernet의 특징](#4-ethernet의-특징)
  - [5. Ethernet switch](#5-ethernet-switch)

<!-- Created by https://github.com/ekalinin/github-markdown-toc -->
<!-- Added by: sungminyou, at: 2022년 7월 31일 일요일 20시 24분 34초 KST -->

<!--te-->

## 1. Ethernet

- 저렴하고 간단해서 다른 기술들보다 많이 쓰이기 시작하면서, 현재는 제일 보편화된 기술
- 10Mbps ~ 10Gbps의 속도를 갖는다

## 2. Ethernet physical topology

![1](https://user-images.githubusercontent.com/48282185/182023730-fa18f065-6003-442e-8d3d-9daeb2e95115.png)

- bus
  - 90년대 중반까지 사용됐던 형태
  - 모든 노드들이 같은 충돌가능한 링크에 연결된 형태
- star
  - 현재 사용되는 형태
  - 스위치에 모든 노드가 Point-to-Point 형태로 연결되어 충돌이 없어 속도를 높임

## 3. Ethernet frame structure

![2](https://user-images.githubusercontent.com/48282185/182023729-2653312d-9833-43b3-8fe4-eb00c4cb36ae.png)

- type은 payload에 들어간 network layer의 protocol이 뭔지 지칭하는 용도
- preamble은 신호가 가다가 안가다가 가다가 안가다가를 반복하는데 이 때, 송신 단말과 수신 단말의 clock rate를 동기화하기 위한 용도
- destination이 source보다 앞에있는 이유는 자신에게 온 것인지 빠른 판단을 하기 위함

## 4. Ethernet의 특징

- connectionless
  - no handshaking
- unreliable
  - ACK과 NACK같은 응답이 없음, 그냥 보내기만 하면 끝
- Unslotted CSMA / CD with binary backoff
- 구리케이블에서 광케이블로 물리 매체가 변하면서 속도를 향상 하였다

## 5. Ethernet switch

- 이더넷의 속도향상을 위해 고안된 장치
- link layer의 device
  - store and forward방식, 링크를 통해 프레임이 들어오면 저장했다가 적절한 output link로 forward
  - 내보내기 전 링크가 사용 중인지 CSMA / CD방식을 이용해서 충돌을 감지하고 내보낸다
- transparent
  - source는 destination의 MAC주소로 switch의 주소를 쓰는게 아니다, 즉 switch는 MAC주소가 없다, 그래서 단말들은 스위치가 중간에 있는지 없는지 알 수 없고, 교체되도 문제가 없다, 즉 투명하다
- plug-and-play, self learning
  - 스위치를 구매 후, 링크들만 꽂아주면 바로 사용 가능
- self-learning 과정
  - incoming packet의 MAC주소와 port(interface)번호를 보고 switch table에 기록
  - TTL이 있는데, 각 포트에 꽂힌 링크들의 위치가 바뀔 수 있기 때문
    - TTL이 지나기 전에 바뀌어도 상관없다, 어차피 재전송 매커니즘은 없음
  - source와 destination의 interface가 같은 경우 frame을 폐기한다
  - dest의 MAC주소가 table에 없으면, broadcast시킨다
