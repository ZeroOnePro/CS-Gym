# 08. email

<!--ts-->

- [08. email](#08-email)
  - [1. Electronic Mail](#1-electronic-mail)
  - [2. SMTP](#2-smtp)
  - [3. Mail message format](#3-mail-message-format)
  - [4. Multipurpose Internet Mail Extensions](#4-multipurpose-internet-mail-extensions)
  - [5. Mail access protocols](#5-mail-access-protocols)

<!-- Created by https://github.com/ekalinin/github-markdown-toc -->
<!-- Added by: sungminyou, at: 2022년 7월 13일 수요일 13시 13분 59초 KST -->

<!--te-->

## 1. Electronic Mail

- 네 가지 구성 요소
  - User agents
  - Mail servers
  - simple mail transfer protocol(mail server칸 사용하는 프로토콜)
  - Mail access protocol(mail server와 user agent간 사용하는 프로토콜)
- User agent
  - 사용자를 대신하여 이메일을 관리하는(메일을 보내고, 편집하고, 읽는) 애플리케이션
- mail server
  - outgoing message queue
    - 송신을 기다리는 메일들이 저장되는 큐
  - user mailbox
    - 각 사용자들의 메일들을 보관하는 storage

## 2. SMTP

- RFC 2821
- 포트번호 25번
- 송신 메일 서버는 client, 수신 메일 서버는 server가 됨
- 메시지의 신뢰성있는 전송을 위해 TCP를 사용한다
- direct transfer
  - 송신 메일 서버와 수신 메일 서버가 직접적으로 연결됨
- Three phase of transfer
  - handshaking
  - transfer of message
  - closure
- Command / response interaction
  - Commands: MAIL FROM, RCPT TO와 같은 ACSII text로 명령
  - response: 상태코드와 그에 대한 설명
- messages는 7bit ASCII로 구성돼있었음
- persistent connection을 사용한다
- 7bit ASCII로 이루어진 header와 body를 필요로 한다
- 줄바꿈.줄바꿈으로 메시지 끝을 결정짓는다
- HTTP와 비교
  - HTTP는 pull 방식, 즉 클라이언트가 서버로 부터 데이터를 받아오는 방식
  - SMTP는 push 방식, 즉 클라이언트가 서버로 데이터를 보내는 방식
  - 두 프로토콜 모두 ASCII command, response 그리고 상태코드를 사용한다
  - HTTP는 하나의 메시지 안에 하나의 object만 가져올 수 있지만 SMTP는 다 수개의 object를 보낼 수 있다

## 3. Mail message format

- RFC 822에 텍스트 메시지 포멧의 기준이 나와있음
- header와 body로 이루어짐
- header는 To, From, Subject, Cc, …, etc로 이루어짐
- 여기서의 header는 SMTP의 MAIL FROM, RCPT TO와는 다른 것임
- body는 7 bit ASCII

## 4. Multipurpose Internet Mail Extensions

- motivation
  - US-ASCII만으로는 body를 구성하기에 부족함을 느낌
  - 다양한 언어, 이미지, 영상, 오디오 등 다양한 컨텐츠를 지원하기 위함
- MIME
  - text message가 US-ASCII 뿐 만 아니라 다른 character set도 지원됨
  - text형식이 아닌 message body도 지원되게 확장됨
  - multi part message body
  - header에도 US-ASCII 뿐 만 아니라 다른 character set을 지원

## 5. Mail access protocols

![1](https://user-images.githubusercontent.com/48282185/178649261-889b2ef1-a780-4dd2-b16b-2517a7849274.png)

- pop
  - post office protocol
  - RFC 1939
  - 단순하게 mail server의 mail box에서 메시지를 가져오는 프로토콜, authorization, download기능 지원
  - pop3
    - authorization phase와 transaction phase 두 단계로 이루어짐
    - 다운로드를 하면 삭제하는게 default
    - 다운로드해도 keep해주는 것도 가능, 여러 client를 지원하기 위해 등장
- IMAP
  - Internet Mail Access Protocol
  - RFC 1730
  - 메일 서버에 있는 메시지들을 조작하는 것을 포함하여 다양한 기능이 지원됨
  - pop의 단순함 대신, 기능이 많은 프로토콜을 개발하자는 차원에서 등장한 프로토콜
  - 이메일 만을 위한게 아닌 document를 관리하고 읽어가고 등등 관리 시스템 프로토콜로도 general하게 사용할 수 있음
  - 클라이언트의 부담이 적고, 서버의 부담이 큰 방식
- HTTP
  - gmail, yahoo mail 등이 HTTP를 이용하여 mail을 access한다
