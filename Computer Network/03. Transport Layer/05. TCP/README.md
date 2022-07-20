# 05. TCP

<!--ts-->

- [05. TCP](#05-tcp)
  - [1. TCP outline](#1-tcp-outline)
  - [2. TCP overview](#2-tcp-overview)
  - [3. TCP segment structure](#3-tcp-segment-structure)
  - [4. TCP sequence number, Ack number](#4-tcp-sequence-number-ack-number)
  - [5. TCP RTT, timeout](#5-tcp-rtt-timeout)
  - [6. TCP reliable data transfer](#6-tcp-reliable-data-transfer)
  - [7. TCP sender events](#7-tcp-sender-events)
  - [8. TCP retransmission scenarios](#8-tcp-retransmission-scenarios)
  - [9. TCP ack generation](#9-tcp-ack-generation)
  - [10. TCP fast retransmit](#10-tcp-fast-retransmit)

<!-- Created by https://github.com/ekalinin/github-markdown-toc -->
<!-- Added by: sungminyou, at: 2022년 7월 20일 수요일 16시 39분 48초 KST -->

<!--te-->

## 1. TCP outline

- connection oriented transfer
  - 상대방이 준비되었을 경우 전송 시작
- TCP connection 과정
- TCP segment 구조
- RTT 추정과 timeout
- 신뢰성 데이터 전송
- flow control
- connection management
- congestion control
  - 이거 때문에 다양한 버전이 나옴
  - RFC 793, 1122, 1323, 2018, 2581, …

## 2. TCP overview

- full duplex data
  - 같은 connection 상에서 양방향으로 데이터 전송
  - MSS: maximum segment size
- connection oriented
  - handshaking
- flow control
  - sender는 receiver의 수용량을 초과하면 안됨
- point-to-point
  - 하나의 sender가 하나의 receiver에게 보냄
- reliable, in-order byte stream
  - application message를 의미 단위로 잘라주지는 않음, MSS 단위로 자름
- pipelined
  - congestion과 flow control이 window size를 결정한다

## 3. TCP segment structure

![1](https://user-images.githubusercontent.com/48282185/179925025-8943abf8-de4f-44a5-af15-3472d30937d1.png)

- sequence number
  - segment data의 첫 번째 byte의 번호, 즉 분할된 message 당 하나 씩 부여됨
- acknowledgements
  - 받기 원하는 순서 번호
  - comulative ACK

![2](https://user-images.githubusercontent.com/48282185/179925022-0c91db66-4f23-4155-80ab-5ef4d4e3d4d3.png)

- MSS 20Byte일 때 순서번호
- 첫 순서 번호가 결정되면, 기본적으로 그 이후로는 공차가 MSS인 등차수열로 순서 번호의 수열이 만들어짐, 근데 push flag나 urg flag같이 고정된 크기가 아닌 것들도 있어서 seq + length로 다음 순서 번호가 결정됨
- flags
  - U(URG): 긴급 데이터
  - A(ACK): ACK이 유효한 데이터인지, 응답에서는 true로 설정
  - P(PSH): Push data, URG보다는 덜 급한 데이터
    - TCP는 MSS만큼 데이터를 채울 때까지 기다렸다가 전송하는데, 이 때문에 수신자가 못받는 상황이 벌어질 수 있는데, 이 flag를 설정하면 버퍼를 꽉 채우지 않고 즉시 보낸다, receiver에서도 순서가 맞을 경우 바로 application으로 전달한다
  - R(RST): 연결 급 종료
  - S(SYN): 연결 시작
  - F(FIN): 연결 종료

## 4. TCP sequence number, Ack number

![3](https://user-images.githubusercontent.com/48282185/179925018-85906206-6098-4e1e-ae46-6163c128dad4.png)

- seq 42는 연결과정에서 설정한 순서 번호의 초기값임, 아무 번호나 가능
- 서버에 요청해서 돌아오는 응답의 ACK이 118임을 볼 수 있는데, seq(42) + length(76)이기 때문이다, 그리고 이 번호는 다음 순서번호로 사용됨

## 5. TCP RTT, timeout

- 정상적으로 보내진 데이터가 timeout이 안나기 위해서 RTT를 계산할 필요가 있다
- timer 세팅을 RTT보다 크게 잡아야함
  - RTT가 가변적인 것인 문제
- 짧게 설정할 경우
  - 조급한 timeout으로 인해 불필요한 재전송이 발생
- 길게 설정할 경우
  - 재전송해야하는 경우에도 불구하고, 재전송 응답이 너무 느려서, receiver에서 답답함을 느낄 수 있다
- estimate RTT

  - sampleRTT

    - 바로 이전의 요청에 의한 응답까지 걸린 시간
    - 재전송에 대한 것은 무시

      - 재전송에 대한 응답인지, 원본 데이터에 대한 응답인지 Host A 입장에서는 알 길이 없기 때문임

        ![4](https://user-images.githubusercontent.com/48282185/179925015-117c147a-d98a-41cf-a04c-151be78e80c5.png)

  - sampleRTT는 매우 가변적이기에, 바로 사용할 수 없다
    - 그래서 sampleRTT의 평균을 내서 사용함
  - exponential weighted moving average

    - 지수적으로 과거의 기록들의 비중을 감소시키고 현재를 최대한 반영한다
    - 추세는 따라가지만 값을 급격하게 바꾸지는 않는다
    - Typical value $\alpha = 0.125$
    - EstimatedRTT = $(1 - \alpha) * EstimatedRTT + \alpha * sampleRTT$

      ![5](https://user-images.githubusercontent.com/48282185/179925011-3e58f62f-0129-4f76-ba02-23d1478d318b.png)

  - Timeout interval
    - 지수 가중 이동 평균 값과 실제 값의 격차가 크기 때문에 margin을 추가한다
    - 차이가 크면 클수록 margin을 늘림
  - Estimate sampleRTT deviation from estimatedRTT
    - Typical value $\beta = 0.25$
    - $DevRTT = (1-\beta) \times DevRTT + \beta \times (| SampleRTT - EstimatedRTT |)$
  - timeout interval
    - estimatedRTT + 4 \* DevRTT(margin)

## 6. TCP reliable data transfer

- TCP는 unreliable service를 제공하는 IP위에서 신뢰성 전송 서비스를 해야한다
  - pipelined segments
  - cumulative acks
  - single transmission timer
- 재전송 trigger
  - timeout event
  - duplicate acks

## 7. TCP sender events

- TCP sender가 동작하는 이벤트
- duplicated ack과 congestion, flow control이 없는 쉬운 상황을 가정
- data received from app
  - 순서번호 #인 segment를 생성
  - 순서번호 #은 segment의 first data byte의 byte-stream 번호
  - timer가 돌고 있는 상태면 그냥 보내고(base는 아니지만 window내에 있는 segment), 안 돌고 있으면 설정(base)
- timeout
  - segment 재전송
  - 이것 마저 제대로 안 갈 수 있으니 timer설정
- ack received
  - GBN의 시나리오를 따름
  - ack을 받으면 sliding window해서 새로 설정
  - 새로운 window에 아직 ack을 받지 못한 segment(new window base)에 대한 타이머 새로 설정
    - sliding한 window가 아직 전송 전이면 timer설정은 필요없고, 전송 전까지 대기 후 전송할 때 타이머 설정

## 8. TCP retransmission scenarios

- 여기서 ACK은 다음에 받아야할 데이터의 번호를 의미함
- 패킷 손실

  ![6](https://user-images.githubusercontent.com/48282185/179925006-865eb6f4-ee98-4db7-ada9-2b77fff7790b.png)

- cumulative Ack, 조급한 timeout

  ![7](https://user-images.githubusercontent.com/48282185/179924999-530c308a-0234-43ae-9deb-0f0cfdabd324.png)

## 9. TCP ack generation

- RFC 1122와 2581에 있는 고전적인 방법, 현재와는 다름

| event at receiver                                                                           | action                                                                                                                                 |
| ------------------------------------------------------------------------------------------- | -------------------------------------------------------------------------------------------------------------------------------------- |
| 기대하는 순서 번호를 가진 segment가 도착한 상황에서 ACK을 보낼 segment가 없는 경우          | 다음 segment가 도착하기 전까지 500ms정도 기다린 후 안 오면, 바로 ack을 보낸다(pipeline이니 cumulative ack을 만들기 위해) → Delayed ACK |
| 기대하는 순서 번호를 가진 segment가 도착한 상황에서 ACK 전송을 대기하는 segemnt가 있는 경우 | 곧바로 single cumulative ACK을 보낸다                                                                                                  |
| 기대하는 것보다 높은 순서 번호를 가진 segment가 도착하는 경우                               | 곧바로 기대하는 순서 번호를 가진 ACK을 보낸다                                                                                          |
| 중간에 누락된 segment가 부분적으로 도착하거나 한꺼번에 도착했을 경우                        | 꽉 찬 부분까지해서 cumulative ACK을 보낸다                                                                                             |

## 10. TCP fast retransmit

![8](https://user-images.githubusercontent.com/48282185/179924968-b6da11c2-0290-4b0c-b289-bd06f5297097.png)

- duplicated ACK을 받았을 때 데이터의 손실을 가정하고 재전송하는 것
- ack을 받고 재설정된 time interval 사이(충분히 길게 설정됨)에 duplicated ACK을 받아 재전송이 일어날 수 있기에 fast retransmit라고 한다(timer보다 일찍 패킷이 손실을 감지)
- 세 개의 duplicated ACK을 받으면 그 순서 번호의 segment를 재전송한다(triple dulicate ACKs, four identical ACKS)
  - 중복 ACK을 받자마자 곧바로 보내면 되지 왜 세 번이나 기다렸다가 보내는가?
  - 네트워크의 특성 상 어느 path를 통해 전달될지 모르기에 위 예시에서 seq=92 segment의 ACK이 seq=120 segment의 ACK보다 긴 경로를 거쳐서 전달되어 더 늦게 도착한다고 해보자 host A의 입장에서는 120의 ACK인지 92의 ACK인지 알 길이 없다, 다시 말해 재전송이 필요한 경우인지 아닌지 알 수가 없다
  - 세 개는 최소의 개수를 의미하는 것이고 이보다 실제로는 더 많이 설정될 수 있음
