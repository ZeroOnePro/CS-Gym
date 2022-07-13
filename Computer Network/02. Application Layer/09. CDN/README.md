# 09. CDN

<!--ts-->

- [09. CDN](#09-cdn)
  - [1. CDN](#1-cdn)
  - [2. Multimedia - Video](#2-multimedia---video)
  - [3. Multimedia - Streaming](#3-multimedia---streaming)
  - [4. CDN challenge](#4-cdn-challenge)
  - [5. Case Study](#5-case-study)

<!-- Created by https://github.com/ekalinin/github-markdown-toc -->
<!-- Added by: sungminyou, at: 2022년 7월 13일 수요일 14시 46분 57초 KST -->

<!--te-->

## 1. CDN

- CDN은 Content Distribution(Delivery) Network으로 컨텐츠를 제공하는 네트워크를 뜻함
- video와 streaming의 트래픽은 거의 전체 트래픽의 40 ~ 50%가량을 차지함
- challenge
  - 10억명 이상의 유저에게 서비스를 제공할 수 있는 시스템 구조를 갖춰야함
  - PC, mobile 등 다양한 기기와 유무선 네트워크 환경에서 접속하는 유저들에게 걸맞는 대응을 해야함
- solution
  - distributed network
    - 전반적으로 흩어져있는 분산 네트워크에서 가까운 사용자를 서비스
  - application level infrastructure
    - 다양한 환경의 유저에 대응하기 위해 애플리케이션 레벨에서 대응함

## 2. Multimedia - Video

- video는 일정한 비율로 이미지들의 시퀸스를 보여주는 것
- 이미지는 array of pixel이고, 그만큼 비디오는 데이터의 양이 매우 큼
- 이 큰 데이터를 곧이 곧대로 보내는 것이 아니라 줄여서 보내야 자원을 아끼고 사용자에게 빠른 응답속도를 제공할 수 있음
  - spatial coding
    - 공간에서 중복되는 픽셀들을 하나로 압축하여 표현
    - 예시로 밤하늘 - 검은색 1000개
  - temporal coding
    - image i+1을 보낼 때 모두 보내는 것이 아니라 image i에서 바뀐 점만 표현하여 보낸다
- CBR
  - constant bit rate
  - 비디오 인코딩 비율을 고정하는 방법
- VBR
  - variable bit rate
  - spatial, temporal coding의 변환시켜 인코딩 비율을 가변적으로 변경시키는 방법
- MPEG1(CD-ROM), MPEG2(DVD), MPEG4(주로 인터넷 환경에서 많이 사용)등의 형식을 갖는다

## 3. Multimedia - Streaming

![1](https://user-images.githubusercontent.com/48282185/178659935-ebadee42-1102-4d58-b5f2-3846853d24a3.png)

- DASH(Dynamic, Adaptive Streaming over HTTP)라는 기술이 현재 많이 사용됨
  - 가변적 데이터 양(Dynamic)이 사용자의 환경에 맞게(Adaptive) 스트리밍
  - 유튜브가 이 방식을 사용
- server
  - multiple chunk로 비디오 파일을 나눈다
  - 서로 다른 비율로 인코딩 된 각각의 chunk를 저장해둔다
  - 서로 다른 비율로 인코딩 된 chunk를 제공하기 위한 URL들이 존재한다(manifest file, MPD)
  - 서로 다른 화면 비율, 프레임 속도 등 사용자 환경에 걸맞게 여러 버전을 준비(720p. 1080p 등등)
  - 서버의 용량이 매우 커야함
- client
  - 주기적으로 server와의 bandwidth를 측정하여 상황에 맞게 한 번에 chunk단위로 요청
    - 현재 bandwidth에서 지속 가능한 maximum coding rate를 선택

## 4. CDN challenge

- 수십 억명의 유저들에게 동시에 다양한 버전의 비디오를 스트리밍 할 수 있게 만드는 기술적 도전 과제
- store/serve multiple copies of videos at multiple geographically distributed sites(CDN)
  - 수 많은 access network 근처에 CDN를 두어 사용자와 가깝게 통신할 수 있도록 한다(enter push)
  - 사용자가 컨텐츠 요청을 하면 메인 서버가 이를 받아서 사용자에게 최적의 CDN copied node를 알려주고, 이 노드를 통해 컨텐츠를 제공받는다
  - 문제가 생긴다
    - 어떤 컨텐츠를 어떤 node가 가지고 있는지 찾는 것
    - 최적의 node를 어떻게 결정해줄 것인가?
    - 캐싱 전략, 노드에게 어떤 컨텐츠를 가져다 놓을 것이냐?

## 5. Case Study

- 다른 사업자의 CDN을 활용하여 컨텐츠를 제공하는 경우

![2](https://user-images.githubusercontent.com/48282185/178659928-db94943b-e979-452f-85d8-7fd05295bff8.png)

- 넷플릭스처럼 자체 CDN망을 가지고 컨텐츠를 제공하는 경우

![3](https://user-images.githubusercontent.com/48282185/178659909-2bd3905a-61ae-4c9f-b932-c8f66d897f34.png)
