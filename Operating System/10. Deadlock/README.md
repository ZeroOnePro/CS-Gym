# 10. Deadlock

<!--ts-->

- [10. Deadlock](#10-deadlock)
  - [1. The Deadlock Problem](#1-the-deadlock-problem)
  - [2. Deadlock의 발생 조건 4가지](#2-deadlock의-발생-조건-4가지)
  - [3. Resource - Allocation Graph(자원할당그래프)](#3-resource---allocation-graph자원할당그래프)
  - [4. Deadlock의 처리 방법](#4-deadlock의-처리-방법)
  - [5. Deadlock Prevention](#5-deadlock-prevention)
  - [6. Deadlock Avoidance](#6-deadlock-avoidance)
  - [7. Resouce Allocation Graph Algorithm](#7-resouce-allocation-graph-algorithm)
  - [8. Banker’s Algorithm](#8-bankers-algorithm)
  - [9. Deadlock detection and recovery](#9-deadlock-detection-and-recovery)
  - [10. Deadlock Ignorance](#10-deadlock-ignorance)

<!-- Created by https://github.com/ekalinin/github-markdown-toc -->
<!-- Added by: sungminyou, at: 2022년 6월 17일 금요일 16시 11분 46초 KST -->

<!--te-->

![1](https://user-images.githubusercontent.com/48282185/174245627-6e8cb1e4-53f0-45ad-af60-8d69a779ef43.png)

## 1. The Deadlock Problem

- Deadlock
  - 일련의 프로세스들이 서로가 가진 자원을 기다리며 block된 상태
- Resource(자원)
  - 하드웨어, 소프트웨어 등을 포함한 개념, 프로세스가 수행되면서 필요한 모든 것
    - IO device, CPU cycle, memory space, semaphore 등
  - 프로세스가 자원을 사용하는 절차
    - Request, Allocate, Use, Release
- Deadlock Example 1
  - 시스템에 2개의 tape drive가 있다.
  - 프로세스 P1과 P2 각각이 하나의 tape drive를 보유한 채 다른 하나를 기다리고 있다.
- Deadlock Example 2

  - binary semaphores A and B

    ![2](https://user-images.githubusercontent.com/48282185/174245625-70cfe9f0-a7e4-4066-8e21-502b4111a327.png)

## 2. Deadlock의 발생 조건 4가지

- 아래 4가지 조건을 모두 만족해야함
- Mutual Exclusion(상호 배제)
  - 매 순간 하나의 프로세스만이 자원을 사용할 수 있다
- No preemptive(비선점)
  - 프로세스는 자원을 스스로 내어놓을 뿐 강제로 빼앗기지 않는다
- Hold and wait(보유 대기)
  - 자원을 가진 프로세스가 다른 자원을 기다릴 때 보유자원을 놓지 않고 계속 가지고 있는다
- Circular wait(순환 대기)
  - 자원을 기다리는 프로세스 간에 사이클이 형성되어야한다
  - 프로세스 P0, …, Pn이 있을 때 P0 → P1, P1 → P2, …, Pn → P0자원을 기다림

## 3. Resource - Allocation Graph(자원할당그래프)

- Deadlock이 발생했는지 확인하기 위해 사용되는 그래프

![3](https://user-images.githubusercontent.com/48282185/174245621-9592c710-6d38-4f8a-8557-651083ae8bf0.png)
![4](https://user-images.githubusercontent.com/48282185/174245617-e7a3e418-89e5-4690-9d0c-a26a51a91a1a.png)

- P → R
  - 프로세스 P가 자원 R을 요청
- R → P
  - 프로세스 P가 자원 R을 보유하고있음
- R내의 점은 자원의 수, 아무거나 가져다써도 자원 쓸 수 있음
- 그래프에 cycle이 없으면 deadlock이 아니다
- 그래프에 cycle이 있으면
  - 자원 당 인스턴스가 1개인 경우, deadlock이다.
  - 자원 당 여러개의 인스턴스가 있는 경우, deadlock의 가능성이 있다.

## 4. Deadlock의 처리 방법

- Deadlock prevention
  - 자원 할당 시 deadlock의 4가지 필요 조건 중 어느 하나가 만족되지 않도록 하는 것
- Deadlock avoidance
  - 자원 요청에 대한 부가적인 정보를 이용하여 deadlock의 가능성이 없는 경우에만 자원을 할당
  - 시스템 state가 원래 state로 돌아올 수 있는 경우에만 자원 할당
- Deadlock Detection and recovery
  - Deadlock발생은 허용하되 그에 대한 detection 루틴을 두어 deadlock발견 시 recover
- Deadlock Ignorance
  - Deadlock을 시스템이 책임지지 않음
  - UNIX를 포함한 대부분의 OS가 채택한 방식
  - 사람이 알아서 처리한다
  - Deadlock은 빈번한 이벤트가 아니기 때문에 이를 위한 루틴은 overhead가 크다

## 5. Deadlock Prevention

- Mutual Exclusion
  - 막을 수 있는 방법이 아님
  - 공유해서는 안되는 자원의 경우 반드시 성립해야 함
- Hold and wait
  - 프로세스가 자원을 요청할 때 다른 어떤 자원도 가지고 있지 않아야 한다
  - 자진 반납으로 해결
  - 방법 1. 프로세스 시작 시 모든 필요한 자원을 할당받게 하는 방법 → 자원이 매순간 마다 쓰이는 것이 아니기에 비효율적
  - 방법 2. 자원이 필요할 경우 보유 자원을 모두 놓고 다시 요청
- No Preemptive
  - process가 어떤 자원을 기다려야 하는 경우 이미 보유한 자원이 선점됨
  - 모든 필요한 자원을 얻을 수 있을 때 그 프로세스는 다시 시작된다
  - State를 쉽게 save하고 restore할 수 있는 자원에서 주로 사용(CPU, Memory)
- Circular Wait
  - 모든 자원 유형에 할당 순서를 정하여 정해진 순서대로만 자원 할당
  - 할당 받는 자원의 순서가 내림차순이나 오름차순이다
    - 1→3, 3→1이런 경우가 안생김
  - 예를 들어 순서가 3인 자원 Ri를 보유 중인 프로세스가 순서가 1인 자원 Rj을 할당받기 위해서는 우선 Ri를 Release해야한다.
- 이 방식의 문제
  - Utilization 저하
  - throughput 감소
  - starvation 문제
  - 생길지도 모를 deadlock을 미리 생각해놓고 이렇게 구현하면 비효율적이다

## 6. Deadlock Avoidance

- 자원 요청에 대한 부가정보를 이용해서 자원 할당이 Deadlock으로부터 안전(safe)한지를 동적으로 조사해서 안전한 경우에만 할당
- 가장 단순하고 일반적인 모델은 프로세스들이 필요로 하는 각 자원별 최대 사용량을 미리 선언하도록 하는 방법임
- safe sequence
  - 프로세스의 sequence <P1, …, Pn>이 safe하려면 즉, safe sequence라면 Pi(1 ≤ i ≤ n)의 자원 요청이 `가용 자원 + 모든 Pj(j < i)의 보유자원(반납될 자원)` 에 의해 충족되어야 함
  - 조건을 만족하면 다음 방법으로 모든 프로세스의 수행 보장
    - Pi의 자원 요청이 즉시 충족될 수 없으면 모든 Pj(j < i)가 종료될 때까지 기다린다
    - Pi-1이 종료되면 Pi의 자원요청을 만족시켜 수행한다
- safe state
  - 시스템 내의 프로세스들에 대한 safe sequence가 존재하는 상태
  - 시스템이 safe state에 있으면 Deadlock이 없다
  - 시스템이 unsafe state에 있으면 Deadlock의 가능성이 있다
- Deadlock Avoidance
  - 시스템이 unsafe state에 들어가지 않는 것을 보장
  - 2가지 경우의 avoidance 알고리즘
    - Single instance per resource types(자원 당 하나의 인스턴스만 존재) → Resource Allocaion Graph Algorithm 사용
    - Multiple instances per resource types → Banker’s Algorithm 사용

## 7. Resouce Allocation Graph Algorithm

![5](https://user-images.githubusercontent.com/48282185/174245615-7d8d9935-7f7d-4b15-ae7c-60a1056c245e.png)

- Claim edge P → R
  - 프로세스 P가 자원 R을 미래에 적어도 1번 요청할 수 있음을 뜻함(점선)
  - 프로세스가 해당 자원 요청 시 request edge로 바뀜(실선)
  - R이 release되면 assignment edge는 다시 claim edge로 바뀐다
- Request edge의 assignment edge 변경 시(점선을 포함하여) cycle이 생기지 않는 경우에만 요청 자원을 할당한다
- cycle 생성 여부 조사시 프로세스의 수가 n일 때 O(n^2)이 걸린다
- 그래프 edge가 실선에서 점선으로 바뀌는 순간 cycle체크를 한다. cycle이 생겼고, 남은 실선이 그래프 상에 1개라면, 자원할당을 하지 않는다. 위의 예시에서 P2 → R2의 경우가 그렇다. P2가 R2요청 시 그래프 상에서 실선인 하나이고 그 실선을 포함하여 cycle이 생기기 때문에 P2 → R2를 하지 않으면 deadlock이 생기지 않는다. R2를 사용할 수 있음에도 불구하고 P2에게 할당해주지 않기때문에 R2가 놀고있다. utilization이 떨어진다

## 8. Banker’s Algorithm

- 가정
  - 모든 프로세스는 자원의 최대 사용량을 미리 명시
  - 프로세스가 요청 자원을 모두 할당받은 경우 유한 시간 안에 이들 자원을 다시 반납한다
- 방법
  - 기본 개념: 자원 요청 시 safe상태를 유지할 경우에만 할당
  - 총 요청 자원 수가 가용 자원 수보다 적은 프로세스를 선택(그런 프로세스가 없으면 unsafe상태)
  - 선택된 프로세스가 있으면 그 프로세스에게 자원 할당
  - 할당받은 프로세스가 종료되면 모든 자원을 반납
  - 모든 프로세스가 종료될 때까지 이러한 과정 반복

![6](https://user-images.githubusercontent.com/48282185/174245611-1ba2353a-e4be-41a7-b432-607043abc85f.png)

- 3개의 resource A, B, C는 각각 10개, 5개, 7개의 인스턴스를 갖는다
- Allocation은 현재 할당된 자원을 나타냄
- Max는 평생 사용할 자원의 최대치를 나타냄
- Available은 가용자원 즉, instance 수 - 현재 할당된 자원(Allocation)을 나타냄
  - A의 경우 10 - 2 - 3 - 2 = 3
- Need는 최대 가능 요청량
- 항상 최대 요청(Need)이 올 것을 가정
- Avaliable이 (0, 0, 0)인 상황은 deadlock이라고 볼 수 없다. 왜냐면 어떤 프로세스 건 반납할 수 있기 때문
- resource types 당 single instance인 경우도 banker’s algorithm으로 해결할 수 있다. 즉 자원할당그래프는 banker’s algorithm의 subset이다.
- 프로세스가 자원을 요청했다고 하자 그러면 바로 할당하는 것이 아니라 Need와 Available을 비교하여 Need ≤ Available이면 할당한다
  - P1이 (0, 2, 0)요청 Available (3, 3, 2) - P1의 Need (1, 2, 2) = (2, 1, 0)이므로 P1에 할당 가능
  - P0가 (1, 1, 1)요청 Available (3, 3, 2) - P0의 Need (7, 4, 3) = (-4, -1, -1)이므로 P0에 할당 불가능 → 쓸 수 있는데 못쓰니 비효율적인 방법이다

## 9. Deadlock detection and recovery

- Deadlock detection
  - resource type 당 single instance인 경우
    - 자원할당그래프에서 cycle이 곧 deadlock을 의미
  - resource type 당 multiple instance인 경우
    - Banker’s algorithm과 유사한 방법 활용
      ![7](https://user-images.githubusercontent.com/48282185/174245607-0fc4f9dc-1075-4064-9230-815ee79ed458.png)
    - 낙관적인 관점으로 접근한다
      - request가 없는 프로세스들은 무조건 자원을 반납한다고 가정
      - 가용자원이 없으니 request가 없는 프로세스들이 있는지 본다 위의 예시에서 request가 없는 프로세스들 P0, P2가 자원을 반납하면 (3, 1, 3)이 되고, 이 반납된 자원으로 들어온 request들을 처리할 수 있으면 deadlock이 발생하지 않는다고 본다.
- Wait-for graph 알고리즘
  ![8](https://user-images.githubusercontent.com/48282185/174245587-2399e672-fcc9-436f-bd52-16dfe6874ad4.png)
  - resource type 당 single instance인 경우
  - Wait-for graph
    - 자원할당 그래프의 변형(자원 노드만 없애면 됨)
    - 프로세스만으로 node 구성
    - Pi가 가지고 있는 자원을 Pk가 기다리는 경우 Pk → Pi
  - Algorithm
    - wait-for graph에 사이클이 존재하는지를 주기적으로 조사
    - O(n^2), 각 노드(n) \* 최대 edge(n-1) , bfs나 dfs로 사이클 탐색 가능
- Recovery
  - process termination
    - deadlock에 관련된 프로세스들 전부 죽인다
    - deadlock cycle이 사라질 때까지 한번에 프로세스 하나씩 죽여본다
  - resource preemption
    - 비용을 최소화할 victim 선정
    - safe state로 rollback하여 process restart
    - starvation 문제
      - 동일한 프로세스가 계속해서 victim으로 선정되는 경우
      - cost factor에 rollback 횟수도 같이 고려

## 10. Deadlock Ignorance

- Deadlock이 일어나지 않는다고 생각하고 아무런 조치도 취하지 않음
  - Deadlock은 매우 드물게 발생하므로 Deadlock에 대한 조치 자체가 더 큰 overhead일 수 있다
  - 만약 시스템에 Deadlock이 발생한 경우 사용자가 시스템이 비정상적으로 동작하는 것을 느낀 후 직접 Process를 죽이는 등의 방법으로 대처
  - UNIX, Windows등 대부분의 범용 OS가 이 방식을 채택했다

> 출처

> ABRAHAM SILBERSCHATZ ET AL., OPERATING SYSTEM CONCEPTS, NINTH EDITION, WILEY, 2013

> 반효경, 운영체제와 정보기술의 원리, 이화여자대학교 출판부, 2008
