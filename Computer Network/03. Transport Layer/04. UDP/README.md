# 04. UDP

## 1. UDP(User Datagram Protocol)

- RFC 768
- 전송만을 위한 기능만 갖추고 있다
- UDP segment는 lost가 가능하고, 순서에 맞게 도착하지 않을 수 있다
  - application level에서 순서를 맞춰줄 필요가 있음
- connectionless
  - no handshaking between UDP sender, receiver
  - 연결 설정 과정이 없어서 receiver가 수용 가능한 상태인지 파악이 안되기때문에 못 받아도 그만이다
  - 각각의 UDP segement는 독립적으로 처리된다(순서 번호없이 그냥 온 곧이곧대로 application으로 전달)
- 패킷이 망가지면 버린다
- UDP가 사용되는 곳
  - DNS
    - binary message이고, 길지 않음
  - SNMP
    - network 장치의 주기적 리포팅에 사용
  - Streaming multimedia apps(손실에 대한 관용, 더 빠르게 받는게 중요(rate sensitive))
    - 요즘에는 DASH방식을 사용해서 신뢰성 있게 주고받는다
    - 90년대 초기에 사용했던 방식
- UDP 위에서 신뢰성 통신을 구현하려면
  - application layer에서 신뢰성 구현해야함

## 2. UDP segment format

![1](https://user-images.githubusercontent.com/48282185/179899619-e40aa84a-d028-4be7-a6c4-ed28c38beeb6.png)

- 32bit인 이유는 CPU에서 한 번에 처리할 수 있는 instruction 크기(word)를 고려
- connection establishment가 필요없는 경우
  - 연결 설정 delay를 추가시키는 거라서 overhead임
  - 메시지가 짧고, 한 번의 Interaction으로 목적을 달성할 수 있는 경우
  - 안되도, 메시지가 짧으니 다시 하면 됨
  - 딜레이가 길면 조심스럽긴 하다
- simple
  - 누구와 상호작용하는지 기록할 필요가 없어 시스템의 부담이 덜하다
- small header
- no congestion control
  - 혼잡에 대해 무심하기때문에 UDP를 사용하면 트래픽을 엄청 발생시킬 수 있다

## 3. UDP의 적절한 사용처

- 내부로 데이터 수집
  - 주기적으로 active되는 혹은 수동적인 data sampling
    - 주기가 길다면 문제가 되겠지만, 충분히 짧은 경우 안와도 다음 데이터 오면 그만임
  - automatic self-test reports
- 외부로 데이터 전파
  - network users에 broadcast하는 경우
    - 모든 user를 파악하는 부담이 크다
    - 신뢰성이 필요한 경우라면 application에서 해결해야함
  - announcement of new node
    - 다른 node들이 이 새로운 node를 알아야할 때
- 한 번의 요청과 응답으로 목적을 달성할 수 있을 때
  - DNS
- realtime application
  - 인터넷 전화

## 4. UDP의 예시

- QUIC protocol stack

  ![2](https://user-images.githubusercontent.com/48282185/179899602-ce878ee2-1be3-4ad6-90c3-6867dd6c6bc3.png)

  - Quick UDP Internet Connections
  - 구글에서 개발한 실험적인 프로토콜
  - 구글 서비스들과 크롬 사이의 통신에서 사용
  - HTTP 3.0에서 사용
