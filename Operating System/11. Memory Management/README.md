# 11. Memory Management

<!--ts-->

- [11. Memory Management](#11-memory-management)
  - [1. Logical vs Physical Address](#1-logical-vs-physical-address)
  - [2. 주소 바인딩(Address Binding)](#2-주소-바인딩address-binding)
  - [3. Memory Management Unit(MMU)](#3-memory-management-unitmmu)
  - [4. Dynamic Relocation](#4-dynamic-relocation)
  - [5. Dynamic Loading](#5-dynamic-loading)
  - [6. Overlays](#6-overlays)
  - [7. Swapping](#7-swapping)
  - [8. Dynamic Linking](#8-dynamic-linking)
  - [9. Allocation of Physical Memory](#9-allocation-of-physical-memory)
  - [10. Contiguous Allocation](#10-contiguous-allocation)
  - [11. Paging](#11-paging)
  - [12. Page Table](#12-page-table)
  - [13. Implementation of Page Table](#13-implementation-of-page-table)
  - [14. Effective Access Time](#14-effective-access-time)
  - [15. Two Level Page Table](#15-two-level-page-table)
  - [16. Two Level Paging Example](#16-two-level-paging-example)
  - [17. Multilevel Paging and Performance](#17-multilevel-paging-and-performance)
  - [18. Valid(v) / Invalid(i) / Protection bit in a page table](#18-validv--invalidi--protection-bit-in-a-page-table)
  - [19. Inverted Page Table Architecture](#19-inverted-page-table-architecture)
  - [20. Shared Page](#20-shared-page)
  - [21. Segmentation](#21-segmentation)
  - [22. Segmentation Architecture](#22-segmentation-architecture)
  - [23. Paged Segmentation](#23-paged-segmentation)
  - [24. 물리 메모리 관리에서 운영체제의 역할](#24-물리-메모리-관리에서-운영체제의-역할)

<!-- Created by https://github.com/ekalinin/github-markdown-toc -->
<!-- Added by: sungminyou, at: 2022년 6월 19일 일요일 03시 13분 29초 KST -->

<!--te-->

## 1. Logical vs Physical Address

- Logical Address(a.k.a virtual address)
  - 프로세스마다 독립적으로 가지는 주소 공간
  - 각 프로세스마다 0번지부터 시작
  - CPU가 보는 주소는 logical address임, 즉 instruction에 포함된 주소는 논리 주소이고, 실행 시 마다 주소의 변환이 일어난다, 이 논리 주소가 변경되려면 재컴파일해야함
- Physical address
  - 메모리에 실제 올라가는 위치
- 주소 바인딩
  - 논리 주소를 물리 주소를 결정하는 작업
  - Symbolic address(프로그래머가 지정하는 변수들, 함수 이름 등) → Logical address → Physical address

## 2. 주소 바인딩(Address Binding)

![1](https://user-images.githubusercontent.com/48282185/174451435-37eb0372-579b-4df6-a7e8-a957b6c4916f.png)

- Compile time binding
  - 물리 메모리 주소가 컴파일 시 알려짐
  - 시작 위치 변경 시 재컴파일
  - 컴파일러는 절대 코드(absolute code, compile time binding으로 생성된 코드)생성
  - 비효율적인 방법이라 현대 컴퓨터 시스템에서는 안 쓰이고 예전 하나의 프로세스만 동작하던 때에 사용했던 방법
- Load time binding
  - Loader의 책임 하에 물리 메모리 주소 부여
  - 메모리에 올라가는 시점에 물리 주소 부여됨
  - 컴파일러가 재배치 가능코드(relocatable code, 실행 시에 특정 위치가 아닌 비어있는 위치 어디던지 배치 가능)를 생성한 경우 가능
- Execution time binding(a.k.a Run time binding)
  - 수행이 시작된 이후에도 프로세스의 메모리 상 위치를 옮길 수 있음
  - 메모리에 올라가는 시점에 물리 주소가 부여되는 것은 load time binding과 같으나 실행 중 주소가 변경될 수 있음
  - 메모리에서 쫓겨날 수 있기때문에 필요하다
  - 현대 컴퓨터 시스템에서 사용되는 주소 결정 방법
  - CPU가 주소를 참조할 때마다 binding을 점검(address mapping table)
  - 하드웨어의 지원이 필요(base and limit registers, MMU)

## 3. Memory Management Unit(MMU)

- logical address를 physical address로 매핑해주는 hardware device
- MMU scheme
  - 사용자 프로세스가 CPU에서 수행되며 생성해내는 모든 주소 값에 대해 base register(=relocation register)의 값을 더한다
- user application
  - logical address만을 다룬다
  - 실제 physical address를 볼 수 없으며 알 필요가 없다

## 4. Dynamic Relocation

![2](https://user-images.githubusercontent.com/48282185/174451434-42d11a9f-add1-412c-a98a-e150d7e6fc83.png)

- 현대 운영체제는 프로세스 전체가 메모리에 올라가는 것이 아니라 필요 일부분만 올라가는데 지금은 실행되면 프로세스 전체가 메모리에 올라가는 것을 가정한다
- MMU는 두 개의 레지스터를 통해 주소변환을 시켜준다
  - relocation register에는 프로세스의 물리 시작 주소가 저장됨
  - limit register에는 프로세스의 최대 크기가 저장됨
- limit register는 운영체제 및 사용자 프로세스 간의 메모리 보호를 위해 사용한다
  ![3](https://user-images.githubusercontent.com/48282185/174451433-4bde61b0-69b8-43a7-ad32-da2e5de6aa9b.png)
  - 악성코드가 자신의 프로세스 공간을 벗어난 주소를 요청하는 경우가 주면 안됨

## 5. Dynamic Loading

- loading은 메모리에 올리는 작업을 말한다
- 프로세스 전체를 메모리에 미리 다 올리는 것이 아니라 해당 루틴이 불려질 때 메모리에 로드하는 것을 말함
- memory utilization을 향상시키기 위해 필요
- 가금씩 사용되는 많은 양의 코드의 경우 특히 유용
  - exception 처리 루틴
- 운영체제의 특별한 지원 없이 프로그램 자체에서 구현 가능(라이브러리를 통해 지원)
- paging과 용어를 섞어 쓰기도 함(본래는 프로그래머가 구현한 동적 로딩만을 말했지만 운영체제가 해주는 paing도 동적 로딩이라고 하기도 한다 뭐 이런 의미)

## 6. Overlays

- 메모리에 프로세스의 부분 중 실제 필요한 정보만을 올림(동적 로딩하고 다를 바 없지만 역사적인 차이가 있다)
- 프로세스의 크기가 메모리보다 클 때 유용
- 운영체제의 지원없이 사용자에 의해 구현
- 작은 공간의 메모리를 사용하던 초창기 시스템에서 수작업으로 프로그래머가 구현
  - Manual Overlay라고도 함
  - 프로그래밍이 매우 복잡

## 7. Swapping

- backing store(= swap area)
  - 디스크, 많은 사용자 프로세스 이미지를 담을 만큼 충분히 빠르고 큰 저장 공간
- 프로세스를 일시적으로 메모리에서 backing store로 쫓아내는 것
- 본래 swapping의 의미는 프로세스 전체가 메모리에서 쫓겨나는 프로세스 단위의 개념이었지만 paging 기법이 등장하며 프로세스의 일부분만 쫓겨나는 것도 paging swapping이라고한다
- swap in(쫓겨났던 프로세스 내용 메모리로 로드) / swap out(메모리에 있는 프로세스 내용 디스크로 쫓아냄)
  - 일반적으로 중기 스케줄러(swapper)에 의해 swap out시킬 프로세스 선정
  - priority-based CPU scheduling algorithm
    - priority가 낮은 프로세스를 swap out시킴
    - priority가 높은 프로세스를 메모리에 올려 놓음
  - compile time 혹은 load time binding에서는 원래 메모리 위치로 swap in 해야함, 그래서 이 두 방식은 비효율적이다, 반면 runtime binding은 빈 아무 위치로나 올릴 수 있음
  - swap time은 대부분 transfer time(swap되는 양에 비례하는 시간)이다

## 8. Dynamic Linking

- linking을 실행 시간까지 미루는 기법
- 컴파일된 여러 파일들을 엮어서 실행파일을 만드는 과정을 linking이라고 함
- static linking
  - 라이브러리가 프로그램의 실행파일 코드에 포함됨
  - 실행 파일의 크기가 커짐
  - 동일한 라이브러리를 가각의 프로세스가 메모리에 올리므로 메모리 낭비
    - 예시로 printf함수의 라이브러리 코드
- dynamic linking
  - 라이브러리가 실행 시 연결(link)됨
  - 라이브러리 호출 부분에 라이브러리 루틴의 위치를 찾기 위한 stub이라는 작은 코드를 둠
  - 라이브러리가 이미 메모리에 있으면 그 루틴의 주소로 가고 없으면 디스크에서 읽어옴
  - 운영체제의 도움이 필요
  - 라이브러리는 여러 프로세스에서 사용되는 경우가 많기때문에 같은 라이브러리를 사용하는 프로세들 끼리 공유해서 사용할 수 있어서 좋다
    - linux - sharing library, sharing object
    - windows - dll(dynamic linking library)

## 9. Allocation of Physical Memory

- 메모리는 일반적으로 두 영역으로 나뉘어 사용
  - OS 상주 영역
    - interrupt vector와 함께 낮은 주소 영역 사용
  - 사용자 프로세스 영역
    - 높은 주소 영역 사용
- 사용자 프로세스 영역의 할당 방법
  - Contiguous allocation
    - 각각의 프로세스가 메모리의 연속적인 공간에 적재되도록 하는 방법
    - fixed partition allocation
    - variable partition allocation
  - Noncontiguous allocation
    - 하나의 프로세스가 메모리의 여러 영역에 분산되어 올라갈 수 있음
    - paging
    - segmentation
    - paged segmentation

## 10. Contiguous Allocation

![4](https://user-images.githubusercontent.com/48282185/174451432-e293d5a6-6ba9-4cb8-a299-d84c584117b6.png)

- 내부조각(internal fragmentation)
  - 프로그램 크기보다 분할의 크기가 큰 경우
  - 하나의 분할 내부에서 발생하는 사용되지 않는 메모리 조각
  - 특정 프로그램에 배정되었지만 사용되지 않는 공간
- 외부조각(external fragmentation)
  - 프로그램 크기보다 분할의 크기가 작은 경우
  - 아무 프로그램에도 배정되지 않은 빈 공간인데도 프로그램이 올라갈 수 없는 작은 분할
- 고정 분할 방식(fixed partition allocation)
  - 사용자 프로세스들을 적재할 공간을 미리 할당해놓는 기법
  - 물리메모리를 몇 개의 영구적 분할로 나눔
  - 각 분할의 크기를 다양하게 혹은 동일하게 만들 수 있다
  - 각 분할당 프로그램 하나 적재
  - 융통성이 없는 방식
    - 동시에 메모리에 load될 수 있는 프로그램의 수가 고정됨 = 분할의 개수
    - 최대 수행 가능 프로그램 크기 제한이 발생 = 최대 크기의 분할
  - 내부조각과 외부조각이 발생
  - 각 조각들은 시시각각 내부조각이 될 수도 외부 조각이 될 수도 있다. 위의 예시에서 프로그램 B가 적재된 시각에서 조각을 나눠본다면, 분할 2번은 외부 조각, 분할 3에는 내부 조각이 생긴다. 하지만 이후 다른 프로그램이 적재되고 분할 2를 쓰게된다면, 분할 2는 더이상 외부 조각이 아니다.
- 가변 분할 방식(variable partition allocation)
  - 안 쓰이는 공간이 생기니 굳이 미리 나눠놓을 필요가 있겠는가? 해서 등장한 프로그램의 크기를 고려해서 할당하는 기법
  - 분할의 크기, 개수가 동적으로 변함
  - 기술적 관리 기법필요
  - 프로그램이 실행될 때마다 연속적으로 순서대로 메모리에 적재
  - 외부조각이 발생
  - 하지만 이 방식에도 안 쓰이는 공간이 발생되는데 위의 예시에서 B가 끝난 시점에 D가 적재된다고 하자 B가 사용 중이던 공간은 크기가 안 맞아서 못들어감, D가 배치되고 난 후 남은 영역도 D가 들어갈 수 없으므로 외부조각이 된다.
- hole
  ![5](https://user-images.githubusercontent.com/48282185/174451430-c96428e5-2175-418f-8694-bfa6923eff37.png)
  - 가용 메모리 공간
  - 다양한 크기의 hole들이 메모리 여러 곳에 흩어져 있음
  - 프로세스가 도착하면 수용 가능한 hole을 할당
  - 운영체제는 다음 정보를 유지해야함
    - 할당 공간
    - 가용 공간(hole)
- Dynamic Storage-Allocation Problem
  - 가변 분할 방식에서 size n인 요청을 만족하는 가장 적절한 hole을 찾는 문제
  - first-fit
    - size가 n이상인 것 중 최초로 찾아지는 hole에 할당
  - best-fit
    - size가 n이상인 것 중 최소 크기의 hole을 찾아서 할당
    - hole들의 리스트가 크기 순으로 정렬되지 않은 경우 모든 hole의 리스트를 탐색해야함
    - 많은 수의 아주 작은 hole들이 형성됨
  - worst-fit
    - 가장 큰 hole에 할당
    - 역시 모든 리스트를 탐색해야함
    - 상대적으로 아주 큰 hole들이 생성됨
    - 더 큰 프로그램이 올 수 있으므로 미래의 가용공간 확보 차원에서 worst-fit은 비효율적인 방법
  - first-fit과 best-fit이 worst-fit보다 속도와 공간 이용를 측면에서 효과적인 것으로 알려짐(실험적인 결과)
- compaction
  - 외부 조각 문제를 해결하는 한 가지 방법
  - 사용 중인 메모리 영역을 한 군데로 몰고 hole들을 다른 한 곳을 몰아 큰 hole하나를 만드는 방법
  - 매우 비용이 많이 드는 방법임(전체 프로세스들의 address binding 점검)
  - 최소한의 메모리 이동으로 compaction하는 방법(매우 복잡한 문제, 어떤 프로그램을 이동시킬 것인가?)
  - compaction은 프로세스의 주소가 실행 시간에 동적으로 재배치 가능한 경우(runtime binding을 지원)에만 수행될 수 있다

## 11. Paging

- process의 virtual memory(주소 공간)를 동일한 사이즈의 page 단위로 나눈다
- virtual memory의 내용이 page단위로 noncontiguous하게 저장됨
- 일부는 backing storage, 일부는 physical memory에 저장
- 흩어져 있기때문에 주소 바인딩이 복잡해진다, 레지스터 두 개로는 안된다
- basic method
  - 물리메모리를 동일한 크기의 frame으로 나눔(page와 동일한 크기, 즉 page가 들어갈 수 있는 공간)
  - 논리메모리를 동일한 크기의 page로 나눔
  - 모든 가용 frame들을 관리
  - page table을 사용하여 논리 주소를 물리 주소로 변환
  - 같은 크기로 잘렸기 때문에 외부 조각 문제 발생 안함
  - 프로그램의 크기가 페이지 크기의 배수라는 보장이 없다. 내부 조각 문제 발생 가능, 프로그램 크기 / 페이지 크기하게되면 나머지가 남는다. (나머지 크기 < page 크기)
- 각 페이지들이 어느 위치인지 알기위해 페이지 별로 주소변환을 해줘야함

## 12. Page Table

![6](https://user-images.githubusercontent.com/48282185/174451429-3c6a7589-bf02-4fd1-b1df-23e5465c4c25.png)

![7](https://user-images.githubusercontent.com/48282185/174451427-d9cab2ab-5e59-4fbc-9bb1-1edc5dc044fa.png)

- 논리 주소의 페이지와 물리 주소의 frame을 매칭시켜주는 자료구조
- table은 배열인데 인덱스를 이용하여 탐색없이 곧바로 찾을 수 있게 한 것을 말함
- p는 페이지의 번호(page table의 인덱스), d는 offset(page내에서 Instruction의 위치)이고 논리 주소와 물리 주소에서 같음 얼마나 떨어져있는지는 변하지 않는다
- page 하나의 크기는 일반적으로 4KB이고 하나의 프로세스에 필요한 페이지 수는 대략 백만개를 넘는다. 그러면 이 page table은 어디에 저장할까? 크기가 크기때문에 레지스터나 캐시에 저장할 수는 없다. 따라서 이 테이블을 메모리에 저장해두게 된다

## 13. Implementation of Page Table

- page table은 메모리에 상주
- page table base register(PTBR)가 page table의 주소를 저장
- page table length register(PTLR)가 테이블 크기를 저장
- 모든 메모리 접근 연산에는 2번의 memory access가 필요
  - page table 접근 1번, 실제 data / instruction 접근 1번
  - 상당히 비효율적이기에 속도 향상을 위해
    - translation lock-aside buffer(TLB, associative register라고도 부름)라 불리는 고속의 lookup hardware cache 사용, 메모리보다 빨라야 효용성 있겠지 → 캐시메모리
      ![8](https://user-images.githubusercontent.com/48282185/174451426-ea431f62-83cd-45a9-ac95-0b338ed1ab9c.png)
    - TLB는 Page table 전체를 보관하는게 아니다, 따라서 p를 인덱스로 사용할 수 없기에 p와 f를 쌍으로 가지고 있어야된다.
    - 전체 탐색이 필요해짐에 따라 TLB는 이 탐색과정을 막고자 Parallel search가 가능하게 구현되어있다. 즉 배열의 모든 엔트리를 한 번에 탐색한다
    - 주소 변환 과정
      - page table 중 일부가 TLB에 보관되어 있음
      - 만약 page n이 TLB에 있을 경우 곧바로 frame n’을 얻음
      - 그렇지 않은 경우 메모리에 있는 page table로부터 frame n’을 얻음
      - page table도 프로세스 하나마다 존재하기에 TLB는 context switch 때 flush(remove old entries)

## 14. Effective Access Time

- 과연 TLB를 사용하는 것이 page table을 직접 참조하는 것보다 효율적일까?
  ![9](https://user-images.githubusercontent.com/48282185/174451425-7719790a-a513-4d96-b542-ab4e92973bef.png)
  - TLB가 없다고 가정하면 메모리에 2번 접근해야하므로 2라는 시간이 걸린다
  - TLB가 있으면 입실론은 매우 작은 값, 알파는 1에 가깝기에 2보다는 작은 값이 된다. 따라서 TLB를 사용하는 것이 효율적이다

## 15. Two Level Page Table

![10](https://user-images.githubusercontent.com/48282185/174451423-5935b435-9d92-412b-80cc-b7f81f5a3e36.png)

- 컴퓨터의 기술이 필요한 이유는 대개 2가지인데 속도 향상이거나 공간 낭비 방지이다
- 현대의 컴퓨터는 address space가 매우 큰 프로그램 지원
  - 32 bit address 사용 시 2^32(4GB)의 주소 공간
  - page size가 4KB일 시 1M개의 page table entry 필요
  - 각 page entry가 4B(=32bit) 시 프로세스 당 4MB page table필요
  - 그러나, 대부분 프로그램은 4GB의 주소 공간 중 지극히 일부분만 사용하므로 page table의 공간이 심하게 낭비됨, 페이지 번호가 인덱스로 사용되기에 사용되지 않음에도 불구하고 page table은 프로세스의 전체 페이지들을 담고 있을 수 있게 만들어져야함
  - 이에 대한 해결책으로 page table 자체를 page로 구성
  - 사용되지 않는 주소 공간에 대한 outer page table의 엔트리 값은 NULL, 대응하는 inner page table이 없음(안 만들어짐)
  - inner page table은 page와 4kb로 크기가 동일(지금 하고있는게 page table을 paging하는 것이다) entry 한 개의 크기는 4b이므로 1k개의 entry가 들어갈 수 있다
  - 1K개의 inner page table(outer page table의 entry 수) _ 1K개의 page(inner page table entry 수) = 1k _ 1k = 1m개의 page table entry가 되어 결국에는 1 level과 같은 page table entry 수가 만들어지지만 안 쓰이는 page의 경우 outer page table의 entry에 NULL로 되어있어 inner page table이 만들어지지 않는다

## 16. Two Level Paging Example

![11](https://user-images.githubusercontent.com/48282185/174451422-2135f6ae-98e1-46a4-b810-f74f37e0006f.png)

- logical address(32bit 머신과 4kb page size)의 구성, offset부터 타고 올라오면서 분석
  - 20bit의 page number, inner page table 하나의 entry는 1k개이고, 각 entry를 구분하기 위해서 10bit가 필요하다
  - 12bit의 page offset, 4k(2^12)b크기의 page내에서 주소는 byte단위로 나눠지기 때문에 page 내에서의 주소 구분을 위해서는 총 12bit가 필요
- page table 자체가 page로 구성되기 때문에 page number는 두 부분으로 나뉜다(각 page table entry는 4b)
  - 10bit의 page number
  - 10bit의 page offset
- 위의 그림에서 p1은 outer page table의 index이고 p2는 outer page table의 page에서의 변위

![12](https://user-images.githubusercontent.com/48282185/174451421-961609ed-5fb5-4389-bd72-d37c0b8e633b.png)

- outer page table로 가서 p1을 이용하여 inner page table의 번호를 확인
- inner page table로 가서 p2를 이용하여 frame의 번호를 확인
- frame으로 가서 d만큼의 offset을 간 후에 원하는 data / instruction 확인

## 17. Multilevel Paging and Performance

- Address space가 더 커지면 다단계 페이지 테이블 필요
- 각 단계의 페이지 테이블이 메모리에 존재하므로 주소 바인딩에 더 많은 메모리 접근 필요
- TLB를 통해 메모리 접근 시간을 줄일 수 있음
- 4단계 페이지 테이블을 사용하는 경우
  - TLB가 없을 경우 500ns(5번, 주소 변환 4번 / 데이터 획득 1번)
  - 메모리 접근 시간이 100ns, TLB 접근 시간이 20ns, TLB hit rate 0.98인 경우
  - TLB에서 hit된다면 4단계 테이블을 통해 물리주소로 변환되어 나왔을 것
  - effective memory access time = 0.98 _ 120(100ns + 20ns) + 0.02 _ 520(500ns + 20ns) = 128ns

## 18. Valid(v) / Invalid(i) / Protection bit in a page table

![13](https://user-images.githubusercontent.com/48282185/174451420-33e8516d-e439-4eef-88f8-8a4a98b3d893.png)

- page table은 프로세스의 가상공간 크기만큼 만들어 지고, 쓰이는 것 안 쓰이는 것 나뉘게 된다고 했다.
- valid / invalid는 page table entry내에 존재하며 page number와 frame number의 쌍이 유효한지 비유효한지를 나타내는데, 여기서 유효의 의미는 해당 주소의 frame에 그 프로세스를 구성하는 유효한 내용이 있음을 의미한다. 위의 예시를 보자 프로세스는 0 - 5까지의 page number를 사용하고, 이는 각각 frame과 쌍을 이뤄 실제로 적재되어있다.(유효) 그러나 page table을 만들며 안쓰이는 6, 7은 entry는 있으나 실제 frame과 짝을 이루지 않는다(비유효), 프로세스가 그 주소부분을 사용하지 않거나 해당 페이지가 swap area에 있는 경우 Invalid가 된다
- protection bit는 page에 대한 접근 권한(read / write / read-only)인데, 이 접근이라는게 다른 프로세스의 접근을 의미하는게 아니다. 주소 공간이 페이징이 되면서 각 페이지는 code, stack, data 중 하나일 건데, code의 경우 변하지 않는 데이터이다(read-only), stack이나 data의 경우 읽쓰 모두 가능이기에 (read / write)권한을 부여한다

## 19. Inverted Page Table Architecture

![14](https://user-images.githubusercontent.com/48282185/174451418-7937b36e-1b69-4dde-af7a-63a2ba51f48e.png)

- 원래 페이지 테이블 추적과정을 역추적한 방법(물리주소를 통해 논리주소를 알아내는 방법)
- page table을 완전 탐색해야함
- 공간을 많이 줄일 수 있으나 시간은 더 걸림
- p는 unique한 값이 아니기에 pid와 합쳐 unique하게 만들어준다
- page table이 큰 이유
  - 모든 process 별로 그 주소공간에 대응하는 모든 page에 대해 page table entry가 존재하기 때문
  - 대응하는 page가 메모리에 있든 아니든 간에 page table에는 entry로 존재
- inverted page table
  - page frame 당 하나의 page table entry를 부여(system-wide)
  - 각 page table entry는 각각의 물리적 메모리의 page frame이 담고 있는 내용을 표시(pid, logical address)
  - 단점
    - 테이블 전체를 탐색해야함
    - 해결책 - associative register 사용(parallel search) but, expensive

## 20. Shared Page

![15](https://user-images.githubusercontent.com/48282185/174451414-d3349d4e-53fb-427f-b818-f7189696f749.png)

- re-entrant code(=pure code)
- read-only로 하여 프로세스 간에 하나의 code만 메모리에 올림
  - 예로 텍스트 에디터, 컴파일러 등 같은 프로그램 여러개를 동시에 띄워놓을 때
- shared code는 모든 프로세스의 logical address space에서 동일한 위치에 있어야함
  - 컴파일된 코드는 logical address가 정해져 있음
- private code and date
  - 각 프로세스들은 독자적으로 메모리에 올림
  - private data는 logical address space의 아무 곳이나 위치 가능

## 21. Segmentation

- 프로그램은 의미 단위인 여러 개의 segment로 구성
  - 작게는 프로그램을 구성하는 함수 하나하나를 세그먼트로 정의
  - 크게는 프로그램 전체를 하나의 세그먼트로 정의 가능
  - 일반적으로는 code, data, stack 부분이 하나 씩 세그먼트로 정의됨
- segment는 아래와 같은 logical unit들로 구성
  - main()
  - function
  - global variables
  - stack
  - symbol table, arrays

## 22. Segmentation Architecture

![16](https://user-images.githubusercontent.com/48282185/174451413-7bb1566b-1ea6-4543-a844-d244d19991d5.png)

- logical address는 <segment-number, offset>으로 구성됨, offset으로 인해 segment의 크기가 제한됨
- paging과 다르게 물리 메모리를 같은 크기로 나눌 수 없기에 실제 물리 주소를 계산할 수 있게해야함
- segment table
  - 각 entry는 base(세그먼트의 물리 주소 시작위치), limit(세크먼트의 크기, 세그먼트의 길이가 균일하지 않기 때문에 필요)를 갖는다
- segment-table base register(STBR)
  - 물리 메모리에서의 segment table 위치
- segment-table length register(STLR)
  - 프로그램이 사용하는 세그먼트의 수
  - 세그먼트 번호 < STLR 저장값
- protection
  - 각 세그먼트 별로 protection bit가 있음
  - 각 entry에 read, write, execution 권한 비트가 들어감
  - paging에서는 같은 크기 분할이기에 한 페이지에 code와 data가 같이 들어가는 경우 권한 설정하는데 추가 작업이 필요하지만 segment는 의미 단위 분할이기에 그런 문제가 발생하지 않는다
- sharing
  - shared segment
  - same segment number
- segment는 의미 단위 이기에 공유와 보안에 있어 paging보다 효과적
- table을 위한 메모리 소비가 적다
- allocation
  - first fit / best fit
  - 외부 조각 발생
  - segment의 길이가 동일하지 않으므로 가변분할방식에서와 동일한 문제들이 발생

## 23. Paged Segmentation

![17](https://user-images.githubusercontent.com/48282185/174451408-b7eef245-c93c-40e3-af05-bbcafb9ba617.png)

- segment를 paging하는 혼합 방법
  - segment가 page로 쪼개져서 메모리에 적재
- sharing, protection은 segement table에서 관리하여 효율 추구, allocation 문제를 없애기 위해 메모리에 적재하는 것은 paging을 사용
- pure segmentation과의 차이
  - segment table entry가 segment의 base address를 가지는게 아니라 segment를 구성하는 page table의 address를 가지고 있음

## 24. 물리 메모리 관리에서 운영체제의 역할

- 없다.
- 왜냐하면 주소 변환 시에 운영체제가 개입한다는 것은 매 클럭사이클마다 운영체제가 개입한다는 얘긴데 이건 너무 비효율적이다
- 운영체제도 프로그램인데 메모리 주소 변환에 운영체제가 개입하면 운영체제의 주소 변환은 자기 스스로 해야한다는 얘긴데 말이 안된다
- 주소 변환은 모두 HW에 의해 이루어진다
- IO접근에 운영체제가 개입하는 것과는 다름

> 출처

> ABRAHAM SILBERSCHATZ ET AL., OPERATING SYSTEM CONCEPTS, NINTH EDITION, WILEY, 2013

> 반효경, 운영체제와 정보기술의 원리, 이화여자대학교 출판부, 2008
