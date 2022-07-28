# 5. BGP

<!--ts-->

- [5. BGP](#5-bgp)
  - [1. Internet inter-AS Routing BGP](#1-internet-inter-as-routing-bgp)
  - [2. BGP basics](#2-bgp-basics)
  - [3. Path attributes and BGP routes](#3-path-attributes-and-bgp-routes)
  - [4. BGP path advertisement](#4-bgp-path-advertisement)
  - [5. BGP message](#5-bgp-message)
  - [6. BGP, OSPF, forwarding table entries](#6-bgp-ospf-forwarding-table-entries)
  - [7. 문제](#7-문제)
  - [8. BGP route selection](#8-bgp-route-selection)
  - [9. Hot potato routing](#9-hot-potato-routing)
  - [10. BGP routing policy](#10-bgp-routing-policy)
  - [11. 정리](#11-정리)

<!-- Created by https://github.com/ekalinin/github-markdown-toc -->
<!-- Added by: sungminyou, at: 2022년 7월 28일 목요일 15시 14분 21초 KST -->

<!--te-->

## 1. Internet inter-AS Routing BGP

![Untitled](https://user-images.githubusercontent.com/48282185/181433836-8ce97c3d-87c6-446e-b936-600c8e3bd81f.png)

- BGP, Border Gateway Protocol은 domain사이에서 사용되는 사실상 유일한 routing protocol
- 기능을 기준으로 두 가지로 나눠진다
  - e(xternal)BGP
    - 서로 다른 AS border gateway 간에 자신이 관리하는 subnet의 정보를 상호작용
  - i(nternal)BGP
    - 받은 서로 다른 AS의 정보를 AS내 모든 라우터들에게 전파
  - AS1내의 AS3와 연결된 border router A가 AS3의 정보를 받아(eBGP) 내부 전파(iBGP)하고, 또 다른 AS2와 연결된 border router는 이 정보를 받고 AS2에게 AS3의 정보를 전파(eBGP)
  - 단순히 reachability information만으로 good route를 결정하는 것이 아니라, AS의 정책에 따라 서로 협력관계가 아니라면 연결하지 않을 수 있다

## 2. BGP basics

- BGP session, 두 개의 BGP border gateway(peers)가 BGP 메시지를 상호작용하는 것
  - border gateway(⇒ 다른 AS와 연결된 router)가 서로 다른 AS 간의 연결일 수도 있고, 내부에서 서로 다른 AS로 연결되는 border gateway끼리의 연결일 수 있다
- 서로 다른 destination network prefixes들로 가는 path들을 뿌린다(path vector protocol)
  - prefix라고 불리는 이유는 주소 블럭을 압축(aggregate)해서 전달하기 때문
- semi-permanent(연결 유지를 보장할 수 없음) TCP conntection 위에서 메시지를 주고 받는다

## 3. Path attributes and BGP routes

- BGP 프로토콜이 주고받는 메시지의 내용
- prefix(주소) + attributes = “route”
- 두 개의 중요한 attributes
  - AS-PATH
    - 목적지에 도착하기 위해 어떤 AS들을 거쳐가야하는 것인지에 대한 정보
    - 앞에 붙어서 하나씩 떼어가면서 목적지 까지 간다, 마치 패킷이 layer의 헤더들을 떼는 것처럼
  - NEXT-HOP
    - 패킷을 받으면 누구에게 줄지에 대한 정보
- 다른 AS로부터 받은 메시지를 내부에 뿌릴지 말지는 정책에 따라 결정된다

## 4. BGP path advertisement

![Untitled 1](https://user-images.githubusercontent.com/48282185/181433810-1e79408b-2bd4-496e-9738-423f53fde573.png)

- AS-PATH가 필요한 이유
  - 위의 그림과 같이 대부분의 네트워크 topology가 이뤄지는데, AS1이 AS3에 대한 정보를 AS2를 통해 받을 수도 있고, 직접 받을 수도 있다.
  - 그럼 AS1이 AS3로 가기위해 어떤 경로를 거쳐야하는가?
    - AS-PATH가 짧은 것, 즉 직접 가는 경로
    - AS2를 거쳐서 가는게 더 짧은 경로일 수 있는데, AS-PATH가 짧은 경로를 택하는 이유는 BGP에서는 물리적인 효율성을 추구하기 보다는 논리적인 효율성을 추구하기에 당장 보기에 더 짧은 경로를 택한다, 이유가 있겠지?
- example

  ![Untitled 2](https://user-images.githubusercontent.com/48282185/181433819-925d7cb1-1c1c-4275-890e-72ec818b8213.png)

  - 다른 AS로 넘길 때 자기의 AS번호를 prepend하고 보내준다
  - 202.32.9.0과 202.32.1.0은 TCP 연결을 맺고 정보를 교환한다(iBGP)
    - 이 사이에 있는 라우터는 BGP의 context(iBGP)를 알 수 없고, forwarding하는 역할만 한다
    - 왜 context를 모를까? 만약 저 중간 라우터가 BGP의 context를 알게 한다고 치자, border gateway도 아닌데 그 역할을 수행하는 꼴이된다

## 5. BGP message

- BGP message는 TCP connection 위에서 피어들 간에 교환된다
- 위의 그림에서 보라색 선이 TCP connection을 의미
- 메시지의 종료
  - OPEN
    - peer들 간에 TCP 연결을 맺고, 인증하는 절차
  - UPDATE
    - 새로운 정보 획득 시 교환하는 메시지
  - KEEPALIVE
    - 한 동안 정보가 없을 때 연결을 유지하기 위해 자신이 살아있음을 알림
  - NOTIFICATION
    - 상대방으로 받은 메시지에 오류가 있을 때 알리는 용도나 기타 알려야할 상황이 있을 때 사용

## 6. BGP, OSPF, forwarding table entries

- 이것들이 어떻게 결합되는지 살펴보자

![Untitled 3](https://user-images.githubusercontent.com/48282185/181433821-3c842651-4397-44dd-b897-4ddcbaa97a69.png)

- 1a, 1b, 1d는 border gateway이고, 1c와 iBGP를 통해 AS 3의 정보를 받게된다
- 1d는 1c로부터 메시지를 받았으므로 x로 가기위해서는 1c로 가야함을 인지한다, 1c로 가는 interface는 1번 포트이다(1d → 1c → 3a → …)
- 1a도 1c로부터 메시지를 받았으므로 x로 가기위해서는 1c로 가야함을 인지한다, 1c로 가는 interface가 2번이라고 할 때 x로 가는 interface는 2번 포트이다(1a → 1d → 1c → 3a → …)
- inferface를 정하는 것은 iBGP의 역할이 아니라 내부 라우팅 프로토콜(OSPF, RIP 등)의 역할이다
- iBGP의 TCP 연결 route는 intra routing protocol이 결정 짓는다

![Untitled 4](https://user-images.githubusercontent.com/48282185/181433825-fe9ae5ad-ecb0-4355-b12c-7d3e7c37600e.png)

- control plane의 프로토콜들은 모두 패킷을 전달하는 프로토콜이 아니다!! 라우팅을 결정짓기 위한 프로토콜이다

## 7. 문제

![Untitled 5](https://user-images.githubusercontent.com/48282185/181433826-5ca8b08d-a317-4ff0-905b-2ce616082222.png)

- Router A는 내부에서 구분 짓는 것이기 때문에 정확한 IP주소(192.32.1.0 / 24)를 알아야함
- Router D는 Router A가 aggregate해서(192.32.0.0 / 18) 전달했을 것이기에 AS2의 aggregate정보를 가지고 있음

## 8. BGP route selection

- 외부의 동일한 AS에 대해 여러 경로로 정보를 받았을 때 해당 AS로 가기 위해 어떤 경로를 선택할 것인가?
  - local의 policy
  - shortest AS-PATH
  - closest NEXT-HOP(hot potato routing, 뜨거운 감자를 여러 사람들이 던지기 놀이를 한다고 했을 때 빨리 던져버리기 위해 가장 가까운 사람에게 던진다고 이해하면 됨)
  - additional criteria

## 9. Hot potato routing

![Untitled 6](https://user-images.githubusercontent.com/48282185/181433827-9d889c69-1a01-4632-8927-ee9ca6a186ae.png)

- 2d가 AS3에 대한 정보를 AS1을 통해 들을 수도 직접 들을 수도 있다 이 때 기본적으로 AS-PATH가 짧은 순으로 건넨다고 했으면, 2c를 통해서 직접 전달했겠지만, hot potato routing은 당장의 상황을 보고 더 적은 비용을 택한다(그리디), 즉 OSPF의 링크 가중치가 적은 곳을 택한다. 2a로 내보내서 결국에는 AS1을 거쳐서 AS3으로 가게된다
- 2a - 2d가 다이렉트로 물려있지 않을 수 있는데 NEXT HOP은 BGP에서 정의되는 것이기에 border gateway 사이의 관계이다 다시말해 다이렉트로 물려있지 않아도 2d의 NEXT HOP은 2a임
- cold potato도 있다, hot potato의 반대

## 10. BGP routing policy

- policy가 왜 필요한지 살펴본다
- A, B, C는 ISP
- x, y, w는 소비자
- x는 B와 C로 부터 서비스 받고 있음(dual-home)
- routing policy 1

  ![Untitled 7](https://user-images.githubusercontent.com/48282185/181433831-a9d2256d-7c21-4af6-b633-915dc5f91f76.png)

  - B내에 z라는 subnet이 있고, B는 x에게 z에 대한 정보를 주고, x는 다시 C에게 z에 대한 정보를 준다 그럼 C는 z에 도달하기 위해 x를 거치게 되는데, 고객이 서비스를하는 모순이 발생한다
  - 이런 모순을 막기 위해 x는 B로부터 z에 대한 정보를 받으면 다른 ISP로 해당 정보를 넘겨주지 않는다

- routing policy 2

  ![Untitled 8](https://user-images.githubusercontent.com/48282185/181433834-ce1e6602-40a6-44ff-930f-0b71af9f5706.png)

  - w에 대한 정보를 A가 B와 C에 알리고, B는 C에게 w의 정보를 다시 알린다, 이 때 C가 w로 가는 route를 B를 통해 가는 것으로 정했다고 가정하자
  - y가 w에게 패킷을 보내는데, y는 C의 고객, w는 A의 고객인데, y와 w사이의 패킷을 B가 전달해주고있다. 기술적으로는 가능할지몰라도 돈을 주고 설치한 망을 자신들의 고객이 아닌 사용자가 사용하고 있는 상황이 발생한다
  - 그래서 동등한 peer로부터 받은 정보를 또 다른 peer에게 전달하지 않는다

## 11. 정리

- policy
  - inter-AS
    - 관리자는 어떤 네트워크를 통해 어떻게 라우트가 결정되는지 제어하길 원하기 때문에 policy가 필요
  - intra-AS
    - single admin이라 policy를 결정할 필요가 없음
- scale
  - hierarchical routing은 table size를 줄여주고, update traffic을 감소시킨다
- performance
  - intra-AS는 성능에 초점을 맞춘다
  - inter-AS는 성능보다 정책에 초점을 맞춰서 자칫하면 비효율적인 경로가 생성될 수 있으나 현재는 많이 해결되었다
