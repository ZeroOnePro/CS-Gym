# 1. 빅데이터 관리 서론

<!--ts-->

- [1. 빅데이터 관리 서론](#1-빅데이터-관리-서론)
  - [1. 빅데이터의 정의](#1-빅데이터의-정의)
  - [2. 빅데이터의 등장 배경](#2-빅데이터의-등장-배경)

<!-- Created by https://github.com/ekalinin/github-markdown-toc -->
<!-- Added by: sungminyou, at: 2022년 9월  3일 토요일 22시 03분 39초 KST -->

<!--te-->

## 1. 빅데이터의 정의

- 일반적인 데이터베이스 sw가 저장, 관리, 분석할 수 있는 범위를 초과하는 규모의 데이터
- 다양한 종류의 대규모 데이터로부터 저렴한 비용으로 가치를 추출하고 초고속 수집, 발굴, 분석을 지원하도록 고안된 차세대 기술 및 아키텍처
- 3V
  - Volume(크기)
  - Velocity(속도)
  - Variety(다양성) ⇒ 각종 센서 및 서비스가 소프트웨어로 구현되면서 다양한 분야에서 데이터를 쌓게됨

## 2. 빅데이터의 등장 배경

- 저장장치가 점차 크기도 커지고 저렴해짐, 데이터를 더 많이 쌓으면서 삭제할 필요도 없음
- 데이터 처리 기술의 발전

  - 데이터 전송 속도, CPU의 처리량 증가
  - 병렬 분산 처리 기술의 등장

    - 제일 bottle neck이 되는 지점은 I/O ⇒ 이것을 극복하는 방법은 병렬처리, 더 많은 저장장치에 데이터를 나눠서 저장하고, 나눈 데이터는 크기가 작으니 읽는 속도가 빠르다, 분산된 저장장치에 있는 내용을 동시에 읽는다
    - 구글 MapReduce 기반 오픈소스 hadoop(여러 컴퓨터에 분산하여 저장하고 병렬로 read / write를 가능하게 하는 파일 시스템) 보급

      [hadoop이란 무엇인가?](https://wikidocs.net/22654)

      - 데이터를 나눠서 여러 대 컴퓨터에 매핑하고(map) 필요 시 합친다(reduce)

      ![1](https://user-images.githubusercontent.com/48282185/188271691-d2f96cd4-29bc-4c9b-b8ce-9bf598c817ec.png)

  - 빅데이터 분석 도구까지 확보

> 출처
>
> - 이화여대 용환승 교수님 강의
> - 홍의경 저, MS SQL Server 기반 데이터베이스 배움터, 생능출판사, 2012년,
> - 이석호 저, 데이터베이스 시스템, 정익사, 2009.
> - A. Silberschatz, H. Korth, S. Sudarshan, "Database System Concepts," 6th Ed., McGraw-Hill, 2010.
> - Elmasri and Navathe, “ Fundamentals of Database Systems”, 6th ed. Addison-Wesley, 2010.
> - C.J.Date, “An Introduction to Database Systems (8th Edition)”, Addison-Wesley, 2003.
> - Won Kim, "Modern Database Systems," ACM Press, 1994
