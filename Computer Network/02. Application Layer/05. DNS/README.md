# 05. DNS

<!--ts-->

- [05. DNS](#05-dns)
  - [1. DNS(domain name system)](#1-dnsdomain-name-system)
  - [2. Domain name rule](#2-domain-name-rule)
  - [3. DNS의 서비스와 구조](#3-dns의-서비스와-구조)
  - [4. Hierarchical domain name structure](#4-hierarchical-domain-name-structure)
  - [5. distributed, hierarchical database](#5-distributed-hierarchical-database)
  - [6. TLD, authoritative server](#6-tld-authoritative-server)
  - [7. Local DNS name server](#7-local-dns-name-server)
  - [8. DNS name resolution - iterative query(반복적 쿼리)](#8-dns-name-resolution---iterative-query반복적-쿼리)
  - [9. DNS name resolution - recursive query(재귀적 쿼리)](#9-dns-name-resolution---recursive-query재귀적-쿼리)
  - [10. Caching, updating records](#10-caching-updating-records)
  - [11. DNS records](#11-dns-records)
  - [12. DNS의 메시지 형식](#12-dns의-메시지-형식)
  - [13. DNS query 날려보기](#13-dns-query-날려보기)
  - [13. IP주소에서 name으로 역변환](#13-ip주소에서-name으로-역변환)
  - [14. Punycode](#14-punycode)

<!-- Created by https://github.com/ekalinin/github-markdown-toc -->
<!-- Added by: sungminyou, at: 2022년 7월  6일 수요일 23시 39분 17초 KST -->

<!--te-->

## 1. DNS(domain name system)

- 사람의 identifier
  - 주민등록번호, 이름, 여권 번호 등
- 인터넷 호스트, 라우터의 identifier
  - IP주소, datagram을 addressing하기 위해 필요
  - name, www.naver.com, 사람이 사용하기 위한 주소
- DNS
  - 다 수개의 name server들의 계층으로 구현된 분산된 데이터베이스
  - 호스트들과 name server가 상호작용하기 위해 고안된 application layer의 프로토콜
    - name - IP 주소 간 address translation
    - 사용자들이 어플리케이션의 서비스를 받기 위해 실행하는 프로토콜이기에 application layer에 속함

## 2. Domain name rule

- 알파벳 문자, 숫자, 하이픈 조합으로 구성됨
- 하이픈으로 시작하거나 끝나지 않는다
- label의 최대 길이는 63byte(label은 dot으로 구분된 단위)
- name의 최대 길이는 253byte

## 3. DNS의 서비스와 구조

- 제일 큰 목적은 IP주소와 hostname 주소의 변환이다
- host aliasing
  - host의 이름이 길 때 별명으로 부름
  - 별명을 canonical, alias names라고 부름
- mail server aliasing
- load distribution
  - replicated web servers의 경우 다 수개의 IP들이 하나의 hostname에 매핑되어야함
- DNS는 왜 중앙집중형으로 구현하지 않을까?
  - 하나의 point에 인터넷 사용자 모두가 몰리면 트래픽은 상상도 할 수 없이 커지고, 이 서버가 고장나면, 주소 매핑 서비스를 이용할 수 없다
  - DNS 서버와 host 사이의 거리가 멀 때 응답시간이 커지는 문제가 생긴다
  - 관리가 어렵다

## 4. Hierarchical domain name structure

![1](https://user-images.githubusercontent.com/48282185/177577056-5b4e8198-31e6-4375-a04f-2f54f95ed888.png)

- Top Level Domain(TLD)
  - ccTLD(country code TLD)
    - 나라 별로 주어진 TLD
    - kr, uk, us, …
  - gTLD(generic TLD)
    - 일반적으로 널리알려진 TLD
    - org, edu, net, com, info, biz, …
- Second Level부터는 각 국가별 또는 사업자 별로 정하기 나름이다

## 5. distributed, hierarchical database

![2](https://user-images.githubusercontent.com/48282185/177577055-94526ff5-f79c-4b48-b168-d1e800b19db2.png)

- client가 www.amazon.com의 IP주소를 얻는 과정
  - root DNS 서버에 요청을 보내서 com DNS 서버의 IP주소를 알아낸다
  - com DNS 서버는 amazon.com을 관리하는 DNS서버의 IP주소를 알려준다
  - amazon.com을 관리하는 DNS서버에 가면 원하는 IP주소를 얻을 수 있다
- root DNS 서버는 전 세계에 퍼져있어 국가 내에서 해결 가능하다

## 6. TLD, authoritative server

- TLD server
  - generic TLD, ccTLD를 책임지는 서버
  - network soultions라는 회사에서 .com의 DNS서버를 관리함
  - educause 기관에서 .edu DNS서버를 관리함
- Ahthoritative server
  - 기관을 대표하는 DNS 서버
  - 유저가 요청한 hostname을 IP주소로 변환시켜주는 책임을 갖는 서버
  - 서비스 제공자가 기관에의해 관리됨

## 7. Local DNS name server

- host가 좀 더 빨리 DNS 서비스를 받기위해 존재하는 서버
- 각각의 ISP에서 하나씩 가지고 있음, default name server라고도 불린다
- host에서 DNS 쿼리를 날리면 local dns server로 요청이 날라간다
  - name-ip주소 쌍을 캐싱해두고 있다(최신화는 안 되있을지 모름)

## 8. DNS name resolution - iterative query(반복적 쿼리)

![3](https://user-images.githubusercontent.com/48282185/177577048-5c7b73d2-b805-4533-a972-0c67f28e92de.png)

- query를 시작한 주체가 반복적으로 query를 날리는 방식

## 9. DNS name resolution - recursive query(재귀적 쿼리)

![4](https://user-images.githubusercontent.com/48282185/177577046-7e7ff979-d02a-4303-b836-310e4906d3d7.png)

- 한 서버에 호출을 해놓으면, 재귀적으로 타고들어가서 답을 얻고 다시 리턴하는 방식

## 10. Caching, updating records

- name과 IP주소는 거의 바뀔일이 없다 그래서 매번 요청하는 것은 비효율적이다
- 하지만 캐싱의 가장 큰 이슈는 정보가 최신화 되었는지 알 수 없다는 점이다
- 그래서 캐싱할 때 유효기간(TTL, Time To Live)을 둬서 기한이 지나면 기록을 지우고 재요청해서 다시 저장해놓는다
- RFC 2136에는 update / notify 메카니즘이 제안되었음
  - Ip주소나 name이 변경되면, 변경된 주체는 자신과 연결되어있는 서버들에게 이 사실을 통보하고 캐시되있는 정보들도 갱신된다

## 11. DNS records

- DNS는 distributed DB storing resource records(RR)
- RR format
  - (name, value, type, ttl)
- type

  - A
    - name이 hostname
    - value는 ip주소
  - MX
    - name은 hostname
    - value가 mailserver의 canonical name이다
  - NS
    - name이 domain name
    - value는 이 도메인의 authoritative name server의 hostname을 담고있다
  - CNAME

    ![5](https://user-images.githubusercontent.com/48282185/177577044-cfaa5990-9d1a-4574-be32-738674a381ca.png)

    - name이 실제 이름(canonical name)의 alias name(별칭)이다
      - 위 예시에서 bar.example.com
    - www.ibm.com의 실제 이름은 servereast.backup2.ibm.com
    - value는 canonical name을 담고있다
      - 위 예시에서 foo.example.com
      - ip와 매핑되는 실제 이름

## 12. DNS의 메시지 형식

![6](https://user-images.githubusercontent.com/48282185/177577037-760c3cd5-aff2-494b-b821-e61bd0607a27.png)

- query와 reply 두 가지 형태의 메시지가 존재하고, 같은 포멧을 갖는다
- 다른 application layer에 속한 프로토콜과는 다르게 사람이 읽기 어렵다, 빠른 응답 속도를 위해 불친절한 방식을 택함
- message header
  - identifier
    - 16bit for query
    - reply to query uses same(query와 동일한 것을 사용)
  - flag
    - query인지 reply
    - recursion desired, 재귀적 질의가 가능한지
    - recursion avaliable, DNS서버가 재귀 질의를 지원하면 응답에 설정값을 남김
    - reply is authoriative
- message data
  - questions
    - 현재 질의에 대한 정보가 기록되있다, 질의되는 이름, 이름에 대해 문의되는 질문의 RR type
  - answers
    - 질의된 이름에 대한 RR을 포함한다, 응답으로 여러개의 RR이 포함될 수 있다, 하나의 호스트 네임이 여러개의 IP를 갖는 것이 가능하기때문이다, 답변이 여러개가 될 수 있기 때문에 question 위에 헤더에 RR들의 개수가 써진다
  - additional record는 주소 변환에 있어 도움이 될 정보들이 실려있다

## 13. DNS query 날려보기

- nslookup 툴을 이용해본다
  - non-authoritative answer는 서버에서 DNS값을 캐시로 가지고 있을 때 그 값을 가져온 것, 변경 사항이 반영이 안 됐을 수 있어서 경고 목적으로 나오는 것임
    ![7](https://user-images.githubusercontent.com/48282185/177577011-7dacda56-e3d6-4d91-89a9-0203d6e1b9a4.png)

## 13. IP주소에서 name으로 역변환

- ip주소를 알고있는 상황에서 name을 알고싶을 때 사용
- RR type: PTR로 요청하면 name을 얻을 수 있다

## 14. Punycode

- Domain name은 숫자, 문자, 하이픈 조합(LDH)으로 이루어진다고 했다. 근데, 한글이나 일본어로 된 domain name들이 존재한다 어떻게 가능한가?
- 결론은 불가능하고, 사용자에게만 그렇게 보여주게하고, application에서 변환해서 기존 domain name의 규칙에 맞게 변환해준다
- RFC 3492에 Punycode는 application의 국제화 도메인 이름에 대한 유니코드의 부트스트링 인코딩이라고 정의돼있다
- 한국 → xn—3e0b707e, 변환된 이름에도 길이 제한이 있기때문에, 이것도 길이 제한이 존재한다
