# 3. distance vector routing algorithm example

<!--ts-->

- [3. distance vector routing algorithm example](#3-distance-vector-routing-algorithm-example)
  - [1. Count to infinity problem](#1-count-to-infinity-problem)
  - [2. Count to infinity problem의 solutions](#2-count-to-infinity-problem의-solutions)
  - [3. 문제 풀이](#3-문제-풀이)

<!-- Created by https://github.com/ekalinin/github-markdown-toc -->
<!-- Added by: sungminyou, at: 2022년 7월 27일 수요일 18시 27분 54초 KST -->

<!--te-->

distance vector는 상용으로 쓰이는 기술은 아님

## 1. Count to infinity problem

- 아래와 같은 topology가 있다

![1](https://user-images.githubusercontent.com/48282185/181212783-4c002566-9d2a-419f-b913-b9c703610120.png)

![2](https://user-images.githubusercontent.com/48282185/181212782-94bb681f-819b-4db4-bbe0-6654b20d922b.png)

- c(x,y)가 60으로 바뀐었다고 하자 그러면 y는 z를 거쳐서 x로 가는게 빠르므로 $D_{y}(x) = 6$으로 변경하고 이웃인 z에게 알린다
- z는 y의 링크에 업데이트가 일어났으므로 $D_{z}(x) = D_{y}(x) + cost(z,y)$ = 7로 변경하게 됨, 업데이트 되었으므로 다시 이웃인 y에게 이 사실을 알리기 계속 업데이트 되다가 TTL값이 0인 시점에 패킷이 사라짐
- 애시당초에 y에서 x로 갈 때도 6이었는데 y → z → y → x로 갔기 때문인데($D_{z}(x) = 5$이므로), c(y,x)의 변화로 인해 y와 z사이에서 계속 핑퐁하게 된다
- 이 문제의 원인은 경로에 대해서는 인지하지 않고 오직, 가중치만 신경썼기 때문이다
  - y의 입장에서는 다시 자기 자신한테로 돌아오는 것을 신경쓰지 않는다

## 2. Count to infinity problem의 solutions

- split horizon
  - 만약 C가 B를 통해 A로 가는 route로 갱신했을 때, C는 B에게 A로 가는 route를 advertise하지 않는다
  - 경로에 속한 이웃들에게는 광고하지 않는다
- poisoned reverse

  - 만약 C가 B를 통해 A로 가는 route로 갱신했을 때, C는 B에게 A로 가는 route를 Infinity로 advertise한다
  - C를 통해 A로 가는게 무한대이므로 B는 C를 통해서 A로 가지 않는다

    ![3](https://user-images.githubusercontent.com/48282185/181212779-ebf34289-caa6-4f82-9ac5-ac985380929b.png)

  - 해결책 적용 이전에 y가 z를 거쳐서 x로 가는게 6(y → z → y → x)이었는데, poisoned reverse에서는 다시 자기 자신을 거치므로 z는 $D_{z}(x) = 5$ 로 갱신하고 y에게 $D_{z}(x) = \infin$로 광고한다
  - 60으로 바뀌고 나서의 변화는 위 그림처럼 되고, 변화될 여지가 없다

## 3. 문제 풀이

![4](https://user-images.githubusercontent.com/48282185/181212773-15c51909-78af-4128-9c19-faa0f848ff2e.png)

- 각각의 라우터의 초기 forwarding table은 이웃 간에 값으로 채우고 시작한다, 채운다는 것은 값을 갱신한다는 뜻
- 이 문제에서는 x에 관한 것을 알고 싶으므로 y와 z의 테이블 그리고, x의 값을 채운 후, y와 z에게 광고한다고 하자

  ![5](https://user-images.githubusercontent.com/48282185/181212762-ea91d5f5-a5ed-473c-9402-b65c06d258b9.png)

  - x에서 z로 가는 경로의 가중치는 5가 선택되므로 변화가 일어나지 않는다

    ![6](https://user-images.githubusercontent.com/48282185/181212736-d409c0a0-2481-4c87-bdf8-ed6fe1fa23d0.png)

  - x에서 z로 가는 경로의 가중치는 1이 선택되므로 변화가 일어나지 않는다
  - 문제에 나왔던 더이상 routing 관련 정보에 변경 사항이 발생하지 않으므로 테이블의 상태는 위와 같이 된다
  - poison reverse가 적용되지 않았으니, 적용하면 달라짐 (1)과 (2) 모두 둘 다 자기 자신을 거쳐 가는 경로였으므로 x → y → z의 경로의 가중치는 $\infin$, 마찬가지로 x → z → y의 경로의 가중치도 $\infin$가 되어 해결책이 적용되기 전과 결과는 같지만, 각 router의 forwarding table의 내용이 다름
  - 헷갈릴 가능성 농후
