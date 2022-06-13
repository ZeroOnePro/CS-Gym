# 8. CPU Scheduling

<!--ts-->

- [8. CPU Scheduling](#8-cpu-scheduling)
  - [1. CPU and IO Bursts in Program Execution](#1-cpu-and-io-bursts-in-program-execution)
  - [2. 프로세스의 특성 분류](#2-프로세스의-특성-분류)
  - [3. CPU burst Time의 분포](#3-cpu-burst-time의-분포)
  - [4. CPU Sheduler &amp; Dispatcher](#4-cpu-sheduler--dispatcher)
  - [5. Scheduling Algorithms](#5-scheduling-algorithms)
  - [6. Scheduling Criteria](#6-scheduling-criteria)
  - [7. FCFS(First Come First Served)](#7-fcfsfirst-come-first-served)
  - [8. SJF(Shortest Job First)](#8-sjfshortest-job-first)
  - [9. Priority Scheduling](#9-priority-scheduling)
  - [10. Round Robin(RR)](#10-round-robinrr)
  - [11. Multilevel Queue](#11-multilevel-queue)
  - [12. Multilevel Feedback Queue](#12-multilevel-feedback-queue)
  - [13. Multiple Processor Scheduling](#13-multiple-processor-scheduling)
  - [14. Real Time Scheduling](#14-real-time-scheduling)
  - [15. Thread Scheduling](#15-thread-scheduling)
  - [16. Algorithm Evaluation](#16-algorithm-evaluation)

<!-- Created by https://github.com/ekalinin/github-markdown-toc -->
<!-- Added by: sungminyou, at: 2022년 6월 13일 월요일 13시 40분 24초 KST -->

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

## 7. FCFS(First Come First Served)

![1](https://user-images.githubusercontent.com/48282185/173281012-b83f91b8-480a-4d2e-9526-c8eff7e359c4.png)
![2](https://user-images.githubusercontent.com/48282185/173281011-ebc90686-3a72-4093-a5e0-5541fd6d9bf1.png)

- 먼저 온 프로세스 먼저 처리
- 비선점형 스케줄 방식
- 수행 시간이 긴 프로세스가 뒤의 다른 수행 시간이 짧은 프로세스들을 기다리게한다는 점(Convoy Effect)에서 비효율적인 스케줄 알고리즘이다.

## 8. SJF(Shortest Job First)

![3](https://user-images.githubusercontent.com/48282185/173281009-e461d306-32ee-49ad-8ec0-641263625c81.png)
![4](https://user-images.githubusercontent.com/48282185/173281008-983fbb38-773f-4bfa-b540-629b56e3db6a.png)

- 각 프로세스의 다음 번 CPU burst time을 가지고 스케줄링에 활용
- CPU burst time이 가장 짧은 프로세스를 제일 먼저 스케줄하는 방식
- Two schemes
  - Nonpreemptive
    - 일단 CPU를 잡으면 이번 CPU burst가 완료될 때까지 CPU를 선점당하지 않음
    - 위의 예시에서 평균 대기 시간 4
      - P2, P3, P4도착 당시 P1이 스케줄 되고 있으므로 모두 대기
      - P1이 끝난 후 burst time이 가장 짧은 P3가 선택됨, P3의 waiting time은 도착 당시 P1의 남은 시간 = 7 - 4 = 3
      - P3가 끝난 후 burst time이 남은게 둘이 같기 때문에 먼저 도착한 P2가 선택됨, P2의 waiting time은 도착 당시 P1의 남은 시간 7 -2 = 5 + P3의 burst time = 1 = 6이 된다.
      - P2가 끝난 후 남은게 P4 하나, P4의 waiting time은 도착 당시 P1의 남은 시간 7 - 5 = 2 + P3의 burst time(1) + P2의 burst time(4) = 7이 된다.
  - Preemptive
    - 현재 수행 중인 프로세스의 남은 burst time보다 더 짧은 CPU burst time을 가지는 새로운 프로세스가 도착하면 CPU를 빼앗김(Shortest Remaining Time First, SRTF라고 부름)
    - 위의 예시에서 평균 대기 시간 3(도착시간부터 남은게 있나 보기)
      - P1의 waiting time (0+2+1+2+4 = 9)
      - P2의 waiting time (0 + 1 = 1)
      - P3의 waiting time (0)
      - P4의 waiting time (2)
- SJF is optimal
  - preemptive 방식에서 주어진 프로세스들에 대해 minimum average waiting time을 보장(대기 줄이 짧아짐, 증명된 방식)
  - 현재 스케줄되고 있는 프로세스보다 더 짧은 burst time을 가지는 프로세스가 도착했을 경우 선점되지 않는 방식이라면(nonpreemptive) 큐가 길어짐 ⇒ 최적화가 아니다.
  - 어떤 스케줄링 알고리즘을 쓰더라도 SJF보다 average waiting time이 짧은 것 없음
- SJF의 문제
  - 수행 시간이 긴 프로세스는 영원히 스케줄 안 될 수있다.(Starvation)
  - CPU 사용 시간을 미리 알 수 없다.
    - 정확히는 측정 불가능하나 예측은 할 수 있다.
    - 과거의 CPU burst time을 이용하여 추정(exponential averaging)
    - `Τn+1 = α * tn + (1 - α) * Τn`
      - tn은 프로세스가 n번째 스케줄 됬을 때 실제 수행 시간
      - Tn(타우)은 프로세스가 n번째 스케줄 됬을 때 수행 시간의 추정치
      - 0 ≤ `α` ≤ 1사이의 값으로 비율이다
      - `α` 가 0(최근 히스토리가 카운트되지 않은 경우)과 1인 경우(마지막 실제 수행시간 = 다음번 예측 수행 시간)는 극단적인 경우
        ![5](https://user-images.githubusercontent.com/48282185/173281007-811bb41d-0b9a-46c2-9b1b-2c722c1d0549.png)
      - n+1을 n으로 치환하고 n을 n-1로 치환하고, … 해서 식을 풀면 위와 같이 되는데, T0를 제외하고 실제 수행시간만 남게되는 것을 볼 수 있다. 이것의 이미는 실제 수행시간들에 일정 비율을 곱하여 더해지게 되는데 `α` 와 `1 - α` 가 둘 다 1이하이므로 tn이 n이 작아질수록 더 작은 값(더 작은 가중치)가 곱해져서 측정한지 오래된 값일 수록 가중치가 적어진다.

## 9. Priority Scheduling

- A priority number(integer)이 각 프로세스에 할당되있다.
- 제일 높은 우선순위를 가진 프로세스에게 CPU를 할당한다.(priority number가 작을 수록 우선순위 높음)
  - Preemptive
    - 우선순위가 더 높은 프로세스가 도착했을 경우 CPU 할당
  - NonPreemptive
    - 이미 스케줄되고 있는 프로세스가 있으면 우선순위가 더 높은 프로세스가 도착하더라도 CPU를 할당하지 않음
- SJF는 일종의 priority scheduling이다.
  - priority = predict next CPU burst time
- Problem
  - 우선순위가 낮은 프로세스는 영원히 스케줄 안될 수 있다.(Starvation)
- Solution
  - aging: 시간이 지날 수록 스케줄 안 된 프로세스들의 우선순위를 높이는 방법

## 10. Round Robin(RR)

- 각 프로세스는 동일한 크기의 할당 시간(time quantum)을 가짐(일반적으로 10 ~ 100ms)
- 현대 컴퓨터 시스템은 RR 스케줄방식에 기반을 둔 방법을 사용함

  - 리눅스 스케줄 알고리즘 CFS(none realtime), FIFO, RR(realtime)

    [스케줄링 Scheduling of Linux](https://waristo.tistory.com/12)

    [linux/core.c at 67850b7bdcd2803e10d019f0da5673a92139b43a · torvalds/linux](https://github.com/torvalds/linux/blob/67850b7bdcd2803e10d019f0da5673a92139b43a/kernel/sched/core.c#L5723)

- 할당 시간이 지나면 프로세스는 선점(preempted)당하고 ready queue의 제일 뒤에 삽입됨
- n개의 프로세스가 ready queue에 있고 할당 시간이 q time unit인 경우 각 프로세스는 최대 q time unit단위로 CPU 시간의 1/n을 얻는다.(어떤 프로세스도 (n-1)q time unit이상 기다리지 않는다.) 큐의 맨 끝 프로세스 기준으로 n-1개의 q시간을 기다리면 스케줄 될 수 있음
- 어떤 프로세스이던 간에 response time(처음 CPU를 얻기까지 걸리는 시간)이 빨라짐
- CPU를 길게 쓰게 쓰는 프로세스는 waiting time이 길어지고(계속 뺏기니까 waiting time 누적), CPU를 짧게 쓰는 프로세스는 waiting time이 짧아진다. 즉 쓰는 시간과 waiting time이 비례한다.
- 일반적(수행시간이 짧은 프로세스와 긴 프로세스가 섞여있는 경우)으로 SJF보다 average turnaround time이 길지만 response time은 짧다.
  - 모든 프로세스의 수행시간이 같다고 가정한 특이 케이스를 생각해본다면 오히려 RR이 안좋은 방식일 수 있다. 수행시간 100초인 프로세스 4개가 FCFS로 도는 경우와 10초씩 잘라서 RR로 돌리는 경우 FCFS의 경우는 먼저 도착하는 경우 waiting time이 짧지만, RR은 다른 프로세스의 수행시간까지 기다려야하기에 waiting time이 길어진다.
- Performance
  - q는 할당 시간
  - q large ⇒ FCFS
  - q small ⇒ context switch 오버헤드가 커진다.

## 11. Multilevel Queue

![6](https://user-images.githubusercontent.com/48282185/173281005-afbfac5b-fd40-41be-985c-8e1245306228.png)

- Ready queue를 여러 개로 분할
  - 큐가 두개인 상황을 가정한 예시
    - foreground(interactive)
    - backgorund(batch - no human interaction)
- 각 큐는 독립적인 스케줄링 알고리즘을 갖는다.
  - 큐가 두개인 상황을 가정한 예시
    - foreground - RR(응답시간 짧게)
    - background - FCFS(CPU 오래 사용, 응답시간 빠를 필요 없음, Context Switch overhead 줄임)
- 큐에 대한 스케줄링이 필요, 어떤 큐에게 CPU를 할당할 것인가?
  - Fixed priority scheduling
    - 모든 foreground job들이 완료되고 그 이후 background job이 스케줄
    - 문제는 starvation 가능성
  - Time slice
    - 각 큐에 CPU time을 적절한 비율로 할당
    - 예시로 80%는 foreground, 20%는 background 등

## 12. Multilevel Feedback Queue

![7](https://user-images.githubusercontent.com/48282185/173281000-99d05b05-913d-48bc-ba05-1841dc232bdc.png)

- 프로세스가 다른 큐로 이동 가능
- aging을 이와 같은 방식으로 구현할 수 있다
- 처음들어오는 프로세는 우선순위 제일 높은 큐로 들어가고, 우선순위가 낮을 수록 quantum 길어짐, 큐 간의 스케줄 방식은 우선순위 우선을 따름
- 수행시간이 짧은 프로세스 우선
- Multilevel feedback queue scheduler를 정의하는 parameter들
  - queue의 수
  - 각 큐의 scheduling algorithm
  - process를 상위 큐로 보내는 기준
  - process를 하위 큐로 보내는 기준
  - 프로세스가 CPU 할당받으려할 때 들어갈 큐를 결정하는 기준

## 13. Multiple Processor Scheduling

- CPU가 여러 개인 경우 스케줄링은 더 복잡해짐
- Homogeneous processor인 경우
  - queue에 한 줄로 세워서 각 프로세서가 알아서 꺼내가게 할 수 있다.
  - 반드시 특정 프로세스에서 수행되어야 하는 프로세스가 있는 경우에는 문제가 더 복잡해짐
- Loading sharing
  - 일부 프로세서에 job이 몰리지 않도록 부하를 적절히 공유하는 메커니즘 필요
  - 별개의 큐를 두는 방법 vs 공유되는 큐를 사용하는 방법
- Symmetric Multiprocessing(SMP)
  - 각 프로세서가 각자 알아서 스케줄링 결정
- Asymmetric Multiprocessing
  - 하나의 프로세서가 시스템 데이터의 접근과 공유를 책임지가 나머지 프로세서는 거기에 따른다

## 14. Real Time Scheduling

- Hard real time systems
  - hard real time 정해진 시간 안에 반드시 끝내도록 스케줄링해야함
  - 보통 미리 real time job들이 주어지고 이를 적재적소에 배치하여 스케줄한다. 미리 스케줄한다.
  - 보통 real time job들은 주기적으로 실행되야하는 성격을 띔
- Soft real time computing
  - 일반 time sharing 시스템에서 사용되는 real time app들 - 스트리밍 같은 것들
  - soft real time 프로세스는 일반 프로세스에 비해 높은 priority를 갖도록 해야함
  - dead line을 반드시 지킬 필요는 없다

## 15. Thread Scheduling

- Local scheduling
  - user level thread의 경우 사용자 수준의 thread librarry에 의해 어떤 thread를 스케줄할 지 결정됨
  - 프로세스가 스케줄됬을 때 그 프로세스 내에서 쓰레드를 스케줄함
- Global scheduling
  - kener level thread의 경우 일반 프로세스와 마찬가지로 커널의 단기 스케줄러가 어떤 thread를 스케줄할 지 결정함

## 16. Algorithm Evaluation

![8](https://user-images.githubusercontent.com/48282185/173280991-76e75eb0-8e6f-4e4b-b01d-c072c4d71d57.png)

- Queueing models
  - 이론적인 방법
  - 여기서 server는 CPU
  - 요즘에는 많이 안쓰이는 방법
  - 확률분포로 주어지는 arrival rate(도착율)와 service rate(치리율) 등을 통해 각종 performance index값을 계산
- Implementation(구현) & Measurement(측정)
  - 실제 시스템에 알고리즘을 구현하여 실제 작업(workload)에 대해서 성능을 측정하고 비교함
  - 원래 스케줄 알고리즘 vs 새로 개발한 스케줄 알고리즘을 비교
- Simulation(모의 실험)
  - 알고리즘을 모의 프로그램으로 작성 후 trace를 입력으로 하여 결과 비교
  - 실제 프로세스들의 데이터(trace)를 수집하여 시뮬레이션으로 돌리면 더 신빙성 있는 결과가 된다.

> 출처

> ABRAHAM SILBERSCHATZ ET AL., OPERATING SYSTEM CONCEPTS, NINTH EDITION, WILEY, 2013

> 반효경, 운영체제와 정보기술의 원리, 이화여자대학교 출판부, 2008
