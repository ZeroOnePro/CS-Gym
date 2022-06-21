# 14. Disk Management and Scheduling

<!--ts-->

- [14. Disk Management and Scheduling](#14-disk-management-and-scheduling)
  - [1. Disk Structure](#1-disk-structure)
  - [2. Disk Management](#2-disk-management)
  - [3. Disk Scheduling](#3-disk-scheduling)
  - [4. Disk Scheduling Algorithm](#4-disk-scheduling-algorithm)
  - [5. Disk Scheduling Algorithm의 결정](#5-disk-scheduling-algorithm의-결정)
  - [6. Swap Area Management](#6-swap-area-management)
  - [7. RAID(Redundant Array of Independent Disks)](#7-raidredundant-array-of-independent-disks)

<!-- Created by https://github.com/ekalinin/github-markdown-toc -->
<!-- Added by: sungminyou, at: 2022년 6월 21일 화요일 16시 38분 33초 KST -->

<!--te-->

강의 들어보니 대부분의 설명이 하드디스크 기준이고 요즘에는 SSD를 주로 사용하니 고전 기술이기는하나 그냥 발전 과정에 대해서 살펴보면 될거 같다.

## 1. Disk Structure

- logical block
  - 디스크의 외부에서 보는 디스크의 단위 정보 저장 공간들
  - 주소를 가진 1차원 배열처럼 취급, 배열의 원소를 요청
  - 정보를 전송하는 최소 단위
- sector
  - logical block이 물리적인 디스크에 매핑된 위치
  - sector 0는 최외곽 실린더의 첫 트랙이 있는 첫 번째 섹터이다, 부팅 관련 정보 저장

## 2. Disk Management

- physical formatting(low level formatting)
  - 디스크를 컨트롤러가 읽고 쓸 수 있도록 섹터들로 나누는 과정, 보통 공장에서 작업돼서 나옴
  - 각 섹터는 header + 실제 data(보통 512B) + trailer로 구성
  - header와 trailer는 sector number, ECC(Error Correcting Code, 제대로 저장됬는지 확인, 저장된 데이트의 축약로 코드 대조하여 잘못되었는지 확인, 네트워크에서 오류 교정과 같은 원리임, 얼마나 축약해놓느냐에 따라 오류 교정까지도 가능) 등의 정보가 저장되며 controller가 직접 접근 및 운영
- partitioning
  - 디스크를 하나 이상의 실린더 그룹으로 나누는 과정
  - OS는 이것을 독립적 disk로 취급(logical disk)
  - 각 파티션을 파일 시스템 용도나 swap area 용도로 활용할 수 있음
- local formatting
  - 파일 시스템을 만드는 과정
  - FAT, inode, free space 등의 구조 포함
- Booting의 간략 과정
  - ROM도 일종의 메모리로 CPU가 접근할 수 있다
  - ROM에 있는 small bootstrap loader실행, 전원을 키면 CPU가 무조건 ROM에 있는 특정 주소에 접근하여 저장된 instruction을 실행하게 되있음, sector 0을 메모리에 올리고 실행하게끔 구현
  - sector 0(boot block, 파일 시스템에서 운영체제 커널의 위치를 찾아 메모리에 로드)을 load하여 실행
  - sector 0는 full bootstrap loader program
  - OS를 디스크에서 load하여 실행

## 3. Disk Scheduling

![1](https://user-images.githubusercontent.com/48282185/174742086-30e61770-f6af-43b8-8721-175bd39e57bb.png)

- seek time이 제일 오래걸리는 시간, 대부분의 시간이 여기서 소요
- rotational time은 seek time의 1 / 10 정도
- 하나의 seek로 transfer를 대량으로 하는 것이 효율적이었다

## 4. Disk Scheduling Algorithm

- 엘리베이터 알고리즘을 생각해보자
- 큐에 다음과 같이 실린더 위치의 요청이 존재하는 경우 디스크 헤드 53번에서 시작한 각 알고리즘의 수행 결과는?(실린더의 위치는 0 ~ 199)

  - 요청 순서
    - 98, 183, 37, 122, 14, 124, 65, 67
  - FCFS

    ![2](https://user-images.githubusercontent.com/48282185/174742082-7947a27f-82ca-4ca0-abe5-ec5e5c3b7d45.png)

    - 들어온 순서대로 처리

  - SSTF(Shortest Seek Time First)

    ![3](https://user-images.githubusercontent.com/48282185/174742079-70e7eee4-450f-4045-acdc-8ebed2bb2464.png)

    - 현재 헤더의 위치에서 큐 내의 있는 요청들 중 가장 가까운 요청 순서로 처리

  - SCAN

    ![4](https://user-images.githubusercontent.com/48282185/174742069-d69b1646-9658-4178-9603-83fa271aea31.png)

    - 엘레베이터 스케줄링이라고도 함
    - disk arm이 디스크의 한쪽 끝에서 다른쪽 끝으로 이동하며 가는 길목에 있는 모든 요청을 처리
    - 다른 한쪽 끝에 도달하며 역방향으로 이동하며 오는 길목에 있는 모든 요청을 처리하며 다시 반대쪽 끝으로 이동한다
    - 공평하면서도 비교적 헤드의 이동거리에 대해서도 효율적인 특징을 갖는다
    - 문제점
      - 실린더 위치에 따라 대기 시간이 다르다, 트랙의 중앙위치와 양 끝 위치의 대기 시간이 다른데, 중앙에 위치한 데이터는 실린더가 양 끝 중 하나를 방문하고 다시 돌아오기에 트랙의 길이의 반 바퀴 왕복을 훑는 시간을 기다리면 되지만 서로 양 끝에 있는 데이터는 반대편 끝을 방문하고 오기에 한 바퀴 왕복을 훑는 시간을 기다려야함

  - C-SCAN

    ![5](https://user-images.githubusercontent.com/48282185/174742064-f2bf32b8-5d4b-42cd-8548-373ab40d95e6.png)

    - 헤드가 한쪽 끝에서 다른쪽 끝으로 이동하며 가는 길목에 있는 모든 요청을 처리
    - 다른쪽 끝에 도달했으면 요청을 처리하지 않고 곧바로 출발점으로 다시 이동
    - SCAN보다 균일한 대기 시간을 제공, 대신 헤드의 이동거리는 다소 길어질 수 있다

  - N-SCAN
    - SCAN의 변형 알고리즘
    - 일단 arm이 한 반향으로 움직이기 시작하면 그 시점 이후에 도착한 job은 되돌아 올 때 service, SCAN은 arm이 움직인 이후에 들어온 job도 움직이며 service한다, 대기시간의 편차를 줄일 수 있다
  - LOOK / C-LOOK

    ![6](https://user-images.githubusercontent.com/48282185/174742060-71d9c0f2-a093-4e9b-90f8-d263ba4d4506.png)

  - SCAN이나 C-SCAN은 헤드가 디스크 끝에서 끝으로 이동, 요청의 여부와 상관없이 이동방향을 틀지 않음 → 비효율적
  - LOOK과 C-LOOK은 헤드가 진행 중이다가 그 방향에 더 이상 기다리는 요청이 없으면 헤드의 이동방향을 즉시 반대로 이동한다

## 5. Disk Scheduling Algorithm의 결정

- SCAN, C-SCAN 및 그 응용 알고리즘인 LOOK, C-LOOK 등이 일반적으로 디스크 입출력이 많은 시스템에서 효율적인 것으로 알려져있음, 현대 시스템에서 사용되는 방식은 여러 요청을 묶어서 한꺼번에 처리한다
- file의 할당 방법에 따라 디스크 요청이 영향을 받음
  - 예시로 연속할당, 실린더 위치 줄일 수 있음
- 디스크 스케줄링 알고리즘은 필요할 경우 다른 알고리즘으로 쉽게 교체할 수 있도록 OS와 별도의 모듈로 작성되는 것이 바람직하다

## 6. Swap Area Management

![7](https://user-images.githubusercontent.com/48282185/174742040-590856b2-3a11-4b33-bc86-37bc2baea750.png)

- disk를 사용하는 두 가지 이유
  - memory의 휘발성(volatile) → file system
  - 프로그램 실행을 위한 memory의 공간 부족 → swap area
- swap area
  - virtual memory system에서는 디스크를 memory의 연장 공간으로 사용
  - 파일 시스템 내부에 둘 수도 있으나 별도 partition(logical disk)사용이 일반적
    - 공간효율성보다는 속도효율성 우선(금방 사라지고 저장될 필요가 없기 때문)
    - 일반 파일보다 휠씬 짧은 시간만 존재하고 자주 참조됨
    - 따라서, block의 크기 및 저장 방식이 일반적인 파일 시스템과 다름

## 7. RAID(Redundant Array of Independent Disks)

- 여러 개의 디스크를 묶어서 사용하여 중복 저장, 분산 저장을 위해 사용되는 기술, 여러개의 버전이 있음
- 사용 목적
  - 디스크 처리 속도 향상
    - 여러 디스크에 block의 내용을 분산 저장
    - 병렬적으로 읽어옴(interleaving, striping)
  - 신뢰성 향상
    - 동일 정보를 여러 디스크에 중복 저장
    - 하나의 디스크가 고장나더더라도 다른 디스크에서 읽어올 수 있음(mirroring, shadowing)
    - 단순한 중복 저장이 아니라 일부 디스크에 parity를 저장하여(오류 복구를 위해 존재하는 ECC) 단순히 하나의 디스크를 다른 디스크에 통째로 복사하는 방식보다 공간 효율성을 높일 수 있다

> 출처

> ABRAHAM SILBERSCHATZ ET AL., OPERATING SYSTEM CONCEPTS, NINTH EDITION, WILEY, 2013

> 반효경, 운영체제와 정보기술의 원리, 이화여자대학교 출판부, 2008
