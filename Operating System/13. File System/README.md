# 13. File System

<!--ts-->

- [13. File System](#13-file-system)
  - [1. File and File System](#1-file-and-file-system)
  - [2. Directory and Logical Disk](#2-directory-and-logical-disk)
  - [3. open()](#3-open)
  - [4. open() example](#4-open-example)
  - [5. file protection](#5-file-protection)
  - [6. File System의 Mounting](#6-file-system의-mounting)
  - [7. Access Method](#7-access-method)
  - [8. Allocation of File Data in Disk](#8-allocation-of-file-data-in-disk)
  - [9. Contiguous Allocation](#9-contiguous-allocation)
  - [10. Linked Allocation](#10-linked-allocation)
  - [11. Indexed Allocation](#11-indexed-allocation)
  - [12. UNIX의 파일시스템 구조](#12-unix의-파일시스템-구조)
  - [13. FAT File System](#13-fat-file-system)
  - [14. Free-Space Management](#14-free-space-management)
  - [15. Directory Implementation](#15-directory-implementation)
  - [16. VFS and NFS](#16-vfs-and-nfs)
  - [17. Page Cache and Buffer Cache](#17-page-cache-and-buffer-cache)
  - [18. Unified Buffer Cache를 사용한 방식과 그렇지 않은 방식에서 File IO 과정](#18-unified-buffer-cache를-사용한-방식과-그렇지-않은-방식에서-file-io-과정)
  - [19. file을 memory에 map하여 쓰는 대표적 예시](#19-file을-memory에-map하여-쓰는-대표적-예시)
  - [20. mmap의 장단점](#20-mmap의-장단점)
  - [21. Ext2 파일시스템(Linux, 1993)](#21-ext2-파일시스템linux-1993)
  - [22. Ext4](#22-ext4)

<!-- Created by https://github.com/ekalinin/github-markdown-toc -->
<!-- Added by: sungminyou, at: 2022년 6월 21일 화요일 16시 29분 27초 KST -->

<!--te-->

## 1. File and File System

- File
  - A named collection of related information
  - 메모리는 주소로 접근, 파일은 이름으로 접근
  - 일반적으로 비휘발성의 보조기억장치에 저장
  - 운영체제는 다양한 저장 장치를 file이라는 동일한 논리적 단위로 볼 수 있게 해줌(저장장치도 파일 중 일부)
  - operation
    - create, read, write, reposition(lseek), delete, open(파일의 메타데이터를 메모리로 올리는 작업), close 등
- file attribute(file metadata)
  - 파일 자체의 내용이 아니라 파일을 관리하기 위한 각종 정보들
    - 파일 이름, 유형, 저장된 위치, 파일 사이즈
    - 접근 권한(읽기 / 쓰기 / 실행), 시간(생성 / 변경 / 사용), 소유자 등
- File system
  - 운영체제에서 파일을 관리하는 부분
  - 파일 및 파일의 메타데이터, 디렉토리 정보 등을 관리
  - 파일의 저장 방법 결정
  - 파일 보호 등

## 2. Directory and Logical Disk

- Directory
  - 파일의 메타데이터 중 일부를 보관하고 있는 일종의 특별한 파일
  - 그 디렉토리에 속한 파일 이름 및 파일 attribute들
  - operation
    - search for a file, create a file, delete a file
    - list a directory, rename a file, traverse the file system
- partition(=Logical Disk)
  - 운영체제가 바라보는 디스크, 논리적 디스크
  - 하나의 물리적 디스크 안에 여러 파티션을 두는게 일반적
  - 여러 개의 물리적인 디스크를 하나의 파티션으로 구성하기도 함
  - 물리적 디스크를 파티션으로 구성한 뒤 각각의 파티션에 file system을 깔거나 swapping 등 다양한 용도로 사용할 수 있음

## 3. open()

![1](https://user-images.githubusercontent.com/48282185/174710431-ebcfe5bd-8e7a-4604-a60b-f99e0dd0cd46.png)

![2](https://user-images.githubusercontent.com/48282185/174710429-ce9c1a7b-97a8-49b6-a7b2-cf3983bc83c6.png)

- process A가 open 시스템 콜을 통해 b파일을 요청
- 커널 메모리 영역에는 각 프로세스들이 사용 중인 파일들의 목록이 open file table이라는 system-wide한 자료구조로 관리되고 있음
- 루트 디렉토리는 운영체제가 알고있으므로 root directory부터 순차적으로 요청 파일의 메타데이터를 획득하기 까지 메타데이터를 얻으면서 찾아감
- PCB에는 프로세스가 요청한 파일의 메타데이터 자료구조에 대한 포인터를 정의한 배열형태의 자료구조(per-process file descriptor table)가 존재, 다시 탐색할 필요없게 저장하는 용도
- 그림에는 안나와있지만 같은 파일을 서로 다른 프로세스가 참조하는 경우 각 프로세스가 참조하고 있는 파일의 포인터는 제각각일 것이다. 때문에 각 프로세스가 파일의 어디를 참조하는지 운영체제가 알고있어야하기에 각 프로세스별로 offset정보를 보관하는 테이블을 하나 더 만들어놓는 것이 일반적이다, 구현하기 나름이라 각 프로세스가 가지고 있을 수도 있겠다
- read()를 하게되면, open()으로 얻은 파일 디스크립터를 인자로 넘겨주는데, 이 인자를 이용하여 open file table에 저장되있는 메타데이터를 가지고 해당 파일을 찾아 파일 내용을 읽어온다.(메모리에 파일 내용이 없을 경우)
- 읽고 난 뒤 곧바로 프로세스에 copy되는 것이 아니라 운영체제는 자신의 메모리 영역에 이 데이터를 저장해두고 프로세스에 전달하게되는데, 운영체제가 이 데이터를 저장하는 이유는 후에 이 데이터가 또 쓰일 것을 고려하여 캐싱해두려는 것이다. 이것이 buffer caching이다.
- buffer caching환경에서는 모든 정보를 운영체제가 관리하기에 LRU, LFU 알고리즘을 사용할 수 있다.

## 4. open() example

- open(’a/b/c’)
  - 결과 → 디스크로부터 파일 c의 메타데이터를 메모리로 가지고온다
  - 이를 위하여 directory path를 traverse
    - 루트 디렉토리 ‘/’을 open하고 그 안에서 파일 a의 위치 획득
    - 파일 a를 open한 후 read하여 그 안에서 파일 b의 위치 획득
    - 파일 b를 open한 후 read하여 그 안에서 파일 c의 위치 획득
    - 파일 c를 open한다
  - directory path의 traverse에 너무 많은이 시간 소요되기에
    - open을 read / write와 별도로 둔다
    - 한 번 open한 파일은 read / write 시 directory traverse 불필요
  - open file table
    - 현재 open된 파일들의 메타데이터 보관소(in memory)
    - 디스크의 메타데이터를 포함하여 몇 가지 정보가 추가
      - open한 프로세스 수
      - file offset, 파일 어느 위치에 접근 중인지 표시(별도 테이블 필요, 프로세스마다 다르기 때문)
  - file descriptor(file handler, file control block)
    - open file table 내에 있는 open한 파일의 위치 정보(프로세스 별)

## 5. file protection

- 각 파일에 대해 누구에게 어떤 유형의 접근(read / write / execution)을 허락할 것인가?
- access control 방법

  - access control matrix
    ![3](https://user-images.githubusercontent.com/48282185/174710428-0fa4b023-bedb-44db-8012-afb7b4a86be9.png)
    - 각 사용자가 각 파일에 대한 권한을 행렬로 저장
    - 행렬자체가 sparse matrix가 될 것임, 낭비 심함
    - 낭비를 막기위해 linked list형태를 자료구조로 접근 권한을 관리할 수 있는 방법을 생각해 볼 수 있다
      - access control list
        - 파일이 주체가 되어 각 접근 권한이 있는 user들을 리스트로 관리
      - capability
        - user가 주체가 되어 접근 권한이 있는 파일들을 리스트로 관리
    - 셋 중 어떤 방법이든 overhead가 크다
  - grouping

    - 일반적으로 사용되는 방법
    - 전체 user를 owner, group, public의 세 그룹으로 구분
    - 각 파일에 대해 세 그룹의 접근 권한(rwx)를 3비트씩으로 표시, 권한을 위해 9비트만 필요
    - UNIX에서의 예시

      ![4](https://user-images.githubusercontent.com/48282185/174710426-def2ab70-44ac-4bb1-9b5a-13467ee57163.png)

  - password
    - 파일마다 password를 두는 방법(디렉토리 파일에 두는 방법도 가능)
    - 모든 접근 권한에 대해 하나의 pasword: all or nothing, 모두 가능하거나 모두 불가능하거나
    - 접근 권할별 password, 암기 문제, 관리 문제가 뒤따름

## 6. File System의 Mounting

![5](https://user-images.githubusercontent.com/48282185/174710425-bf424796-4384-465c-96f6-ea5427c16141.png)

- 하나의 논리 디스크 내의 파일시스템에서 다른 논리 디스크의 파일 시스템의 정보가 필요할 때 두 파일 시스템을 연결하는 방법

## 7. Access Method

- 시스템이 제공하는 파일 정보의 접근 방식
  - 순차 접근(sequential access)
    - 카세트 테이프를 사용하는 방식처럼 접근, 이전 정보를 재참조하기 위해 되감기 필요
    - 읽거나 쓰면 offset은 자동적으로 증가
  - 직접(임의) 접근(direct access, random access)
    - LP 레코드 판과 같이 접근하도록 함
    - 파일을 구성하는 레코드를 임의의 순서로 접근할 수 있음
    - 임의 접근이 가능한 매체일지라도 관리법에 따라 순차접근으로 구현될 수 있다

## 8. Allocation of File Data in Disk

- contiguous allcoation
- linked allocation
- indexed allocation
- 이론적인 방법
- sector라는 동일한 사이즈의 단위로 분할하여 관리됨
  - paging과 유시한 기법

## 9. Contiguous Allocation

![6](https://user-images.githubusercontent.com/48282185/174710423-51cb9d90-8551-4b1a-b559-4c254ae00d82.png)

- directory는 하위 파일들의 정보를 가진다
- 단점
  - 외부조각이 생길 수 있다
  - file grow가 어려움
    - file은 크기가 동적으로 변할 수 있음
    - file 생성 시 얼마나 큰 hole을 할당할 것인가?
    - grow 가능(커지더라도 할당한 영역 내에서만) vs 낭비(내부조각)
- 장점
  - fast IO
    - 한 번의 seek/rotation으로 많은 byte transfer
    - realtime file용으로, 또는 이미 run 중이던 process의 swapping 용
  - direct access(=random access) 가능
    - 어느 sector든 바로 접근 가능

## 10. Linked Allocation

![7](https://user-images.githubusercontent.com/48282185/174710420-f76e9e09-1dd0-4441-9be4-3691b5075806.png)

- 비어있는 sector면 어디던지 들어갈 수 있음
- 장점
  - 외부조각이 발생하지 않음
- 단점
  - 직접 접근 불가능, 파일의 중간 위치를 보려면 시작 위치부터 차례대로 탐색해야함, 즉 건너 뛰는게 불가능
  - 디스크는 직접 접근이 가능한 매체이지만 linked allocation관리법을 사용하기에 순차접근으로 파일을 읽어들이게됨
  - reliablility 문제
    - 파일 중 중간의 한 sector가 고장나 pointer가 유실되면 많은 부분을 잃어버린다
  - pointer를 위한 공간이 block의 일부가 되어 공간 효율성이 감소한다
    - 512 bytes/sector, 4 bytes/pointer
    - 이를테면 4byte를 제외한 파일의 전체 내용이 모두 섹터로 분리되고 4byte남아 채울 수 있었는데, 포인터로 인해 하나의 섹터를 더 만들어줘야하는 상황이 올 수도 있다
- 변형
  - file-allocation table(FAT)파일 시스템
    - 포인터를 별도의 위치에 보관하여 reliability와 공간효율성 문제 해결

## 11. Indexed Allocation

![8](https://user-images.githubusercontent.com/48282185/174710418-a0ad5e33-d440-4fa8-b30d-62991c9220ea.png)

- 각 파일별로 Index block을 두어 내용이 어디어디 위치해 있는지 저장해 놓는 방식
- 장점
  - 외부조각이 생기지 않음
  - direct access 가능
- 단점
  - small file의 경우 공간 낭비(실제로 많은 file들이 small), 작은 파일일지라도 두 개의 block을 필요로함
  - Too large file의 경우 하나의 block으로 Index를 저장하기에 부족
    - 해결 방안으로 linked scheme(파일의 마지막 block에 다음 index block의 위치를 표시), multi-level index가 있다

## 12. UNIX의 파일시스템 구조

![9](https://user-images.githubusercontent.com/48282185/174710416-6ed9b9dc-dc54-40a5-90be-82e02610dba1.png)

- 가장 기본적인 파일 시스템, 이로 부터 발전해왔다
- boot block
  - 어떤 파일 시스템이건 boot block은 존재
  - 부팅 시 필요한 정보(bootstrap loader)
  - 운영체제 커널의 위치를 메모리에 올려서 정상적인 부팅을 할 수 있게하는 역할
- super block
  - 파일 시스템에 관한 총체적인 정보를 담고있다
  - 어디가 빈 블록이고 어디가 사용 중인 블록인지 관리, inode list와 data block 구분 등
- i(index)node
  - 파일 이름을 제외한 파일의 모든 메타 데이터를 저장
  - 파일 하나 당 inode 하나씩 할당
  - inode의 크기는 고정
  - directory에는 파일들의 메타데이터가 극히 일부만 존재하고 대부분은 inode에 있음
    - directory는 file의 이름과 inode번호를 가지고 있다
  - indexed allocation 방식을 거의 그대로 사용
    - direct blocks
      - 크기가 작은 파일의 경우 인덱스없이 바로 참조가능
    - single indirect
      - direct block으로 커버가 안될 경우 index block을 두어 더 큰 파일을 표현
    - double indirect
      - single indirect로 커버가 안될 경우 index block을 두 단계로 구성하여 더 큰 파일을 표현
    - triple indirect
      - double indirect로 커버가 안될 경우 index block을 세 단계로 구성하여 더 큰 파일을 표현
      - 이 방식으로 얼만큼의 크기 커버 가능할까?
  - 이 방식이 효율적일까?
    - 대부분의 파일은 크기가 작음, 작은 파일의 경우 바로 파일의 내용을 참조할 수 있다
    - 큰 파일도 커버 가능
- data block
  - 파일의 실제 내용을 보관

## 13. FAT File System

![10](https://user-images.githubusercontent.com/48282185/174710413-5f3d87c7-26da-40e2-9caf-2c43779bafd6.png)

- MS DOS 시스템에서 처음 등장, 최근에도 Windows 계열에서 일부 사용, 모바일 계열에서도 일부 사용
- 파일의 메타 데이터 중 극히 일부를 FAT에 보관
  - 파일의 위치 정보
  - 이외의 메타데이터들은 directory가 갖도록한다, 시작위치도 포함
  - bad sector문제를 FAT을 이용하여 해결, 파일의 모든 Link정보를 저장해놓음
  - FAT이 손상될 것을 대비해 두 개 이상의 복제본을 저장해놓는다
- 직접 접근이 가능
  - FAT을 탐색해보면 바로 참조할 수 있음, 디스크 헤더를 옮길필요가 없음

## 14. Free-Space Management

- 빈 블록을 관리하는 방법
- Bit map or bit vector

  ![11](https://user-images.githubusercontent.com/48282185/174710412-00727f7b-72e5-4aca-8034-f5fb355fd2cb.png)

  - block의 번호와 비트를 이용하여 비어있는지 아닌지를 나타냄

- linked list

  ![12](https://user-images.githubusercontent.com/48282185/174710411-7d3c7762-a7d6-4040-832e-17ae8f39c2b6.png)

  - 모든 free block들을 연결
  - 연속적인 가용공간을 찾는 것은 쉽지 않다
  - 공간의 낭비가 없다(따로 리스트를 두지 않고 각 블록안에 포인터가 위치)

- grouping
  - linked list 방법의 변형
  - 첫 번째 free block이 n개의 pointer를 가짐
    - 1 ~ n-1 pointer는 free data block을 가리킴
    - 마지막 pointer가 가리키는 block은 또 다시 n개의 pointer를 가짐
    - 빈 블록을 한꺼번에 찾기에는 쉽지만 연속적인 빈 블록을 찾기는 여전히 어렵다
- counting
  - 프로그램들이 종종 여러 개의 연속적인 block을 할당하고 반납한다는 성질에 착안
  - first free block, # of contiguous free blocks을 쌍으로 유지, 빈 블록의 시작위치와 그 이후로 몇 개가 연속적으로 비어있는지 저장

## 15. Directory Implementation

![13](https://user-images.githubusercontent.com/48282185/174710410-a4f5c2c8-f9e7-4f0b-9415-82b3b20a5cb5.png)

- linear list
  - <file name, file metadata>의 리스트
  - 각 entry 내에서 각 정보의 크기가 고정되있어 파일에 대한 정보를 얻기 위한 연산을 할 때 구현이 쉬워진다
  - 구현이 간단
  - 디렉토리 내에 파일이 있는지 찾기 위해서는 linear search가 필요
- hash table
  - linear list + hasing
  - hash table은 file name을 이 파일의 linear list의 위치로 바꿔줌, h(file name) = file linear list pointer
  - search time을 없앰
  - collision 발생 가능
- file의 metadata의 보관 위치
  - 디렉토리 내에 직접 보관
  - 디렉토리에는 포인터를 두고 다른 곳에 보관
    - inode, FAT 등
- long file name의 지원
  ![14](https://user-images.githubusercontent.com/48282185/174710409-44f3de75-2500-4e76-9fd2-a487fe66503b.png)Untitled.png)
  - <file name, file metadata>의 list에서 각 entry는 일반적으로 크기가 고정
  - file name이 고정 크기의 entry길이보다 길어지는 경우 entry의 마지막 부분에 이름의 뒷부분이 위치한 포인터를 두는 방법
  - 이름의 나머지 부분은 동일한 directory file의 일부에 존재

## 16. VFS and NFS

![15](https://user-images.githubusercontent.com/48282185/174710408-4aae7ffc-6102-4321-9818-9f3c42111cbf.png)

- virtual file system(VFS)
  - 서로 다른 다양한 file system에 대해 동일한 시스템 콜 인터페이스(API)를 통해 접근할 수 있게 해주는 OS의 layer
- network file system(NFS)
  - 분산 시스템에서는 네트워크를 통해 파일이 공유될 수 있음
  - NFS는 분산 환경에서의 대표적인 파일 공유 방법임, 다른 방법도 있음

## 17. Page Cache and Buffer Cache

- Page Cache
  - virtual memory의 paging system에서 사용하는 page frame을 caching의 관점에서 설명하는 용어(swap area는 디스크에 위치하므로 page frame은 메모리에 올라와있다는 점에서 캐시로 보는 관점)
  - memory-mapped IO를 쓰는 경우 file의 IO에서도 page cache사용
- Memory Mapped IO
  - read / write 시스템 콜을 이용하여 접근하는 방법 대신 사용할 수 있는 방법
  - file의 일부를 virtual memory(프로세스의 주소 공간)에 mapping 시킴
  - 매핑시킨 영역에 대한 메모리 접근 연산은 파일의 입출력을 수행하게 함
  - 메모리 접근 연산들을 이용하여 파일 입출력을 한다
    - mmap을 했더라도 파일의 내용이 메모리 상에 없다면 page fault, 가상 메모리와 같음, invalid bit가 설정되있겠지?
    - mmap을 한 이후 파일의 내용이 메모리 상에 있으면 OS의 관여가 없어짐
- Buffer cache
  - 파일 시스템을 통한 IO연산은 메모리의 특정 영역인 buffer cache 사용
  - File 사용의 locality 활용
    - 한 번 읽어온 block에 대한 후속 요청 시 buffer cache에서 즉시 전달
  - 모든 프로세스가 공용으로 사용
  - replacement algorithm 필요(LRU, LFU 등)
- Unified buffer cache
  - page cache + buffer cache
  - buffer cache도 page 단위로 관리를 하는 방법
  - OS의 page 관리 루틴에 buffer cache와 page cache가 동시에 관리됨, 서로 관리법이 다른 것이 아님
    - 예전에는 buffer cache의 경우 512B, page 4KB 이런 식으로 크기도 다르고 관리 방법도 다르고 했었지만 지금은 같은 취급을 받으며 관리됨
  - 최근의 OS에서는 기존의 buffer cache가 page cache에 통합됨
    - 리눅스에서도 사용되는 방식
    - File IO가 필요하면 메모리에 page frame을 할당하여 buffer cache로 활용
    - process address space의 일부를 page frame을 할당하여 page cache로 활용

## 18. Unified Buffer Cache를 사용한 방식과 그렇지 않은 방식에서 File IO 과정

![16](https://user-images.githubusercontent.com/48282185/174710405-4c7a1c06-0b20-4c07-a202-8e9e9cac8616.png)

## 19. file을 memory에 map하여 쓰는 대표적 예시

![17](https://user-images.githubusercontent.com/48282185/174710395-5a4bceac-55cb-436f-a478-c43030831c74.png)

- code 영역은 read-only이기에 메모리에서 쫓겨나는 상황이 오면 저장할 필요가 없기 때문에 swap area로 내려가지 않는다
- loader라는 프로세스가 프로그램을 메모리로 로드할 때 프로그램의 code부분을 memory mapped하여 올려놓는다 즉 code 영역은 실행 파일과 memory mapped되어 virtual memory의 일부를 차지하고 있다
- 데이터가 mmap되있는 상황도 마찬가지로 메모리에서 쫓겨날 때 swap area에 써주는 것이 아니라 실제 데이터 파일의 해당 위치에 변경 내용을 써줘야한다

## 20. mmap의 장단점

- 장점
  - 한 번 메모리에 올려놓으면 운영체제의 관여없이 작업을 수행할 수 있어서 시간이 덜 걸린다
  - 캐시한 내용을 주소 공간에 copy할 필요 없이 바로 가져다 쓸 수 있기에 overhead가 적다
- 단점
  - buffer cache가 여러 프로세스에 공유되고 있다면 일관성 문제가 발생될 수 있어서 주의해야함
  - read / write는 복제본을 전달하기에 일관성 문제가 없다

## 21. Ext2 파일시스템(Linux, 1993)

![1](https://user-images.githubusercontent.com/48282185/174741238-f1c7dd66-0dd5-44da-b86a-d58573a9f9ec.png)

- UNIX의 file system과 별 차이 없어보이나 조금 있다
- 특징: 전체 파일시스템을 그룹화해서 나눠놓았다
- 헤드의 이동이 빈번해지기에 메타 데이터와 실제 데이터를 가까이 위치시킬 수는 없을까?
- 블록의 그룹화

  ![2](https://user-images.githubusercontent.com/48282185/174741233-2eaf2258-83ad-4f98-b881-ffd844365241.png)

- 메타데이터와 실제 데이터를 인접하게 배치하여 디스크 탐색 시간 감소
- 슈퍼블록의 중복 저장
  - 슈퍼블록을 그룹마다 중복저장하여 디스크 오류에 대비
- 그룹 구성

  ![3](https://user-images.githubusercontent.com/48282185/174741210-5d53cefb-8251-4e68-a88f-da8894708220.png)

  - 슈퍼 블록(전체 파일시스템에 대한)
    - inode 수, 가용 inode 수, 데이터 블록 수, 가용 데이터 블록 수
    - 그룹 당 블록 수, 시간 정보
  - 그룹 디스크립터(그룹에 대한 슈퍼블록)
    - 데이터블록 비트맵의 시작위치, 아이노드 비트맵의 시작위치
    - 첫 번째 inode의 시작 주소, 가용 inode의 수
  - 데이터 블록 비트랩
    - 사용 중인 데이터블록과 빈 데이터블록의 표시
  - inode bitmap
    - 사용 중인 inode와 빈 inode 표시
  - Inode table
    - 실제 inode의 저장 위치

## 22. Ext4

- Ext4 = Ext2 + 저널링
- 갑작스런 전원 공급 중단은 파일 시스템의 일관성 훼손(inconsistency)을 발생시킨다
- inconsistency를 해결하기 위해 현대 파일시스템에서는 저널링을 지원한다
- 저널링(journaling)
  - Ext3, Ext4, ReiserFS, XFS, btrFS 등 에서 사용됨
  - 5 ~ 30초 단위로 버퍼캐시에서 수정된 내용을 저널 영역에 기록
  - 만약 저널영역에 쓰고있던(transaction) 도중 crash가 난다면, 다 써진 경우라면 원래 파일에 써주고, 아니라면 변경 이전의 내용이 사용하여 일관성 훼손을 해결
  - checkpointing
    - 수정된 내용을 파일시스템의 원래 위치에 반영(예시로 5분 주기)
- Ext4의 저널링(메타데이터 저널링 모드)
  - 메타데이터만 저널링(default)
    - 저널링 주기가 도래하면 데이터를 disk의 원래 위치에 저장한 후 메타데이터를 저널영역에 기록
    - 체크포인팅 주기가 도래하면 메타데이터를 파일시스템에 반영
    - 크래쉬 발생 시 파일시스템 자체가 깨어지는 것 방지(일부 데이터 훼손 가능)
  - 메타데이터와 일반데이터를 모두 저널링
    - 저널링 주기가 도래하면 데이터와 메타데이터를 저널영역에 기록
    - 체크포인팅 주기가 도래하면 데이터와 메타데이터를 파일시스템에 반영(disk의 원래 위치)
    - 크래쉬 발생 시 파일 시 데이터 자체의 복구 보장, 신뢰성이 높아야할 때 사용
    - 어떤 경우에도 일관성이 깨지지 않는다, 옛날 정보이거나 최신 정보이거나 둘 중하나 써지다 만 것은 없다

> 출처

> ABRAHAM SILBERSCHATZ ET AL., OPERATING SYSTEM CONCEPTS, NINTH EDITION, WILEY, 2013

> 반효경, 운영체제와 정보기술의 원리, 이화여자대학교 출판부, 2008
