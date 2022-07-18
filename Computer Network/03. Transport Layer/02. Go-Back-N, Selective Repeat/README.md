# 02. Go-Back-N, Selective Repeat

<!--ts-->

- [02. Go-Back-N, Selective Repeat](#02-go-back-n-selective-repeat)
  - [1. Pipelined protocols](#1-pipelined-protocols)
  - [2. Sliding window protocol](#2-sliding-window-protocol)
  - [3. increase utilization](#3-increase-utilization)
  - [4. pipelined protocols - overview](#4-pipelined-protocols---overview)
  - [5. GBN sender](#5-gbn-sender)
  - [6. GBN receiver](#6-gbn-receiver)
  - [7. GBN in action](#7-gbn-in-action)
  - [8. send window size for GBN](#8-send-window-size-for-gbn)
  - [9. Selective repeat](#9-selective-repeat)
  - [10. Selective repeat의 dilemma](#10-selective-repeat의-dilemma)

<!-- Created by https://github.com/ekalinin/github-markdown-toc -->
<!-- Added by: sungminyou, at: 2022년 7월 18일 월요일 19시 52분 29초 KST -->

<!--te-->

## 1. Pipelined protocols

- pipelining
  - client가 한 번에 다 수개의 패킷을 보내고, server도 한 번에 다 수개의 응답으로 처리하는 방식
  - 한 패킷에 대한 ACK을 받지 않고 다음 패킷을 보낸다
- 파이프라이닝을 위해서 순서 번호의 범위는 반드시 증가되야함, stop-and-wait에서 사용했던 0과 1만으로는 커버 안됨
- 어떤 패킷을 없어지거나 망가질 지 모르기때문에 sender와 receiver 모두 buffering을 하고있다
  - sender입장에서는 빠진 것만 버퍼링 했다가 재전송
  - receiver입장에서는 잘 받은 것들은 버퍼링, 빠진 것만 재요청
- 파이프라이닝을 이용하는 일반적 두 가지 기법
  - go-Back-N
  - selective repeat
  - 상용적으로는 이 두 개를 적절히 섞어서 씀

## 2. Sliding window protocol

![1](https://user-images.githubusercontent.com/48282185/179496695-7a095c2c-e09c-4245-8a1c-2689727ce2b1.png)

- pipelining을 부르는 다른 이름
- window는 sender가 보낼 수 있는 패킷의 범위
- window size는 window에 담겨져잇는 패킷의 개수, receiver가 수용가능한 한 최대로 보낸다
- window의 크기는 고정되있다고 가정하지만 실제로는 가변이다

## 3. increase utilization

![2](https://user-images.githubusercontent.com/48282185/179496692-42eb6a54-9fac-4a32-b291-242cef8eade6.png)

- stop-and-wait 방식 대비 window의 크기에 비례해서 utilization이 증가한다
- utilization은 $네트워크를\ 사용한\ 시간 \over 처음\ 데이터를 \ 내보낸\ 후\ 응답을\ 받기까지\ 걸린\ 시간$ 으로 정의
- propagation delay는 15ms로 가정 즉 RTT = 30ms
- utilization을 100으로 활용하기 위한 window size는?
  - $U_{sender} = {{W \times {L \over R}} \over {{L \over R} + RTT}} = 1$ 식을 풀면, $W = 1 + {{RTT} \over {{L \over R}} }$
  - 이 값은 window size의 upper bound이다

## 4. pipelined protocols - overview

- Go-back-N

  - ack을 받지 않은 상태로 N개의 패킷을 파이프라이닝으로 보낼 수 있음
  - receiver는 정상적은 받은 N개의 패킷 중 순서 번호가 제일 큰 ACK만 응답하고(cumulative ACK), sender는 이것을 받으면 그 순서 번호 이전까지 다 받은 것으로 간주

    ![3](https://user-images.githubusercontent.com/48282185/179496690-c1a4b5b4-07fe-494a-a8f1-a7cd0cc589ba.png)

    - 3부터 제대로 못받아서 receiver는 계속 ACK 2를 보낸다(정상적으로 받은 것 중에 순서번호가 제일 큼)
    - 4는 정상적으로 받았음에도 불구하고 버려진다 어차피 3부터 재전송 될 것이기 때문
    - 5도 마찬가지

  - sender는 ack을 받지 못한 가장 오래된 패킷에 대한 타이머 한 개만 가지고 있는다
    - 만약 timer expired되면, 가장 오래된 패킷부터 이후 N개까지 다시 재전송

- Selective repeat

  - ack을 받지 않은 상태로 N개의 패킷을 파이프라이닝으로 보낼 수 있음
  - receiver는 각 패킷에 대한 각각의 응답을 따로 보낸다

    ![4](https://user-images.githubusercontent.com/48282185/179496688-e7db0e86-fd42-406e-a583-6d7bb5c63f99.png)

    - sender는 3번에 대한 ack만 못받으므로 3번의 timer가 expired되면 재전송
    - receiver는 3을 못받아도 4와 5를 버퍼링 해둔다

  - sender는 각 패킷에 대한 타이머가 다 따로 존재한다

- 비교
  - sender 입장
    - timer를 여러 개 유지해야한다는 입장에서는 selective repeat가 부담되는 방식이지만, 저장 공간 측면에서 본다면, go-back-n이 부담되는 방식이다(버퍼링 해야될 게 더 많다)
  - receiver 입장
    - go-back-n은 정상적으로 받은 가장 큰 순서 번호만 유지하고 있으면 되고, 받지 못한 패킷 이후의 패킷들은 버퍼링할 필요가 없으므로 굉장히 simple하다, 반면 selective repeat는 각 패킷에 대한 순서 번호를 각각을 관리해야하고, 버퍼링도 해야하므로 go-back-n에 비해 부담이 크다, receiver가 단순하게 구현되거나 사양이 안 좋은 기기라면, go-back-n이 더 현명한 방법

## 5. GBN sender

![5](https://user-images.githubusercontent.com/48282185/179496687-d929312d-8362-4ced-a4ce-b790ff0adba0.png)

- go-back-n을 이하 GBN이라고 표현
- 패킷의 헤더에 k bit의 순서 번호를 할당
- n은 window size를 의미
- 전송했지만 ACK을 받지 못한 가장 오래된 패킷에 대한 타이머 하나만을 가지고 있고, 만약 expired된다면, 보냈지만, ack을 받지 못한 전부를 다시 보낸다(위 예시에서 노란색 막대기들)
- FSM

  ![6](https://user-images.githubusercontent.com/48282185/179496684-caf247fb-a7fd-4ae9-845e-4881f6f77c31.png)

  - base window의 가장 왼쪽 패킷의 순서 번호
  - nextseqnum < base + N == 다음 보낼 패킷 순서 번호가 window 내에 포함 됨
  - sndpkt[] → buffering을 위한 배열
  - base = getacknum(rcvpkt) + 1 → sliding
  - base == nextseqnum → 다음 보낼 순서 번호가 base와 같다면, 이전 순서 번호의 패킷은 잘 받은 것이므로 그것에 대한 타이머를 종료시킨다
  - base ≠ nextseqnum → window 내에 아직 ack을 받지 못한 패킷이 있으므로 타이머 갱신

## 6. GBN receiver

- FSM

  ![7](https://user-images.githubusercontent.com/48282185/179496683-83abdf50-65ed-4625-8da7-1694bb4128a4.png)

  - expectedseqnum만 가지고 있으면 됨

## 7. GBN in action

![8](https://user-images.githubusercontent.com/48282185/179496681-6e3c33af-8ef3-4ec8-aec5-70457a04c3f7.png)

- ack 1을 중복으로 받는다
- ack i가 손실되어도 ack i+1을 받으면 i+1까지 잘 받은 것으로 간주되고, 정말 극단적으로 ack i, i+1, i+2, i+3이 손실되어도 문제가 없는게, receiver가 expectedseqnum으로 ack을 주기 때문에, sender가 packet i, i+1, i+2, i+3을 재전송 해도 receiver는 받지않고 expectedseqnum을 지속적으로 요청해서 정상적으로 복구될 수 있다

## 8. send window size for GBN

![9](https://user-images.githubusercontent.com/48282185/179496676-3b3cdd60-bfe0-4e7c-82a2-db221647545c.png)

- ack이 모두 손실된 극단적 상황을 가정
- receiver가 유지하는 expectedseqnum은 0이고, sender는 0에 대한 timer가 expired돼서 0 ~ 3까지 재전송한다. 그래서 receiver가 받은 0번은 잘못된 패킷인데도 불고하고, accepted된다
  - 파란색 0과 흰색 0은 다른 패킷, 순서 번호만 같은 거임
- 윈도우 사이즈와 순서번호가 같기 때문에 발생하는 상황, 그래서 순서 번호는 윈도우 사이즈보다 충분히 커야한다

## 9. Selective repeat

- receiver에도 window가 존재함(=buffer)

![10](https://user-images.githubusercontent.com/48282185/179496671-36d3fcee-aa6f-4fa3-8d2e-9fa5abaf4c45.png)
![11](https://user-images.githubusercontent.com/48282185/179496664-f3294a46-c839-4687-b501-b80db5b42aa3.png)

- ack이 손실되고, sender가 재전송하면 그에 맞는 ack을 재전송 해줘야 sender의 window가 sliding이 정상적으로 돼서 정상적으로 재개할 수 있다
- sender
  - window 내에 포함되는 순서 번호를 붙여서 전송, 만약 순서 번호가 window에 포함되지 않으면 무시
  - timeout되면 해당 패킷만 재전송하고, 다시 문제가 생길 수 있으므로 timer재가동
  - ack i를 받으면 i를 받았다는 것을 체크하고, 만약 i가 base였다면, sliding하면 됨
- receiver
  - packet i를 받으면 ack i를 보낸다
  - 순서에 맞지 않지만, window 범위 내에 있으면 버퍼링
  - 만약 어떤 패킷이 도착해서 연속적인 범위의 패킷 묶음이 발생한 경우 application으로 데이터 전송하고, sliding
    - `1`, 2, 3, `4` ,5
    - 1을 못 받고 4를 못 받은 상황을 가정
    - 2, 3은 받았으므로 버퍼링
    - 1이 sender로 부터 재전송 되면 1, 2, 3 연속적인 범위의 패킷 묶음이 생겼으므로 1, 2, 3을 application으로 전송하고 sliding
    - window가 다 채워지기까지 기다리지 않는다, 연속 묶음 발생하면 보낸다
  - 잘 받은 패킷에 대한 재전송이 온 경우 해당 번호의 ack을 다시 보내준다

## 10. Selective repeat의 dilemma

![12](https://user-images.githubusercontent.com/48282185/179496648-7f1bfb42-ccb6-4fab-af18-4c98f032beaa.png)

- GBN과 Selective Repeat 모두 packet loss를 고려하여 설계된 것들이지만 ack 손실에 대해서는 고려가 되지 않음
- 이 문제는 순서 번호의 size < sender window size + receiver window size이기 때문에 발생함
  - 순서 번호의 size ≥ sender window size + receiver window size면 문제 없어짐
  - 최악의 케이스 작성해서 문제 없나 테스트해보면 쉽게 이해감
