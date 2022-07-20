# 07. TCP congestion control

<!--ts-->

- [07. TCP congestion control](#07-tcp-congestion-control)
  - [1. Additive increase multiplicative decrease(AIMD)](#1-additive-increase-multiplicative-decreaseaimd)
  - [2. TCP congestion control](#2-tcp-congestion-control)
  - [3. Slow start](#3-slow-start)
  - [4. Congestion avoidance](#4-congestion-avoidance)
  - [5. Slow start and Congestion avoidance example](#5-slow-start-and-congestion-avoidance-example)
  - [6. detecting, reacting to loss](#6-detecting-reacting-to-loss)
  - [7. TCP Reno example](#7-tcp-reno-example)

<!-- Created by https://github.com/ekalinin/github-markdown-toc -->
<!-- Added by: sungminyou, at: 2022년 7월 20일 수요일 19시 25분 45초 KST -->

<!--te-->

- linux에서 사용되는 TCP는 TCP Cubic(2008)과는 차이가 있다 여기서 배우는 것은 TCP Tahoe(1983)와 TCP Reno(1990)

## 1. Additive increase multiplicative decrease(AIMD)

![1](https://user-images.githubusercontent.com/48282185/179960024-25c86527-7d50-4764-8545-5fd9e32e4220.png)

- approach
  - loss가 생기기 전 까지 transmission rate(window size)를 증가시킴, 사용가능한 bandwidth 탐색
- additive increase
  - loss가 감지되기 전 까지 매 RTT마다 1 MSS 씩 cwnd를 증가한다(선형 증가)
- multiplicative decrease
  - loss가 감지되면 cwnd를 절반으로 줄임

## 2. TCP congestion control

- sender의 전송 한계
  - swnd = min(rwnd, cwnd)
  - 일반적으로 rwnd >> cwnd
- cwnd는 네트워크 혼잡도에 따른 동적인 값이다

  ![2](https://user-images.githubusercontent.com/48282185/179960019-7f9cc63d-befe-478f-b9e8-4065ae2df616.png)

  - LastByteSent - LastByteAcked ≤ cwnd

- TCP sending rate
  - RTT동안 cwnd bytes만큼 보내는 것이므로 $rate \approx {cwnd \over RTT}$bytes/second
  - 어느 한 시점에서 cwnd에 따라 전송률이 결정된다

## 3. Slow start

![3](https://user-images.githubusercontent.com/48282185/179960017-60f2bee9-c61c-4deb-bc0c-12c7481dde5e.png)

- congestion control의 첫 번째 phase
- 처음에는 작은 값으로 시작하여 지수적으로 rate를 늘리다가 첫 loss나 임계값(ssthresh)에 다다르면 rate를 줄인다
- initially cwnd = 1 MSS
- 매 RTT마다 cwnd를 2배 증가
- 매 ACK마다 cwnd를 1 증가 시킴
- RTT가 길면 아무리 지수적 증가라도 찾는데 오래걸리므로 이것 조차 빨리하자는 시도가 있음

## 4. Congestion avoidance

![4](https://user-images.githubusercontent.com/48282185/179960012-5c8ee1e4-45a4-4f2e-b638-0385be5624bd.png)

- congestion control의 두 번째 phase
- slow start로 loss없이 ssthresh까지 증가한 경우 cwnd ≥ ssthresh
- 각각의 ACK마다 partial하게 증가
  - cwnd = cwnd + $MSS^2 \over cwnd$
- cwnd의 linear한 증가
  - 매 RTT마다 cwnd = cwnd + MSS
- cwnd = 8인 경우

  ![5](https://user-images.githubusercontent.com/48282185/179960009-114b8ba1-4660-44e0-ab84-64c2e144bff6.png)

## 5. Slow start and Congestion avoidance example

![6](https://user-images.githubusercontent.com/48282185/179960005-c1592591-2fa4-428c-9e11-c395be382c94.png)

- cwnd = [cwnd]
  - 4 = [4.75]

## 6. detecting, reacting to loss

- loss detecting
  - timeout
  - 3 duplicate ACKs
- 위의 두 가지 Loss의 상태가 같은가? No
  - 3 duplicate ACKs는 일부 패킷의 손실만을 예상할 수 있지만 충분히 많이 보냈는데 timeout은 다량의 패킷 손실이 예상됨
  - 그래서 각 loss 상태에 따라 다르게 대응함
- timeout에 의한 loss 감지 시
  - cwnd를 1 MSS로 세팅
  - 이후에는 slow start처럼 지수적으로 증가시켜서 다시 treshold를 찾는다
- 3 duplicate ACKs에 의한 loss 감지 시
  - 몇몇의 segments는 올바르게 전달할 수 있음
  - cwnd를 반으로 줄임(AIMD), 이후 선형 증가시켜서 treshold를 찾는다
  - timeout보다 많은 경우
- TCP tahoe에서는 timeout과 dup acks를 둘 다 cwnd를 1로 줄여버렸는데 reno에서는 dup acks 시 cwnd를 절반으로만 줄이는 방식으로 변경

  ![7](https://user-images.githubusercontent.com/48282185/179960002-b5a60994-73d2-4701-904d-c1c5b39f8a93.png)

## 7. TCP Reno example

![8](https://user-images.githubusercontent.com/48282185/179959982-f040e782-ec36-4869-b3f8-a5852ffcba31.png)

- slow start 구간
  - A - B, F -
- congestion avoidance 구간
  - B - C, D - E
- D에서 window size 감소 이유
  - 3 dup ACKs
- F에서 window size 감소 이유
  - timeout
