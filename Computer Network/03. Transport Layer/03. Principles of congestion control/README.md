# 03. principles of congestion control

<!--ts-->

- [03. principles of congestion control](#03-principles-of-congestion-control)
  - [1. principles of congestion control](#1-principles-of-congestion-control)
  - [2. causes / costs of congestion](#2-causes--costs-of-congestion)
  - [3. Approach towards congestion control](#3-approach-towards-congestion-control)

<!-- Created by https://github.com/ekalinin/github-markdown-toc -->
<!-- Added by: sungminyou, at: 2022년 7월 19일 화요일 15시 18분 13초 KST -->

<!--te-->

## 1. principles of congestion control

- congestion
  - 너무 많은 소스가 너무 많은 데이터를 너무 빠르게 전송하여 네트워크가 처리할 수 없음
  - 라우터의 처리 용량보다 트래픽이 많은 경우
- flow control과 차이가 있음
  - flow control은 receiver가 수용할 수 있는 만큼 맞춰서 보내는 것
- 야기되는 문제
  - lost packets(라우터의 buffer overflow) → 재전송 유발
  - long delays(큐잉 지연) → 타이머에 설정된 시간보다 딜레이가 길면 재전송
- 해결하기 어려운 문제 중 하나
  - layered architecture이기에 서로 에게 서비스만 요청할 뿐 데이터를 주고 받지 않아 서로에 대한 정보를 모르기때문에 추정을 통해 해결해야함, 추정에 따라 너무 오버일 수도 있고, 너무 언오버일 수 있다

## 2. causes / costs of congestion

- 라우터의 버퍼가 가득 찬 경우 패킷이 손실됨
- sender가 조급하게 timer를 설정해서 원본과 재전송 데이터가 둘 다 전달됨

![1](https://user-images.githubusercontent.com/48282185/179679016-8b834b20-1844-4fb6-9ecd-a58f70db14a5.png)
![2](https://user-images.githubusercontent.com/48282185/179679000-325dc2b8-13f2-4647-8216-5ae3ada520a1.png)

- $\lambda_{in} <= \lambda^\prime_{in}$
  - 재전송이 있을 수 있기 때문에 application이 보내는 데이터보다 transport가 보내는 데이터는 같거나 크다
- $\lambda^\prime_{in} >= \lambda^\prime_{out}$
  - 중간에 패킷 손실이 날 수 있기에 sender의 transport가 보내는 데이터의 량이 receiver의 transport가 받는 데이터의 량과 같거나 크다
- $\lambda^\prime_{out} >= \lambda_{out}$
  - 재전송이 있을 수 있기 때문에 receiver transport가 받는 데이터의 양이 receiver의 application이 받는 데이터의 양과 같거나 크다
- $\lambda_{in} = \lambda_{out}$
  - sender의 application이 보내는 데이터량과 receiver의 application이 보내는 데이터의 양은 같다
- 혼잡으로 인해 보내는 노력 대비 받는게 점점 적어지게 때문에 비용이 상당히 크다
  - 새로운 데이터를 보내지 못하게 계속 재전송만 하게 됨
- congestion control의 목적은 $\lambda_{out}$과 $\lambda^\prime_{in}$이 비례 관계를 이루게 유지하는 선에서 보내는 데이터의 량을 조절하겠다
  - 재전송을 최대한 적게 만들어 주는것

## 3. Approach towards congestion control

- congestion control을 구현하는 방법은 여러가지가 있다
- TCP에서 사용하는 end-to-end congestion control
  - 말그대로 end와 end가 congestion을 서로 상호작용하여 조절하는 방법
  - network layer에 대해서 알지 못함(because layered architecture)
  - congestion을 loss와 delay(RTT같은거 계산 때리면 되지 않을까?)로 추론한다
    - 패킷 손실이 라우터 버퍼가 꽉찼을 때도 생기지만 요즘 같은 무선 네트워크 환경에서는 불안정한 전파때문에 생기기도 한다, 그래서 정확한 추정은 되지 못함
  - 추론이기에 정확한 방법은 아님
- network-assisted cogestion control
  - network layer의 feedback을 받아 정확히 상황을 파악하고자 하는데 목적을 두는 방법
  - 라우터가 패킷에 자신의 정보를 실어보낸다, 라우터는 자신의 정보를 모니터링할 필요가 있음
  - 오래전에 개발된 방법이긴 하나 지원하는 라우터는 많지 않다
