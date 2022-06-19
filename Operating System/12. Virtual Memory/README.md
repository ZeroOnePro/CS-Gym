# 12. Virtual Memory

<!--ts-->

- [12. Virtual Memory](#12-virtual-memory)
  - [1. Demand Paging](#1-demand-paging)
  - [2. Memory에 없는 Page의 Page Table](#2-memory에-없는-page의-page-table)
  - [3. Page Fault](#3-page-fault)
  - [4. Performance of Demand Paging](#4-performance-of-demand-paging)
  - [5. empty frame이 없는 경우](#5-empty-frame이-없는-경우)
  - [6. page replacement](#6-page-replacement)
  - [7. Optimal Algorithm](#7-optimal-algorithm)
  - [8. FIFO(First In First Out) algorithm](#8-fifofirst-in-first-out-algorithm)
  - [9. LRU(Least Recently Used) algorithm](#9-lruleast-recently-used-algorithm)
  - [10. LFU(Least Frequently Used) algorithm](#10-lfuleast-frequently-used-algorithm)
  - [11. LRU와 LFU 알고리즘 예제](#11-lru와-lfu-알고리즘-예제)
  - [12. LRU와 LFU의 구현](#12-lru와-lfu의-구현)
  - [13. 다양한 캐싱 환경](#13-다양한-캐싱-환경)
  - [14. LRU나 LFU을 Paging System에서 활용할 수 있을까?](#14-lru나-lfu을-paging-system에서-활용할-수-있을까)
  - [15. Clock Algorithm](#15-clock-algorithm)
  - [16. Clock Algorithm Example](#16-clock-algorithm-example)
  - [17. Page Frame의 Allocation](#17-page-frame의-allocation)
  - [18. Global vs Local Replacement](#18-global-vs-local-replacement)
  - [19. Thrashing](#19-thrashing)
  - [20. Working-Set Model](#20-working-set-model)
  - [21. Working-Set algorithm](#21-working-set-algorithm)
  - [22. PFF(Page Fault Frequency) algorithm](#22-pffpage-fault-frequency-algorithm)
  - [23. Page Size의 결정](#23-page-size의-결정)

<!-- Created by https://github.com/ekalinin/github-markdown-toc -->
<!-- Added by: sungminyou, at: 2022년 6월 20일 월요일 05시 16분 04초 KST -->

<!--te-->

## 1. Demand Paging

- 페이지가 요청될 때 page를 메모리에 올리는 것
  - IO양의 감소
  - memory 사용량 감소
  - 빠른 응답 시간(IO가 줄어듦)
  - 더 많은 프로세스 수용
- valid / invalid bit의 사용
  - invalid의 의미
    - 사용되지 않는 주소 영역인 경우
    - 페이지가 물리 메모리에 없는 경우
  - 처음에는 모든 page entry가 invalid로 초기화
  - address translation 시에 invalid bit이 set되어 있으면 page fault

## 2. Memory에 없는 Page의 Page Table

![1](https://user-images.githubusercontent.com/48282185/174500513-f1aec44b-d9b1-4a7e-b81a-69c461029d1e.png)

- 실제로 사용되는 페이지는 A ~ F
- G, H는 사용 안됨

## 3. Page Fault

![2](https://user-images.githubusercontent.com/48282185/174500511-e087cbec-fbf8-4146-ac38-4fa89bd9e5db.png)

- invalid page를 접근하면 MMU가 page fault trap을 발생시킴(IO가 필요하기에 운영체제가 관여됨)
- kernel mode로 들어가서 page fault handler가 invoke됨
- 다음과 같은 순서로 page fault가 처리된다
  - invalid reference?(bad address, protection violation) ⇒ abort process
  - get an empty page frame(frame이 부족한 상황이면 교체한다)
  - 해당 page를 disk에서 memory로 읽어온다
    - disk IO가 끝나기까지 이 프로세스는 CPU를 preempt당함(block 상태)
    - disk read가 끝나면 page tables entry 기록 후 valid bit를 valid로 세팅
    - ready queue에 process를 insert → dispatch는 나중에
  - 이 프로세스가 CPU를 잡고 다시 running
  - 아까 중단되었던 Instruction을 재개

## 4. Performance of Demand Paging

- page fault rate 0 ≤ p ≤ 1
  - p가 0이면, page fault가 없음
  - p가 1이면, 매 요청마다 fault
  - 실제 실험적 결과로는 0.0x, 대부분의 경우 fault가 안남
- effective access time
  - (1-p) \* memory access time + p
  - OS & HW page fualt처리 overhead + [if memory full, then swap page out] + swap page in + OS & HW restart overhead

## 5. empty frame이 없는 경우

- OS의 업무
- page replacement
  - 어떤 frame을 내쫓을지 결정해야함
  - 곧바로 사용되지 않을 page를 쫓아내는 것이 좋음
  - 동일한 페이지가 여러 번 메모리에서 쫓겨났다가 다시 들어올 수 있음
- replacement algorithm
  - page fault rate를 최소화하는 것이 목표
  - 알고리즘의 평가
    - 주어진 page reference string에 대해 page fault를 얼마나 내는지 조사
    - reference string의 예시, 참조된 순서를 나열
      - 1, 2, 3, 4, 1, 2, 5, 1, 2, 3, 4, 5

## 6. page replacement

![3](https://user-images.githubusercontent.com/48282185/174500510-e8cbc6ba-787e-4612-b12a-53582fbb8e21.png)

- victim을 쫓아낼 때 단순히 쫓아내는 것이 아니라 변경된 내용 이를테면 write를 한 경우 이를 반영하여 swap area에 저장해야한다

## 7. Optimal Algorithm

![4](https://user-images.githubusercontent.com/48282185/174500509-1f19c5f1-989c-47ae-afb0-3359b558006f.png)

- 미래에 참조될 페이지 목록을 알고있다고 가정, 실제 시스템에서는 사용될 수 없음(offline algorithm)
- 빨간 색은 page fault난 경우, 연보라는 메모리 reference가 된 경우
- 5가 들어오는 시점에서 보면 현재 메모리에 있는 페이지 중 가장 먼 미래에 참조되는 것은 1, 2, 3, 4이므로 4이다. 그래서 4가 쫓겨남, 미래를 볼 수 있기에 미래를 본다
- 어떤 알고리즘을 쓰더라도 이 알고리즘보다는 page fault가 많이난다, 다른 알고리즘의 최대 성능 척도로 활용할 수 있다

## 8. FIFO(First In First Out) algorithm

![5](https://user-images.githubusercontent.com/48282185/174500508-bcd061a5-fb2b-4a3f-a79a-91a6f3183491.png)

- 먼저 들어온 것을 내쫓음
- 재사용된 것을 고려하지 않음
- 메모리 공간을 늘려주더라도 성능이 나빠질 수 있는 특징이 있다
  - FIFO Anomaly(Belady’s Anomaly)라고함

## 9. LRU(Least Recently Used) algorithm

![6](https://user-images.githubusercontent.com/48282185/174500507-e8086311-300e-470e-90da-c38d1717ce42.png)

- 제일 많이 사용되는 알고리즘 ⇒ 왜일지 생각해보자
- 미래를 모를 때 참고할 수 있는 단서는 과거다
- 가장 오래 전에 참조된 것을 쫓아낸다
- 참조가 몇 번되었었는지 고려하지 않음
- 5가 들어오는 시점에서보면, 1, 2, 3, 4, 1 ,2 순으로 참조가 이뤄졌는데 3이 제일 참조된지 오래됬다.

## 10. LFU(Least Frequently Used) algorithm

- LFU는 참조 횟수가 가장 적은 페이지를 쫓아내는 알고리즘
- 최저 참조 횟수인 Page가 여러 개인 경우
  - LFU 알고리즘 자체에서는 여러 page 중 임의로 선정한다
  - 성능 향상을 위해 가장 오래 전에 참조된 page를 지우게 구현할 수도 있다
- 장단점
  - LRU처럼 직전 참조 시점만 보는 것이 아니라 장기적인 시간 규묘를 보기 때문에 page의 인기도를 비교적 정확히 반영할 수 있다 → 장점
  - 참조 시점의 최근성을 반영하지 못함 → 단점
  - LRU보다 구현이 복잡하다 → 단점

## 11. LRU와 LFU 알고리즘 예제

![7](https://user-images.githubusercontent.com/48282185/174500506-f0bc7870-86b8-47ed-8fd3-de1c40c3ab65.png)

## 12. LRU와 LFU의 구현

![8](https://user-images.githubusercontent.com/48282185/174500505-2648f2c8-7bb6-4dda-9391-1b25767881aa.png)

- LRU
  - 참조 시간 순서대로 운영체제가 링크드 리스트형태로 저장
  - 어떤 페이지가 메모리에 들어오거나 재참조 되었을 경우 리스트의 꼬리에 추가, doubley linked list로 하면 중간 삭제 꼬리 삽입 쉽다
  - 쫓아낼 때는 리스트의 머리를 쫓아내면 됨
  - 쫓아내기 위해 O(1)의 시간이 걸린다
- LFU
  - LRU처럼 링크드 리스트 형태로 구현할 경우
    - 참조 회수의 순서대로 운영체제가 링크드 리스트형태로 저장
    - 줄세우기 불가능하다 왜냐하면 재참조 되었다 한들 모든 원소와 참조횟수를 비교해야하므로 비효율적이기 때문
    - 최신화를 위해 O(n)이 걸림
  - heap 자료 구조를 이용
    - 최신화를 위해 O(logn)의 시간복잡도를 갖는다

## 13. 다양한 캐싱 환경

- 캐싱 기법
  - 한정된 빠른 공간(캐시)에 요청된 데이터를 저장해 두었다가 후속 요청 시 캐시로부터 직접 서비스하는 방식
  - paging system 외에도 cache memory(CPU 메모리 접근 전 캐시메모리 접근), buffer caching(File IO), web caching(브라우저에서 요청했던 페이지 저장) 등 다양한 분야에서 활용되는 기법
- 캐시 운영의 시간 제약
  - 교체 알고리즘에서 삭제할 항목을 결정하는 일에 지나치게 많은 시간이 걸리는 경우 실제 시스템에서 사용할 수 없음, 서비스 응답시간을 줄이는 목적의 효용이 없어짐
  - buffer caching이나 web caching의 경우
    - 보통 O(1) ~ O(logn) 정도까지 허용
  - paging system의 경우
    - page fault인 경우에만 OS가 관여
    - 페이지가 이미 메모리에 존재하는 경우 참조시각 등의 정보를 OS가 알 수 없음
    - O(1)인 LRU의 list 조작조차 불가능

## 14. LRU나 LFU을 Paging System에서 활용할 수 있을까?

- 안된다
- LRU나 LFU의 구현 방식에서 봤듯이 각 페이지의 참조를 운영체제가 트래킹을 해야 자료구조 형태로 관리할 수 있을건데, 메모리 참조에서 운영체제의 관여는 오직 page fault일 때만이다. 그래서 이미 메모리에 존재하는 page라면 이 페이지의 참조를 트래킹할 수 없다는 것이다.
- 대신 buffer caching이나 web caching에서 사용될 수 있다. 즉 참조 과정을 전반적으로 운영체제가 책임을 질 때 사용할 수 있는 환경에서 사용가능한 알고리즘이란 것

## 15. Clock Algorithm

- LRU의 근사(approximation) 알고리즘
- 최근에 사용되지 않은 페이지를 쫓아냄
- 여러 명칭으로 불린다
  - second chance algorithm
  - NUR(Not Used Recentry) or NRU(Not Recently Used)
- reference bit는 page내에 존재하며 MMU가 메모리를 참조하면서 1로 세팅한다
- reference bit를 사용해서 교체 대상 페이지 선정(circular linked list)
- reference bit가 0인 것을 찾을 때까지 포인터를 하나씩 앞으로 이동
- 포인터를 이동하는 중에 reference bit를 모두 1로 바꿈
- reference bit가 0인 것을 찾으면 그 페이지가 교체 대상
- 한 바퀴를 되돌아와서도(=second chance) 이면 그 때에는 replace 당함
- 자주 사용되는 페이지라면 second chance가 올 때 1
- 개선
  - reference bit와 modified bit(dirty bit)을 함께 사용
  - reference bit = 1, 최근 참조된 페이지
  - modified bit = 1, 최근 변경된 페이지(IO를 동반하는 페이지), 수정되었으므로 디스크로 쫓겨날 때 변경이 필요, 가능하면 이것을 쫓아내지 않는다(IO가 동반되므로)

## 16. Clock Algorithm Example

![9](https://user-images.githubusercontent.com/48282185/174500504-979605a2-3435-47c3-98be-df8de738ccff.png)

- 회전 시계 방향
- 포인터를 이동하면서 reference bit가 1이었다면 0으로 바꾸고 다음으로 이동(second chance, 다시 돌아 왔을 때는 기회 없다)
- reference bit가 0인 것이 가장 오래된 것이라는 보장이 없기에 LRU와 동일한 운영은 못하지만 흉내낼 수는 있다.
- 만약 모든 reference bit가 1인 상황을 가정해보자 OS는 포인터를 옮기면서 0으로 bit를 바꿔준다했다. 한 바퀴 돌고 나서 시작위치로 되돌아 올 경우 시작할 때 포인터가 가리킨 페이지가 victim

## 17. Page Frame의 Allocation

- allocation problem
  - 각 프로세스에 얼마만큼의 page frame을 할당할 것인가?
  - 각 프로세스가 page fault가 잘 나지 않기 위해 필요한 최소한의 frame개수
- allocation의 필요성
  - 메모리 참조 명령어 수행 시 명령어, 데이터 등 여러 페이지 동시 참조
    - 명령어 수행을 위해 최소한 할당되어야하는 frame수가 있음
  - loop를 구성하는 page들은 한꺼번에 allocation되는 것이 유리함
    - 최소한의 allocation이 없으면 매 loop마다 page fault
- allocation scheme
  - equal allocation
    - 모든 프로세스 동일 개수 할당
  - proportinal allocation
    - 프로세스 크기에 비례하여 할당
  - priority allcoation
    - 프로세스의 priority에 따라 다르게 할당

## 18. Global vs Local Replacement

- Global replacement
  - replace 시 다른 process에 할당된 frame을 빼앗아 올 수 있다
  - process 별 할당량을 조절하는 또 다른 방법임
  - FIFO, LRU, LFU 등의 알고리즘을 global replacement로 사용 시에 해당, 이런 알고리즘들을 쓰다보면 미리 할당 안해도 알아서 할당된다
  - working set, pff 알고리즘 사용
- local replacement
  - 자신에게 할당된 frame 내에서만 replacement
  - FIFO, LRU, LFU 등의 알고리즘을 process 별로 운영 시(할당된 영역 내에서 알고리즘을 돌려서 운영)

## 19. Thrashing

![10](https://user-images.githubusercontent.com/48282185/174500502-e66f84cb-67db-46c0-b781-7608607a775b.png)

- 프로세스의 원활한 수행에 필요한 최소한의 page frame 수를 할당 받지 못한 경우 발생하는 현상
- page fault rate가 매우 높아짐
- CPU utilization이 낮아짐(page fault 처리에, IO가 필요하기 때문)
- OS는 MPD(Mutiprogramming Degree)를 높여야 한다고 판단
- 또 다른 프로세스가 시스템에 추가됨(higher MPD)
- 프로세스 당 할당된 frame 수가 더욱 감소
- 프로세스는 page의 swap in / swap out으로 매우 바쁨
- 대부분의 시간에 CPU는 한가함
- low throughput

## 20. Working-Set Model

- thrashing을 방지하기 위해서 MPD를 조절해야하는데 이 때 사용되는 알고리즘 중 하나
- locality of reference
  - 프로세스는 특정 시간 동안 일정 장소만을 집중적으로 참조한다
  - 집중적으로 참조되는 해당 Page들의 집합을 locality set이라 함
  - 예시로 루프돌 때랑 함수 실행 시 함수 페이지만 활용
- working-set model
  - locality에 기반하여 프로세스가 일정 시간 동안 원활하게 수행되기 위해 한꺼번에 메모리에 올라와 있어야 하는 page들의 집합을 working set이라 정의함
  - working set모델에서는 process의 working set 전체가 메모리에 올라와 있어야 수행되고 거렇지 않을 경우 모든 frame을 반납한 후 swap out(suspend), 예를 들어 어떤 프로세스의 working set이 page 5개로 구성되는데 현재 할당할 수 있는 frame이 3개인 경우 3개를 받지 않고 모든 frame을 반납하고 suspend상태로 전환된다, MPD가 조절됨

## 21. Working-Set algorithm

![11](https://user-images.githubusercontent.com/48282185/174500501-ef40f63d-5ea2-40f3-a5af-73cd7650b779.png)

- 미리 working set을 알 수 없음, 과거를 통해서 추정
- working set의 결정
  - working set window를 통해 알아냄
  - 윈도우 내에 있는 페이지 번호들을 모은 집합(unique elements)
  - 진행됨에 따라 set의 크기는 달라짐, 위의 예시
  - window size가 $\Delta$인 경우
    - 시각 ti에서의 working set WS(ti)
      - time interval[ti - $\Delta$, ti] 사이에 참조된 서로 다른 페이지들의 집합
    - working set에 속한 page는 메모리에 유지, 속하지 않은 것은 버림(즉, 참조된 후 $\Delta$시간 동안 해당 page를 메모리에 유지한 후 버림)
- working set algorithm
  - process들의 working set size의 합이 page frame의 수보다 큰 경우
    - 일부 process를 swap out시켜 남은 process의 working set을 우선적으로 충족시켜준다(MPD 감소)
  - working set을 다 할당하고도 page frame이 남는 경우
    - swap out되었던 프로세스 에게 working set을 할당(MPD 증가)
  - window size $\Delta$
    - working set을 제대로 탐지하기 위해서 window size를 잘 결정해야함
    - $\Delta$ 값이 너무 작으면 locality set을 모두 수용하지 못할 우려
    - $\Delta$ 값이 크면 여러 규모의 locality set을 수용
    - $\Delta$ 값이 $\infty$이면 전체 프로그램을 구성하는 page를 working set으로 간주

## 22. PFF(Page Fault Frequency) algorithm

![12](https://user-images.githubusercontent.com/48282185/174500500-61a84336-8c00-4f4a-a5a0-1b28752a6d36.png)

- thrashing을 방지하기 위해서 MPD를 조절해야하는데 이 때 사용되는 알고리즘 중 하나
- working set을 추정하지 않고 직접 page fault rate을 본다
- page fault rate의 상한값과 하한값을 둔다
  - page fault rate가 상한값을 이상이면 frame을 더 할당한다
  - page fault rate가 하한값을 이하이면 할당 frame 수를 줄인다
- 할당할 수 있는 빈 frame이 없을 경우 프로세스를 swap out(suspend, MPD 감소)

## 23. Page Size의 결정

- page size를 감소시킬 경우
  - 페이지 수 증가
  - 페이지 테이블 크기 증가, entry 더 많이 필요
  - internal fragmentation 감소
  - disk transfer의 효율성 감소
    - seek(디스크 헤드가 이동하는 시간, 오래걸림, 한 번 찾으면 큰 뭉치로 가져오는게 효율적) / rotation vs transfer
  - 필요한 정보만 메모리에 올라와 메모리 이용이 효율적, 페이지 내에서 불필요한 부분이 있을 수 있다
    - locality의 활용 측면에서는 좋지 않음, 한 페이지내에 묶여있일 가능성
- trend
  - larger page size

> 출처

> ABRAHAM SILBERSCHATZ ET AL., OPERATING SYSTEM CONCEPTS, NINTH EDITION, WILEY, 2013

> 반효경, 운영체제와 정보기술의 원리, 이화여자대학교 출판부, 2008
