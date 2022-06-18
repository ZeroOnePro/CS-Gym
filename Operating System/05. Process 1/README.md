# 5. Process 1

<!--ts-->

- [5. Process 1](#5-process-1)
  - [1. 프로세스의 개념](#1-프로세스의-개념)
  - [2. 프로세스의 상태(Process State)](#2-프로세스의-상태process-state)
  - [3. Process Control Block(PCB)](#3-process-control-blockpcb)
  - [4. Context Switch](#4-context-switch)
  - [5. 프로세스를 스케줄링하기 위한 큐](#5-프로세스를-스케줄링하기-위한-큐)
  - [6. 스케줄러](#6-스케줄러)
  - [7. 프로세스 상태도](#7-프로세스-상태도)

<!-- Created by https://github.com/ekalinin/github-markdown-toc -->
<!-- Added by: sungminyou, at: 2022년 6월 11일 토요일 15시 35분 51초 KST -->

<!--te-->

## 1. 프로세스의 개념

![1](https://user-images.githubusercontent.com/48282185/172328052-0fe2d521-46fb-4631-a0af-252169f64a0e.png)

- process is a program in execution
  - 실행중인 프로그램
- 프로세스의 context

  - 특정 시점에서 프로세스가 어디까지 수행을 했고, 어떤 상태에 있는지 나타내는 모든 요소
  - CPU의 수행 상태를 나타내는 하드웨어 context
    - Program Counter
      - 코드(or 커널 코드)의 어느 부분을 가리키고 있는가?
    - 각종 레지스터 내에 들어있는 값
  - 프로세스의 주소 공간 메모리의 상태
    - code
      - 현재 어느 instruction이 실행되고 있는가?
    - data
      - 현재 변수의 값은 무엇인가?
    - stack
      - 무슨 내용을 어디까지 쌓아놓고 있는가?
  - 프로세스 관련 커널 자료 구조
    - PCB(Process Control Block)
    - kernel stack
      - system call을 사용
      - 프로세스들 간에 공유 자원으로써 각 프로세스마다 별도로 관리되어야 충돌문제를 막을 수 있다.
  - 프로세스 여러개가 같이 실행되는 multi tasking환경에서 context를 모른다면, 다음 실행차례가 왔을 때 어디를 실행하고 있던건지 알 수 없기 때문에, 이어서 작업을 진행하려면 반드시 필요한 정보이다.

## 2. 프로세스의 상태(Process State)

![2](https://user-images.githubusercontent.com/48282185/172328048-f9109d21-96e1-454b-ab37-094d48d3ead7.png)

- 프로세는 상태(state)가 변경되며 수행된다.

  - Running
    - CPU를 잡고 instruction을 수행 중인 상태
  - Ready
    - CPU를 기다리는 상태
    - 메모리에 다음에 수행할 instruction을 올려놓아야함 CPU만 얻으면 바로 수행할 수 있는 상태, 즉 수행중이지 않더라도 최소한의 메모리 공간은 있음
  - Blocked(wait, sleep)
    - CPU를 주어도 당장 instruction을 수행할 수 없는 상태
    - 프로세스 자신이 요청한 event(IO 등)가 즉시 만족되지 않아 이를 기다리는 상태
  - Suspended(stopped)
    - 외부적인 이유로 프로세스의 수행이 정지된 상태
    - 프로세스는 통째로 디스크에 swap out된다.
    - 예시로 사용자가 프로그램을 일시 정지시킨 경우(break key), 시스템이 여러 이유로 프로세스를 잠시 중단시킴(메모리에 너무 많은 프로세스가 올라와 있을 때)
  - New
    - 프로세스가 생성중인 상태
  - Terminated

    - execution이 끝나고 종료 중인 상태
    - 수행이 끝나고 프로세스를 끝내기 위한 정리작업이 있다.
    - 프로세스가 끝났다면 프로세스가 아니다.

  - Blocked vs Suspended - blocked는 자신이 요청한 event가 만족되면 다시 ready상태로 돌아감 - suspended는 외부에서 재개시켜줘야 다시 active한 상태로 돌아감
    ![3](https://user-images.githubusercontent.com/48282185/172328041-931bdd51-bac2-4dc3-b23a-0a3760fb34f4.png)
    ![4](https://user-images.githubusercontent.com/48282185/172328039-0f346b27-72f2-4ee8-804e-ca11ac23a356.png)

- 기본적으로 프로세스들은 수행 차례를 위해 Ready Queue에 들어가서 차례를 기다린다.(FIFO는 아님, 우선순위에 따라 스케쥴됨)
- IO작업이 필요한 프로세스의 경우 해당 장치에 맞는 IO대기 큐로 들어가서 자신이 요청했던 IO를 기다린다.(waiting 상태), IO가 끝나면 IO의 컨트롤러들은 CPU에 인터럽트를 보내고, 해당 인터럽트를 받은 CPU는 커널 핸들러를 수행한다음 다음 작업을 이행한다. IO 큐에 있던 프로세스는 다시 ready queue로 보내진다. 프로세스의 상태 변경 코드도 커널 핸들러에 포함되어 있지 않을까?
- 공유데이터는 프로세스간에 공유되는 데이터인데, 다수개의 프로세스가 이 공유데이터에 동시에 접근하면, 무결성이 깨지기 때문에 한 순간에 한 개의 프로세스만 접근할 수 있도록 구현해야한다. 이 때 한 프로세스가 공유데이터를 읽기 또는 쓰기하고 있다면, 다른 프로세스는 그 작업을 기다려야하기때문에 오래 걸린다. 따라서 리소스 큐에 들어가서 해당 작업이 끝나기를 기다린다.(waiting 상태)
- 각종 queue들은 커널의 data 영역에 위치하고 있다.

## 3. Process Control Block(PCB)

![5](https://user-images.githubusercontent.com/48282185/172328036-4cf85477-4ecb-42d4-b15f-20413c7c4867.png)

- 운영체제가 각 프로세스를 관리하기 위해 각 프로세스마다 유지하는 정보
- 구조체 형태로 다음 구성 요소가 존재한다.
  - OS가 관리에 사용하는 정보
    - Process State, Process ID
    - scheduling information, priority
  - CPU 수행 관련 하드웨어 값
    - Program Counter
    - registers
  - 메모리 관련
    - Code, data, stack의 메모리 상위치 정보
  - 파일 관련
    - Open file descriptors, 열고있는 파일 정보
    - etc…

## 4. Context Switch

- CPU를 한 프로세스에서 다른 프로세스로 넘겨주는 과정
- CPU가 다른 프로세스에게 넘어갈 때 OS는 다음을 수행한다.
  - CPU를 내어주는 프로세스의 상태를 그 프로세스의 PCB에 저장
  - CPU를 새롭게 얻는 프로세스의 상태를 PCB에서 읽어옴

![6](https://user-images.githubusercontent.com/48282185/172328028-93c68e96-f777-4ded-a665-e8f78ecf519e.png)

- System call이나 interrupt 발생 시 반드시 context switch가 일어나는 것은 아님
  - ISR = interrupt service routine
  - 사용자 프로세스 to 사용자 프로세스로 넘어가는 과정을 context switch라고 부르는 것이지 사용자 프로세스에서 운영체제로 CPU 제어권이 넘어가는 것을 context switch라고 하지는 않는다.

## 5. 프로세스를 스케줄링하기 위한 큐

![7](https://user-images.githubusercontent.com/48282185/172328026-c067775a-295e-40f5-abbe-72148c39b647.png)

- Job Queue
  - 현재 시스템 내에 있는 모든 프로세스의 집합
- Ready Queue
  - 현재 메모리 내에 있으면서 CPU를 잡아서 실행되기를 기다리는 프로세스의 집합
- Device Queue
  - IO device의 처리를 기다리는 프로세스의 집합
- 프로세스들은 각 큐들을 오가며 수행된다.

## 6. 스케줄러

- Short-term scheduler(a.k.a. 단기 스케줄러, CPU scheduler)

  - 어떤 프로세스를 다음번에 running시킬지 결정
  - 프로세스에 CPU을 주는 문제
  - 충분히 빨라야함(millisecond 단위)

- Medium-term scheduler(a.k.a. 중기 스케줄러, Swapper)

  - 여유 공간 마련을 위해 프로세스를 통째로 메모리에서 디스크로 쫓아냄
  - 프로세스에게서 메모리를 빼앗는 문제
  - degree of Multiprogramming을 제어(성능향상을 위해 메모리 상에 있는 프로세스의 수를 조절)
  - time sharing system에서 사용됨

- Long-term scheduler(a.k.a. 장기 스케줄러, job scheduler)
  - 시작 프로세스 중 어떤 것들을 ready queue로 보낼지 결정
  - 프로세스에 memory 및 각종 자원을 주는 문제(실제로는 메모리에 올라감 그래야 뭔가 작업을 할 수 있으니)
  - degree of Multiprogramming(메모리에 올라가있는 프로세스의 수)을 제어
    - 메모리에 프로세스가 너무 많아도 성능이 안좋고(메모리 공간 부족 당장 필요한 부분이 메모리에 안올라와 있을 수 있다), 너무 적어도 안좋다.(CPU의 idle time이 길어짐)
  - time sharing system에는 보통 장기 스케줄러가 없음(무조건 ready)
    - 현재 시스템에서는 프로그램 실행시키면 결정 없이 ready 상태로 진입(메모리에 올라감)

## 7. 프로세스 상태도

![8](https://user-images.githubusercontent.com/48282185/172328011-5240b97f-2ab3-4520-a5f5-565ca1b25425.png)

- 주의할 점이 어떤 이유로 인해 유저 모드에서 커널 모드(모니터 모드)로 넘어갔을 때 운영체제가 running 상태가 되는 것이 아닌 프로세스가 커널모드에서 running상태이다라고 말하는 것이 맞다.
- blocked에서 suspended가 되었나, ready에서 suspended가 되었나로 suspended도 두 가지 상태로 나눠놓을 수 있다.
  - suspended상태에서 어떤 이벤트가 일어나거나(예로 정지상태 해제 등) suspended가 되기 전 시켰던 IO작업이 완료된 경우 blocked에서 ready로 상태가 변경될 수 있다.
  - 메모리에 올라와있지 않은 상태로 CPU를 사용하여 무엇인가 할 수 없다. 프로세스 스스로 어떤 것도 할 수 없고, 외부에서 재개해줘야함

> 출처
> ABRAHAM SILBERSCHATZ ET AL., OPERATING SYSTEM CONCEPTS, NINTH EDITION, WILEY, 2013
> 반효경, 운영체제와 정보기술의 원리, 이화여자대학교 출판부, 2008
