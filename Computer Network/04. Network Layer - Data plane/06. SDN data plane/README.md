# 6. SDN data plane

<!--ts-->

- [6. SDN data plane](#6-sdn-data-plane)
  - [1. Generalized Forwarding and SDN](#1-generalized-forwarding-and-sdn)
  - [2. match and action](#2-match-and-action)
  - [3. OpenFlow example](#3-openflow-example)

<!-- Created by https://github.com/ekalinin/github-markdown-toc -->
<!-- Added by: sungminyou, at: 2022년 7월 26일 화요일 16시 49분 13초 KST -->

<!--te-->

## 1. Generalized Forwarding and SDN

![1](https://user-images.githubusercontent.com/48282185/180952508-f869d721-2d21-4970-ae4c-85441cc17f73.png)

- 각각의 router들은 flow table이라는 것을 가지는데, 이 테이블은 중앙의 routing controller에 의해 계산되고 분배됨
- openflow data plan abstraction

  - openflow는 SDN의 소프트웨어 구현체
  - generalized forwarding

    - simple packet handling rules

      ![2](https://user-images.githubusercontent.com/48282185/180952505-a8bba741-f573-4e77-bd3b-79e088621bee.png)

    - pattern
      - packet의 헤더 필드의 값과 매칭시켜 어떤 action이 될 것인지 정의
    - action
      - matched packet에 대해 버리거나 포워딩 하거나 수정하거나 쏠림 방지를 위한 패킷 분배, 컨트롤러에 리포팅 등이 있음
    - priority
      - 어떤 패킷에 대해 여러 패턴이 매칭될 때 취할 action의 우선순위
    - counters
      - 얼마나 많은 패킷을 처리했는지 데이터를 처리했는지 기록

## 2. match and action

- openflow에서 사용하는 match and action은 새로운 컨셉이 아니라 이전부터 사용돼왔던 것이지만, SDN에서 더 섬세하게 다룰 수 있게됨
- router
  - match: longest destination IP prefix
  - action: forward out a link
- switch
  - match: destination MAC address
  - action: forward or flood
- firewall
  - match: IP addresses and TCP / UDP port numbers
  - action: permit or deny

## 3. OpenFlow example

![3](https://user-images.githubusercontent.com/48282185/180952478-36871d29-f934-4f66-ad16-55467b1b04f1.png)
