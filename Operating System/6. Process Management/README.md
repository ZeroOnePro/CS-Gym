# 7. Process Management

[1. 프로세스 생성](#1-프로세스-생성process-creation)

[2. 프로세스 종료](#2-프로세스-종료process-termination)

[3. fork()](#3-fork-시스템-콜)

[4. exec()](#4-exec-시스템-콜)

[5. wait()](#5-wait-시스템-콜)

[6. exit()](#6-exit-시스템-콜)

[7. 프로세스 간 협력](#7-프로세스-간-협력)

## 1. 프로세스 생성(Process Creation)

- 부모 프로세스(Parent Process)가 자식 프로세스(Children Process)생성
- 프로세스의 트리(계층 구조)형성
- 프로세스는 자원을 필요로 함
  - 운영체제로부터 생성시점에 새로 할당받는다.
  - 부모와 자원을 공유한다.
- 자원의 공유
  - 부모와 자식이 모든 자원을 공유하는 모델
  - 일부를 공유하는 모델
  - 전혀 공유하지 않는 모델
  - 원칙적으로는 공유하지 않음
  - 같은 내용이기때문에 새로 복제하는 것보다는 부모와 자식이 자원을 공유하다가 부모와 자식이 다르게 수행되는 지점이 생긴다면 그 때 부모의 것을 복제한다.(Copy On Write, COW, 쓸 때 복제한다), 리눅스도 이렇게 구현되있음.
- 수행(Execution)
  - 부모와 자식은 공존하며 수행되는 모델
  - 자식이 종료(terminate)될 때까지 부모가 기다리는(wait) 모델
- 주소 공간
  - 자식은 부모의 context를 복제함
  - 자식은 그 공간에 새로온 프로그램을 올린다
- UNIX에서의 예시
  - 시스템 콜을 쓴다는 거부터가 운영체제를 통해서만 가능하다는 것을 의미
  - fork() 시스템 콜을 통해 새로운 프로세스를 생성한다.
    - 부모를 그대로 복사한다.(PID를 제외한 모든 OS 데이터)
    - 주소공간 할당
      [linux/fork.c at 34f4335c16a5f4bb7da6c8d2d5e780b6a163846a · torvalds/linux](https://github.com/torvalds/linux/blob/34f4335c16a5f4bb7da6c8d2d5e780b6a163846a/kernel/fork.c#L1975)
    - 리눅스에서 fork 구현체 일부 프로세스 복제 과정
  - fork 다음에 이어지는 exec() 시스템 콜을 통해 새로운 프로그램을 메모리에 올린다.

## 2. 프로세스 종료(Process Termination)

- 프로세스가 마지막 명령을 수행한 후 운영체제에게 이를 알려줌(exit() 시스템 콜)
  - 명시적 호출을 하지 않더라도 컴파일러가 exit()를 넣어 프로세스를 종료시킨다.
  - 자식이 부모에게 wait() 시스템콜을 사용하여 output data를 보낸다.
  - 프로세스의 각종 자원들이 운영체제에게 반납된다.
- 부모 프로세스가 자식의 수행을 종료시킴(abort() 시스템 콜)
  - 자식이 할당 자원의 한계치를 넘어설 때 종료시킨다.
  - 자식에게 할당된 태스크가 더 이상 필요하지 않을 때 종료시킨다.
  - 부모가 종료하는 경우
    - 운영체제는 부모 프로세스가 종료하는 경우 자식이 더 이상 수행되도록 두지 않는다.
    - 단계적으로 종료, 프로세스 트리 중간에 어떤 노드가 삭제된다고 했을 때 그 노드의 하위 트리가 모두 제거된다.

## 3. fork() 시스템 콜

```c
#include <unistd.h>
#include <stdio.h>

int main(void){
 pid_t pid;
 pid = fork();
 if(pid == 0) printf("\nHello, I am child!\n");
 else if(pid > 0) printf("\nHello, I am Parent\n");
 return 0;
}
```

- man page 참조해보니 pid, 부모 pid, 부모의 descriptors(파일 디스크립터 같은 것들), resource utilizations가 모두 0으로 설정 되는 거 외에는 부모 프로세스와 동일하다.
- fork()가 호출된 시점에서 부모의 context가 복제되기에 자식의 program counter는 if문을 가리킬 것이다. 그래서 main함수부터 실행하는 것이 아니기에 무한 fork는 일어나지 않는다.
- fork가 성공적으로 수행되면, 부모는 자식에게 pid로 0을 반환하고, 자식은 자신의 프로세스 id를 부모에게 반환한다.

## 4. exec() 시스템 콜

![1](https://user-images.githubusercontent.com/48282185/172799070-0fc41eb1-e233-4128-ba05-36cad68652ac.jpeg)

- l, v
  - argv인자를 넘겨줄 때 사용한다. (l일 경우는 char _로 하나씩 v일 경우에는 char_[]로 배열로 한번에 넘겨줌)
- e
  - 환경변수를 넘겨줄 때 사용한다. (e는 위에서 v와 같이 char \*[]로 배열로 넘겨줌)
- p
  - p가 있는 경우에는 환경변수 PATH를 참조하기 때문에 절대경로를 입력하지 않아도 된다.

```c
#include <unistd.h>
#include <stdio.h>

int main(void){
 pid_t pid;
 pid = fork();
 char *some_args[] = {"hello", "world", NULL};
 if(pid == 0) {
    // bin에 있는 명령어들의 경우 이미 환경변수로 등록되있으므로 p계열을 쓰면 됨
    // execlp("/bin/date", "/bin/date", (char *)0);

    // 인자를 char *[]로 배열로 한번에 넘겨줌
    execv("./sample", some_args);
  }
 else if(pid > 0) {
    printf("\npid: %d\n", pid);
  }
 return 0;
}
```

```c
/* sample.c */

#include <stdio.h>

int main(int argc, char *argv[]){
  int num_of_args = 0;
  while(argv[num_of_args] != NULL){
    printf("%s\n", argv[num_of_args]);
    num_of_args += 1;
  }
  return 0;
}
```

- 굳이 자식을 만들지 않고도 exec계열을 쓸 수 있으나 굳이 그럴필요가 있을까? exec 이후로는 다 짤림

## 5. wait() 시스템 콜

![2](https://user-images.githubusercontent.com/48282185/172799095-d0c399b5-653a-4b80-8053-9cc6affb38e3.png)

- 부모 프로세스가 wait()시스템 콜을 호출하면
  - 커널은 자식 프로세스가 종료될 때까지 부모 프로세스를 sleep시킨다(waiting 상태)
  - 자식 프로세스가 종료되면 커널은 부모 프로세스를 깨운다(ready 상태)
  - shell이 대표적으로 wait를 사용하는 예시 명령어 종료 전까지 waiting 상태

## 6. exit() 시스템 콜

- 프로세스의 종료
  - 자발적 종료
    - 마지막 statement 수행 후 exit() 시스템 콜을 통해 종료
    - 프로그램에 명시적으로 적어주지 않아도 main함수가 리턴되는 위치에 컴파일러가 exit를 삽입
  - 비자발적 종료
    - 부모 프로세스가 자식 프로세스를 강제 종료시킴
      - 자식 프로세스가 한계치를 넘어서는 자원 요청
      - 자식에게 할당된 태스크가 더 이상 필요하지 않음
    - 키보드로 kill, break 등을 입력한 경우
    - 부모가 종료하는 경우
      - 부모 프로세스가 종료하기 전에 자식들이 먼저 종료됨

## 7. 프로세스 간 협력

- 독립적 프로세스(independent process)
  - 프로세스는 각자의 주소 공간을 가지고 수행되므로 원칙적으로 하나의 프로세스는 다른 프로세스의 수행에 영향을 미치지 못함
  - 부모와 자식 프로세스는 예외
- 협력 프로세스(cooperating process)
  - 프로세스 협력 메커니즘을 통해 하나의 프로세스가 다른 프로세스의 수행에 영향을 미칠 수 있음
- 프로세스 간 협력 메커니즘(IPC: interprocess communication)
  - 프로세스 간 정보를 주고받는 방법
  - 커널을 통한 메시지 전달(message passing)
    - 프로세스가 직접 메시지를 전달하는 방법은 없음
    - 프로세스 사이에 공유 변수를 일체 사용하지 않고 통신하는 시스템
    - direct communication
      - 통신하려는 프로세스의 이름을 명시적으로 표시
    - indirect communication
      - mailbox(또는 port)를 통해 메시지 간접 전달
      - 누가 받을지 명시하지 않아 꺼내볼 수 있는 대상이 정해져있지 않다(어떤 프로세스 간에 작업 처리가 가능하다면 받을 수 있다)
  - 주소 공간을 공유하는 방법(shared memory)
    - 서로 다른 프로세스 간에도 일부 주소 공간을 공유하게 하는 shared memory 메커니즘이 있음
    - 주소 공간에 겹치는 영역이 있음
  - thread
    - thread는 사실상 하나의 프로세스이므로 프로세스 간 협력으로 보기는 어렵지만 동일한 process를 구성하는 thread들 간에는 주소 공간을 공유하므로 협력이 가능

> 출처

> ABRAHAM SILBERSCHATZ ET AL., OPERATING SYSTEM CONCEPTS, NINTH EDITION, WILEY, 2013

> 반효경, 운영체제와 정보기술의 원리, 이화여자대학교 출판부, 2008
