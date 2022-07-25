# 1. Router

## 1. Network layer

- transport segment를 sending host에서 receiving host로 전송해주는 역할
- sending host에서는 segment를 캡슐화한다(datagram)
- receiving host에서는 segment를 transport layer로 전달한다
- network layer protocol들은 모든 host와 router내에 존재한다
- router는 datagram의 header를 보고 이것을 어디로 보내야하는지 결정한다

## 2. Network layer의 두 가지 핵심 기능

- forwarding
  - 들어온 packet을 적절한 output으로 옮기는 과정
  - 적절한 output은 routing으로 결정됨
- routing
  - source에서 destination으로 가는 route를 결정하는 과정
  - routing algorithm에는 RIP, OSPF, IS-IS, BGP 등이 있다

## 3. Data plane, Control plane

- plane은 처리가 이루어지는 공간
  - data plane → 데이터의 처리가 다뤄지는 공간
  - control plane → 제어 처리가 다뤄지는 공간
- data plane은 forwarding, control plane은 routing을 뜻함
- data plane
  - 각 router 모두가 수행하는 기능
  - forwarding function
- control plane
  - network-wide logic
  - routing function
  - two control-plane approach
    - traditional routing algorithms
      - 각 라우터가 개별적으로 라우팅 알고리즘을 통해 만든 forwarding table을 관리함
    - software-defined networking(SDN)
      - 네트워크를 내려다보는 서버가 route를 만들어 주는 방식
      - 구글에서 사용하는 방법
      - 5G에 반영된 방법

## 4. Per-router control plane

![Untitled](https://user-images.githubusercontent.com/48282185/180678793-e3695b8b-da26-4ea2-86cb-efa9379b2bbf.png)

- 전통적인 방식
- 각각의 라우터들은 모두 routing algorithm이 구현되어있고, path를 만들기 위해서는 공통된 protocol과 algorithm을 통하여 협력해야한다

## 5. Logically centralized control plane

![Untitled 1](https://user-images.githubusercontent.com/48282185/180678779-4c21e7da-8e11-4712-813b-b8069a2e272d.png)

- SDN 방식
- 하나의 중앙 controller가 모든 local control agents(CAs, reporting하는 용도)와 상호작용
- 라우팅 정책이 바뀌더라도 각각의 라우터의 소프트웨어를 교체해줘야되는 것이 아니라 중앙 컨트롤러의 소프트웨어만 교체하면 되기 때문에 부담이 적고, 이 방향으로 발전 중임

## 6. Network service model

- segment가 network layer로 부터 받아야하는 서비스에 대한 모델
- 각각의 datagram을 위한 서비스 예시
  - 전달을 보장, 중간에 손실나도 복구
  - 40msec 보다 딜레이가 적게 전달을 보장
- datagram의 flow(상호 작용)를 위한 서비스 예시
  - 순서대로 전달
  - flow를 위한 최소한의 bandwidth 보장, 단위 시간 당 몇 개를 forwarding
  - 패킷 사이의 간격을 일정하게 제한, 통화하는 상황을 생각해보면 됨
- 현재 사용되는 IP는 best effort(어떤 부가 기능 없이 최선을 다해 전송하는 기본적인 역할만 수행), 즉 전달 이외에 아무것도 보장해주지 않음

## 7. Router architecture overview

![Untitled 2](https://user-images.githubusercontent.com/48282185/180678789-92f74bfd-091c-469f-a2be-2cbfcdb6c4b3.png)

- router는 특수 목적의 컴퓨터임
- routing을 처리하는 processor와 들어온 신호를 routing에 따라 output링크로 switching해줘야함
- 논리적으로는 input port와 output port를 구분하지만 실제로는 한 port가 둘 다 수행
- 일반적으로 switching은 굉장히 빨리 처리되지만, 느려지는 경우 input에서 queuing delay가 생길 수 있다, 하지만 거의 없다
- 일반적으로 transmission rate보다 fabric으로 부터 도착하는 비율이 더 높기 때문에 output port에서 내보내기 위해 버퍼링이 필요하고, 이로 인해 queuing delay가 생길 수 있다
- 어떻게 스케줄하느냐에 따라 다양한 서비스를 구현할 수 있기에 datagram들 중 어떤 것을 먼저 내보낼 것인가에 대한 scheduling도 필요하다, 예를 들어 위에서 봤던 packet 사이의 간격을 일정하게 유지시켜주겠다 이런 서비스를 스케줄을 이용하여 구현할 수도 있는 것임

## 8. Longest prefix matching

![Untitled 3](https://user-images.githubusercontent.com/48282185/180678791-04b8050f-3124-40b3-bc33-39dae7f25295.png)

- 어디로 내보낼지 어떻게 결정되는가?
- 들어온 패킷의 header의 destination address를 보고 forwarding table에 있는 entry와 매칭시켜보고, prefix가 제일 길게 매칭되는 entry에 적힌 output으로 내보낸다
- 서울로 들어오는 패킷들 중 제일 인구 많은 강남만 따로 처리되는 상황을 가정해보면, 서울까지는 prefix가 일치하다가 그 이후로는 불일치 할 것이므로 제일 많이 일치되는 entry를 선택하는 것임

## 9. HOL blocking

![Untitled 4](https://user-images.githubusercontent.com/48282185/180678792-7ac23758-673e-4c66-908f-878bef503858.png)

- HOL blocking이란 큐 내에서 나갈 수 있는 뒤에있는 요소가 앞의 요소에 가로막혀 나가지 못하는 상황을 의미, 위의 예시에서 연두색 패킷은 나갈 수 있는데, 빨간 색 패킷에 가로막혀 못 나가는 상황을 뜻함

## 10. Packet scheduler

- selection strategies
  - 큐 내에 대기하고 있는 패킷들을 선택하는 전략들
  - FCFS scheduling
  - weighted fair queueing(우선순위가 정해져있는 큐가 여러개)
- drop strategies
  - 큐의 오버플로를 방지하기 위해 버려야할 패킷을 선택하는 전략들
  - 제일 늦게 들어온 것(tail) 버리기
  - 제일 오래된 것(head) 버리기
  - active queue management
    - 큐가 꽉찰 때까지 대기하는 게 아니라 미리 이런 상황을 방지하고자 아무거나 골라서 버리는 전략
    - 실제로 버려지는 것이 아니라 패킷에 마킹을 해서 host들이 큐가 꽉 찬 것을 알게함, 받은 sender는 이를 확인하고, 보내는 량을 조절
