# 2. IP

## 1. The Internet network layer

- routing protocols
  - path selection
  - RIP, OSPF, BGP 등
- DHCP
  - 단말에 주소를 부여하는 프로토콜
- IP
  - 주소 규칙
  - datagram format 구성
  - packet handling 규칙
- ICMP
  - error reporting
  - router “signaling”

## 2. IP datagram format

![1](https://user-images.githubusercontent.com/48282185/180704744-3b734cb3-9fcd-4f52-94f3-8e0c4b059579.png)

- ver
  - 버전 번호
  - 버전에 따라서 format이 해석되는 과정이 달라지기 때문에 필요
- type of service
  - 패킷이 서비스를 받기 위해 필요한 영역
  - 라우터에서 우선 순위 스케줄링이 이루어진다고 할 때 이 우선순위가 이 영역에 들어가게됨
- time to live(TTL)
  - network에서 살아있을 수 있는 시간
  - 라우터 하나를 거칠 때마다 1씩 줄어듦
- upper layer
  - 어떤 transport layer가 segment를 처리할 것인지 명시
  - TCP일 수도, UDP일 수도 있음
- header checksum
  - payload에 대한 오류 검출은 transport layer에서 이뤄지니, IP에서는 header가 오류가 났는지만 확인하면 됨

## 3. IP fragmentation, reassembly

- network의 link들은 각각 서로 다른 MTU(Maximum Transmission Unit)를 갖는다, 즉 한 번에 최대로 전송할 수 있는 데이터의 크기가 제각각이다
- IP의 목적은 이 서로 다른 link들로 이뤄진 네트워크를 통해 전달하는 것이다
- MTU가 큰 link에서 작은 link로 갈 때 패킷은 작은 link의 MTU에 맞춰서 분할되어야함(fragmented)
- 네트워크 사이에서 다시 재조립(reassemble)될 수는 없다, 왜냐하면, 패킷 단위로 스위칭이 일어나기에 나눠진 패킷이 한 라우터에서 모이라는 보장이 없으므로 재조립은 destination에서 담당한다
- 예시

  ![2](https://user-images.githubusercontent.com/48282185/180704727-59086e39-1b41-4de7-aaa9-c7d3824e835b.png)

  - 4000bytes datagram
  - 1500bytes MTU
  - 4000 = 20(header) + 3980(payload)
  - 1500 = 20(header) + 1480(payload)
  - 3980 = 1480 \* 2 + 1020
  - 총 세 개의 분할이 되야하고, 1480으로 나눈 이유는 각 분할에 헤더가 반드시 포함되야하기 때문이다
  - more bit는 뒤에 다른 조각이 있음을 의미
  - offset은 첫 시작으로부터 얼마나 떨어졌는지를 나타내는데 8로 나누는 이유는 작은 비트 수를 가지고, 큰 offset을 표현하려고 쓰는 것 같은데, 이게 필요한 이유는 몇 번째 분할인지 파악하기 위함이다

- fragment로 인해 네트워크 상에 패킷이 증가한다
- 동일한 IP header가 중복된다
- network layer에서 존재하는 서로 다른 MTU를 갖는 링크들을 타고가야하는 `문제` 를 fragmentation이라는 `해결책` 으로 극복함
