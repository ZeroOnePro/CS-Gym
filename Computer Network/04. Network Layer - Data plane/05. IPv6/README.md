# 5. IPv6

<!--ts-->

- [5. IPv6](#5-ipv6)
  - [1. motivation](#1-motivation)
  - [2. 주소 체계](#2-주소-체계)
  - [3. Addressing model](#3-addressing-model)
  - [4. IPv4와 IPv6의 헤더 비교](#4-ipv4와-ipv6의-헤더-비교)
  - [5. Stateless Address Auto-Configuration](#5-stateless-address-auto-configuration)
  - [7. IPv6 Fragmentation Example](#7-ipv6-fragmentation-example)
  - [8. IPv4에서 IPv6로의 전환 및 공존](#8-ipv4에서-ipv6로의-전환-및-공존)

<!-- Created by https://github.com/ekalinin/github-markdown-toc -->
<!-- Added by: sungminyou, at: 2022년 7월 26일 화요일 15시 19분 03초 KST -->

<!--te-->

[IPv6 - Google](http://google.com/intl/en/ipv6/statistics.html)

전 세계적으로 IPv6로 전환되고 있다

## 1. motivation

- 32bit 주소로는 사용자를 감당할 수 없게됨
- IPv4의 단점을 개선
  - 고속화되는 Link 속도에 맞춰서 processing와 forwarding을 빠르게 할 수 있도록 header format을 설계
  - QoS를 제공하기 위해 header에 필드를 추가(realtime appliaction의 수요 증가)
- IPv6 datagram format
  - 40byte의 고정 header
  - fragmentation을 허용하지 않음
  - 기존에 process를 느리게 하던 요소를 제거하고, sw대신 hw처리가 가능하게 하여 속도를 높인다

## 2. 주소 체계

- 128bit 주소를 갖는다
- 16bit 잘라서, 그 안에서 또 4bit씩 잘라서 16진수로 표현

  - 왜 0을 없애냐면, 불필요한 정보이고, 자리채움 용도이기 때문
  - 각 영역의 시작이 0이라면 생략 가능
  - 0:0이 되는 것은 `::`으로 압축해서 표현
  - 압축 표현은 주소에서 딱 한번만 사용할 수 있음

    ![1](https://user-images.githubusercontent.com/48282185/180937200-59fc3de0-648a-4335-a470-2d16fe3b7b62.png)

    2031::130F::9C0:876A:130B했다고 하면 ::이 :0:인지 :0:0:인지 알 수 없다 첫번째 나오는 ::이 :0:0:일 수도 있는데 그러면 잘못된 주소가 되버리잖음

## 3. Addressing model

- link local
  - 하나의 서브넷 내에서만 식별 가능한 주소
  - IPv4에서 private address하고 같음
  - FE80:: / 10
- unique local
  - 기관, 조직의 boundary내에서만 식별 가능한 주소
  - FC00:: / 7
- global
  - 망 내에서 네트워크를 식별하는 주소
  - IPv4의 public address하고 같음
  - 2000:: / 3
- multicast
  - one-to-many communications
  - 뒤의 세 개는 모두 특정 단말을 가리켰다면, multicast는 여러 개의 단말을 동시에 가리킴
  - FF00:: / 8

## 4. IPv4와 IPv6의 헤더 비교

![2](https://user-images.githubusercontent.com/48282185/180937199-66111eb0-d25a-4fb9-915c-8de5e9d43965.png)

- IPv4에 비해 크기는 커졌지만, 필드 수는 줄어들었음
- option이 없어졌다
  - 필수 정보만 담겨있다
  - header의 length를 알 필요가 없어짐
- fragmentation이 금지
  - identification하고 offset필요 없어짐
- checksum이 없어짐
  - transport layer와 link layer에 맡긴다
- traffic class
  - 패킷의 중요도를 나타냄
  - realtime은 빨리 전달되야함
  - packet scheduling에서 빨리 선택되도록
- flow label
  - flow level의 서비스
  - 패킷을 일정 간격으로 보내기 등이 있음
  - 실시간 스트리밍에서 유용
  - traffic class와 같이 쓰이지는 않는다
- next header
  - 일반적으로는 payload 해석할 transport layer protocol을 가리킴
  - payload뒤에 header가 더 붙을 수 있다(option 역할)
- header의 크기가 크기 때문에 컴퓨팅 능력이 떨어지는 사양이 안좋은 장치에서는 배보다 배꼽일 수 있는데, IP의 헤더는 거의 바뀔일이 없기때문에 압축해서 보내기도한다(header compression)

## 5. Stateless Address Auto-Configuration

![3](https://user-images.githubusercontent.com/48282185/180937195-c2d946b0-799f-4afb-a44c-872ca045fa12.png)

- IPv4에서 주소를 얻어올 때처럼 DHCP나 직접 설정하는 방식을 이용할 수 있지만 IPv6에서는 그것 이외에, link layer의 address를 이용하여 하나의 global한 IPv6주소를 만들어낼 수 있는 매커니즘이 존재한다
- DHCP나 직접 설정할 필요없이 운영체제가 모든 것을 해결함
- IPv6의 특장점 중 하나
- router가 모든 Host에게 자신의 존재를 알리거나(router advertisement), 단말이 broadcast해서 라우터를 찾아내던가 해서 network의 subnet주소를 알아낸 다음, 자신의 맥주소와 이 prefix를 결합해서 새로운 global한 IP주소를 만들어낸다
- 충돌이 없고, plug and play방식으로 동작
- mac주소는 48bit이라서, 64bit로 변환시키기 위한 알고리즘이 존재한다
- 알고리즘을 역변환해서 mac주소를 알아낸다음 공격하는 경우도 있어서, 랜덤으로 숫자를 생성하여 mac주소 대신 이용하기도 한다
  - mac주소는 global unique하지만, 랜덤넘버는 그렇지 않기에 충돌이 발생할 수 있어 사전에 이 주소를 쓰고 있는 host가 있는지 확인하는 절차가 필요하다

## 7. IPv6 Fragmentation Example

![4](https://user-images.githubusercontent.com/48282185/180937192-d7394fd9-6af1-4a01-aa1b-0ba9ee5eeaf0.png)

- 처음 만나는 link의 MTU는 1500bytes이므로 이렇게 설정해서 일단 보내고 본다
- 이후 MTU가 1000bytes인 링크를 만나게 되면, 이 라우터는 source에게 packet이 크다는 icmp메시지를 보낸다
- 응답을 받은 source는 MTU를 1000bytes로 다시 분해해서 보낸다
  - 메시지를 받은 후 다시 분해하는데, network layer가 다시 메시지를 분해할 수도 있겠지만 부담이 크다 그래서 성능 최적화를 위해 전송 기능만 수행시키는데, transport layer의 MSS를 조절해서 재조정된 크기를 보내야하기에 프로토콜 계층 간에 feedback이 불가한 상황이다. 운영체제 내에 둘 다 구현되있기 때문에 parameter 형태로 보내도 되긴하지만 IPv6에는 이 명세까지는 나와있지 않아 운영체제에서 구현하기 나름이다
- 이 때, 재조립을 위한 순서 정보가 필요하므로, 확장 헤더(8bit)가 붙게된다

## 8. IPv4에서 IPv6로의 전환 및 공존

![5](https://user-images.githubusercontent.com/48282185/180937171-d266a6f1-0c93-4733-9b66-c7991556adad.png)

- tunneling
  - IPv6의 datagram이 IPv4의 payload가 되어 IPv4를 쓰는 네트워크를 통과
- configured tunnels
  - manual tunnel(RFC 2893)
  - GRE(Generic Routing Encapsulation)(RFC 2473)
- Automatic tunnels
  - Tunnel Brokers(RFC 3053) - Router to router
  - 6to4(RFC 3056)
  - ISATAP(intra-Site Automatic Tunnel Addressing Protocol)
    - host to router, router to host
