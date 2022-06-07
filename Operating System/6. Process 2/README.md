# 6. Process 2

## 1. Thread

- A thread(a.k.a. lightweight process) is a basic unit of CPU utilization
  - 하나의 프로세스 내부에서 하나의 제일 작은 기본 CPU 수행단위(=instruction을 수행하기 위해 필요한 것들)
- Thread의 구성
  - program counter
  - register set
  - stack space
- Thread가 동료 thread와 공유하는 부분(task라고도 부른다)
  - code section
  - data section
  - OS resources
- 전통적인 개념의 heavyweight process는 하나의 thread를 가지고 있는 task로 볼 수 있다.
- 같은 일을 하는 process를 여러개 띄워놓으면 각 프로세스마다 주소공간을 갖기에 메모리 낭비다. 그래서 하나의 주소공간 내에서 여러개의 작업단위를 두어 같은 일을 시키게끔 하기 위해 thread가 등장하였다.

## 2. Thread를 사용하는 이유

- 다중 스레드로 구성된 태스크 구조에서는 하나의 서버 스레드가 blocked(waiting)상태인 동안에도 동일한 태스크 내의 다른 스레드가 실행(running)되어 빠른 처리를 할 수 있다.
- 동일한 일을 수행하는 다중 스레드가 협력하여 높은 처리율(throughput)과 성능 향상을 얻을 수 있다.
- 스레드를 사용하면 병렬성을 높일 수 있다.(cpu가 여러개인 상황에서 볼 수 있는 특수 상황)
  - 행렬 곱셈, 각 행과 각 열을 곱하는 것을 각각의 thread가 수행하여 나중에 결과를 합친다.

![1](https://user-images.githubusercontent.com/48282185/172348288-988ba30d-e3c5-4cb2-81c5-c8a7190fcda9.png)
![2](https://user-images.githubusercontent.com/48282185/172348274-2cfdcbb0-a534-45d6-8a06-6fb1dcf4a773.png)

## 3. Thread의 장점

- 응답성(Responsiveness)
  - 예시로 브라우저가 멀티쓰레드로 구성되있다고 하면, 서버에 요청하여 응답 결과물을 받을 것이다. 예를들어 html를 반환한다고 하자. html내에는 이미지들도 있을것이고, 텍스트도 있을 것이다. 이미지들은 재요청을 해서 들고와야되지만(IO로 인한 blocked), 텍스트의 경우 바로 렌더링 할 수 있다.(running) 이 경우 이미지 요청 쓰레드와 텍스트 렌더링 쓰레드를 따로 두어 텍스트를 바로 렌더링 해준다면 사용자입장에서 답답함을 느끼지 않을 것이다. 이 경우 일종의 비동기식 입출력이다.
- 자원 공유(Resource Sharing)
  - 주소공간 낭비 방지
- 경제성(Economy)
  - 프로세스의 생성과 프로세스 간의 컨텍스트 스위치는 오버헤드가 큰데 비해 쓰레드의 생성과 쓰레드 간의 CPU switching의 경우 간단하다.(동일 주소공간이기에 대부분의 context가 동일하다.)
  - Solaris OS의 경우 overhead가 생성에서 30배, 컨텍스트 스위칭의 경우 5배 차이가 난다.
- 멀티프로세서환경의 활용(CPU가 여러개 일 때)
  - 각 쓰레드가 서로 다른 CPU에서 병렬적으로 동작할 수 있다.

## 4. Thread의 구현

- kernel thread
  - 커널 쓰레드의 경우 커널에서 쓰레드가 여러개가 관리되는 것을 알고있다.
- user thread
  - 프로세스 내에 여러개의 쓰레드가 있다는 것을 커널에서는 모른다. 사용자 어플리케이션에서 관리
  - 커널 시점에서는 추상화 되어있기때문에 구현 상에 제약이 있을 수 있다.

> 출처

> ABRAHAM SILBERSCHATZ ET AL., OPERATING SYSTEM CONCEPTS, NINTH EDITION, WILEY, 2013

> 반효경, 운영체제와 정보기술의 원리, 이화여자대학교 출판부, 2008
