# 07. DHT

<!--ts-->

- [07. DHT](#07-dht)
  - [1. Distributed Hash Table (DHT)](#1-distributed-hash-table-dht)
  - [2. 키 부여 원칙](#2-키-부여-원칙)
  - [3. DHT identifier](#3-dht-identifier)
  - [4. Circular DHT](#4-circular-dht)
  - [5. Circular DHT에서 질의(검색) 과정](#5-circular-dht에서-질의검색-과정)
  - [6. Circular DHT의 파일 공유 예시](#6-circular-dht의-파일-공유-예시)
  - [7. Circular DHT with shortcut](#7-circular-dht-with-shortcut)
  - [8. shortcut 예시](#8-shortcut-예시)
  - [9. Peer churn](#9-peer-churn)
  - [10. DHT의 응용](#10-dht의-응용)

<!-- Created by https://github.com/ekalinin/github-markdown-toc -->
<!-- Added by: sungminyou, at: 2022년 7월 12일 화요일 04시 44분 35초 KST -->

<!--te-->

## 1. Distributed Hash Table (DHT)

- DHT는 P2P를 이용하여 file sharing을 하는 방법 중 하나
- 해시테이블(database)이 여러 peer들 사이에 분산돼 저장되는 구조
- key, value쌍으로 된 database를 가지고 있다
  - content가 키, 위치가 value

## 2. 키 부여 원칙

- 어떤 피어에게 어떤 정보를 저장시킬 것인가에 대한 원칙
- basic idea
  - 각 키를 integer로 변환한다(해싱기법을 이용)
  - 각 피어에게 integer를 부여한다
  - 숫자값에 가깝거나 같은 피어에게 이 정보를 저장시킨다
    ![1](https://user-images.githubusercontent.com/48282185/178345704-0c89dd52-3370-4d07-b203-c2e73406da82.png)

## 3. DHT identifier

- [0, $2^n$-1]의 범위 내의 컨텐츠를 각 피어에게 할당한다, 이 때 각 identifier를 나타내기 위해 n비트가 필요
- 각 키들은 해당 범위 내의 정수여야함
- ip주소와 컨텐츠가 각각 hash funtion의 결과물로 identifier를 얻었을 텐데 나온 identifier가 서로 다르면 어떻게 할 것인가?

  ![2](https://user-images.githubusercontent.com/48282185/178345703-13b890d8-28c6-4090-8fbf-157e9ee9d185.png)

  - 각 피어들의 ip주소들의 hash funtion 결과물이 위와 같다고 할 때 key가 13인 것과 15인 것은 어디에 저장되어야할까?
  - 가장 가까운 key에 저장시킨다
  - 가까우면 key보다 큰 것인가? 작은 것인가?
    - 큰 것으로 선택한다 이유는, key를 peer에 저장시키기 위해 peer들의 결과물을 확인할 때 만약 key보다 작으면서 제일 큰 값으로 저장한다면 해당 Key가 있는지 검색하기 위해 key보다 작으면서 제일 큰 값 이후로도 key를 찾기 위해 검색이 지속되지만, key보다 크면서 제일 작은 값으로 설정한다면, 그 값까지 검색하고나서 key가 없다는 것을 확신할 수 있다

## 4. Circular DHT

![3](https://user-images.githubusercontent.com/48282185/178345700-2f85b76f-969a-4bf5-a2ba-5f5eab5c4e8d.png)

- 각 피어들은 자신의 바로 앞(predecessor)과 뒤(successor)의 정보를 가지고 있고, 쿼리가 들어오면 자신에게 있으면 처리하고 없으면 인접으로 쿼리를 넘긴다
- 각 그룹에서 피어가 빠지거나 추가되면 그 피어의 앞과 뒤 피어들의 정보들만 바꿔주면 된다
- 파일의 양이 굉장히 많고, 한 서버의 부담이 심한 상황에서 고려할만한 옵션이다

## 5. Circular DHT에서 질의(검색) 과정

![4](https://user-images.githubusercontent.com/48282185/178345696-1fb55047-205b-4843-a7ab-cdecfdb1534f.png)

- closest를 key보다 크면서 가장 작은 노드라고 정의
- 질의를 시작하는 피어는 DHT에 참가하고 있는 주변 피어들의 정보를 알고 있어야 질의 가능
- 최악의 경우 O(N)이 소요됨

## 6. Circular DHT의 파일 공유 예시

![5](https://user-images.githubusercontent.com/48282185/178345691-26a7e165-d4c2-4710-9c23-7ba597df1b16.png)

- ip주소와 file 이름을 hash function에 넣으면 key가 나오고 이 key의 범위는 정해져있다
- closest neighbor는 key보다 크면서 제일 작은 노드 번호로 정의했으므로 liberty파일에 대한 정보는 17번 노드가 저장한다
- 하드디스크를 서로 공유하자는 아이디어에서 출발
  - 파일을 가지고있는 컴퓨터 따로 파일 위치 정보에 대한 컴퓨터 따로, 한 서버가 모든 정보를 관리하는 방식보다 서버의 분담을 줄여줄 수 있고, 남는 PC의 용량을 활용할 수 있다

## 7. Circular DHT with shortcut

![6](https://user-images.githubusercontent.com/48282185/178345687-2cb0fbae-4cb9-49f5-a696-f481ddd5c465.png)

- 질의 과정이 O(N)으로 비효율적이라 이를 해결하고자 나온 방법
- 각 피어들은 자신의 이웃들 뿐만아니라 shortcut(이웃이 아닌 DHT의 다른 노드)의 정보를 갖는다

## 8. shortcut 예시

![7](https://user-images.githubusercontent.com/48282185/178345668-9132dc3f-bd58-4a7d-bc78-b8e6897ce9ed.png)

- 구현하기 나름이지만 이 예시에서는 아래 규칙을 shortcut으로 정한다
- 자신의 id + $2^k$(k=0, 1, 2, 3, 4)
- N5를 보면 id가 5이므로
  - k =1, 6번 파일 관리 → N10
  - k=2, 7 ~ 8번 파일 관리 → N10
  - k=3, 9 ~ 12번 파일 관리 → N10
  - k=4, 13 ~ 20번 파일 관리 → N20
  - k=5, 20 ~ 25번 파일 관리 → N25
  - 즉 이렇게 몇 번 파일을 누가 관리하고 있는지 알고있으니 질의를 보낼 필요없이 바로 알 수 있다 → O(1)

## 9. Peer churn

- peer가 DHT에 참가하거나 빠져나가거나 할 때 어떻게 해야할까?
- handling peer churn
  - 각 피어들은 갑자기 예기치 못하게 빠져나갈 수 있기에 빠져나가는 피어가 handling하는 방법은 적절치 못하다
  - 각 피어들이 다음과 다다음 피어의 주소를 알게한다
  - 각 피어는 주기적으로 다음과 다다음 피어가 DHT에서 떠났는지 확인한다
  - 만약 떠났다면 떠난 자리를 떠난 피어의 다음 피어로 매꾼다

## 10. DHT의 응용

- file sharing 뿐 만아니라 검색 시스템을 설계한다고 했을 때 적용 고려해볼만한 옵션이 될 수 있다
