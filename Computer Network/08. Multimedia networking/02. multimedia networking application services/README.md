# 2. multimedia networking application services

<!--ts-->

- [2. multimedia networking application services](#2-multimedia-networking-application-services)
  - [1. Streaming stored video](#1-streaming-stored-video)
  - [2. Streaming stored video, Real World](#2-streaming-stored-video-real-world)
  - [3. HTTP streaming](#3-http-streaming)
  - [4. DASH](#4-dash)
  - [5. Voive-over-IP(VoIP)](#5-voive-over-ipvoip)
  - [6. SIP](#6-sip)

<!-- Created by https://github.com/ekalinin/github-markdown-toc -->
<!-- Added by: sungminyou, at: 2022년 8월  4일 목요일 12시 22분 54초 KST -->

<!--te-->

## 1. Streaming stored video

![1](https://user-images.githubusercontent.com/48282185/182756023-4e82af14-2c2c-44f0-bbd3-52cd8f885253.png)

- 비디오는 초당 30개의 이미지로 샘플링을 하고(CBR), 서버는 초당 30개의 이미지를 사용자에게 보낸다, 클라이언트의 영상 어플리케이션은 이것을 받고 초당 30개의 이미지를 보여주며 영상을 재생시켜준다
- 어느 한 시점에서 보면 서버가 열 번째 프레임의 일부를 보내고 있을 때, 클라이언트는 두 번째 프레임의 일부를 재생시키고 있다
- 위의 그래프는 네트워크에서 샘플링된대로 일정한 시간 간격으로 데이터가 전달되는 이상적인 상황을 나타내는데, 현실에서는 힘들기에 도전과제가 된다
- challenges
  - 연속적으로 플레이되게 해야한다
    - 네트워크의 딜레이는 가변적이라(jitter) 클라이언트에서 original timing에 맞게 영상을 재생시키려면 버퍼링할 필요가 있다
  - 영상 정지, 빨리 감기, 되감기, 건너 뛰기 등을 했을 때 어떻게 대처해야하는지도 문제임

## 2. Streaming stored video, Real World

![2](https://user-images.githubusercontent.com/48282185/182756020-44fab8b0-861e-4d1b-bf78-f1169f33d05e.png)

- 가변적 네트워크 딜레이로 인해 클라이언트에서 데이터를 받게되는 시간이 가지각색이다
- 하지만 사용자에게는 일정 시간 간격으로 같은양의 데이터를 보여줘야한다
- 같은양의 데이터를 일정 시간 간격으로 보여주게 하기 위해 데이터를 받자마자 바로 재생하는 것이 아니라 한 frame의 데이터가 찰 때까지 기다렸다가 보여준다, 이 때 준비될 때까지의 딜레이를 playout delay라고 한다
- playout delay를 잘 정하는 것이 중요하다

## 3. HTTP streaming

- HTTP GET을 통해 멀티미디어 파일을 검색한다
- sender는 보낼 수 있는 최대량을 TCP연결 위에서 보낸다
- TCP의 congetsion control, retransmission으로 인해 클라이언트의 버퍼에는 데이터가 가변적인 비율로 쌓인다
- HTTP / TCP가 firewall이나 NAT를 통과하기 더 쉽다
  - 대부분의 엑세스 라우터는 방화벽을 가지고 있다
  - 방화벽은 IP주소와 포트번호를 기반으로 패킷을 막을 수 있고, TCP연결을 추적하고 승인된 연결의 데이터그램만 받도록 설정할 수도 있다
  - UDP는 혼잡제어나 흐름제어가 없기 때문에 패킷 증폭을 시킬 수 있어 많은 방화벽 시스템은 UDP를 아예 차단시키지만, HTTP 트래픽은 통과시키기 때문이다

## 4. DASH

- dynamic(여러 선택지를) adaptive(현재 상황에 맞춰) streaming over HTTP
- server
  - file을 다수 개의 chunk들로 잘라서 비디오를 나눈다
  - 각 chunk는 저장되고, 서로 다른 비율로 인코딩된다
  - manifest file은 서로 다른 비율로 인코딩된 영상들의 URL들을 제공한다
- client
  - 주기적으로 서버와의 연결의 bandwidth를 체크한다
  - 체크한 bandwidth를 바탕으로 manifest파일에서 지금 상황에서 받을 수 있는 최대 비율을 고르고 한 번에 하나의 chunk를 요청한다, 시간이 지남에 따라 bandwidth도 변경되고, 그 때마다 다시 지금 받을 수 있는 최대 비율로 요청한다
  - 서버가 알아서 주면되지 않느냐고 할 수 있는데, 접속한 모든 클라이언트와의 연결을 시시각각 체크하는 건 매우 부담이다

## 5. Voive-over-IP(VoIP)

- 대화한다는 측면에서 end-end-delay가 요구된다
  - 높은 딜레이는 상호작용에 악영향을 미친다
  - 일반적으로 150msec보다 딜레이가 적어야 좋고, 400msec보다 딜레이가 크면 나쁘다
  - 여기서의 딜레이는 application level에서 패킷으로 만들고 재생하는 작업과 네트워크 딜레이를 합친 것을 의미
- 전화를 받는 사람의 IP주소, 포트 번호, 인코딩 알고리즘을 어떻게 얻어올 것인가에 대한 사전 협의 필요
  - SDP 프로토콜이 그 예시
- 착신, 기록, 영상 통화 등의 기능도 지원가능해야 한다
- 응급 전화의 빠른 전달을 위한 기법도 필요
- 특징
  - 음성은 초당 8000번 샘플링되고, 샘플은 256bits로 구성된다, 20msec 당 하나의 컨텐츠가 만들어진다 → 64,000bits / sec → 64 bits / ms → 8bytes / ms → 160bytes / 20ms
  - 각 chunk에 application layer header가 붙는다, 데이터의 양이 작아 배보다 배꼽일 수 있다
  - UDP나 TCP를 통해 보낼 수 있음
  - application은 말하는 동안 매 20msec마다 소켓에 segment를 실어서 보낸다
  - 통화 품질을 높이기 위해 digital audio를 44.1 kHz sampling rate, 16bits로 각 샘플을 구성하기도한다, 44100 \* 16 = 705,600 bits / sec
  - delay loss, 딜레이에 따라 어쩔 땐 빨리말하는 것 처럼 보이고, 어쩔 때는 늦게 뚝뚝 끊길 수 있어 말이 이상하게 들릴 수 있기에 수신자가 재생하는 것 보다 늦게 도착하는 패킷은 버리는게 낫다
    - 일반적으로 400msec이상 지연된 것은 버린다
  - loss tolerance
    - voice encoding기법에 의존하는데, loss된 사실을 숨길 수 있다, 1 ~ 10% 정도의 패킷 손실 비율은 괜찮다
- 비디오와 마찬가지로 20msec로 샘플링됐을 때 수신측에서도 20msec당 재생시켜줘야하기에 버퍼링이 필요하다
- adaptive playout delay
  - playout delay를 줄이고, late lose rate를 줄이는 것이 목표
  - approach
    - playout delay를 dynamic하게 조정하자
    - 네트워크 딜레이를 측정한 후, 말이 시작될 때 마다 playout delay(slient period)를 조정해주는 방법
    - 딜레이가 크다면 playout delay를 늘리고, 반대의 경우는 줄인다
    - 말할 때마다 chunk들을 20msec으로 playout되는 것은 동일하게 한다

## 6. SIP

- 인터넷을 통한 전화와 화상회의를 지원하기 위한 프로토콜
- 전화번호 대신 이름이나 email을 통해 사람을 인터넷 상에서 식별한다
- 수신자가 어떤 IP장치를 사용하던, 어디있던 닿을 수 있게 하겠다
- service
  - 상대의 IP주소 알아내는 기능 지원
  - 통화 관리기능 지원
  - 통화 연결을 맺고, 끊고하는 기능 지원
- 과정

  ![3](https://user-images.githubusercontent.com/48282185/182756014-8e39e57f-b06e-45b5-ac68-2af196493809.png)

  - 소통 미디어, 인코딩 방식, 포트 번호, IP주소를 상대에게 보내고(SDP에서 정해놓은 규칙에 따름), 상대방도 마찬가지로 같은 정보를 보낸다
  - 서로 인코딩되는 방식이 다를 수 있음에 유의하자 상대방이 보낸 인코딩 방식으로 각자 디코딩해주면 된다
  - 3-way handshaking을 통해 연결이 이뤄진다
  - SIP는 5060번의 포트번호를 갖는다
  - 연결이 이뤄지고 나면 SIP의 역할은 끝이고, 그 이후로는 RTP나 다른 프로토콜을 통해 미디어 데이터를 주고 받다가 나중에 연결을 끊을 때 다시 SIP를 이용한다
  - 상대방의 IP를 모를 경우(mobile user를 가정), 상대방을 대행하는 서버를 통해 대신 받아온다
    - 이 방식을 이용하기 위해 SIP 서버가 필요
    - mobile IP의 home agent와 동작이 상당히 유사
  - registrar
    - SIP를 사용하는 단말들이 자신의 위치 정보를 등록할 수 있게 SIP 서버가 지원하는 기능 중 하나
    - 사용자가 SIP client를 시작하면, client는 서버에 SIP REGISTER 메시지를 보낸다
  - proxy
    - SIP서버의 또 다른 기능
    - 사용자는 자신의 proxy 서버를 통해 초대 메시지를 보낼 수 있다
    - proxy 서버는 registrar에서 상대방의 정보를 넘겨준다
    - SIP message를 라우팅할 책임을 갖고, 여러 프록시를 통과해서 라우팅할 수 있다
    - 결론적으로 client는 자신의 proxy서버 정보만 알고있으면 통화 가능
    - proxy가 통화 연결 설정의 모든 작업을 대행한다
  - 동작 과정

    ![4](https://user-images.githubusercontent.com/48282185/182755997-235266c1-90f5-4240-bf2e-c1c11610fa28.png)

    - keith가 upenn.edu에 붙어있다가 eurecom으로 옮겨간 상황
    - 새로운 네트워크에서 기존에 있던 네트워크의 registrar서버에 새로운 정보를 REGISTER한다
