# 1. introduction to routing, link state routing algorithm

<!--ts-->

- [1. introduction to routing, link state routing algorithm (50분)](#1-introduction-to-routing-link-state-routing-algorithm-50분)
  - [1. Per router control plane](#1-per-router-control-plane)
  - [2. Routing algorithms](#2-routing-algorithms)
  - [3. Graph abstraction](#3-graph-abstraction)
  - [4. Routing algorithm classification](#4-routing-algorithm-classification)
  - [5. A Link State Routing Algorithm](#5-a-link-state-routing-algorithm)

<!-- Created by https://github.com/ekalinin/github-markdown-toc -->
<!-- Added by: sungminyou, at: 2022년 7월 27일 수요일 13시 39분 03초 KST -->

<!--te-->

## 1. Per router control plane

- routing을 수행하고 있는 라우터가 source가 돼서 목적지까지 경로를 찾는다
- routing을 수행하기 위해 네트워크의 topology를 알아야 하기 때문에 서로 상호작용해야한다

## 2. Routing algorithms

- 목표
  - 네트워크의 라우터들을 통해서 sending host(혹은 알고리즘을 수행하고 있는 라우터)에서 receiving host까지 가는 좋은 route를 결정짓는다
- route는 목적지 까지 가는데 거쳐가는 라우터들의 시퀸스
- 좋은 경로란?
  - 기준에 따라 다르긴 하겠지만, 어떤 링크를 사용하는 비용이라던가, 빠르게 전송할 수 있다던가, 혼잡이 적은 곳이라던가 자원을 적게 소모하면서 갈 수 있는 경로
- 라우터의 위치가 바뀔 수 있고, 해당 라우터들이 항상 동작하고있다는 보장이 없기에 정답을 내놓는다고 해결되는 문제가 아니다

## 3. Graph abstraction

- 라우터를 정점, 링크를 간선으로 모델링한다
- 간선의 가중치는 처리량, 속도, 비용, 혼잡 정도가 될 수 있다

## 4. Routing algorithm classification

- 분류법 1
  - global
    - 모든 라우터가 자신이 속해있는 하나의 광역(당연히 international 아님) network topology를 완전히 알고있다
    - link state algorithms
    - 정확한 판단 가능하지만, 거대한 정보를 주고 받고 처리하고 저장해야함
  - decentralized
    - 자신이 속해있는 network의 topology를 완전히 알지 못함, 대신 물리적으로 연결된 이웃들까지의 비용은 알고있음
    - 이웃을 통해 목적지 까지 갈 수 있다면, 해당 이웃에게 넘긴다
    - 이웃들끼리 정보 교환을 하면서 반복적으로 계산 작업을 한다
    - distance vector algorithms
    - 정확한지 확신할 수 없지만, 적은 정보로 처리가능
  - 왜 책에서 global이랑 decentralized라고 설명하는지는 모르겠지만 알고리즘의 처리 대상을 생각해보면, global은 전체 네트워크이고, decentralized는 이웃들인데, global을 중앙이라고 생각해보면(SDN에서도 전체 네트워크를 대상으로 하니까 그런 거 같음), decentralized는 global의 도움이 없어도 되는 것이라고 이해하면 될 듯
- 분류법 2
  - static
    - route의 업데이트를 관리자가 메뉴얼하게 지정함
  - dynamic
    - router간의 상호작용으로 자주 route를 업데이트한다
    - 주기적 업데이트
    - 변화가 생겼을 때 업데이트

## 5. A Link State Routing Algorithm

![1](https://user-images.githubusercontent.com/48282185/181162978-5d81ffb0-50ba-4fc7-9708-c2d83cd996b6.png)

- 다익스트라 알고리즘
  - 한 정점으로 부터 다른 모든 정점까지의 최단 거리를 구하는 알고리즘
- 네트워크의 topology를 알기 위해서 모든 라우터가 모든 링크의 비용을 알고있어야함
  - link state를 broadcast해서 모든 노드에게 전달함
  - 모든 노드는 같은 정보를 가지고 있다
- 정보를 받은 라우터는 자신으로 부터 다른 모든 라우터까지의 최단 거리를 계산한다
- 링크 개수를 E라 할 때 우선순위 큐를 이용한 개선된 다익스트라 알고리즘의 시간 복잡도는 O(ElogE)
- 라우터의 큐에 쌓인 패킷들의 수로 혼잡도를 대략 추측할 수 있다
- oscillations possible

  - 당장의 트래픽의 양을 가중치로 뒀을 때 벌어지는 상황

    ![2](https://user-images.githubusercontent.com/48282185/181162959-ddb58e1b-b5f6-46d5-b247-15b4e5176606.png)

  - 초기에 C → A를 (C, B, A)로 두고, route를 갱신하는데, 현재 route의 가중치 합은 1 + e이고, D를 통해가는 가중치는 1이다, C → A를 (C, D, A)로 갱신하고, C의 갱신으로 B도 A까지의 경로를 (B, C, D, A)로 수정하면 D로 모든 트래픽이 쏠리는 현상이 발생한다
  - D를 통해 A로 가는 링크에 트래픽이 쏠리므로 B에서 A로 가는 경로는 한산하다, 때문에 route가 다음 번에 갱신될 때는 위에서와 마찬가지로 B로 트래픽이 쏠리게 되고, A로 가는 트래픽이 B와 D를 왔다 갔다 하게되는 모순에 빠질 수 있다
  - 이처럼 트래픽을 가중치로 두면, 정확한 정보를 둔다는 link state 알고리즘의 장점을 상쇄시킬 수 있으므로 가중치를 어떤 것으로 둘 지가 이 알고리즘에서의 핵심이다

- 의문점
  - link state 알고리즘을 어떤 한 광역 network에서 수행한다면 다른 광역 network까지의 경로는 커버할 수 없다
  - 다른 광역 네트워크와의 경로를 설정할 수 있는 다른 방법이 있을 것이다
