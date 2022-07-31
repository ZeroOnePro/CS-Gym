# 4. ARP

<!--ts-->

- [4. ARP](#4-arp)
  - [1. MAC address(LAN address)](#1-mac-addresslan-address)
  - [2. ARP](#2-arp)
  - [3. kernel processing](#3-kernel-processing)
  - [4. ARP protocol](#4-arp-protocol)

<!-- Created by https://github.com/ekalinin/github-markdown-toc -->
<!-- Added by: sungminyou, at: 2022년 7월 31일 일요일 17시 09분 30초 KST -->

<!--te-->

## 1. MAC address(LAN address)

- local area network내에서 단말을 식별하기 위한 주소
- 하나의 인터페이스에서 물리적으로 연결된 다른 인터페이스로 부터 프레임을 얻기 위해 지역적으로 사용하는 주소
- 48bit로 구성되있고, 제조 과정에서네트워크 카드의 ROM에 설정함, 때로는 소프트웨어적으로 설정 가능
- 16진수로 표현된 경우가 대부분
- 각각의 LAN adaptor(네트워크 카드)는 unique한 MAC 주소를 갖는다
  - IEEE에서 주소를 관리하고 판매자에게 주소를 할당함
  - 앞 24bit는 판매자에게 부여된 번호, $2^{24}$개의 장치를 만들어낼 수 있고, 부족하면 더 사면된다
- MAC은 flat address라서, 다른 장치로 카드를 옮겨서 사용할 수 있다

## 2. ARP

- Address Resolution Protocol
- MAC주소를 얻어내기 위한 프로토콜
- 운영체제에서 관리됨
- ARP table이 LAN내에 있는 각각의 IP node에서 관리됨, 운영체제가 관리
  - <IP, MAC, TTL>로 entry 구성
  - TTL은 일반적으로 20분으로 설정되며, 이 시간이 지나면 갱신한다
- 테이블 조회
  ```bash
  $ arp -a
  ```

## 3. kernel processing

![1](https://user-images.githubusercontent.com/48282185/182016283-08ee63cf-f984-4979-a0b0-6f3492053769.png)

- network device driver는 ARP frame을 잡아서 ARP table entry를 구성하고, IP패킷이 들어오면, ARP주소와 매핑 시켜서 적절한 interface로 전송해준다

## 4. ARP protocol

![2](https://user-images.githubusercontent.com/48282185/182016281-f231bee0-7e08-4b40-abee-38e5e17775cc.png)

- same LAN
  - destination의 MAC주소를 모르면 destination의 IP주소를 포함한 ARP query 패킷을 LAN으로 broadcast한다
  - 이 boardcast packet의 destination MAC주소는 FF-FF-FF-FF-FF-FF로 세팅
  - LAN에 있는 모든 노드는 ARP query를 받는다
  - boardcast packet의 IP주소와 매칭되는 노드는(destination) source에게 자신의 MAC주소를 실어서 응답한다(unicast), destination이 source의 정보를 몰랐다면, ARP table에 기록한다
  - source는 응답을 받고 ARP table에 기록한다
  - plug-and-play, 관리자의 중재없이 ARP table을 만들 수 있다
  - 누구든 받을 수 있으므로 broadcast는 항상 보안상 취약이 생기기에 해결책을 구현에 포함해야한다
- another LAN
  - A에서 B로 패킷을 보낸다고 하자
  - A는 B의 IP주소를 알고 있다(DNS를 통해 혹은 관리자를 통해)
  - A는 LAN을 외부와 연결시켜주는 first hop router의 IP를 알고있다(DHCP를 통해 혹은 관리자를 통해)
  - A는 first hop router의 MAC주소를 알고있다(ARP를 통해)
  - 위 세가지 정보를 알고있으면 패킷을 만들 수 있다
  - destination의 MAC주소는 B의 MAC주소가 아니라 frist hop router의 MAC주소임에 주의하자
  - first hop 라우터는 패킷을 받아서 destination의 IP를 확인하고 ARP table을 뒤져서 해당 IP에 맞는 MAC주소로 교체한다, 단순 망(A→Router→B)인 경우에는 바로 B의 MAC주소가 나오겠지만, 아닌 경우에는 second hop router의 MAC주소가 나오고 이런식으로 계속 router를 거치면서 MAC주소가 교체되다가 B가 있는 LAN의 router를 만나서 그제서야 B의 MAC주소를 알 수 있게된다
