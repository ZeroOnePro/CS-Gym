# 8. CPU Scheduling

<!--ts-->

- [8. CPU Scheduling](#8-cpu-scheduling)
  - [1. CPU and IO Bursts in Program Execution](#1-cpu-and-io-bursts-in-program-execution)
  - [2. 프로세스의 특성 분류](#2-프로세스의-특성-분류)
  - [3. CPU burst Time의 분포](#3-cpu-burst-time의-분포)
  - [4. CPU Sheduler &amp; Dispatcher](#4-cpu-sheduler--dispatcher)
  - [5. Scheduling Algorithms](#5-scheduling-algorithms)
  - [6. Scheduling Criteria](#6-scheduling-criteria)

<!-- Created by https://github.com/ekalinin/github-markdown-toc -->
<!-- Added by: sungminyou, at: 2022년 6월 11일 토요일 15시 34분 13초 KST -->

<!--te-->

## 1. CPU and IO Bursts in Program Execution

![1](https://user-images.githubusercontent.com/48282185/173064960-c86adcba-638e-4b36-b2be-2a74ff8212bc.png)

- CPU burst
  - CPU만 사용하는 단계
- IO burst
  - IO만 사용하는 단계
- 프로그램의 실행은 일반적으로 위 두 단계를 번갈아가면서 이뤄진다. user interactive한 프로그램이 대표적 예시다.

## 2. 프로세스의 특성 분류

- 프로세스는 그 특성에 따라 다음 두가지로 나뉜다
  - IO bound job
    - CPU를 잡고 계산하는 시간보다 IO에 많은 시간이 필요한 job
    - many short CPU bursts
  - CPU bound job
    - 계산 위주의 job
    - few very long CPU bursts

## 3. CPU burst Time의 분포

![2](https://user-images.githubusercontent.com/48282185/173064944-b4a94b6f-6fba-4590-868b-bc19cafa7d96.png)

- IO작업이 일어날 때 CPU burst의 frequency가 높은 이유는 자주 인터럽트를 당해 짧은 시간동안의 job이 빈번하게 발생하기 때문이다. 실제로 CPU를 많이 쓰는 작업은 CPU bound job이다.
- 여러 종류의 job(= process)가 섞여 있기 때문에 CPU스케줄링이 필요하다
- interactive job에게 적절한 response 제공 요망
  - 공평함 보다도 사람에게 답답함을 주지 않는 효율성이 중요
- CPU와 IO 장치 등 시스템 자원을 골고루 효율적으로 활용

## 4. CPU Sheduler & Dispatcher

- CPU Scheduler
  - ready 상태의 프로세스 중에서 이번에 CPU를 줄 프로세스를 고른다.
- Dispatcher
  - CPU의 제어권을 CPU Sheduler에 의해 선택된 프로세스에 넘긴다.(Context Switch)
- CPU 스케줄링이 필요한 경우는 프로세스에게 다음과 같은 상태 변화가 있는 경우이다.
  1. Running ⇒ Waiting(예시로 IO요청 시스템 콜)
  2. Running ⇒ Ready(예시로 타이머 인터럽트)
  3. Waiting ⇒ Ready(예시로 IO완료 인터럽트)
     1. 일반적으로는 인터럽트 당하는 시점에 실행되고 있던 프로세스가 계속 진행되는게 맞지만 IO완료된 프로세스가 우선순위가 높은 것이라면 이 프로세스에게 빼앗길 수도 있다.
  4. Terminated
  - 1, 4번 스케줄링은 nonpreemptive(강제로 빼앗지 않고 자진 반납 → 비선점형)
  - 2, 3번 스케줄링은 preemptive(강제로 빼앗음 → 선점형)

## 5. Scheduling Algorithms

- FCFS(First Come First Served)
- SJF(Shortest Job First)
- SRTF(Shortest Remaining Time First)
- Priority Scheduling
- Round Robin
- Mutilevel Queue
- Multilevel Feedback Queue

## 6. Scheduling Criteria

- performance index(= performance measure)이라고도 함, 번역하면 성능 척도
- CPU utilization(이용률)
  - CPU 이용률, 가능한한 CPU를 제일 바쁜상태로 둔다
  - 컴퓨터 사용시간 대비 CPU사용시간을 척도로 나타냄
  - 시스템 관점에서 프로세스들을 최대한 놀지 않고 처리
- Throughput(처리율)
  - 주어진 시간 동안에 몇 개의 작업을 처리했는가?
  - 시스템 관점에서 최대한 많은 프로세스들을 처리
- Turnaround time(소요시간, 반환시간)
  - 프로세스가 레디큐에 들어와서 CPU 작업을 마치기까지 걸린 시간
  - 프로세스 관점에서 CPU사용하러 레디큐로 들어와서 CPU쓰고 나가기까지의 시간
- Waiting time(대기 시간)
  - CPU를 사용하기 전 대기하는 시간
  - 선점형 스케줄링에서 한 번의 CPU turn에 끝나지 않는다. 그러면 여러번 레디큐에 들어오게 될텐데 이 때 모든 레디큐에서의 시간을 합친 것을 waiting time이라고 함
- Response time(응답 시간)
  - 레디큐에 들어와서 처음으로 CPU를 쓰기까지 걸린 시간
  - 선점형 스케줄링에서 여러 가지 이유로 밀려 작업이 안되는 프로세스들이 있으므로 입장에서 처음 실행되기 까지의 시간이 중요하게 작용한다.
  - waiting time과의 차이는 waiting time은 프로세스가 종료될 때까지 레디큐에서 대기한 시간을 모두 합친 것이라면 response time은 프로세스가 시작되고 처음 CPU를 얻기까지 레디큐에서 대기한 시간이다.
