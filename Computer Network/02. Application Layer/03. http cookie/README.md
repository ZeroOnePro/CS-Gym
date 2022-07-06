# 03. http cookie

<!--ts-->

- [03. http cookie](#03-http-cookie)
  - [1. HTTP request message](#1-http-request-message)
  - [2. Method types](#2-method-types)
  - [3. Uploading form input](#3-uploading-form-input)
  - [4. HTTP response message](#4-http-response-message)
  - [5. User-server state: cookies](#5-user-server-state-cookies)

<!-- Created by https://github.com/ekalinin/github-markdown-toc -->
<!-- Added by: sungminyou, at: 2022년 7월  6일 수요일 10시 27분 15초 KST -->

<!--te-->

## 1. HTTP request message

![1](https://user-images.githubusercontent.com/48282185/177447752-6e27bd56-0ba7-4c11-9bfe-d315d5394ec3.png)

- ASCII(human readable format)

## 2. Method types

[Hypertext Transfer Protocol (HTTP) Method Registry](https://www.iana.org/assignments/http-methods/http-methods.xhtml)

- HTTP/1.0
  - GET
  - POST
  - HEAD
    - header정보만 얻어오는 method
- HTTP/1.1
  - GET, POST, HEAD
  - PUT
  - DELETE
  - CONNECT
- http는 hyper text를 교환하기 위한 용도로 개발되었지만, 간단하고 실용성이 좋아서 다양한 곳에서 통신 프로토콜로 사용된다

## 3. Uploading form input

- Post method

  ![2](https://user-images.githubusercontent.com/48282185/177447750-2b3cbeab-a238-40d0-b142-92ff72ae1676.png)

  - 웹페이지는 종종 form input이 요구된다
  - input은 body전체가 서버에 업로드 된다

- URL method

  ![3](https://user-images.githubusercontent.com/48282185/177447748-fc210768-1029-43ad-b006-6a975f237447.png)

  - GET메소드를 사용하는 방법
  - input은 URL의 request line이 업로드된다
  - 비교적 간단하여 작은 크기의 Input인 경우 사용됨

## 4. HTTP response message

![4](https://user-images.githubusercontent.com/48282185/177447735-93587b26-391b-4651-a6e6-1c3860dd1a8f.png)

## 5. User-server state: cookies

- 수 많은 웹 사이트들에서 쿠키가 사용된다
  - cookie는 어떤 특정한 웹사이트에 접근했을 때 다운로드되는 문자들과 숫자들로 이루어진 작은 파일이다
  - 서버가 사용자의 상태정보를 관리하기 위한 도구로 이용된다
- 네 가지 컴포넌트로 구성된다
  - HTTP 응답 메시지의 cookie header line
  - HTTP 요청 메시지의 cookie header line
  - user의 브라우저에 의해 관리되고, cookie 파일을 user의 host에 유지된다
  - web site의 backend database에 쿠키정보가 저장됨
- 로그인이 필요없게 만들어 주는데, 브라우저에 의해 관리된다고 했다, 그럼 브라우저를 통해 접근할 수 있다는 말이 되기도 한다. 브라우저에서 쿠키값을 조회할 수 있고, 보안 상 취약하다
- 어떻게 사용될 수 있을까?
  - Authorization
  - shopping cart
  - recommendations
  - user session state
