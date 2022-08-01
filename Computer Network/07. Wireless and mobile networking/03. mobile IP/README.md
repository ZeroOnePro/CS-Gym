# 3. mobile IP

<!--ts-->

- [3. mobile IP](#3-mobile-ip)
  - [1. mobile IP의 목적](#1-mobile-ip의-목적)
  - [2. Motivation of Mobility Support](#2-motivation-of-mobility-support)
  - [3. Approach](#3-approach)
  - [4. 용어 정리](#4-용어-정리)
  - [5. Indirect routing 상세 과정](#5-indirect-routing-상세-과정)
  - [6. mobile IPv6](#6-mobile-ipv6)
  - [7. handover procedure](#7-handover-procedure)
  - [8. wireless, mobility impact on higher layer protocols](#8-wireless-mobility-impact-on-higher-layer-protocols)

<!-- Created by https://github.com/ekalinin/github-markdown-toc -->
<!-- Added by: sungminyou, at: 2022년 8월  1일 월요일 18시 24분 32초 KST -->

<!--te-->

## 1. mobile IP의 목적

- mobile user, 지속적인 연결을 유지하면서 다수 개의 access point를 통과하는 사용자들, 핸드폰 이용이 대표적 예시인데, 이렇게 access point를 계속 바꿔도 연결이 유지될 수 있도록 하는것이 mobile IP의 궁극적 목표이다

## 2. Motivation of Mobility Support

- mobile host가 이동해서 새로운 subnet에 접속했을 때 정상적으로 인터넷에 연결되려면 새로운 subnet의 주소를 할당받아야한다
- 어떤 host가 TCP연결을 맺은 채로 다른 subnet으로 옮겨갔다고 하자, 그 host의 IP주소는 바뀌었고, 바뀐 주소로 패킷을 보내면 TCP연결을 맺은 상대방측은 모르는 주소로 들어왔기때문에 패킷을 폐기한다
  - TCP는 transport layer이고, IP주소와 무관한게 원칙적으로 맞지만, 운영체제에서는 TCP연결을 <source IP주소:port번호, destination IP주소:port번호>로 관리하고 있기때문에 이런 일이 발생
- subnet을 옮기고 인터넷에 접속하려면, 주소가 바뀌어야하고, 바뀐 주소로는 기존의 연결들을 유지할 수 없는 문제가 발생한다, network layer에서 이 문제를 해결해야하고(IP주소가 문제가 되는 것이기에) 이에 대한 해결책으로 mobile IP가 나오게 되었음

## 3. Approach

- routing handle it
  - 각 라우터에게 자신의 서브넷에 속한 단말이 변경됬음을 broadcast하여 forwarding table에 단말의 변경된 주소를 기록하여 제대로 포워딩 될 수 있도록 한다
  - 그럼 모든 mobile device들을 다 forwarding table에 기록해야되는데, 그 수를 생각하면 현실적으로 불가능하다
- end-system handle it
  - direct routing
    - 이동한 단말이 현재 연결을 맺고있는 상대방에게 자신의 변경된 주소를 직접 알리고, 알림을 받은 상대방들은 단말의 주소를 바꿔치기한다, 모든 end system을 변경해야하기에 부담이 큰 방법
  - indirect routing
    - mobile IP에서 사용하고 있는 방식
    - 이동하기 전에 home agent라는 것을 심어놓고, 연결을 맺고 있는 상대방들에게서 들어오는 패킷들을 home agent가 단말이 이동한 위치로 forwarding해주는 방법
    - 이동한 단말은 home agent에게 자신이 접속한 새로운 위치를 알려줘야 포워딩이 가능해지고, 이동한 위치에서 다시 destination으로 응답을 보낼 때, home agent를 굳이 안거쳐도 되지만 source의 IP는 변경되면 안된다

## 4. 용어 정리

- home network
  - mobile의 영구적인 home, 뿌리가 되는 네트워크
- permanent address
  - home network로부터 부여받은 어느 네트워크에서던지 host로 도달할 수 있게 만들어주는 주소
- home agent
  - host가 다른 네트워크에서 접속했을 때, host를 대신하여 mobility 기능들을 수행하는 개체
  - 별도의 컴퓨터로 존재할 수도 있지만, 라우터 내에서 도는 프로세스로 구현하는게 편함
- visited network
  - host가 home network를 벗어나서 접속한 다른 네트워크
  - permanent address는 여기서도 변하지 않는다
- care-of-address
  - visited network에서 부여받은 주소
- foreign agent
  - visited network에 접속한 외부 host를 서비스하기 위한 용도로, mobile IPv4에서 32비트로 모든 외부 host를 수용할 수 없어서 필요한 개체, mobile IPv6에서는 없어짐
  - 외부 host들은 동일한 care-of-address를 가지고, agent가 link layer의 MAC주소를 이용해 forwarding한다
- correspondent node(CN)
  - 상호작용하고 있는 상대 노드

## 5. Indirect routing 상세 과정

![1](https://user-images.githubusercontent.com/48282185/182117668-9f334473-b44f-425b-92f7-33295d04965a.png)

- host는 visited network의 foreign agent에 자신을 등록한다
- foreign agent는 home agent에게 자신을 등록하고, host로 들어오는 모든 트래픽을 자신에게 forwarding하게끔 설정한다
- CN은 home network로 패킷을 보내고, 이를 home agent가 낚아채서 foreign agent에게 주고, foreign agent는 Link layer protocol을 통해 host에게 패킷을 포워딩한다

  - home agent가 foreign agent에 패킷을 보낼 때 터널링이 사용된다 왜냐면, dest는 permanant인데, 받아야하는 것은 foreign agent의 IP, 즉, care-of-addres이기 때문

    ![2](https://user-images.githubusercontent.com/48282185/182117660-579095b7-aaf7-4a2c-a7c1-649785d519a3.png)

- 하지만 이런 노력에도 불구하고, mobile IPv4는 상용에 실패한다

## 6. mobile IPv6

- RFC 6275
- 상용 기술
- Ipv6에서도 IP주소의 변화 없이, 다른 네트워크에서 접속할 수 있도록 했음
- 주소가 차고 넘쳐서 foreign agent가 필요 없다
- registration
  - mobile node는 auto configuration(MAC주소와 IPv6 prefix로 global 주소 만드는 기법)을 이용해서 주소를 만들고, home agent에게 binding update요청을 보낸다(binding은 permanent address와 care-of-address를 매핑시키는 작업)
  - Home agent는 주소를 바인딩하고 node에게 ACK을 보내줌으로서 등록 과정이 완료된다
- triangular routing

  ![3](https://user-images.githubusercontent.com/48282185/182117655-4f2dec50-a1c4-4cd9-8a1e-2200bc38014b.png)

  - mobile IPv6에서 했던 과정에 별 차이 없다
  - 비효율적이기에 node가 네트워크를 옮겨가면, correspondent에게 자신의 주소를 binding update요청을 해서 correspondent가 옮겨진 주소로 패킷을 보내는 route optimization을 한다, 이 때 해커의 간섭이 있을 수 있기에 정말 node가 보낸 것이 맞는지 확인하는 인증절차가 수반된다

    ![4](https://user-images.githubusercontent.com/48282185/182117647-3da19d83-7554-423c-8f29-037b9a8faa18.png)

## 7. handover procedure

![5](https://user-images.githubusercontent.com/48282185/182117619-aca59cc5-77f7-41f1-a3d5-f29f76e22a54.png)

- 2001:610:80:5030:: / 64(A)에서 2001:610:80:5035:: / 64(B)로 옮겨가는 과정은 어떻게 될까
  - A에 있는 BSS내에 있는 경우는 A의 BSS AP의 신호가 강하기 때문에, A를 선택하는데, 점점 B쪽으로 이동할 경우 B의 BSS의 AP신호가 A의 신호보다 우세하기 때문에 B로 옮겨가게 되고, auto-configuration을 통해 IP주소를 만든 다음 triangular routing과정을 거치고 나면 B의 서비스를 정상적으로 받을 수 있다, 이 때 home network는 A가 된다

## 8. wireless, mobility impact on higher layer protocols

- 네트워크를 옮겨도 연결이 끊어지지는 않지만 성능면에서 손실을 보게된다
- mobile host가 새로운 네트워크에 접속해서 home agent로 binding update요청을 하기 전까지 correspondant는 계속 패킷을 보내고 있을텐데 ACK을 보내는 상대방이 없으니 계속 패킷 손실이 생기게 되고, transport layer에서는 이게 혼잡에 의한 것인지 handover에 의해 생긴 것인지 알 수 없기에 구현된 대로 congestion control에 의해 window size가 감소하고, 결론적으로 성능이 저하된다
- home agent를 거쳐가기때문에 패킷 딜레이도 늘어난다
