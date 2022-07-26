# 4. NAT

## 1. NAT

![1](https://user-images.githubusercontent.com/48282185/180916816-5f8ff408-4502-4bd0-b5b2-b339f59403f9.png)

- 주소 변환 방법
- network address translation
- private addresses

  ```bash
  # NAT 주소 확인

  ifconfig | grep inet
  ```

  - 로컬 네트워크 내에서만 라우팅 가능한 IP 주소들
  - 집에서 흔히 사용하는 공유기의 주소
  - 10.0.0.0 ~ 10.255.255.255(10.0.0.0 / 8)
  - 172.16.0.0 ~ 172.31.255.255(172.16.0.0 / 12)
  - 192.168.0.0 ~ 192.168.255.255(192.168.0.0 / 16)

- 외부와 라우팅이 되도록하려면 어떻게 해야할까?

  ```bash
  # 외부와 통하는 IP주소 확인

  curl ifconfig.me
  ```

  - 외부와 연결된 network address translator가 주소를 변환해줌

## 2. NAT이 필요한 이유와 장점

- local network에서 외부와 연결되기 위해 하나의 IP주소만 사용했는데, 인터넷 사용자가 급격히 늘어나면서 부족해지게됨
- ISP로 부터 받은 주소 블럭이 필요가 없다
- local network에 있는 디바이스들의 주소를 외부망에 알리지 않고 변환할 수 있다
- local network에 있는 디바이스들은 주소 변환없이 ISP를 변경할 수 있다
- 외부에 주소를 명시적으로 알리지 않아서 보안성을 높인다

## 3. NAT 구현

![2](https://user-images.githubusercontent.com/48282185/180916810-88e45296-bb06-4af6-9396-77e48e598085.png)

- NAT router의 역할
  - outgoing datagram들의 헤더의 정보를 바꿔치기 (source IP, port #) → (NAT IP, new port #)
    - 새로운 포트 번호가 필요한 이유는 local network내의 서로 다른 host들에서 같은 port번호를 가지고 동작하는 application이 있을 수 있는데, 두 호스트의 IP가 하나의 NAT IP로 바뀌기 때문에 구분할 수 없게된다. 포트 번호를 안 바꾼다고 했을 때, (10.0.0.1, 20)과 (10.0.0.2.20)이 모두 (131.121.111.0, 20)으로 바뀌고 이를 역변환한다 그러면 둘 중에 뭔지 알 수없다는 뜻임
  - (source IP, port #), (NAT IP, new port #)쌍을 NAT 테이블에 기록해둔다
  - incoming datagram들을 NAT 테이블에 적혀있는 값을 참조하여 올바른 host로 포워딩해야함
- entry는 한 번 써놓으면 내부에서 host의 IP주소가 바뀌지 않는 한 계속 쓸 수 있음
  - 연결이 종료되면, entry 삭제
  - 삭제를 언제 시킬지는 구현하기 나름
- 여기까지 봤을 때의 한계점은 NAT table은 host가 외부망으로 outgoing해야 작성되기때문에 이전 연결 없이 외부에서 들어오는 패킷은 내부로 전달될 수 없다, 물론 이를 위한 해결책이 존재한다

## 4. 주소 변환

- NAT address translator는 port번호로 connection을 구분 짓기에 최대 port의 표현 범위($2^{16}$) 만큼의 연결을 생성할 수 있다
- NAT의 논란점
  - router가 layer 4의 헤더를 교체하기 때문에 프로토콜 설계원칙에 위반된다 ⇒ 포트 번호 변경
  - 프로토콜 설계 원칙에 따라 통신에 대한 모든 책임은 end가 져야하고(end-to-end argument), 라우터는 전달 역할만 해야되는데, 중간에 바꿔치기하는 것은 위반이다
    - application 개발자가 NAT의 가능성을 고려해야함
    - 예시로 P2P에서 자신의 주소를 private address로 등록해놓으면 외부에서 local host를 못찾게된다
  - 주소의 부족때문에 NAT가 등장했고, 이를 해결하기 위한 근본책은 IPv6이다
- private address는 여러 곳에 중복돼서 있을 수 있기 때문에 주소 부족을 해결할 수 있다

## 5. NAT traversal problem

- 외부 client가 NAT 내의 client에게 닿지 못하는 문제
- 해결책 1 - 포트 포워딩
  - NAT router의 관리 프로그램에서 특정 포트 번호를 내 host로 예약해두고, 이 포트 번호로 들어오면, 내 host오게 미리 예약해 놓는 방법
  - 외부에서 내 port번호를 알고 있어야함
- 해결책 2 - UPnP IGD protocol
  - Universal Plug and Play Internet Gateway Device
  - host가 post번호를 DHCP가 IP주소 할당해준 것 처럼 NAT router로 부터 port번호를 할당받는다
- 해결책 3 - relaying

  ![3](https://user-images.githubusercontent.com/48282185/180916804-a8282706-fc67-45f4-ae78-4a3de067fc44.png)

  - skype에서 사용하는 방법
  - Relay server를 통해 연결 설정을 하는 방법
  - 외부 client는 relay를 통해 NAT 내부 host의 정보를 얻을 수 있다

- 위 세가지 방법 이외에도 NAT traversal problem의 해결책은 많다

## 6. ICMP

![4](https://user-images.githubusercontent.com/48282185/180916797-95e81bb5-009a-4f2d-a215-da1ae2177e4d.png)

- internet control message protocol
- 패킷들이 잘 전달되는지 모니터링하기 위한 프로토콜
- host와 router가 network 정보를 소통하기 위해 고안됨
  - error reporting, 특정 host, network, port, protocol에 도달할 수 있는게 생기면 기록
  - host가 online인지 offline인지 확인(ping pong)
- ICMP의 메시지는 IP datagram속에 실어서 보낸다(IP보다 위에 있는 network layer)
- traceroute할 때 사용할 수 있다

  [리눅스 네트워크 모니터링 툴 명령어 - 2 ( traceroute, arp )](https://wlsvud84.tistory.com/31)

  ![5](https://user-images.githubusercontent.com/48282185/180916790-14bf869b-f262-4cc7-ac19-8ec13725242f.png)

  - type 11을 활용한다
  - 목적지를 향해 UDP segment를 TTL 값을 1씩 증가시키면서 던진다
    - default로 3번 던져봄
    - TTL = 1일 때는 처음으로 만난 라우터가 응답
    - TTL = 2일 때는 두 번째로 만난 라우터가 응답
    - etc…
  - destination이 icmp를 받게되면 사용하지 않는 port번호 이기에 type 3, code 3으로 reporting하게 되고, 목적지 까지 거쳐가는 router를 모두 알 수 있게됨
  - 위의 예시에서 총 10개의 라우터를 거치면 구글 서버에 도달할 수 있음을 알게됨

    - \*\*\*은 주소 가리기 위해 관리자가 마스킹했거나 timeout된 경우
    - 시간이 세 번 나오는 이유는 질의 default값이 3이라 그런 것임
    - 각 홉당 최대 세 개씩 뜨는 이유는 같은 router에 도달할 수 있는 ip주소가 여러개인 것임(대규모 gateway)를 갖는 것인데, 질의를 3번 보내고, 답변도 3개를 받을 수 있는 것임

      [Why does traceroute display many ip addresses for the same hop?](https://unix.stackexchange.com/questions/421637/why-does-traceroute-display-many-ip-addresses-for-the-same-hop)
