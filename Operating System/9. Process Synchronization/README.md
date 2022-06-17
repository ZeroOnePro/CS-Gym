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
  - [12. Semaphores](#12-semaphores)
  - [13. Critical Section of n Processes](#13-critical-section-of-n-processes)
  - [14. Block / Wakeup 구현](#14-block--wakeup-구현)
  - [15. Busy wait vs Block / wakeup](#15-busy-wait-vs-block--wakeup)
  - [16. Semaphore의 두 가지 유형](#16-semaphore의-두-가지-유형)
  - [17. Deadlock and Starvation](#17-deadlock-and-starvation)
  - [18. Bounded Buffer Problem(Producer - Consumer Problem)](#18-bounded-buffer-problemproducer---consumer-problem)
  - [19. Reader - Writers Problem](#19-reader---writers-problem)
  - [20. Dining - Philosophers Problem](#20-dining---philosophers-problem)
  - [21. Monitor](#21-monitor)

<!-- Created by https://github.com/ekalinin/github-markdown-toc -->
<!-- Added by: sungminyou, at: 2022년 6월 17일 금요일 10시 14분 26초 KST -->

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

## 12. Semaphores

- 앞의 방식들을 추상화(논리적 object + operation, 구현 측면 x, 논리 측면)시킴
- semaphore S

  - 정수값
  - 사용 가능 자원의 개수(S=5, 자원 5개)
  - 아래의 두 가지 atomic 연산에 의해서만 접근 가능

  ```c
  P(S) // 자원을 가져가는 연산, lock

  while(S<=0) do no-op; // 작업없이 대기
  S--; // 자원의 수 감소
  ```

  ```c
  V(S) // 자원을 반납하는 연산, unlock

  S++; // 자원의 수 증가
  ```

  - S가 양수라면, 감소시키고 critical section으로 진입 가능
  - Ohterwise, 양수가 되기까지 대기(busy wait)

## 13. Critical Section of n Processes

```c
semaphore mutex; // 초기값을 1로 설정, 1개가 CS내로 진입가능

do{
	P(mutex);
	critical section
	V(mutext);
	remainder section
}while(1);
```

- busy wait(spin lock)는 효율적이지 못함
- block & wakeup(sleep lock) 방식의 구현
- 추상자료형이니 구현은 하기 나름

## 14. Block / Wakeup 구현

- semaphore를 아래와 같이 정의

```c
typedef struct {
	int value; // 세마포어 변수 S
	struct process *L; // block된 프로세스들을 연결할 자료구조
}semaphore;

void P(semaphore& S){
	S.value--;
	if(S.value < 0){
		add this process to S.L;
		block();
	}
}

void V(semaphore& S){
	S.value++;
	if(S.value <= 0){
	// S의 value가 증가되었음에도 음수인 이유는 이 자원을 기다리는 다른 프로세스들이
	// value를 감소시키고 block되었다는 의미
	// 즉, block되있는 프로세스가 1개 이상이라는 이야기
	// busy wait에서의 value와는 다른 의미
		remove a process P from S.L;
		wakeup(P);
	}
}
```

- block과 wakeup을 다음과 같이 가정
  - block
    - 커널은 block을 호출한 프로세스를 suspend시킴
    - 이 프로세스의 PCB를 semaphore의 wait queue에 삽입
  - wakeup(P)
    - block된 프로세스 P를 wakeup시킴
    - 이 프로세스의 PCB를 ready queue로 옮김

## 15. Busy wait vs Block / wakeup

- block/wakeup overhead vs critical section의 길이
  - CS의 길이가 긴 경우 Block/Wakeup이 적당
  - CS의 길이가 매우 짧은 경우 Block/Wakeup 오버헤드가 busy wait의 오버헤드보다 더 커질 수 있음
  - 일반적으로는 Block/Wakeup방식이 더 좋음

## 16. Semaphore의 두 가지 유형

- Counting semaphore
  - 도메인이 0이상인 임의의 정수값
  - 주로 Resource counting에 사용, 남은 자원의 개수를 세는 용도
- Binary semaphore
  - 0또는 1값만 가질 수 있는 semaphore
  - 자원의 개수가 1인 counting semaphore
  - 주로 mutual exclusion(lock / unlock)에 사용, 자원이 1개인 특별한 경우

## 17. Deadlock and Starvation

- Deadlock
  - 둘 이상의 프로세스가 서로 상대방에 의해 충족될 수 있는 event를 무한히 기다리는 현상
- S와 Q가 1로 초기화된 semaphore라 하자

  ![1](https://user-images.githubusercontent.com/48282185/173813703-041419c4-f2b0-4655-9d73-699b823bcd38.png)

  - 하드디스크 A에서 B로 카피를 하는 작업을 한다하자. S와 Q는 각 하드디스크에 걸려있는 세마포어이다. 즉 S와 Q를 둘 다 얻어야 작업이 진행될 수 있다.
  - 자원 획득 순서를 정해주면 해결 가능
    - 위의 예시에서 P1 P(S) ↔ P(Q)순서 변경

- Starvation
  - indefinite blocking. 프로세스가 suspend된 이유에 해당하는 세마포어 큐에서 빠져나갈 수 없는 현상
  - deadlock도 일종의 starvation으로 볼 수 있겠다.

## 18. Bounded Buffer Problem(Producer - Consumer Problem)

![1](https://user-images.githubusercontent.com/48282185/174202784-3194dab7-0435-4fb4-b395-3a1b61e8fd2e.png)

- 두 가지 타입의 프로세스가 각각 여러개 있는 상황을 가정, Producer는 데이터를 생산하여 버퍼에 넣어주고, Consumer는 데이터를 꺼내가는 역할
- 두 개의 Producer가 동시에 도착하여 버퍼의 같은 위치에 동시에 데이터를 쓰는 상황이 문제가 된다.
- 두 개의 Consumer가 동시에 같은 위치의 데이터를 꺼내가려할 때 문제가 된다.
- 여러개의 Producer가 버퍼에 데이터를 모두 채우고 난 후 Consumer가 데이터를 꺼내가지까지 기다려야하는 문제
- 여러개의 Consumer가 빈 버퍼 상태에서 Producer가 데이터를 만들 때까지 기다려야하는 문제
- 여기서 producer의 resource는 empty buffer이고, consumer의 resource는 full buffer이다.
- Shared Data
  - buffer 자체
  - buffer의 포인터 변수들(empty, full buffer의 시작 위치 등)
- Synchronization variables
  - mutual exclusion
    - binary semaphore가 필요
    - shared data의 mutual exclusion을 위해
  - resource count
    - integer semaphore가 필요
    - 남은 full / empty buffer의 수 표시

![2](https://user-images.githubusercontent.com/48282185/174202783-2481db10-36f5-4f2d-b140-9a7de199c39a.png)

- 생산자 소비자 문제를 세마포어를 통해 해결할 수 있다.

## 19. Reader - Writers Problem

- 한 프로세스가 DB(공유 자원)에 write중일 때 다른 프로세스가 접근하면 안됨
- read는 동시에 여러 프로세스가 해도 상관없음
- solution
  - writer가 DB에 접근 허가를 아직 못한 상태에서는 모든 대기 중인 reader들을 다 DB에 접근하게 해준다
  - writer는 대기 중인 reader가 하나도 없을 때 DB접근이 허용된다
  - 일단 writer가 DB에 접근 중이면 reader들의 접근이 금지된다
  - writer가 DB에 빠져나가야만 reader의 접근이 허용된다
- shared data
  - DB 자체
  - read count(DB에 접근 중인 reader의 수)
- synchronization variables
  - mutex
    - 공유 변수 readcount를 접근하는 코드(critical section)의 mutual excultion을 보장하기 위해 사용
    - binary semaphore
  - DB
    - reader와 writer가 공유 DB자체를 올바르게 접근하게 하는 역할
    - binary semaphore

![3](https://user-images.githubusercontent.com/48282185/174202782-068748a4-ed2e-47cc-84f2-0d14926a294e.png)

- Reader가 지속적으로 도착하는 경우 writer는 계속 대기해야하는 상황에 빠질 수 있음
- writer가 오래 기다리는 경우 우선순위를 높여(aging) starvation을 방지하는 방법도 생각해 볼 수 있겠다.
- starvation을 해결하기 위해 reader의 읽기 회수에 제한을 두는 방법도 생각해 볼 수 있겠다.

## 20. Dining - Philosophers Problem

![4](https://user-images.githubusercontent.com/48282185/174202780-ada7637a-28e1-4708-8427-7749024254f6.png)

- 밥을 먹기위해 자신의 왼쪽과 오른쪽 젓가락을 필요로하고, 5명의 철학자가 젓가락을 공유한다.
- semaphore를 활용한 soultion의 문제점
  - 모든 철학자가 동시에 배가 고파져 왼쪽 젓가락을 집어버린 경우 deadlock의 가능성이 있다.
  - 아무도 오른쪽 젓가락을 잡을 수 없다
- deadlock을 막기 위한 해결방안
  - 4명의 철학자만이 테이블에 동시에 앉을 수 있게 제약을 만든다
  - 젓가락을 두 개 모두 집을 수 있을때에만 집을 수 있게한다
  - 비대칭
    - 짝수(홀수)철학자는 왼쪽(오른쪽) 젓가락부터 잡도록 한다

## 21. Monitor

![5](https://user-images.githubusercontent.com/48282185/174202779-d931db10-a935-44cc-a813-c16c51fdd8ee.png)

- semaphore의 문제
  - 코딩하기 힘들다
  - 정확성의 입증이 어렵다(디버깅이 어렵다)
  - 자발적 협력이 필요하다
  - 한번의 실수가 모든 시스템에 치명적 영향을 미친다
- 언어 차원에서 제공되는 synchronization문제 해결방안
  ![6](https://user-images.githubusercontent.com/48282185/174202778-fec6da3e-5c60-4cbb-bff0-5e98ced1b7be.png)
  ![7](https://user-images.githubusercontent.com/48282185/174202775-e0b13c41-558e-4274-a28d-c009c37845b6.png)

  - 동시 수행중인 프로세스 사이에서 abstract data type의 안전한 공유를 보장하기 위한 high-level synchronization construct
  - 모니터 내부에 공유 데이터와 procedure를 정의하고, procedure를 통해서만 공유데이터에 접근할 수 있도록 한다
  - 기본적으로 모니터에 대한 접근을 허락하지 않아 프로그래머 입장에서는 lock을 걸 필요가 없어서 편하다.

    - 모니터 내에서는 한번에 하나의 프로세스만이 활동 가능
    - 프로세스가 모니터 안에서 기다릴 수 있도록 하기 위해 condition variable을 사용
    - condition variable은 wait와 signal연산에 의해서만 접근 가능
    - wait()를 invoke한 프로세스는 다른 프로세스가 signal()을 Invoke하기 전까지 suspend된다.
    - signal()은 정확하게 하나의 suspend된 프로세스를 resume한다. suspend된 프로세스가 없다면, 아무일도 일어나지 않는다.

    ```c
    condition x; // x자원에 대한 condition variable

    x.wait(); // x를 사용할 수 없는 경우 대기줄에 서게하는 역할

    x.signal(); // x를 사용할 수 있게 된 경우 대기줄에서 프로세스 하나를 꺼내오는 역할
    ```

  - Bounded - Buffer Problem을 monitor를 통해 해결
    ![8](https://user-images.githubusercontent.com/48282185/174202761-0ed4697f-9e60-486c-9eae-8c7c11df6b5e.png)
    - 생산자와 소비자의 개수를 신경쓸 필요가 없어짐
