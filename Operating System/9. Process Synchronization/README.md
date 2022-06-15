# 9. Process Synchronization

<!--ts-->

- [9. Process Synchronization](#9-process-synchronization)
  - [1. 데이터의 접근](#1-데이터의-접근)
  - [2. Race Condition(경쟁 상태)](#2-race-condition경쟁-상태)
  - [3. OS에서 race condition이 생기는 사례들](#3-os에서-race-condition이-생기는-사례들)
  - [4. Process Synchronizaion 문제](#4-process-synchronizaion-문제)
  - [5. The Critical Section(임계 구역) Problem](#5-the-critical-section임계-구역-problem)
  - [6. Initial Attempts to Solve Problem](#6-initial-attempts-to-solve-problem)
  - [7. 프로그램적 해결법의 충족 조건](#7-프로그램적-해결법의-충족-조건)
  - [8. Algorithm 1](#8-algorithm-1)
  - [9. Algorithm 2](#9-algorithm-2)
  - [10. Algorithm 3(Peterson’s Algorithm)](#10-algorithm-3petersons-algorithm)
  - [11. Synchronization Hardware](#11-synchronization-hardware)

<!-- Created by https://github.com/ekalinin/github-markdown-toc -->
<!-- Added by: sungminyou, at: 2022년 6월 15일 수요일 19시 15분 11초 KST -->

<!--te-->

## 1. 데이터의 접근

![1](https://user-images.githubusercontent.com/48282185/173804124-9207d180-de79-43d6-b36e-fc3e9332d470.png)

- 어느 데이터이던 간에 위의 과정을 거치게 됨
- 데이터를 읽기만 하면 문제가 없는데, 데이터의 수정이 이루어지기때문에 누가 먼저 읽었느냐에 따라 데이터가 달라질 수 있기때문에 Process Synchronizaion문제가 발생함

## 2. Race Condition(경쟁 상태)

![2](https://user-images.githubusercontent.com/48282185/173804122-74cb88e6-45c4-4a77-ac7f-9b25e077e802.png)

## 3. OS에서 race condition이 생기는 사례들

- kernel task 수행 중 인터럽트 발생 시
  ![3](https://user-images.githubusercontent.com/48282185/173804117-530c1c70-2de1-479c-8e0f-c79497ea68c6.png)
  - 커널 태스크 수행 중 인터럽트가 발생하여 다른 인터럽트 핸들러로 컨텍스트 스위치가 발생한 상황을 가정
    - store를 한 시점에서 context switch가 일어났다고 가정을해보자
    - 레지스터에 store한 상태로 저장되고 컨텍스트 스위치가 일어났다. 즉, 더하기 연산 이전의 상태
    - 그리고 나서 인터럽트 핸들러로 건너가서 수행을 거친 후 다시 컨텍스트 스위칭이 일어나서 더하기 연산부터 하려고 할 것이다. 그러면 인터럽트 핸들러에서 수행한 빼기 연산의 결과는 반영이 안되고, 최종적으로 값은 그대로여야하는데 1 증가된 값이 결과가 될 것이다.
    - 이를 해결하기 위해 중요한 값을 수정하는 과정이 포함된 경우 인터럽트가 들어오더라도 인터럽트 처리 루틴으로 건너가지 안게끔 인터럽트를 무시한다. 순서를 정해주면된다.
- process가 system call을 하여 kernel model로 수행 중인데 context switch가 일어나는 경우
  ![4](https://user-images.githubusercontent.com/48282185/173804112-7433fcfd-3277-4d91-a22d-ee5fbcc85694.png)
  - 읽기만 한 상태에서 timer interrupt에 의해 context switch가 발생, 레지스터에 저장된 값 PCB에 저장
  - Pb도 커널코드를 수행하고 1증가
  - Pa의 차례가 다시 돌아왔을 때 결과적으로 커널 내의 어떤 데이터는 2가 증가되어야하지만, 1이 증가된 상태가 됨
  - 일반적으로 time sharing 시스템에서는 이를 해결하기 위해 커널모드로 진입한 경우 인터럽트를 무시한다.
- multiprocessor에서 shared memory 내의 kernel data
  ![5](https://user-images.githubusercontent.com/48282185/173804105-f6932a6f-01e1-45c2-9d6b-fdbd562df567.png)
  - 방법 1은 커널 전체를 Lock을 거는 방법으로 비효율적이다. 그래서 방법 2를 쓰는 편이 낫다.

## 4. Process Synchronizaion 문제

- 공유 데이터(shared data)의 동시 접근(concurrent access)은 데이터의 불일치(inconsistency) 문제를 발생시킬 수 있다.
- 일관성(consistency)유지를 위해서는 협력 프로세스(cooperating process)간의 실행 순서(orderly execution)를 정해주는 메커니즘 필요
- race condition
  - 여러 프로세스들이 동시에 공유 데이터를 접근하는 상황
  - 데이터의 최종 연산 결과는 마지막에 그 데이터를 다룬 프로세스에 따라 달라짐
  - race condition을 막기 위해서는 concurrent process는 동기화(synchronize)되어야 한다.

## 5. The Critical Section(임계 구역) Problem

![6](https://user-images.githubusercontent.com/48282185/173804095-888e567d-0257-4240-aaf0-44b143350d21.png)

- n개의 프로세스가 공유 데이터를 동시에 사용하기를 원하는 경우
- 각 프로세스의 code segment에는 공유 데이터를 접근하는 코드인 critical section이 존재, 즉 공유 데이터를 접근하는 코드가 문제가 되는 것임
- 문제
  - 하나의 프로세스가 critical section에 있을 때 다른 모든 프로세스는 critical section에 들어갈 수 없어야한다.

## 6. Initial Attempts to Solve Problem

![7](https://user-images.githubusercontent.com/48282185/173804089-742ecf13-b6e5-4fe6-a198-6146f508ae3e.png)

- 두 개의 프로세스가 있다고 가정 P0, P1
- 프로세스들의 일반적인 구조
- 프로세스들은 수행의 동기화(synchronize)를 위해 몇몇 변수를 공유할 수 있다 → synchronization variable

## 7. 프로그램적 해결법의 충족 조건

- Mutual Exclusion(상호 배제)
  - 프로세스 Pi가 critical section 부분을 수행 중이면 다른 모든 프로세스들은 그들의 critical section에 들어가면 안 된다.
- Progress
  - 아무도 critical section에 있지 않은 상태에서 critical section에 들어가고자 하는 프로세스가 있으면 critical section에 들어가게 해줘야함
- Bounded Waiting(유한 대기)
  - 프로세스가 critical section에 들어가려고 요청한 후부터 그 요청이 허용될 때까지 다른 프로세스들이 critical section에 들어가는 횟수에 한계가 있어야 한다.
  - starvation이 생기면 안된다.
- 가정
  - 모든 프로세스의 수행 속도는 0보다 크다
  - 프로세스들 간의 상대적인 수행 속도는 가정하지 않는다.

## 8. Algorithm 1

- synchronization variable
  - int turn
  - 초기값 0
- Pi는 turn이 i일 때 critical section에 진입가능하다.
- Process P0(Process 1을 위한 코드는 while(turn != 1)로 변경)
  ```c
  do{
  	while(turn != 0); // P0이외에는 진입 불가
  	critical section
  	turn = 1 - turn; // 상대방 진입 가능하게
  	remainder section
  }while(1);
  ```
- mutual exclusion 만족
- progress 불만족
  - P0 5개, P1 1개, P0 5개 이렇게 스케줄 된다했을 때, P0가 나올 때 순서를 P1로 바꾸기 때문에 다음차례가 P0임에도 불구하고 못들어간다. 즉 서로 교대하지 않으면 진행 불가능, 즉 특정 프로세스가 더 빈번히 critical section에 들어가야하는 경우 progress 불만족
- bounded waiting 불만족
  - 상대방이 들어가지 않으면 더이상 못들어가고 무한 대기

## 9. Algorithm 2

- synchronization variable
  - boolean flag[2];
  - flag[all] = { false, }
- Pi는 flag[i]가 true일 때 critical section으로 진입 가능
- Process Pi(Process j를 위한 코드는 while(flag[i]);로 변경)
  ```c
  do{
  	flag[i] = true;
  	while(flag[j]); // 상대방 Pj가 critical section 내에 있는지 체크
  	critical section
  	flag[i] = false;
  	remainder section
  }while(true);
  ```
- mutual exclusion 만족
- progress 불만족
  - 프로세스 i가 flag[i] = true까지 수행되고, context switch가 일어나 j에게 CPU수행이 넘어감 j도 flag[j] = true까지 하고 다시context switch가 일어나 i에서 CPU수행이 넘어감 이 상황에서 i의 입장에서는 flag[j]가 true이기 때문에 진입이 막히고 j의 입장에서는 flag[i]가 true이기 때문에 진입이 막혀 더 이상 진행이 불가능하다.
- bounded waiting 불만족

## 10. Algorithm 3(Peterson’s Algorithm)

- algorithm1과 2가 혼용된 방식이다.
- Process Pi(Process j를 위한 코드는 while(flag[i] && turn == i)로 변경)
  ```c
  do{
  	flag[i] = true;
  	turn = j; // 상대방 turn으로 바꿔줌
  	while(flag[j] && turn == j);
  	// turn == j는 Pi가 자신의 차례임을 나타냄, 그 때 flag[j]가 true이면 진입 불가
  	critical section
  	flag[i] = false;
  	remainder section
  }while(1);
  ```
- mutual exclusion 만족
- progress 만족
- bounded waiting 만족
- busy waiting(Spin lock)
  - 계속 CPU와 memory를 쓰면서 wait
  - 비효율적인 방법

## 11. Synchronization Hardware

- 하드웨어적으로 test & modify를 atomic하게 수행할 수 있도록 지원하는 경우 앞의 문제는 간단히 해결
  - 데이터를 읽는 것과 쓰는것이 하나의 instruction으로 처리 가능하다면 소프트웨어적으로 복잡해지지 않는다.
- Mutual Exclusion with Test & Set

  ```c
  bool lock = false;

  do{
  	while(test_and_set(lock));
  	ciritcal section
  	lock = false;
  	remainder section
  }while(1);
  ```

  - test_and_set(lock)은 lock이 true인지 검사하고 값을 true로 바꾸는 것이 atomic하게 실행됨
