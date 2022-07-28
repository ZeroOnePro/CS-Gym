# 4. OSPF

<!--ts-->

- [4. OSPF](#4-ospf)
  - [1. Internet approach to scalable routing](#1-internet-approach-to-scalable-routing)
  - [2. Interconnected ASes](#2-interconnected-ases)
  - [3. Inter-AS tasks](#3-inter-as-tasks)
  - [4. Intra-AS routing](#4-intra-as-routing)
  - [5. OSPF](#5-ospf)
  - [6. Hierarchical OSPF](#6-hierarchical-ospf)

<!-- Created by https://github.com/ekalinin/github-markdown-toc -->
<!-- Added by: sungminyou, at: 2022년 7월 28일 목요일 13시 12분 46초 KST -->

<!--te-->

## 1. Internet approach to scalable routing

- autonomous(자치) system(domain)
  - 하나의 기관에서 관리하는 여러 개의 서브넷 집합이고, 이 네트워크는 하나의 routing protocol을 통해 관리됨, 공통되고 명확한 라우팅 정책을 갖는다
- routing prefix
  - subnet을 의미함
- 인터넷에 접속하는 단말들이나 기관이 점점 늘어나기 때문에 라우팅은 확정성있게 설계되어야한다
- intra(내부)-AS(autonomous system) routing
  - 같은 AS내에 있는 라우터들, host들 간의 라우팅
  - AS내의 모든 라우터들은 반드시 같은 intra-domain protocol(라우팅 프로토콜)을 사용해서 동작해야함
  - 서로 다른 AS에 있는 라우터들은 서로 다른 intra-domain routing protocol이 적용될 수 있음
  - gateway router는 AS의 edge에 있는 라우터로, 서로 다른 AS와 연결되는 입구 역할이다
- inter(상호 간의)-AS routing
  - AS들 간의 routing
  - gateway router들은 서로의 서브넷의 존재를 알리고 소통하는 역할을 담당한다

## 2. Interconnected ASes

- 하나의 라우터 안에는 inter-AS routing protocol과 intra-AS routing protocol이 정의되있고, 하나의 forwarding table로 관리된다, 이를 통해서 내부는 물론 외부와도 통신할 수 있는 것임

## 3. Inter-AS tasks

- AS 1에 있는 라우터는 어떻게 AS 1외부로 어떻게 datagram을 보내는 상황을 가정해보자
- AS 1가 외부와 연결되는 과정
  - AS 2와 AS 3을 통해 도달할 수 있는 대상을 파악한다
    - 각각의 AS들은 자신이 갖추고 있는 subnet들에 대한 정보를 서로 다른 AS에게 뿌린다
  - AS 1내에 있는 모든 라우터들은 AS2와 AS3가 뿌린 정보(reachability info)를 전달받는다
  - 이를 통해 AS 간의 routing이 가능해진다

## 4. Intra-AS routing

- IGP, Interior Gateway Protocol이라고 부름
- 주로 사용되는 routing protocols
  - RIP, Routing Information Protocol
    - distance vector algorithm 사용
    - 서비스할 수 있는 최대거리가 15로 제한돼있어 상용으로는 어렵다
  - OSPF, Open Shortest Path First
    - 상용되는 프로토콜
    - link state routing algorithm 사용
    - 전체 topology를 알아야하는 부담을 덜어주는 기법이 있음
  - IGRP, Interior Gateway Routing Protocol(Cisco 소유)

## 5. OSPF

- open
  - 표준 스펙이 공개돼있다
- use link state algorithm
  - LS(link state)패킷을 뿌린다
  - 각각의 노드는 topology의 정보를 갖는다
  - 다익스트라 알고리즘을 사용하여 route를 계산한다
- neighbor들에게 자기 자신의 정보를 뿌린다
- router들 간의 상호작용이기에 IP를 사용한다
  - IP헤더 + OSPF message(OSPF 헤더 + OSPF 바디)
- IS-IS는 OSI 7 layer에서 고안된 프로토콜이고, 이를 실제로 구현한 것이 OSPF
- RIP에는 없는 OSPF의 장점
  - security, OSPF message는 인증절차를 거친다
  - 서로 다른 다섯가지의 cost들을 적용해서 shortest path를 계산할 수 있는데, 상용에서는 하나만 사용
  - 통합된(같은 topology data를 가지고) unicast, multicast를 지원한다
  - 큰 domain에서는 Hierarchical OSPF(확장성)

## 6. Hierarchical OSPF

![1](https://user-images.githubusercontent.com/48282185/181418641-9b833b87-60b7-464e-9fa9-c95ac53e784a.png)

- 두 개의 계층으로 구분됨
  - local area
  - backbone
- 이점
  - 하나의 라우터는 자신이 속한 area의 link state정보만 알고있으면 되고, 다른 area의 link state는 알 필요 없고, 와 연결하기 위해서 거쳐야하는 라우터하나만 알고있으면 된다
- area border router
  - 자신의 area의 network 정보(link state, IP주소 등)를 압축하여 서로 다른 area border router와 주고 받는다
- backbone router
  - backbone에 속해있는 라우터이고, backbone으로 제한된 영역 내에서 OSPF알고리즘을 통해 상호작용한다
  - 서로 다른 area들을 연결해주는 역할을 맡는다
- boundary router
  - 서로 다른 AS들을 연결해주는 역할을 맡는다
- backbone과 local area는 관리자가 정하는 것이지 어떤 규칙이 있는 것은 아니다
- area 3의 Internal router가 area 1에 있는 internal router와 연결하려면, area 1의 border router가 area 1의 정보를 압축해서 area 3의 border router에게 전달했을 것이고, border router는 area 3내에 있는 모든 router에 area 1으로 가기 위해 자신의 정보를 전파한다
- hierarchical이 없다면 전체 topology를 알아야하기 때문에 부담이 큰 반면, 계층 구조를 도입함으로서, 계산량도 줄이고, link state가 변했을 때 다시 전파하는 부담도 줄이고 기타 등등 여러가지 장점이 있다
- 계층 구조는 확장성 있는 시스템에서 적용해 볼만한 system design 아이디어로 활용될 수 있다
