# 3. IP address, DHCP

## 1. IP addressing

- ip 프로토콜에서 단말(host, router)의 interface에 주소를 부여하는 것
- interface는 물리 링크와 단말 사이의 연결을 의미함
  - router는 일반적으로 다 수개의 interface를 갖는다
  - host는 일반적으로 유선 / 무선 두 개의 interface를 갖는다
- 네트워크 카드에 부여된 identifier이다
- 하나의 단말이 여러 개의 inferface를 갖는다면, 인터페이스 당 주소도 하나이기에 주소도 여러 개이다

## 2. Subnets

![Untitled](https://user-images.githubusercontent.com/48282185/180721515-cf014f15-3702-4a26-b7a8-f9a49ab86e33.png)

- IP 주소는 두 개의 파트로 구성된다
  - subnet part - high order bits
  - host part - low order bits
  - 같은 subnet에 속하면 high order bits는 같다
- subnet
  - router의 개입 없이 서로 물리적으로 닿을 수 있는 영역, link layer의 프로토콜만으로 통신이 가능한 네트워크 집합
  - IP 주소의 같은 subnet part를 갖는 device의 Interface들의 집합
  - 어떤 집합이 서브넷인지 확인하려면 라우터를 제거하고 통신이 가능한지 본다
  - subnet mask는 같은 서브넷에 존재하는 디바이스 끼리 IP주소의 prefix가 얼마만큼 일치하는지를 나타낸 bit수, 예를 들어 subnet mask가 24bits라면, 이 서브넷에 존재하는 디바이스의 IP주소는 앞에서부터 24bit만큼 일치한다, host를 구분하기 위해 subnet주소가 어디까지 인지 표시할 필요가 있음

## 3. Subnet을 인지할 때 헷갈릴 수 있는 점

![Untitled 1](https://user-images.githubusercontent.com/48282185/180721475-d81ceaf1-2d57-41c0-bb7c-6460ffc0583f.png)

- host들만이 하나의 서브넷에 속한 게 아니라 host들을 연결하는 router도 subnet에 속한다, 왜냐면 이 router와 host 사이에 통신하는데 다른 네트워크 장치가 필요한게 아니기 때문임
  - 위의 예시에서 223.1.3.27 라우터와 223.1.3.1, 223.1.3.2 호스트가 같은 서브넷임
- 라우터와 라우터 사이의 링크도 하나의 서브넷이다. 왜냐면 중간에 아무런 네트워크 장치 없이 두 라우터 간에 통신이 가능하기 때문
- 그래서 위의 예시에서 서브넷은 총 6개임, host들만 묶어서 서브넷이라고 착각하기 쉽다

## 4. CIDR

- classless interdomain routing
- 왜 classless인가?
  - 초기 IP 주소 부여 정책에는 클래스가 있었다
  - A class는 앞 8bit는 네트워크 식별자, 뒤 24bit는 host 식별자
  - B class는 앞 16bit는 네트워크 식별자, 뒤 16bit는 host 식별자
  - C class는 앞 24bit는 네트워크 식별자, 뒤 8bit는 host 식별자
  - 사용자가 많아지면서 이 방식이 문제가 됨
- 서브넷 식별이 주소에서 차지하는 비율을 고정해놓지 말자라는 관점에서 접근
- format
  - a.b.c.d/x, x가 네트워크 식별자를 나타내는 비트 수
  - $2^x$개의 서브넷을 식별할 수 있고, 서브넷 내의 $2^{32-x}$만큼의 host를 식별할 수 있다
- asterisk를 0으로 변환하면 CIDR이 됨

  ![Untitled 2](https://user-images.githubusercontent.com/48282185/180721494-a4d5964f-dcdf-4d6c-83d2-ac578a4cb3fa.png)

- 여러개의 IP주소들이 주어지면 이들을 CIDR로도 바꿀 수 있음
  - 이진수로 변환 후 어디까지 일치하는지 보면 됨

## 5. Subnetting

![Untitled 3](https://user-images.githubusercontent.com/48282185/180721497-1cdb17de-eb3f-44ba-ba7a-cce25d5305b9.png)

- ISP로 부터 할당 받은 주소를 몇 개의 그룹으로 분할하는 작업
- 라우터에 223.1.17.0/24라는 주소 블럭을 할당 받고, 이를 세 개의 서브넷으로 분할해야하는 상황을 가정
- 세 개의 서브넷 각각에 주소를 할당해줘야함
- 호스트가 많은 서브넷 순으로 결정해 나간다
- subnet 1, 90개를 식별하기 위해서는 7비트 필요
  - 223.1.17.0/25 혹은 223.1.17.128/25 가능
- subnet 2, 60개를 식별하기 위해서는 6비트 필요
  - subnet 1에서 223.1.17.128/25을 사용하기로 했다면, subnet 2에서는 `223.1.17.00`**\*\*** 또는 `223.1.17.01`**\*\***로 정해야함, 왜냐하면 마지막 byte의 첫 번째 비트가 1이 되면, subnet 1이랑 충돌남
  - 223.1.17.0/26 혹은 223.1.17.64/26 가능
- subnet 3, 30개를 식별하기 위해서는 5비트 필요
  - subnet 2에서 223.1.17.64/26을 사용하기로 했다면, subet 3에서는 `223.1.17.000`**\*** 또는 `223.1.17.001`**\***로 정해야함, 왜냐하면 마지막 byte의 첫 번째 비트가 1이 되면, subnet 1이랑 충돌나고, 마지막 byte의 두 번째 비트가 1이 되면, subnet 2이랑 충돌남

## 6. IP 주소를 할당 받는 법

- 사람이 매뉴얼하게 할당
  - unix에서는 /etc/rc.Config
- DHCP
  - dynamic host configuration protocol
  - 서버로 부터 동적으로 주소를 얻어오는 방법
  - plug-and-play
    - 네트워크에 연결하는 순간 동적으로 IP 주소 할당

## 7. DHCP

- 목적
  - 이동하는 다양한 단말들이 쉽게 network에 접속해서 인터넷을 사용할 수 있도록 만들어 주는 것
  - 제한된 주소 블럭 내의 주소 수보다 더 많은 단말들이 접속을 요청할 경우 이를 관리하기 위함
- 사용중인 주소를 갱신할 수 있다
  - 주소가 처음 할당받은 이후로 갱신안된다고 하면, 만약 사용 중인 유저가 떠나지 않고 네트워크를 안쓰고 있는 상황이라고 할 때 주소의 낭비가 일어난다. 그래서 일정 시간 동안만 유효한 주소를 부여하여 수시로 active되있는지 체크하는 것임
- 주소를 재사용할 수 있다
- 동작 overview
  - Host는 DHCP discover 메시지를 broadcase한다(optional)
  - DHCP 서버는 DHCP offer 메시지를 host에게 응답한다(optional)
  - Host는 IP주소를 요청한다, DHCP request
  - DHCP 서버는 할당할 IP주소를 Host에게 보내고(DHCP ack), host는 네트워크에 접속해서 인터넷을 사용함

## 8. DHCP client-server scenario

![Untitled 4](https://user-images.githubusercontent.com/48282185/180721508-98ee2f4c-7258-4f52-bc70-8af2dfd73f3f.png)

- DHCP의 정해진 port는 67번
- transaction ID는 broadcast형태의 상호작용에서 수신자를 식별하기 위해 사용, 누구든 받을 수 있으므로
- 255.255.255.255는 broadcast에서 사용하는 IP
- yiaddr은 할당 받은 IP주소
- 주소를 받은 후에도 확정 짓기 위해 request를 보낸다
  - 왜 확정을 다시 지어야할까?
  - 서브넷 내에는 DHCP 서버가 여러 대 존재할 수 있고, 그 중 하나에게 서비스를 받기 위해서임
  - 처음 접속할 때 이후로는 DHCP 서버의 주소를 알고 있기 때문에, 처음에 선택한 DHCP 서버에 DHCP request를 보내고, ack을 통해 주소를 갱신함
- client는 주소의 유효 시간의 50%가 남은 시점에 주소 갱신 요청을 보내고, 새로 갱신 받음, but, 서버에서 너무 오래 사용했다고 판단했을 때 갱신을 거부하고 네트워크 내에서 쫓아낼 수 있음
- client가 연결을 종료할 때 DHCP release를 보내서 네트워크를 떠남을 알려준다
- IP주소 없이 broadcast가 가능한 이유는 같은 subnet내에 있어서 그렇다

## 9. DHCP more than IP address

- IP주소 할당 이외에 DHCP가 하는 작업
  - 단말이 인터넷에 접속하기 위해 거치는 첫 번째 router(first-hop router)의 주소를 단말에게 알려줌
  - DNS 서버의 IP주소와 이름을 알려줌
  - subnet mask를 알려줌
  - client가 요청 메시지에 flag를 세팅하면 위의 정보들도 같이 온다

## 10. Hierarchical addressing: route aggregation

![Untitled 5](https://user-images.githubusercontent.com/48282185/180721510-5a6182d4-3c8f-47f8-a984-3d6cde403517.png)

- supernetting이라고 부름
- subnetting의 반대 작업
- 여러 개의 그룹의 IP를 묶어서 인터넷으로 내보내는데 각각의 주소를 내보내는 것이 아니라 하나로 압축하는 작업을 의미
- 인터넷으로 서비스하는 단말들을 알릴 때 필요
- organization 1이 ISPs-R-Us로 서비스 제공자를 변경했을 때 벌어지는 상황

  ![Untitled 6](https://user-images.githubusercontent.com/48282185/180721513-58434ea6-ff56-459b-828f-1b3563e47b06.png)

  - 주소가 변환되지 않고, ISPs-R-Us로 붙었는데, 제대로 된 forwarding을 위해서는 ISPs-R-Us가 organization 1이 자신이 서비스하는 사용자임을 알려야하기에 supernetting으로 압축된 주소 이외에 organization 1의 주소까지 함께 인터넷으로 알려야한다
  - longest prefix matching으로 인해 패킷은 정상적으로 organization 1 서브넷으로 포워딩된다
