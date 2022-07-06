# 04. web cache

<!--ts-->

- [04. web cache](#04-web-cache)
  - [1. Web caches(proxy server)](#1-web-cachesproxy-server)
  - [2. 캐시의 이슈](#2-캐시의-이슈)
  - [3. HTTP의 역사](#3-http의-역사)

<!-- Created by https://github.com/ekalinin/github-markdown-toc -->
<!-- Added by: sungminyou, at: 2022년 7월  6일 수요일 12시 11분 17초 KST -->

<!--te-->

## 1. Web caches(proxy server)

![1](https://user-images.githubusercontent.com/48282185/177460040-b338d8b1-d0ff-451b-b4b6-8e159c024d6f.png)

- proxy의 뜻은 가짜인데 진짜인 척 하는 것
- 실제 서버없이도 클라이언트의 요구사항을 만족시키는 것이 목표
- 예전 인터넷 속도가 느릴 때 사용자는 증가하는데 서버의 속도가 따라오지 못하는 문제를 해결하기 위해 등장하였음
- 브라우저에 캐시 서버의 주소를 설정해놓으면 요청이 캐시 서버로 간다
- 클라이언트가 프록시 서버에 `요청` 하는 것이 아님!!!, 프록시 서버는 URL에 대응하는 content가 존재할 때 대응을 해주는 것이고, 없으면 origin server에서 받아와야된다
- 캐시는 client이면서 server이다
  - 클라이언트 입장에서는 서버이고, origin 서버 입장에서는 클라이언트다
- 캐시 서버는 주로 ISP에 의해 설치되었었으나 통신 속도가 빨라지며 무용론이 돌았었다. 하지만 요즘에 다시 컨텐츠가 많아지고 크기가 커지면서 다시 대두되었음
- origin server가 느린 네트워크와 연결되있는 경우 특히 효과적, 등장 배경이기도 하다
- 캐시가 효용이 있으려면, 사용자가 요청하는 컨텐츠가 캐시에 있어야된다, 즉 hit rate가 높아야함
  - 만약 hit rate가 40%였다고 했을 때 캐시의 효용성을 알아보기 위해 total delay를 계산한다고 할 때, 어떻게 구하면될까? → 기댓값을 계산해보면된다 → 0.6 \* (delay from origin server) + 0.4 \* (delay from cache server)
- 캐시의 목적
  - 클라이언트의 응답시간을 줄여주자
  - origin server가 받는 트래픽을 줄여주자

## 2. 캐시의 이슈

- hit rate
  - 사용자가 요청한 컨텐츠가 캐시 서버에 없다면 캐시 서버는 무용지물이다
- 데이터가 최신 것이 맞느냐?
  - 이 문제를 해결하기 위해 Conditional GET 요청이 등장하였음
  - 클라이언트가 요청한 데이터가 있음에도 바로 응답을 보내주지 않고, 캐시 서버는 데이터가 변경되었는지 확인하기 위해 origin server에 `If-modified-since: <date>` 필드를 실어 요청을 보내면 origin server는 데이터가 date이후에 변경이 있었는지 확인시켜준다
    - 변경되지 않았다면, 304 - Not modified
    - 변경되었다면, 200 - data
  - 그럼 origin server에 요청을 보내게 되는데, 이게 효용이 있을까라는 의문이 든다, conditional get의 요청과 응답 메시지는 굉장히 짧기 때문에 요청에 의해 데이터 응답이 오는 경우보다 훨씬 작은 데이터의 크기가 오간다. 따라서 효용이 있다고 할 수 있다
  - 브라우저도 conditional get을 사용한다

## 3. HTTP의 역사

- HTTP 1.1 이전
  - HTTP 0.9에서는 GET요청뿐이었으며, HTML 문서만 주고 받았다.
  - HTTP 1.0에서는 헤더와 응답코드가 생겼다
    - POST, HEAD 메소드가 생겼다.
    - 헤더에 있는 `Content-Type` 필드를 이용하여 HTML 파일 이외의 다른 문서들도 전송이 가능해졌음
    - 응답코드로 성공과 실패를 확인할 수 있게 됨
    - 같은 요청을 여러 번 하더라도 매번 새로운 연결을 하므로 성능이 저하되고, 헤더가 중복되므로 서버 부하 비용이 크다는 문제가 있었다.
- HTTP 1.1의 특징
  - OPTIONS, PUT, DELETE, TRACE 메소드가 추가되었으며, Accept(클라이언트의 사용가능 미디어타입), Via(중계서버 정보)와 같은 헤더가 추가되었음
  - 성능 향상을 위한 기능들도 추가됨
    - Persistent Conntection
      - 하나의 TCP 연결 상에서 여러 개의 HTTP 요청 / 응답을 주고 받을 수 있다
    - Pipelining
      - 이전 요청에 대한 응답을 받기 전에 다음 요청을 할 수 있다, 순차적으로 다 수개의 요청을 보내고, 응답은 요청 순서대로 받는 방식
- HTTP 1.1의 문제

  - connection 하나 당 하나의 요청을 처리하도록 설계됨
    - 동시에 리소스를 주고 받는 것이 불가능
    - 요청과 응답이 순차적으로 이뤄짐
    - http 문서 내에 포함된 다 수의 리소스(image, css, js 등)를 처리하려면 요청할 리소스의 개수에 비례하여 latency(대기 시간이 길어짐)
  - HOL(Head of Line) blocking이 발생할 수 있다

    ![2](https://user-images.githubusercontent.com/48282185/177460037-e922eb62-5822-4517-a1fc-b4112e2d6ba3.png)

    - HOL블로킹은 네트워크 상에서 같은 큐에 있는 패킷들이 첫 번째 패킷에 의해 지연될 때 발생하는 성능저하를 말한다.
    - RTT의 증가
      - connection 하나에 요청 하나를 처리하는 특성때문에 매번 요청 별로 connection이 생성되고, TCP 상에서 동작하는 HTTP의 특성 상 3-way handshaking가 반복적으로 일어나고, 이는 불필요한 RTT 증가와 네트워크 지연을 초래시켜 성능을 떨어뜨린다.
    - header의 중복과 크기 증가
      - 매 요청마다 중복된 헤더 값이 전송되고, 서버 도메인에 관련된 쿠키 정보도 헤더에 함께 포함되어 전송되기 때문에 헤더 크기가 증가됨, 연결을 지속하고 있으면 쿠키가 필요없죠? 한 번 연결되면 누군지 아니깐

  - HTTP 2.0의 등장 이전 문제를 해결하기 위한 노력
    - 요청이 자꾸 가는게 문제였으니 요청 수를 줄여야겠죠??
    - image spriting
      - 여러 이미지 파일들에 대해 각각 요청을 하기 보다 한 번의 요청으로 모두 받아오게끔 하는 방법
      - 여러 이미지를 모아 하나의 큰 이미지를 만든 후, CSS로 해당 이미지의 좌표값을 지정해서 사용
    - domain sharding
      - 하나의 domain에 대해 여러 개의 connection을 생성하여 병렬로 요청을 보내는 행위
      - 브라우저 별로 domain 당 connection 개수의 제한이 존재하므로 근본적이 해결책이 될 수 없음
    - CSS, js 최소화
      - 전송되는 데이터 용량을 줄이기 위해 CSS와 JS 파일을 최소화하여 통신한다(~.min.js 이런 파일들)
    - Data URI scheme
      - html 문서 내 이미지 리소스를 base64로 인코딩된 이미지 데이터로 직접 기술하는 방법, 이미지를 받아오기 위한 요청이 사라짐
    - Google SPDY
      - 구글에 만든 더 빠른 웹을 실행하기 위해 throughput관점이 아닌 latency관점에서 HTTP를 고속화한 새로운 프로토콜
      - HTTP의 대체재가 아닌 HTTP를 통한 전송을 재정의하는 형태로 구현되었고 2.0 초안의 참고 규격이 된다
  - HTTP 2.0의 특징

    - binary framing layer
      - 일반 텍스트를 보내는 대신 바이너리로 인코딩해서 데이터를 전송하고, 데이터는 프레임이란 단위로 나눠진다.
      - 헤더와 메시지 페이로드가 각각 따로 프레임화 되어 보내지고, 메시지 페이로드도 여러 개의 프레임으로 분리된다
      - 프레임들은 프레임의 헤더에 삽입된 스트림 식별자를 통해 이 프레임을 재조립한다
      - binary framing의 장점은 파싱, 전송속도가 높다는 점이고, 오류 발생 가능성이 줄어든다
      - binary framing을 통해 multiplexed stream이 가능해졌다.
    - Multiplexed Streams

      ![3](https://user-images.githubusercontent.com/48282185/177460028-5a3a2877-e51f-41c3-a3c1-a90e1c2df9e1.png)

      - 하나의 connection으로 동시에 여러 개의 메시지를 주고 받을 수 있다
      - 프레임 단위로 정보가 오가기때문에 순서가 상관이 없다. 때문에 HOL blocking 문제를 해결할 수 있다

    - request prioritization
      - 응답에 대한 우선순위를 정해주는 방법
      - 크기가 작은 파일들을 우선적으로 받는 것이 효율적
    - server push
      - 클라이언트의 요청없이 서버가 응답을 보내는 방법
      - 기존에는 html파일에 resource들을 각각 받아오는 방식이었는데, 서버가 이를 미리 보내줌으로서 결과적으로 클라이언트의 요청을 줄일 수 있게됨
    - header compression

      ![4](https://user-images.githubusercontent.com/48282185/177460022-03ab1192-8ce4-49a7-914c-ce383935e4db.jpeg)

      - header table과 huffman encoding을 사용하는 HPACK 압축방식으로 중복되는 header를 압축하여 크기를 줄였음
      - 클라이언트와 서버는 각각 header table을 관리하고 이전 요청과 동일한 필드는 Table의 Index만 보내고, 변경되는 값은 huffman encoding 후 보낸다
