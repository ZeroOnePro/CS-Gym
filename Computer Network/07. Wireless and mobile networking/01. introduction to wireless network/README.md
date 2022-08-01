# 1. introduction to wireless network

<!--ts-->

- [1. introduction to wireless network](#1-introduction-to-wireless-network)
  - [1. Elements of a wireless network](#1-elements-of-a-wireless-network)
  - [2. Wireless link의 특징](#2-wireless-link의-특징)

<!-- Created by https://github.com/ekalinin/github-markdown-toc -->
<!-- Added by: sungminyou, at: 2022년 8월  1일 월요일 12시 45분 24초 KST -->

<!--te-->

## 1. Elements of a wireless network

- wireless hosts
- base station(AP)
  - 무선 네트워크 내에 단말들 사이의 중계, 외부망 연결
  - cell tower, wifi 공유기
- wireless link
  - backbone link에서도 사용(군사 목적)
  - 미리 정해진 주파수가 있고, 단말들은 이 주파수 스펙트럼내의 주파수를 이용해서 연결됨
- infrastructure mode
  - 서비스 공급자들이 이미 갖춰놓은 망에서 통신하는 모드
  - 중계기끼리 연결돼서 mesh net을 구성하기도 함
  - handoff기능을 제공
    - 이동하면서 한 중계기의 서비스를 받다가 서비스 지역을 벗어나서 다른 중계기의 서비스를 받는데, 데이터의 끊김이 없도록 변경하는 기능
- Ad hoc mode

  ![1](https://user-images.githubusercontent.com/48282185/182068160-8331dafc-d13f-46ef-a7c0-3d7616ae8e65.png)

  - 인프라가 미리 갖춰져있지 않음
  - 중계기가 없고, 각 단말이 자신의 신호 범위 내의 있는 다른 단말과 통신
  - 각 단말이 라우터 역할하는 것
  - bluetooth

## 2. Wireless link의 특징

- 물질을 따라 전파되는 중에 신호가 감쇄한다(path loss)
- 표준화된 무선 네트워크는 같은 주파수 대역을 다른 source들과 공유하기 때문에 신호가 섞여 간섭이 일어날 수 있다
- source부터 destination까지 가는데 신호가 반사되면서 여러 경로를 거쳐서 도달하게되는데, 도착하는 시간이 모두 다르다(multipath propagation)
- line of sight
  - 두 중계기 사이에 방해물이 있으면 안된다
- free path loss
  - 방해물이 없더라도 신호가 감쇄하여 사라질 수 있음
- reflection
  - 신호가 반사됨
- absorption
  - 어떤 특정 매질을 통과할 때, 신호가 흡수되어 약해질 수 있다
- hidden terminal problem

  ![2](https://user-images.githubusercontent.com/48282185/182068148-5735fbb5-bb71-4337-9ff4-69192c244a88.png)

  - B와 A는 가까이 있어서 서로 신호범위내에 있음
  - B와 C는 가까이 있어서 서로 신호범위내에 있음
  - C와 A는 멀리 떨어져있어서 서로 신호범위내에 없음
  - 이때 CSMA를 적용한다고 했을 때, A는 C의 신호를 받지 못하기 때문에 C가 보내던지말던지 그냥 보냄, 반대의 경우도 마찬가지임, 중간의 B는 A와 C의 신호가 충돌하기 때문에 정상적인 신호를 받을 수 없음
