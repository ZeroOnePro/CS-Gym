# 4. Delay throughput

<!--ts-->

- [4. Delay throughput](#4-delay-throughput)
  - [1. Internet structure: network of networks](#1-internet-structure-network-of-networks)
  - [2. Four sources of packet delay](#2-four-sources-of-packet-delay)
  - [3. Queueing delay](#3-queueing-delay)
  - [4. Packet loss](#4-packet-loss)
  - [5. Throughput](#5-throughput)

<!-- Created by https://github.com/ekalinin/github-markdown-toc -->
<!-- Added by: sungminyou, at: 2022년 7월  2일 토요일 01시 45분 39초 KST -->

<!--te-->

## 1. Internet structure: network of networks

- end system끼리는 ISP들을 통해서 인터넷으로 연결된다
  - 지역, 회사, 대학 ISP들
- ISP들은 반드시 상호 연결되어야 된다
  - 서로 다른 ISP의 서비스를 받고있는 end system들 간에 연결을 시켜줘야한다
  - 서로 직접 연결하거나(peering link) 아니면 거대한 라우터(IXP, Internet exchange point)를 통해 연결한다
- 인터넷은 매우 복잡하다
  - 나라의 정책과 경제에 따라 발전 정도가 다르다
- 컨텐츠 제공자들(구글, 넷플릭스 등)은 독자적 네트워크를 구축하여 더 빠른 서비스를 제공한다(ISP 수준의 규모)
- 핵심은 계층 구조로 되어있다는 것이다

  ![1](https://user-images.githubusercontent.com/48282185/176937206-0b93368d-9de3-42fa-ac11-501cba0158d8.png)

  - Tier 1은 상업적인 ISP들(KT, SKT 등), 국가적, 국제적 커버리지, Content provider network(구글 등)

## 2. Four sources of packet delay

- $d_{proc}$: nodal processing
  - 도착한 데이터에 이상이 없는지 체크(bit error)
  - output link 결정
  - 일반적으로 msec 이하이다
- $d_{queue}$: queueing delay
  - output link로 나가기 전에 router buffer에서 대기하는 시간
  - 라우터의 혼잡 레벨에 의존한다, 전송을 대기하는 다른 패킷들의 수에 따라 달라짐
  - 예측 불가능
- $d_{trans}$: transmission delay
  - 라우터가 output link로 데이터를 완전히 내보내는데 까지 걸리는 시간
  - L은 packet length(bits), R은 link의 bandwidth(bps)
  - $d_{trans} = {L \over R}$
  - L은 변동폭이 작고, R은 고정값이다
- $d_{prop}$: propagation delay
  - 패킷이 완전히 내보내진 후 다음 라우터까지 링크를 타고가는데 걸리는 시간
  - d는 물리 링크의 길이
  - s는 링크 매질의 propagation speed(2억 ~ 3억m/s)
  - $d_{prop} = {d \over s}$
- $d_{nodal} = d_{proc} + d_{queue} + d_{trans} + d_{prop}$
  - 한 라우터에 들어와서 링크타고 다음 라우터까지 가는데 걸리는 시간의 총합
- 이 사실들을 알고있으면 네트워크 도식과 함께 위의 딜레이들이 주어졌을 때 걸리는 시간을 계산할 수 있음

## 3. Queueing delay

- L은 packet length(bits), R은 link의 bandwidth(bps), a를 평균 패킷 도착 비율(packet per second)라 할 때 평균 queueing delay를 보자

  ![2](https://user-images.githubusercontent.com/48282185/176937197-5fae3274-3bbc-4c86-b4e9-b7038f595ca7.png)

  - 이 값이 0에 가깝다면 큐잉 지연이 작은 상황
  - 이 값이 1에 가깝다면 큐잉 지연이 큰 상황
  - 이 값이 1을 초과한다면 무한 대기 상태
  - 굉장히 가변적인 요소이다
  - transport layer와 router에서 이 문제를 다룬다

## 4. Packet loss

- queue의 유한한 용량으로 인해 수용하는데 한계가 있음
- 패킷이 도착했을 때 큐가 가득차있다면 버려진다
- 손실된 패킷은 아마 이전 노드에서 재전송될 것이다
- transport layer에서 이 문제를 다룬다

## 5. Throughput

- sender와 receiver사이에 주고받는 단위 시간 당 데이터의 양
- 두가지 관점에서 생각해볼 수 있는데 주어진 시간 구간에서의 throughput과 평균 throughput, 애플리케이션의 목적에 따라 다르다
- 이 값은 route 상에서 처리량이 가장 낮은 link(bottleneck link)가 결정한다

  ![3](https://user-images.githubusercontent.com/48282185/176937173-604d1182-f950-472a-afa4-96b9da244d7c.png)

- 일반적으로 access network의 링크가 bottleneck이 된다
