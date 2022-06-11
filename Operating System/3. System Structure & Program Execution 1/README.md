# System Structure & Program Execution 1

<!--ts-->

- [System Structure &amp; Program Execution 1](#system-structure--program-execution-1)
  - [1. 컴퓨터 시스템의 구조](#1-컴퓨터-시스템의-구조)
  - [2. 입출력의 수행](#2-입출력의-수행)
  - [3. 인터럽트](#3-인터럽트)

<!-- Created by https://github.com/ekalinin/github-markdown-toc -->
<!-- Added by: sungminyou, at: 2022년 6월 11일 토요일 15시 36분 51초 KST -->

<!--te-->

## 1. 컴퓨터 시스템의 구조

![1](https://user-images.githubusercontent.com/48282185/170663025-4fcac455-505a-458a-bd36-5cc191e9a913.png)

![2](https://user-images.githubusercontent.com/48282185/170663015-3fb87e40-bfea-489f-bd9e-6c5112215f29.png)

- cpu 매 클럭 사이클 마다 메모리에서 intruction을 읽어서 실행
- memory는 cpu의 작업 공간
- device controller는 각 IO장치의 cpu역할을 하는데, CPU가 모든 역할을 하기에는 CPU와 IO장치들의 속도차이가 굉장히 커서 비효율적이다. 그래서 각 장치를 관장하는 컨트롤러가 필요해지게 되었다.
  - 제어 정보를 위해 control register, status register를 가진다.
    - local buffer를 갖는다.(일종의 data register)
  - IO는 실제 device와 local buffer 사이에서 일어난다.
  - Device controller는 IO가 끝났을 경우 인터럽트를 통해 CPU에 그 사실을 알린다.
  - device driver는 OS 코드 중 각 장치별 처리루틴을 의미 소프트웨어로 구현됨, device controller로 CPU가 명령을 내리는 코드가 구현되어있다.
  - 장치의 각각의 firmware가 장치를 제어하는 것임, 디바이스 드라이버가 아님
- mode bit는 CPU에서 실행되는 것이 OS인지 사용자 어플리케이션인지 구분할 때 사용
  - 사용자 어플리케이션의 잘못된 수행으로 다른 어플리케이션 및 OS에 피해가 가지 않도록 하기 위한 보호장치가 필요하여 고안됨
  - mode bit를 통하여 하드웨어적으로 두 가지 모드의 operation이 지원됨
    - 0은 모니터 모드(a.k.a 커널 모드, 시스템 모드): OS 코드 수행, 어떤 작업이던 간 수행 가능
    - 1은 사용자 모드: 사용자 어플리케이션 수행, 제한된 instuction만 실행 가능
    - 보안을 해칠 수 있는 중요한 명령어는 모니터 모드에서만 수행 가능한 특권 명령으로 규정
    - 인터럽트나 exeception 발생 시 하드웨어가 mode bit를 0으로 바꾼다.
    - 사용자 어플리케이션에게 CPU를 넘기기 전에 mode bit를 1로 세팅한다.
- interrupt line은 CPU 작업에 제어를 걸 때 필요하다. 예시로 IO장치와 CPU의 상호작용을 위해 필요하다. 왜냐하면 CPU가 IO장치와 직접적으로 작업이 수행되는 것이 아니기 때문이다. device controller에 명령하여 각 IO장치에 요구된 작업을 처리한다.
- IO는 오래 걸리는 작업이기 때문에 무조건 기다리지 않고, IO의 결과가 필요한 작업이 생기면 다른 작업으로 넘어간다.
- timer는 특정 프로세스가 CPU를 독점하는 것을 막기 위해 고안됨
  - 예로 무한루프 도는 프로세스
  - OS는 타이머에 값을 세팅하고 프로세스를 스위칭한다.
  - timer는 인터럽트를 통해 CPU에게 할당된 시간이 끝났음을 알린다.
  - CPU는 instruction을 실행하고 인터럽트라인을 체크한다.
  - 인터럽트가 걸리면 OS로 제어권이 넘어간다.
    - 왜 OS를 넘어가는지? 자원 할당을 OS가 담당하기 때문에 OS로 제어권을 넘기는 것임
    - OS가 사용자 어플리케이션에게 제어권을 넘겨준 후에 자력으로 다시 뺏어올 수는 없다 왜냐하면 뺏어 올라고 해도 어떤 명령이 실행되야하는데, 자원의 제어권이 넘겨준 사용자 어플리케이션에 있기 때문에 명령을 실행할 방법이 없기 때문에, timer라는 특별한 하드웨어가 필요하다.
- 프로세스가 종료되면 제어권을 OS로 넘김
- IO를 위한 코드가 프로세스 실행에 포함되있으면, OS로 제어권을 넘긴다.
  - IO는 보안 상의 이유로 항상 OS를 통해서 발생되게 구현됨
  - OS는 device controller에 작업을 시키고 다른 사용자 애플리케이션으로 제어권을 넘긴다
  - 요청된 IO작업이 끝나고 device controller가 인터럽트가 걸면, CPU에서 인터럽트를 확인하고 입력된 값을 입력 요청한 애플리케이션 메모리 공간에 값을 copy해준다.
  - 그리고 보통은 인터럽트를 당할 때 실행되고 있던 프로세스에게 제어권을 넘겨준다. timer에 남은 시간에 따라 다르겠지만 그러면 IO작업 완료되고 나서 인터럽트 당했던 프로세스에게 넘겨준다고 했는데, 인터럽트 당할 때 실행되고 있던 프로세스도 기억하고 있을 필요가 있겠구나
- DMA controller는 너무 자주 일어나는 인터럽트로 인해 CPU의 작업이 자주 중단되는 비효율을 막기 위해 고안됨.
  - Direct Memory Access의 약자로 메모리 접근은 CPU만 가능하도록 고안되었으나 빈번히 일어나는 인터럽트의 비효율성으로 인해 DMA Controller를 두어 이를 해결하고자 함
  - 메모리에 CPU도 접근할 수 있고, DMA controller도 접근할 수 있기 때문에 두 장치 간에 데이터를 같은 공간에 쓴다던지 하는 문제가 발생할 수 있어서 memory controller를 따로 두어 이를 방지한다.
  - 이 장치는 각 IO 장치들의 인터럽트를 대신 받고, 사용자 어플리케이션 메모리 공간에 데이터를 copy해주는 작업까지 수행한다. 사용자 어플리케이션의 IO 작업이 끝났음을 알리기 위해 CPU로 인터럽트를 보낸다. batch 처리가 되야하지 싶다. 각 장치마다 인터럽트 걸면 존재이유가 사라진다.

## 2. 입출력의 수행

- 모든 입출력 명령은 특권 명령이다.
- 사용자 어플리케이션은 어떻게 IO를 하는가?
  - system call을 사용하여 운영체제에게 IO요청
  - trap(소프트웨어 인터럽트)을 사용하여 인터럽트 벡터의 특정 위치로 이동
  - 제어권이 인터럽트 벡터가 가리키는 인터럽트 서비스 루틴으로 이동
  - 올바른 IO요청인지 확인 후 IO 수행
  - IO완료 시 device controller가 인터럽트(하드웨어 인터럽트)를 걸어서 제어권을 system call 다음 명령으로 옮김, system call의 진행 상황은 커널 함수의 메모리 공간 어딘가에 저장되있을 것이다
- 일반적인 함수 호출의 경우 해당 프로세스 내의 메모리 공간에서 instuction들이 점프를 해가면서 실행되는데, IO같이 운영체제가 필요한 작업의 경우 system call을 사용한다. system call은 사용자 어플리케이션이 CPU로 직접 인터럽트 라인을 세팅해서 OS로 제어권을 넘긴 다음에 작업을 하는 것을 말한다.

## 3. 인터럽트

- 현재의 운영체제는 인터럽트에 의해 구동됨. OS는 인터럽트가 들어올 때마다 사용됨
- 인터럽트 당한 시점의 레지스터와 program counter를 save한 후 CPU의 제어를 인터럽트 처리 루틴에 넘긴다.
- 인터럽트의 넓은 의미
  - interrupt(하드웨어 인터럽트): 하드웨어가 발생 시킨 인터럽트
    - timer
    - IO device controller
  - trap(소프트웨어 인터럽트)
    - exception: 프로세스가 오류를 범한 경우
    - system call: 프로세스가 커널 함수를 호출하는 경우
- 인터럽트 관련 용어
  - 인터럽트 벡터: 해당 인터럽트의 처리 루틴 주소들을 가지고 있다. 인터럽트 번호와 주소의 쌍
  - 인터럽트 처리 루틴(인터럽트 핸들러): 해당 인터럽트를 처리하는 커널 함수

> 출처

> ABRAHAM SILBERSCHATZ ET AL., OPERATING SYSTEM CONCEPTS, NINTH EDITION, WILEY, 2013

> 반효경, 운영체제와 정보기술의 원리, 이화여자대학교 출판부, 2008
