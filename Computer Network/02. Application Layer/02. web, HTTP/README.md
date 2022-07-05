# 02. web, HTTP

<!--ts-->

- [02. web, HTTP](#02-web-http)
  - [1. Web and HTTP](#1-web-and-http)
  - [2. HTTP overview](#2-http-overview)
  - [3. HTTP connections](#3-http-connections)
  - [4. Dependency and timing information of a page load](#4-dependency-and-timing-information-of-a-page-load)
  - [5. Non-persistent HTTP: response time](#5-non-persistent-http-response-time)
  - [6. Persistent HTTP](#6-persistent-http)
  - [7. HTTP Connection Management](#7-http-connection-management)

<!-- Created by https://github.com/ekalinin/github-markdown-toc -->
<!-- Added by: sungminyou, at: 2022년 7월  5일 화요일 18시 26분 52초 KST -->

<!--te-->

## 1. Web and HTTP

- Web page = HTML file + referenced objects
  - JPEG image, java applet, audio file, …
- 각 오브젝트들은 URL(Uniform Resource Locator)에 의해 위치를 알 수 있다

  ![1](https://user-images.githubusercontent.com/48282185/177296460-ff8daed4-5192-4f8e-ba82-e197da4a3e2f.png)

- Hyper text
  - 참조(하이퍼 링크)를 통해 독자가 한 문서에서 다른 문서로 즉시 접근할 수 있는 텍스트

## 2. HTTP overview

- HyperText Transfer Protocol
- Web’s application layer protocol
- client - server model
  - client: 주로 browser
  - server: web server
- TCP를 사용한다
  - 작업 중간에 데이터 손실이 되면 안됨
  - client가 80번 포트로 TCP연결을 초기화한다
  - server는 client의 TCP연결을 받아들인다
  - browser와 web server 사이에 HTTP message 교환이 이루어진다
  - TCP 연결이 종료된다
- HTTP는 기본적으로 stateless이다
  - 서버가 과거 클라이언트의 요청에 대한 정보를 유지하고 있지 않는다
  - 상태가 없다
  - 관리하기 시작하면 너무 복잡해짐, 이 간단함 덕분에 현재까지 널리 사용된다

## 3. HTTP connections

- Non-persistent HTTP
  - 하나의 TCP 연결 위에서 최대 한 개의 object만을 전송할 수 있음
  - 다 수개의 objects를 다운로드 받기 위해서는 TCP 연결도 그에 맞게 다 수개 생성되야함
  - 예전 웹 페이지가 단순했을 때는 문제가 없었지만, 점점 한 페이지에 다양한 object들이 포함되다 보니 문제가 되었음
- Persistent HTTP
  - TCP 연결 위에서 다 수개의 object들을 전송할 수 있음
  - 현재 HTTP의 연결은 persistent로 발전하는 중이다

## 4. Dependency and timing information of a page load

![2](https://user-images.githubusercontent.com/48282185/177296455-f73d1920-58d8-490a-814e-24ec0517e290.png)

- 위의 예시는 한 번의 연결에 다양한 object를 요청하기에 persistent connection이다

## 5. Non-persistent HTTP: response time

![3](https://user-images.githubusercontent.com/48282185/177296452-23a1c39e-b230-449c-8bcc-8886c8a8fb8b.png)

- RTT(Round Trip Time)
  - 작은 패킷이 클라이언트에서 서버에 갔다가 되돌아오기까지의 시간
- HTTP response time
  - 요구한 첫 object를 받기 전까지 걸리는 시간(응답시간)
  - One RTT to initiate TCP connnection
  - One RTT for HTTP request and first few bytes of HTTP response to return
- small packet을 대상으로 하기에 transmission delay와 processing delay는 무시되고 주로 queueing delay와 propagation delay가 관심 대상
- Non-persistent HTTP의 응답 시간은 2RTT + file transmission delay가 걸린다 이것을 최소화하는 것이 문제다

## 6. Persistent HTTP

- Non-persistent HTTP의 이슈
  - 각 object마다 2 RTT가 요구됨
  - 각 TCP 연결마다 운영체제의 overhead가 생김
  - 브라우저는 종종 병렬 TCP 연결들을 개설하여 referenced objects들을 fetch한다
- Persistent http
  - server는 명시적인 연결 종료 메시지를 받기 전까지는 연결을 끊지 않는다
  - 이미 개설된 client - server 사이의 연결 상에서 후속 HTTP message들이 전송된다
  - client는 referenced object를 만나자마자 곧바로 요청을 보낸다. parsing하다가 요청이 필요한 object가 발견되면 곧바로 요청을 보낸다는 것
  - 모든 referenced object들을 받기 위해 1 RTT만 필요하다

## 7. HTTP Connection Management

![4](https://user-images.githubusercontent.com/48282185/177296426-539b8206-cf03-446e-91ee-e3c55c2ca137.png)
