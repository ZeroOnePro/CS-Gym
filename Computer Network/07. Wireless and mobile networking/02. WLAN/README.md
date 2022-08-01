# 2. WLAN

<!--ts-->

- [2. WLAN](#2-wlan)
  - [1. IEEE 802.11 Wireless LAN](#1-ieee-80211-wireless-lan)
  - [2. 802.11 LAN architecture](#2-80211-lan-architecture)
  - [3. 802.11 Channels, association](#3-80211-channels-association)
  - [4. 802.11 passive scanning](#4-80211-passive-scanning)
  - [5. 802.11 active scanning](#5-80211-active-scanning)
  - [6. 802.11 multiple access](#6-80211-multiple-access)
  - [7. Avoiding collision](#7-avoiding-collision)
  - [8. 802.11 frame structure, addressing](#8-80211-frame-structure-addressing)
  - [9. 802.11 advanced capabilities](#9-80211-advanced-capabilities)
  - [10. 802.11 mobility within same subnet](#10-80211-mobility-within-same-subnet)

<!-- Created by https://github.com/ekalinin/github-markdown-toc -->
<!-- Added by: sungminyou, at: 2022년 8월  1일 월요일 16시 16분 52초 KST -->

<!--te-->

## 1. IEEE 802.11 Wireless LAN

- 802.11은 무선 네트워크를 연구하는 working group의 명칭인데, 기술의 이름이 돼버렸음
- 802.11b, 802.11a, 802.11g, 802.11n 등이 존재함 현재 흔히 쓰이는 wifi 공유기의 표준은 a와 g
- 모든 버전이 multiple access를 지원하기 위해 CSMA / CA를 사용한다
- 모든 버전이 base-station(infrastructure)과 ad-hoc network version을 지원한다
- link layer는 physical layer에 묶여있음, 물리 특징을 link layer 구현체가 반영할 수 밖에 없다

## 2. 802.11 LAN architecture

![1](https://user-images.githubusercontent.com/48282185/182093633-54edec1e-6d41-4d3f-a81e-214884d20473.png)

- wireless host는 base station(AP)와 통신한다
- BSS(Basic Service Set, Cell)은 하나의 중계기의 신호 범위 내에서 중계기를 통해 서로 통신할 수 있는 단말들의 그룹, 위 그림에서 파란 원
- Extended service set은 스위치로 연결된 BSS들은 같은 IP주소 블럭을 사용하므로, 하나의 서브넷으로 취급되는 BSS들의 그룹을 의미

## 3. 802.11 Channels, association

- 802.11b는 2.4GHz ~ 2.485GHz 스펙트럼을 11개 채널로 나눠서 이 중 아무거나 선택해서 AP가 사용함
  - 관리자가 11개 중 선택해줘야함
  - 인접 AP와 주파수 영역 겹침으로 인해서 간섭 발생할 수 있다
  - 11개 이상의 AP가 한 곳에 있게 되면, 충돌 무조건 발생
- 기본적으로는 infrastructure 모드이고, 이 경우 반드시 AP와 연결(association)돼야한다
  - 단말은 channel들을 스캔하고, AP의 ServiceSetID(SSID)와 MAC주소를 포함한 beacon(존재를 알리는 신호)를 듣는다
  - AP는 무선 네트워크에서 유선 네트워크로 매체가 바뀌는 중간 다리 역할을 수행한다,
  - 스위치와는 다르게 MAC주소가 존재한다
  - 단말은 신호를 받고 AP를 선택하고, 연결한다
  - AP는 인증기능을 제공할 수 있다(공유기 비번)
  - AP는 NAT기능을 제공하기도 하고, AP의 서브넷의 IP주소를 DHCP를 통해 단말에게 제공하기도 한다

## 4. 802.11 passive scanning

- 채널을 옮겨다니면서, 해당 채널을 중계하는 AP가 내보내고있는 beacon frame을 받는다
- passive는 단말이 어떤 메시지를 뿌리지 않고도, AP의 메시지를 받고 동작한다는 점에서 수동적
- scanning은 채널을 옮겨 다니면서 AP가 있는지 확인하는 동작
- 3-way로 association이 이뤄진다
  - beacon frame, APs → 단말
  - association request frame, 단말 → AP
  - association response frame, AP → 단말

## 5. 802.11 active scanning

- active는 능동적으로 자신을 알린다는 뜻
- 채널을 옮겨다니면서 단말이 자기 존재를 알리는 방식
- 4-way로 association이 이뤄진다
  - 단말이 AP들에게 broadcast한다, 단말 → APs
  - AP들은 단말에게 response한다, APs → 단말
  - 단말은 AP 중 하나를 고르고 association request frame을 보낸다, 단말 → AP
  - association response frame, AP → 단말

## 6. 802.11 multiple access

- 신호가 거리에 비례하여 감쇄되기에 무선 네트워크에서는 collision을 detect 하는 것이 불가능하다(fading, hidden terminal)
- 기본적으로는 CSMA, 즉 보내기 전에 sensing하는 역할은 한다
- collision detect가 불가능하기에 원인을 차단하는 Collision Avoidance기법을 통해 충돌을 막는다
- 802.11 CSMA의 기본 동작(CA는 아님)
  - sender
    - DIFS라는 일정 시간 동안 channel이 idle하면, 전체 frame을 내보낸다
    - 만약 DIFS동안 channel이 busy하면
      - random backoff time동안 대기
      - 대기 후 channel이 idle하다면 바로 보내지 않고 일정 시간동안 timer를 세팅하여 idle이 유지되는지 확인 후 데이터를 내보낸다
      - receiver로부터 ACK이 오지 않으면, random backoff interval을 늘리고 다시 위 과정을 반복한다
    - receiver
      - 데이터를 받고나서 SIFS라는 일정 시간 동안 대기 후 ACK을 보낸다
        - ACK이 필요한 이유는 hidden terminal problem때문에 알지 못하는 노드의 신호와 충돌나서 데이터가 제대로 전송안됬을 수도 있고, 무선 환경에서 신호의 감쇄로 인해 손실됬을 수도 있기때문에 재전송을 위함이다

## 7. Avoiding collision

![2](https://user-images.githubusercontent.com/48282185/182093631-9a9ab91e-be53-44e1-8047-6780d8211e3b.png)

- 충돌을 막기 위해 random access를 사용하지 않고, sender가 채널을 예약하는 하는 방법
- sender는 전송 전에 RTS(response-to-send)라는 작은 크기(20bytes)의 패킷을 CSMA를 사용해서 AP로 보낸다
  - 서로 다른 단말들이 보낸 RTS는 충돌날 위험이 있다, 위의 CSMA대로 동작
  - 하지만 짧은 크기의 패킷이기에 충돌 확률은 적고 전송하는데 부담이 없다
- AP는 RTS에 대한 응답으로 CTS(clear-to-send, 14bytes)를 broadcast한다
- CTS를 받은 단말은 전송을 시작하고, 이외의 단말들은 CTS를 받고 전송을 대기시킨다, 얼만큼 대기하는지는 sender가 RTS를 보낼 때 데이터크기를 고려해서 계산하여 duration field에 명시해두고, AP는 CTS를 전파할 때 이 duration field에 duration을 기입한다, CTS를 받은 다른 단말들은 이 시간만큼 대기하게 되는데 이렇게 하는 이유는 무작정 대기하면 배터리 소모가 심하기 때문에, duration만큼 sleep상태로 전환한다(Network Allocation Vector, NAV)라고 함

## 8. 802.11 frame structure, addressing

![3](https://user-images.githubusercontent.com/48282185/182093626-edd8af97-8d52-412e-a2d8-712dec4a80ee.png)

- 주소가 이더넷과 달리 네 개나 되는 것을 볼 수 있는데, 이유는 중계기를 거쳐서 변환돼서 데이터를 보내야하기 때문임
- address1은 receiver(AP or another host)의 MAC주소
- address2는 sender(AP or another host)의 MAC주소
- address3 AP가 연결된 router의 interface의 MAC주소, 중간 swap을 위한 용도
- address4는 ad hoc모드에서만 사용되는 특별한 주소
- intra-BSS addressing example

  ![4](https://user-images.githubusercontent.com/48282185/182093622-b5221475-f49d-4ee2-931e-c3b9e9d66a22.png)

  - address3가 swap하는 과정을 잘 보자

- inter-DSS addressing example
  ![5](https://user-images.githubusercontent.com/48282185/182093620-28e8d6f0-009e-47c1-a5cd-5d37b2918b42.png)

## 9. 802.11 advanced capabilities

- power management
  - mobile기기의 특성 상 배터리를 이용하기 때문에, 전원 관리가 까다롭게 되어야한다
  - node는 AP에게 다음 beacon frame이 있을 때까지 sleep 상태에 빠진다는 것을 알린다
    - sleep된 node에게는 데이터 전송을 하지 않는다
  - AP는 내보낼 데이터를 가지고 수신 대상 node들에게 sleep상태에 빠지지 말라고 통보할 수도 있다
  - 이런 매커니즘으로 단말들이 idle시간 동안 전원을 아낄 수 있게됨

## 10. 802.11 mobility within same subnet

![6](https://user-images.githubusercontent.com/48282185/182093601-43e71f97-5a78-4b3b-a4f3-934800a98365.png)

- H1이 BSS1에서 BSS2로 옮겨간 상황
- 옮겨가는 동안 H1으로 들어온 패킷을 스위치는 2번으로 내보낼 건데, H1이 BSS2에 association하기 전에는 이 패킷들이 손실된다
- 손실이 관용되지 않는 환경이면 스위치에 특별 로직이 들어가도 되고, H1이 BSS2에 붙고나서 스위치에게 변동을 알리기 위한 패킷을 보내는 등 다양한 처리를 생각해볼 수 있다
