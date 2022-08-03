# 1. introduction to multimedia networking

<!--ts-->

- [1. introduction to multimedia networking](#1-introduction-to-multimedia-networking)
  - [1. Protocols for Multimedia Data Delivery](#1-protocols-for-multimedia-data-delivery)
  - [2. Schematic view of multimedia networking](#2-schematic-view-of-multimedia-networking)
  - [3. Compression](#3-compression)
  - [4. Multimedia networking, 3 application types](#4-multimedia-networking-3-application-types)

<!-- Created by https://github.com/ekalinin/github-markdown-toc -->
<!-- Added by: sungminyou, at: 2022년 8월  3일 수요일 19시 34분 47초 KST -->

<!--te-->

## 1. Protocols for Multimedia Data Delivery

![1](https://user-images.githubusercontent.com/48282185/182587776-12492cdc-17d1-4790-b4a6-759f4ea8788a.png)

- 현재는 HTTP위에서 multimedia를 주고 받는다, Youtube
- DCCP는 온전히 multimedia만을 위해 개발된 transport layer의 프토토콜
- RTSP는 재생, 정지, 건너뛰기, 되감기 등 제어 기능을 위해 제안된 프로토콜, 90년대 초반에 사용됐던 기술
- RSVP는 일정 시간 간격으로(Date Rate)로 데이터를 수신해야되는데, 네트워크 내에서는 엄청난 량의 패킷들과 경쟁한다, 일정 비율을 유지하기 위해 자원을 예약하여(큐의 할당량, 패킷 처리 순서 등), QoS를 보장한다
- RTP는 multimedia의 encoding(MP4 등등)을 위한 제어정보를 전달하기 위해 고안된 프로토콜
- RTCP는 보내는 양의 데이터를 전달, 상호 간의 데이터량을 조절하기 위한 제어정보를 전달하기 위해 고안된 프로토콜
- SDP는 Session Description Protocol로 화상 회의가 언제 시작되며, 회의에 사용되는 오디오 / 비디오 포멧, 몇 번 포트를 사용할 건지 등 세션 관련 정보를 주고 받기 위해 고안된 프로토콜, 90년대에 사용됐던 기술
- SIP는 화상 회의를 참석하는 사람들에게 회의에 대해 알리는 메시지를 전달하기 위해 고안됐던 프로토콜

## 2. Schematic view of multimedia networking

![2](https://user-images.githubusercontent.com/48282185/182587747-bf142907-c33a-4c6a-bfd2-a0c439fbacde.png)

- 얼마나 정교하게 sampling 할 것인가?
  - 데이터가 원본과 가깝게 사용자에게 전달될 수 있도록한다
  - sampling된 데이터를 몇 비트로 표현할 것인가? quantized, 표현범위 넓을 수록 정교하다
  - 정교할수록 데이터가 커져서 trade-off가 필요
- 일정한 시간 간격으로 데이터가 도착해야 사용자가 끊김없이 멀티미디어를 이용할 수 있다

## 3. Compression

- 멀티미디어는 데이터가 크기때문에 그대로 보내는 것보다 압축해서 보내는 것이 효율적이다
- run-length encoding
  - 반복되는 data를 빈도수와 data하나로 묶어서 줄이는 방법
  - aaaaabbccc → a5b2c3

## 4. Multimedia networking, 3 application types

- streaming, stored audio, video
  - 저장된 것을 받아오는 방식
  - streaming, 전체 파일을 다운로드 받기 전에 플레이될 수 있다
  - stored, audio / video가 render되기 전보다 빨리 전송받아서 재생의 시간 간격을 맞춘다(client의 버퍼링이 필요)
  - 유튜브, 넷플릭스 등
- conversational voice / video over IP
  - 양방향으로 실시간 상호작용
  - delay에 민감함
- streaming live audio, video
  - 라이브 스트리밍
