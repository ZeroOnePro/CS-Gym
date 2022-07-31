# 3. MAC

<!--ts-->

- [3. MAC](#3-mac)
  - [1. Multiple access protocols](#1-multiple-access-protocols)
  - [2. An ideal multiple access protocol](#2-an-ideal-multiple-access-protocol)
  - [3. MAC protocols의 종료](#3-mac-protocols의-종료)
  - [4. Channel partitioning MAC protocols](#4-channel-partitioning-mac-protocols)
  - [5. Random access protocols](#5-random-access-protocols)
  - [6. ALOHA](#6-aloha)
  - [7. Slotted ALOHA](#7-slotted-aloha)
  - [8. CSMA](#8-csma)
  - [9. CSMA / CD(Colision Detection)](#9-csma--cdcolision-detection)
  - [10. Vulnerable Time](#10-vulnerable-time)
  - [11. Binary(exponential) backoff](#11-binaryexponential-backoff)
  - [12. CSMA / CD flow chart](#12-csma--cd-flow-chart)
  - [13. Taking turns MAC protocols](#13-taking-turns-mac-protocols)

<!-- Created by https://github.com/ekalinin/github-markdown-toc -->
<!-- Added by: sungminyou, at: 2022년 7월 31일 일요일 16시 04분 11초 KST -->

<!--te-->

## 1. Multiple access protocols

- 하나의 링크를 서로 다른 단말들이 공유할 때 구분하기 위해서 필요
- 두 개 이상의 전송이 하나의 링크에서 동시에 일어날 때 간섭이 발생한다
  - 충돌은 같은 시각에 두 개 이상의 신호들을 한 노드가 받을 때 발생
- 하나의 채널을 여러 단말들이 어떻게 공유할 것인지 결정짓는 분산 알고리즘
  - 중앙 통제를 따르지 않고, 단말들이 각각 언제 전송할 수 있는지 결정짓는다(단말들에서 각각 실행되기에 분산이라는 단어를 사용)
- 빠르게 넘겨줘야하는 link layer특성 상 공유를 위한 다른 채널을 두는 것은 복잡하고 좋은 해결책이 아니다, 그래서 전송하는 링크를 통해 언제 보낼지를 결정 짓는다

## 2. An ideal multiple access protocol

- 아래의 조건들을 목표로 두고 설계
- R bps의 전송 비율을 갖는 broadcast채널이 공유될 때
  - 한 노드만 전송한다고 할 때 R bits를 꽉 채워서 보낸다
  - M개의 노드가 동시에 전송한다고 할 때 평균적으로 R / M bits씩 꽉채워서 보낸다
  - 완전한 탈중앙화
    - 전송을 통제하는 특별한 노드를 두지 않는다
    - 동기화를 위한 클럭이나, 슬롯들을 두지 않는다
  - 간단해야한다

## 3. MAC protocols의 종료

- MAC = Media Access Control
- best는 없다 상황에 맞춰서 제일 좋아보이는 것을 사용
- channel partitioning
  - channel을 시간 슬롯, 주파수 단위의 작은 조각으로 분할
  - 조각들을 각 노드에 할당해서 독점해서 사용할 수 있도록하는 방법
  - 앞서 봤던 동기화를 위한 슬롯이 아니라 각 노드들을 위한 슬롯이다
  - 안 보내는 노드들에게도 무조건 할당되므로 낭비가 일어날 수 있다
- random access
  - 필요할 때 아무때나 보낼 수 있도록 하자
  - 채널을 나누지 않고, 충돌을 허용한다
  - 대신 충돌이 발생했을 때 복구할 수 있게 만들어야한다
- taking turns
  - 보내는데 순서를 두자
  - 돌아가면서 보내는데 보낼 데이터의 량이 많은 노드의 경우는 더 길게 보낼 수 있도록 한다
  - 자칫하면 길어지면 starvation되는 노드들이 생길 수 있다

## 4. Channel partitioning MAC protocols

- 모든 단말이 쉬지 않고 보낼 데이터가 있을 때 유용한 방법
- TDMA, Time Division Multiple Access

  ![Untitled](https://user-images.githubusercontent.com/48282185/182014302-31ff060d-6506-4e19-8447-2e9ad703a16a.png)

  - round, 모든 노드들이 한 번씩 다 전송하고 난 단계
  - 고정된 길이(packet transsmission time)의 슬롯을 각 라운드마다 할당
  - 안 쓰이는 slot들은 낭비
  - m개의 노드가 있을 때 round는 m개의 슬롯을 갖는다

- FDMA, Frequency Division Multiple Access

  ![Untitled 1](https://user-images.githubusercontent.com/48282185/182014287-3e2abcc2-1b57-486e-b7fc-4be6b9491f37.png)

  - channel spectrum을 frequency band들로 나눈다
  - 각 노드들은 하나의 고정된 frequency band를 할당받는다
  - 사용되지 않는 band는 낭비된다

## 5. Random access protocols

- 어떤 노드가 보낼 패킷이 있을 때 링크의 용량을 전부 사용해서 보낸다
- 링크가 한가할 때 유용한 프로토콜, 단말이 많으면 많을 수록 충돌날 확률이 올라가니 단말이 많은 환경에서는 그다지 좋지 못한 방법
- 보내기 전에 노드들 사이에 미리 조정하는 매커니즘이 없다, 그래서 랜덤인 것임, 만약 조정했다면 랜덤하게 못 보내지
- 두 개 이상의 노드가 보낼 때 collision이 발생한다
- collision때문에 random access 방식을 사용하면 다음과 같은 spec을 구현해야함
  - collision detect
  - recover collision(각 노드들이 충돌 후에 보내기 까지 대기하는 시간을 차등화한다)
- 종류
  - ALOHA
  - Slotted ALOHA
  - CSMA, CSMA / CD, CSMA / CA

## 6. ALOHA

![Untitled 2](https://user-images.githubusercontent.com/48282185/182014290-5e15d4a0-8565-4b00-9d88-382c8789a421.png)

- 간단, 동기화 없음
- link layer에 패킷이 도착하면 즉시 전송
- 각 노드들이 데이터를 간헐적으로 보내는 것이 아닌 이상 충돌날 확률이 너무 높다

## 7. Slotted ALOHA

![Untitled 3](https://user-images.githubusercontent.com/48282185/182014291-c0a457a5-7768-4824-92f0-47151f48b9d4.png)

- 가정
  - 모든 프레임의 크기는 동일
  - 동일한 길이(1 frame의 transmission delay)의 slot으로 시간을 분할
  - 노드들은 슬롯의 시작점에서만 transmission을 시작할 수 있다
  - 노드들은 동기화되어있다(슬롯이 언제 시작되는지 모든 노드들이 알고있다)
  - 두 개 이상의 노드가 하나의 슬롯이 시작할 때 동시에 보낼 때, 충돌이나고, 모든 노드는 충돌을 감지할 수 있다(unslotted ALOHA보다 충돌확률을 획기적으로 줄었다)
- operation
  - network layer에서 데이터 전송 요청이 오면 다음 slot이 시작될 때까지 대기한다
    - 만약 충돌 없으면, 다음 slot에 데이터를 싣는다
    - 만약 충돌 있으면, 성공할 때까지 보낼 확률 p를 가지고 후속 slot을 통해 재전송한다, 여기서 p는 다음 slot에 보낼지 말지 정할 수 있는데 보내는 확률을 의미하고, 충돌나면 값이 조정되고, 노드들마다 모두 다른 값을 가짐
- 장단점
  - 장점
    - 하나의 active node만 있다고 했을 때, channel의 full rate로 지속적으로 이용하며 전송할 수 있다
    - 높은 탈중앙성, 노드들은 slot에 대한 sync만 맞추면 된다
    - 간단하다
  - 단점
    - 충돌은 여전히 피할 수 없다, 낭비되는 슬롯이 생긴다
    - 노는 슬롯도 생긴다
    - 데이터를 내보내기 까지 오랜 시간이 걸릴 수 있다(위의 예시보면 이해갈 것)
    - clock synchronization이 필요

## 8. CSMA

- Carrier Sense Multiple Access
- 동기를 맞추는 건 부담스럽다
- ALOHA의 문제는 링크가 사용 중인지 살펴보지 않고, 데이터를 내보내서 충돌이 많이 났던 것이다
- 전송 전에 미리 링크가 사용 중인지를 본다
- 만약 채널이 idle로 감지되면, 전체 frame을 내보낸다
- 만약 채널이 busy로 감지되면, 전송을 미룬다
- collision은 여전히 있다
  ![Untitled 4](https://user-images.githubusercontent.com/48282185/182014293-cc33eea7-83aa-473c-97d3-a878ab4190e7.png)
  - propagation delay, 같은 링크 상에서 어떤 노드가 내보냈을 때 다른 노드까지 신호가 도착하기까지 delay가 있어서 도착하기 전에 link가 idle하다고 판단하여 다른 노드가 내보내면 충돌난다
  - y축이 시간이고, 시간이 지날 수록 B가 보낸 신호가 나머지 노드들에 도착하는 시간을 초록색 도형으로 표현한 것임 $t_{0} + \alpha$시점에는 B가 보낸 신호가 아직 D에 도착하지 않았고, D는 link가 idle하다고 판단하여 신호를 내보낸다
  - CSMA에서는 충돌 후 복구 매커니즘이 존재하지 않고, 다른 layer에 맡긴다

## 9. CSMA / CD(Colision Detection)

- CSMA에서는 복구를 안했다면, 여기서는 충돌을 감지하고 재전송하는 매커니즘을 추가
- 데이터를 내보낸 뒤, 일정 시간동안 collision을 감지한다
- 충돌을 감지하면, 기다렸다가 재시도
- 충돌 감지

  - 유선 링크에서는 내보내고 있는 신호와 링크 상에서 흐르고 있는 신호를 비교하면 충돌이 났는지 쉽게 확인할 수 있다
  - 무선에서는 신호의 감쇄가 급격하게 일어나고, 간섭도 많기 때문에 유선과 같은 방법은 사용 불가능

    ![Untitled 5](https://user-images.githubusercontent.com/48282185/182014294-d359fd36-928d-4e70-9648-41cf36dbe76d.png)

  - B와 D모두 충돌을 감지하고 난 후에는 전송을 멈추기 때문에 CSMA에서의 예시 보다 각 노드가 링크를 사용할 수 있는 시간이 약간 늘어났다, 이전에는 충돌이 나건말건 무조건 보냈기 때문에 B와 D가 내보내는 중에는 모든 노드가 대기 상태에 있었음
  - 이 방식은 신호를 내보내고 있는 와중에 상대방의 신호를 감지해야 충돌을 확인할 수 있는데, 작은 량의 데이터를 보내고 그 이후 상대방의 신호가 도착했을 때는 감지할 수 없다, 이로 인해 초기의 이더넷은 프레임의 크기에 제한을 둔다던지, 링크의 길이에 제한을 두는 경우가 있었다

## 10. Vulnerable Time

![Untitled 6](https://user-images.githubusercontent.com/48282185/182014295-e372b7c2-30b8-4e5f-9c75-31ecf54dfa82.png)

- 신호가 propagation되어, 감지하기 전까지 신호를 내보내서 충돌될 수 있기에 위험 구간으로 지칭
- 문제

  - 초기 이더넷에서 CSMA / CD에서 충돌 감지 못하는 상황을 회피하기 위해 거리에 제한을 뒀다 했는데 이 거리는 어떻게 정해지는 것인가?

    ![Untitled 7](https://user-images.githubusercontent.com/48282185/182014296-425b30d2-ef22-4f9b-bed1-db5e1babe82f.png)

  - 초기 이더넷에서 CSMA / CD에서 충돌 감지 못하는 상황을 회피하기 위해 프레임 크기에 제한을 뒀다 했는데 이 거리는 어떻게 정해지는 것인가?

    ![Untitled 8](https://user-images.githubusercontent.com/48282185/182014299-a572ca0f-b6bf-4dd0-92d4-a84a37cb63c3.png)

  - 충돌 감지를 못하는 문제를 피하기 위해서는 propagation delay보다 transmission delay가 커야함이 핵심이다

## 11. Binary(exponential) backoff

- 충돌 후 각 노드들은 일정 시간만큼 대기했다가 재전송하는데, 이 대기시간을 정하는 매커니즘이다
- m개의 collision이 발생했다 했을 때 k개의 노드들은 [0, $2^{m-1}$]구간에서 숫자를 랜덤하게 고르고 그 숫자와 512bit를 내보내는데 걸리는 시간을 곱해서, k \* 512 bit times를 기다렸다가 재전송한다, 물론 그 시간이 지났는데, 링크가 사용 중이라면 더 기다려야함
- m과 숫자 구간은 비례하므로 충돌이 많이 날수록 각 노드들의 대기 시간을 더 다분화해야되기에, 구간의 길이를 늘린다
- 이 재전송을 무한정하는 게 아니고, 재전송 시도 횟수를 카운트한다음에, 15가 넘어가면 위 layer에게 맡기고, 폐기한다

## 12. CSMA / CD flow chart

![Untitled 9](https://user-images.githubusercontent.com/48282185/182014300-1581d133-e7b0-4ec2-a0ae-511c4e37b74e.png)

- persistence methods는 링크가 idle임을 감지했을 때 바로 보낼거냐, 아니면 기다렸다 보낼거냐에 대한 여러 가지 전략을 의미

## 13. Taking turns MAC protocols

- Polling
  - 중앙의 특별한 노드가 데이터 전송을 조정한다
  - 네트워크에 연결된 단말들이 충돌을 감지하고, 재전송하고 이러기에는 사양이 부족한 부족한 단말들일 때 유용한 방법
  - round robin이므로 master는 polling에 드는 overhead가 있고, 각 단말들은 바로 못 보내고, master가 물어봐 줄 때까지 기다려하고(latency), master가 다운되면 네트워크가 다운되는 문제가 있다
- token passing
  - Token을 가진 노드만 전송 가능하다
  - 보낼 데이터가 없다면 토큰을 다음 단말에게 넘긴다
  - 데이터가 성공적으로 보내졌는지 확인하는 방법은 링크로 내보내고, 링크를 한바퀴 돌아 다시 자기자신에게 왔을 때 깨져있지 않다면, 제대로 전송된 것이기에 토큰을 다음 단말에게 넘긴다
  - 토큰을 관리하는 overhead, 토큰이 올 때까지 못보내고(latency), 토큰이 망가지면 네트워크가 다운되는 문제가 있다
- bluetooth, FDDI, token ring이 taking turn방법을 이용하는 대표적 프로토콜이다
