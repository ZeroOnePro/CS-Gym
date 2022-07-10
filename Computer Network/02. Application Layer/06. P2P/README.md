# 06. P2P

<!--ts-->

- [06. P2P](#06-p2p)
  - [1. Pure P2P architecture](#1-pure-p2p-architecture)
  - [2. P2P file sharing tech evolution](#2-p2p-file-sharing-tech-evolution)
  - [3. Napster, Centralized directory](#3-napster-centralized-directory)
  - [4. Gnutella](#4-gnutella)
  - [5. KaZaA, Exploiting Heterogeneity](#5-kazaa-exploiting-heterogeneity)
  - [6. 분산 파일 시스템에서 클라이언트 - 서버 구조 vs P2P](#6-분산-파일-시스템에서-클라이언트---서버-구조-vs-p2p)
  - [7. P2P file distribution, BitTorrent](#7-p2p-file-distribution-bittorrent)
  - [8. P2P의 응용](#8-p2p의-응용)

<!-- Created by https://github.com/ekalinin/github-markdown-toc -->
<!-- Added by: sungminyou, at: 2022년 7월 11일 월요일 06시 03분 03초 KST -->

<!--te-->

## 1. Pure P2P architecture

- No always-on server
  - 특정 어플리케이션이 항상 서버의 역할을 하지 않는다
- 흩어져있는 end systems들 간에 직접적인 상호작용이 일어난다
- peer들은 서로 간헐적으로 연결되고 IP가 수시로 변한다
- 주로 파일 공유(토렌트), 스트리밍(시청자를 중계 역할 시킴), VoIP(Skype, 전화 연결의 중간 다리 역할을 사용자에게 시킴), 비트 코인

## 2. P2P file sharing tech evolution

- 다음은 P2P를 이용한 파일 공유 시스템이 어떻게 발전되어왔는지를 보여준다
- Nasper
- eDonkey
- Gnutella
- Kazaa
- freenet
- BitTorrent

## 3. Napster, Centralized directory

- 누가 어떤 파일을 가지고 있는지, 누가 어떤 파일을 원하는지 이 두 가지 정보가 P2P filesharing의 핵심 정보이다
- 중앙 서버를 두고 이 서버가 누가 어떤 파일을 가지고 있는지 관리하고 있다
- IP 주소와 컨텐츠를 쌍으로 관리하고 요청이 들어오면 그 컨텐츠를 가지고 있는 피어들이 요청을 받아들임
- 중앙 서버가 고장나면 서비스를 이용하지 못하는 단점이 있다

## 4. Gnutella

![1](https://user-images.githubusercontent.com/48282185/178162056-71740499-f1d7-4c0a-a41c-7270a33a62c9.png)

- TCP 연결 상에서 query message를 보낸다
- 하나의 peer는 네트워크에 참가하는 peer들 전체는 알 수 없지만 이웃 peer들은 알고있다
- 컨텐츠가 없는 피어는 query를 forward하는 역할을 맡는다
- 컨텐츠가 있는 peer는 queryHit message를 query message가 왔던 경로를 역으로 타고가서 전달한다
- HTTP 프로토콜을 이용하여 쿼리 및 파일 전송이 이루어진다
- 후보가 여러 개인 경우 최선의 후보를 선택하는 방법은 구현하기 나름
- 이 방식의 문제점은 query 메시지로 인해 네트워크의 트래픽에 낭비를 줄 수 있다는 것이다 즉, scalability가 문제이고 limited scope flooding을 둬서 낭비를 방지할 수 있으나 어느 정도가 적당한지를 알 수 없으므로설정이 어렵다.

## 5. KaZaA, Exploiting Heterogeneity

![2](https://user-images.githubusercontent.com/48282185/178162054-bd286294-7897-4f13-b75c-1937c2f22ce9.png)

- naster의 중앙 서버가 여러 개로 분리되고, 이 서버는 그룹을 대표하는 피어가 된 시스템 구조
- 각 피어는 그룹 리더이거나 그룹 멤버가 된다
  - TCP 연결로 그룹 리더와 그룹 멤버들의 통신이 이뤄지고, 그룹 리더와 다른 그룹의 리더의 통신이 이뤄진다
- 각 그룹 리더는 그 그룹의 멤버들이 가지고 있는 컨텐츠를

## 6. 분산 파일 시스템에서 클라이언트 - 서버 구조 vs P2P

![3](https://user-images.githubusercontent.com/48282185/178162053-2d752ef8-8ff1-4f4b-ae73-3fbf0e748bf3.png)

- u는 업로드 d는 다운로드
- 클라이언트 - 서버 구조
  - 서버가 하나의 파일을 내보내는데 걸리는 시간 $F \over u_s$
  - N개의 클라이언트들에게 보낼 파일을 전부 내보내는데 걸리는 시간 $N \times F \over u_s$
  - 각 클라이언트들은 파일을 다운로드 받아야하므로
    - $d_{min}$은 제일 전송률이 낮은 링크에 달려있는 클라이언트의 다운로드 비율로 정의
    - 제일 다운로드가 느린 클라이언트의 다운로드 시간은 $F \over d_{min}$
  - 사이즈가 F인 파일을 N개의 클라이언트에게 보내는 시간은 $D_{clinet-server} \geq max({N \times F \over u_s}, {F \over d_{min}})$
- P2P 구조
  - 서버가 하나의 파일을 내보내는데 걸리는 시간 $F \over u_s$
  - 제일 다운로드가 느린 피어의 다운로드 시간은 $F \over d_{min}$
  - 총 N x F 비트의 크기가 다운로드 받아져야함
    - 최대 upload 비율은 $u_s + \sum_{i=1}^{N} {u_i}$
  - 사이즈가 F인 파일을 N개의 클라이언트에게 보내는 시간은 $D_{p2p} \geq max({N \times F \over u_s}, {F \over d_{min}}, {N \times F \over u_s + \sum_{i=1}^{N} {u_i}})$
- 비교

  ![4](https://user-images.githubusercontent.com/48282185/178162050-db255d6b-cc83-4ae1-86af-6fa2845c8d96.png)

  - F / u = 1시간, $u_s = 10u$, $d_{min} \geq u_s$일 때 두 시스템 간의 시간 비교

## 7. P2P file distribution, BitTorrent

- 가장 큰 특징은 파일을 잘랐다는 것임
- file을 256KB의 chunk로 분리
- 장점은 하나의 파일을 동시에 여러 피어들로부터 받을 수 있어서 한 피어에게 한 파일을 통째로 받는 것보다 시간절감을 할 수 있고, 동시에 받은 파일을 다른 피어에게 제공자 역할을 할 수 있다
- tracker(중앙 서버)는 각 피어들이 가지고 있는 컨텐츠와 주소를 관리
- 다운로드하면서 chunk를 다른 피어에게 나눠준다
- 한 피어가 속한 부분 그룹을 neighbors라 부르고, 이 neighbors는 수시로 변할 수 있다
- Churn은 피어가 그룹에 들어가거나 떠나거나할 수 있음을 뜻하는 용어
- 다운로드가 끝났음에도 그룹을 떠나지않고 계속 제공하는 역할을 하는 피어를 seed라고 부름
- chunk가 중복되는 경우가 가장 적은 경우를 우선적으로 다운로드 받는다(rarest first), 여러 피어들이 동일한 chunk를 가질 수 있기 때문에 생기는 우선순위
- tit-for-tat
  - 네트워크 활성화를 위한 일종의 패널티
  - 각 피어들의 기여 기록을 가지고 있다가 도움이 안되는 피어는 neighbor에서 내쫓고 새로운 피어를 가입시키는 상호작용을 한다
  - 일종 시간 단위로 계속 기여 리스트를 갱신한다

## 8. P2P의 응용

- 기술 자체를 접목시킨다기 보다 큰 파일을 서로 공유할 때 덩어리 vs chunk
- 중앙 서버 모델 vs 피어 모델
- 각 기법이 왜 나왔는지 어떤 이유로 이렇게 설계됐는지 그로 인해 어떤 장단점을 갖는지에 대해 생각해보자
