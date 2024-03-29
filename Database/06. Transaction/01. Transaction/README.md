# 1. 트랜잭션

<!--ts-->

- [1. 트랜잭션](#1-트랜잭션)
  - [1. 트랜잭션(transaction)](#1-트랜잭션transaction)
  - [2. 예시](#2-예시)
  - [3. 트랜잭션의 특성(ACID 특성)](#3-트랜잭션의-특성acid-특성)
  - [4. ACID를 지원하기 위한 DBMS의 기능](#4-acid를-지원하기-위한-dbms의-기능)
  - [5. 트랜잭션의 상태](#5-트랜잭션의-상태)
  - [6. 동시성 제어](#6-동시성-제어)
  - [7. 로킹(locking)](#7-로킹locking)
  - [8. 2단계 로킹 프로토콜(2-phase locking protocol, 2PL)](#8-2단계-로킹-프로토콜2-phase-locking-protocol-2pl)
  - [9. 데드록(deadlock)](#9-데드록deadlock)
  - [10. 다중 로크 단위(multiple granularity)](#10-다중-로크-단위multiple-granularity)
  - [11. 팬텀 문제(phantom problem)](#11-팬텀-문제phantom-problem)
  - [12. 회복(recovery)](#12-회복recovery)

<!-- Created by https://github.com/ekalinin/github-markdown-toc -->
<!-- Added by: sungminyou, at: 2022년 9월  2일 금요일 11시 03분 40초 KST -->

<!--te-->

## 1. 트랜잭션(transaction)

- 항공기 예약, 은행, 신용 카드 처리, 대형 할인점 등에서는 대규모 데이터베이스를 수천 명 이상의 사용자들이 동시에 접근함
- 많은 사용자들이 동시에 데이터베이스의 서로 다른 부분 또는 동일한 부분을 접근하면서 데이터베이스를 사용함
- 동시성 제어(concurrency control)
  - 동시에 수행되는 트랜잭션들이 데이터베이스에 미치는 영향은 이들을 순차적으로 수행했을 때 데이터베이스에 미치는 영향과 같도록 보장
  - 다 수 사용자가 데이터베이스를 동시에 접근하도록 허용하면서 데이터베이스의 일관성을 유지
- 정보 시스템(하드웨어 + 애플리케이션 + DBMS의 집합)의 성능은 TPS(Transaction Per Second, 초 당 트랜잭션 처리 비율)로 평가한다
- 회복(recovery)
  - 데이터베이스를 갱신하는 도중에 시스템이 고장 나도 데이터베이스의 일관성을 유지
  - 어떤 갱신 쿼리 하나를 실행하다가 중간에 시스템이 다운됬을 때 DBMS에서 추가로 정보를 유지하지 않는다면 재가동 된 후에 어느 튜플 까지 갱신이 진행됐는지 알 수 없음 → 로그(log) 유지해야함
  - 트랜잭션으로 정의된 SQL문들의 집합은 모두 완전하게 수행되거나 하나도 수행되어서는 안되도록 해야한다 (all or nothing, 원자성)

## 2. 예시

- 고객 A가 자신의 계좌에서 돈을 인출해서 고객 B에게 이체하는 상황을 가정해보자
- 인출을 위한 UPDATE, 이체를 위한 UPDATE가 하나로 이뤄져야 정상적인 동작인데 만약 인출을 위한 UPDATE만 일어나고, 시스템이 다운돼버리면, A의 돈만 날라가는 상황이 발생한다
- 두 개의 UPDATE문은 둘 다 완전하게 수행되거나 한 UPDATE문도 수행되어서는 안되도록, 즉 하나의 트랜잭션(단위)처럼 DBMS가 보장해야함
- 기본적으로 각각의 SQL문이 하나의 트랜잭션으로 취급됨
- 두 개 이상의 SQL문들을 하나의 트랜잭션으로 취급하려면 사용자가 이를 명시적으로 표기해야 함
- 트랜잭션은 한 번에 처리되야하는 SQL문(들)의 집합 단위 → 원자성(atomicity)

## 3. 트랜잭션의 특성(ACID 특성)

- 원자성(atomicity)
  - 한 트랜잭션 내의 모든 연산들이 완전히 수행되거나 전혀 수행되지 않음(all or nothing)을 의미
  - DBMS의 리커버리 모듈은 시스템이 다운되는 경우에, 부분적으로 데이터베이스를 갱신한 트랜잭션의 영향을 취소함으로서 트랜잭션의 원자성을 보장함
  - 완료된 트랜잭션이 갱신한 사항은 트랜잭션의 영향을 재수행함(redo)으로서 트랜잭션의 원자성을 보장함 → commit된 트랜잭션이 갱신한 사항이 확인해보니 제대로 안되었음 → DBMS가 알아서 재수행한다
- 일관성(consistency)

  ![Untitled](https://user-images.githubusercontent.com/48282185/188043231-ef1307d2-71eb-45ad-9011-10a872bb6c06.png)

  - 어떤 트랜잭션이 수행되기 전에 데이터베이스가 일관된 상태를 가졌다면 트랜잭션이 수행된 후에 데이터베이스는 또 다른 일관된 상태를 갖는다
  - 트랜잭션이 수행되는 도중에는 데이터베이스가 일시적으로 일관된(불일치) 상태를 갖지 않을 수 있음

- 고립성(isolation)
  - 한 트랜잭션이 데이터를 갱신하는 동안 이 트랜잭션이 완료되기 전에는 갱신 중인 데이터를 다른 트랜잭션들이 접근하지 못하도록 해야 함 → 운영체제에서 공유 자원 lock
  - 다수의 트랜잭션들이 동시에 수행되더라도 그 결과는 어떤 순서에 따라 트랜잭션들을 하나씩 차례대로 수행한 결과와 같아야 함
  - DBMS의 동시성 제어 모듈이 트랜잭션의 고립성을 보장함
  - DBMS는 애플리케이션의 요구사항에 따라 다양한 고립 수준(isolation level)을 제공함
    - 레벨이 낮을수록 동시에 수행되는 트랜잭션들이 많아지지만 데이터 일관성이 비교적 낮고, 레벨이 높을수록 동시에 수행되는 트랜잭션들이 적어지는 대신 데이터의 일관성이 높아진다
- 지속성(Durability)
  - 한 번 갱신이 발생한 경우에는 갱신을 보장해줘야됨
  - 일단 한 트랜잭션이 완료되면(commit) 이 트랜잭션이 갱신한 것은 그 후에 시스템이 고장이 발생하더라도 손실되지 않음
  - 완료된 트랜잭션의 효과는 시스템이 고장난 경우에도 데이터베이스에 반영됨
  - DBMS의 리커버리 모듈은 시스템이 다운되는 경우에도 트랜잭션의 지속성을 보장함

## 4. ACID를 지원하기 위한 DBMS의 기능

![Untitled 1](https://user-images.githubusercontent.com/48282185/188043194-7653c062-7ba1-4ebd-ba3e-99423c6ec7af.png)

## 5. 트랜잭션의 상태

- 트랜잭션의 완료(commit)
  - 트랜잭션에서 변경하려는 내용이 데이터베이스에 완전하게 반영됨 → 지속성
  - SQL 구문상으로 COMMIT WORK
- 트랜잭션의 철회(abort)
  - 트랜잭션에서 변경하려는 내용이 데이터베이스에 일부만 반영된 경우에는 원자성을 보장하기 위해서, 트랜잭션이 갱신한 사항을 트랜잭션이 수행되기 전의 상태로 되돌림(undo, 했던 걸 취소, old value, new value 모두 log에 기록할 필요가 있음)
  - SQL 구문상으로 ROLLBACK WORK

## 6. 동시성 제어

- 대부분의 DBMS들은 다수 사용자용
- 여러 사용자들이 동시에 동일한 테이블을 접근하기도 함
- DBMS의 성능을 높이기 위해 여러 사용자의 질의나 프로그램들을 동시에 수행하는 것이 필수적
- 동시성 제어 기법은 여러 사용자들이 다수의 트랜잭션들을 동시에 수행하는 환경에서 부정확한 결과를 생성할 수 있는, 트랜잭션들 간의 간섭이 생기지 않도록 함
- 직렬 스케줄(serial schedule)
  - 여러 트랜잭션들의 집합을 한 번에 한 트랜잭션씩 차례대로 수행함, 뽑는 순서는 무관
  - 병렬처리가 아님
- 비직렬 스케줄(non-serial schedule)
  - 여러 트랜잭션들을 동시에 수행함
  - 동시에 수행됨에 따라 어떤 트랜잭션이 수행됐는지 순서를 알기 힘들기에 일관성 유지가 어렵다
- 직렬성 스케줄(serializable)
  - 비직렬 스케줄의 결과가 어떤 직렬 스케줄의 수행 결과와 동등함
  - ideal 스케줄
  - 어떤 트랜잭션이 언제 들어올지 모르고, 얼마나 걸릴지 모르기때문에 최적의 스케줄을 알 수 없지만, 어느 트랜잭션이던 규칙에 따라 직렬성 스케줄을 보장해준다
- 데이터베이스의 연산

  ![Untitled 2](https://user-images.githubusercontent.com/48282185/188043199-77a47c2e-5ca2-42c7-b2c5-2d3db82b744f.png)

  - input(x) 연산은 데이터베이스 항목 x를 포함하고 있는 블록을 주기억장치의 버퍼로 읽어들임
  - output(x) 연산은 데이터베이스 항목 x를 포함하고 있는 블록을 디스크에 기록함
  - read_item(x) 연산은 주기억 장치 버퍼에서 데이터베이스 항목 x의 값을 프로그램 변수 x로 복사함
  - write_item(x) 연산은 프로그램 변수 x의 값을 주기억 장치 내의 데이터베이스 항목 x에 기록함

- 동시성 제어없이 다수의 트랜잭션이 병렬처리할 때 생길 수 있는 문제

  - 갱신 손실(lost update)

    ![Untitled 3](https://user-images.githubusercontent.com/48282185/188043209-6f917177-e543-4c07-912a-c73d2776dc4e.png)

    - 수행 중인 트랜잭션이 갱신한 내용을 다른 트랜잭션이 덮어씀으로서 갱신이 무효화되는 것, 일관성이 깨진다

  - 오손 데이터 읽기(dirty read)

    ![Untitled 4](https://user-images.githubusercontent.com/48282185/188043211-1771bacf-d2a7-4915-846d-720024a25252.png)

    - clean ⇒ 완료된 트랜잭션, dirty ⇒ 진행 중인 트랜잭션
    - 완료되지 않은 트랜잭션이 갱신한 데이터를 읽는 것
    - cascade rollback이 일어날 수 있음, 어떤 튜플이 업데이트하는 트랜잭션 A, 그 업데이트를 이용해야하는 트랜잭션 B 사이에는 순서가 있다, A가 롤백하면 B도 롤백해야된다, B(dirty한 데이터를 읽은 모든 트랜잭션)는 완료되었더라도 A가 commit될 때까지 commit하지 말고 대기해야함

  - 반복불가능한 읽기(unrepeatable read)

    ![Untitled 5](https://user-images.githubusercontent.com/48282185/188043213-329a7da5-68e7-44de-8a10-71afc1c9ac0a.png)

    - 한 트랜잭션이 동일한 데이터를 두 번 읽을 때 서로 다른 값을 읽는 것 → 고립성이 깨짐

## 7. 로킹(locking)

- 각 트랜잭션들을 어떤 객체가 스케줄하는 것이 아니라 각자 어떤 규약을 지키면서 병렬로 수행될 수 있게만드는 것이 목표
- 동시성 문제는 공유 자원(데이터베이스에서는 데이터)때문에 생기는데, 서로 동시에 공유 자원을 갱신하려고 할 때 충돌(conflict)이 발생하기 때문임
- 데이터 항목을 로킹하는 개념은 동시에 수행되는 트랜잭션들의 동시성을 제어하기 위해서 가장 널리 사용되는 기법
- 로크(lock)는 추상적 개념이 아닌 데이터베이스 내의 각 데이터 항목과 연관된 하나의 변수
- 각 트랜잭션이 수행을 시작하여 데이터 항목을 접근할 때마다 요청한 로크에 관한 정보는 로크 테이블(lock table) 등에 유지됨
- 트랜잭션에서 갱신을 목적으로 데이터 항목을 접근할 때는 독점 로크(X-lock, eXclusive lock)를 요청함
- 트랜잭션에서 읽을 목적으로 데이터 항목을 접근할 때는 공유 로크(S-lock, Shared lock)를 요청함
  - 읽기는 문제가 없는데 왜 lock을 쓰나? 어떤 트랜잭션이 데이터를 읽는지 알기 위해 S-lock을 사용한다
- 트랜잭션이 데이터 항목에 대한 접근을 끝낸 후에 로크를 해제(unlock)함
  - commit할 때 unlock하는게 가장 안전한 방법이지만 여러 가지 방법이 사용된다
- 로크의 양립성

  ![Untitled 6](https://user-images.githubusercontent.com/48282185/188043215-b943fb4e-e50f-4a9b-9757-edd4325c5f62.png)

  - 서로 다른 로크 사이의 성질
  - s-lock끼리는 양립 가능

- 로킹만으로 동시성 제어 문제를 해결하기에 충분한가?
  - 결론부터 말하면 그건 아니다
  - 충돌은 방지할 수 있는데, 트랜잭션 A가 업데이트 후 언락하고 그 이후 트랜잭션 B가 들어와서 업데이트 하는 상황에서 커밋되지 않은 데이터를 읽는 상황은 여전히 발생한다(commit과 함께 unlock이 되는 건 구현을 어떻게 하느냐에 따라 다르기 때문에 꼭 그런거 아님), 그리고 unlock과 commit 사이에 트랜잭션이 들어올 수 있다, 이런 순서도 중요

## 8. 2단계 로킹 프로토콜(2-phase locking protocol, 2PL)

- 로크만으로 동시성 제어문제를 해결할 수 없기에 로크를 거는 과정을 두 단계로 나눠서 문제를 해결함
- 로크를 요청하는 것과 로크를 해제하는 것이 2단계로 이뤄진다
- 로크 확장 단계가 지난 후에 로크 수축 단계에 들어간다
- 일단 로크를 한 개라도 해제하면 로크 수축 단계에 들어간다
- 만일 튜플 10개를 업데이트하는 트랜잭션이 있다고 하자
  - 튜플 각각에 대해서 록을 걸고 해제하고 록을 걸고 해제하고 이런 방법이 될까? 안 된다, 중간에 다른 트랜잭션이 그 데이터에 락을 걸어버릴 수 있기 때문이다
  - 그럼 어떻게 해결하느냐? 트랜잭션은 자신이 업데이트할 모든 데이터에 lock을 걸 때까지 해제못하게 하면 된다, 1번 튜플 걸고 → 2번 튜플 걸고 → … 이렇게 lock을 확장할 수는 있어도 해제는 못함 ⇒ 로크 확장 단계
  - 더 이상 lock을 걸 데이터가 없을 때 해제(release)하기 시작하고 해제가 시작되면 추가적인 lock은 걸 수 없다 ⇒ 로크 수축 단계
- 로크 확장 단계
  - 로크 확장 단계에서는 트랜잭션이 데이터 항목에 대하여 새로운 로크를 요청할 수 있지만 보유하고 있던 로크를 하나라도 해제할 수 없음
- 로크 수축 단계
  - 로크 수축 단계에서는 보유하고 있던 로크를 해제할 수 있지만 새로운 로크를 요청할 수 없음
  - 로크 수축 단계에서는 더이상 로크 걸 데이터가 없을 때 조금씩 해제를 시작할 수도 있고, 트랜잭션이 완료 시점에 이르렀을 때 한꺼번에 모든 로크를 해제할 수도 있음
- 로크 포인트(lock point)는 한 트랜잭션에서 필요로 하는 모든 로크를 걸어놓은 시점을 의미하는 용어
- 로크 수축 단계 방법들

  ![Untitled 7](https://user-images.githubusercontent.com/48282185/188043219-4fa24eed-cebf-44d5-a162-98d9fe32f1c2.png)

  - 로크를 commit 시점에 한꺼번에 해제하는 방법이 일반적임
  - 로크를 하나씩 해제하게되면 아직 commit된 것은 아니기에 dirty read나 unrepeatable read를 방지할 수 있다

- 2PL과 비슷하게 2PC(2-phase commit)이라는 개념도 있는데 분산 시스템 환경에서 여러 DBMS가 각각 commit하면 안되고, 다른 DBMS들과 같이 commit해야됨
- 2PL말고도 동시성 제어문제를 해결할 수 있는 다양한 기법들이 있다 → 뭐 있는지 조사하기

## 9. 데드록(deadlock)

- 2PL에서 데드록이 발생할 수 있음
- 데드록은 두 개 이상의 트랜잭션들이 서로 상대방이 보유하고 있는 로크를 요청하면서 기다리고 있는 상태를 말함
- 데드록을 해결하기 위해서는 데드록을 방지하는 기법(avoidance)이나, 데드록을 탐지하고 희생자를 선정하여 데드록을 푸는 기법 등(lgnorance 또는 detect and recover)을 사용함
  - victim선정은 여러가지 기준이 있을 수 있는데, FCFS나 lock을 많이 가진 애 먼저 주거나 등등 다양함

## 10. 다중 로크 단위(multiple granularity)

![Untitled 8](https://user-images.githubusercontent.com/48282185/188043221-1e2c5210-97fe-4523-a616-23808284ece3.png)

- 대부분은 트랜잭션들이 소수의 튜플들을 접근하는 데이터베이스 응용에서는 튜플 단위로 로크를 해도 로크 테이블을 다루는 시간이 오래 걸리지 않음
- 트랜잭션들이 많은 튜플을 접근하는 데이터베이스 응용에서 튜플 단위로만 로크를 한다면 로크 테이블에서 로크 충돌을 검사하고, 로크 정보를 기록하는 시간이 오래 걸림
- 트랜잭션이 접근하는 튜플의 수에 따라 로크를 하는 데이터 항목의 단위로 구분하는 것이 필요함
- 한 트랜잭션에서 로크할 수 있는 데이터 항목이 두 가지 이상 있으면 다중 로크 단위라고 말함
- 데이터베이스에서 로크할 수 있는 단위로는 데이터 베이스 그 자체, 릴레이션, 디스크 블록, 튜플 등이 있음
  - 애트리뷰트 레벨까지 lock걸 수 있게하면 관리할 lock이 너무 많아져서 비용이크다 그래서 현재는 튜플이 최소 단위임
  - 블록에 lock을 건다는 것은 블록 내의 튜플들에 lock을 건다는 것
- 10억개 튜플을 갖고 있는 릴레이션을 전체 업데이트해야된다고 가정하자
  - 10억개 튜플 각각 lock vs 릴레이션 하나 lock
  - lock이 많아질수록 lock이 걸려있나 확인하는 작업의 overhead가 증가하고, lock table의 크기도 커진다
  - 이런 것 때문에 lock을 걸 수 있는 단위가 계층적으로 관리되어야하고, 필요한 단위만큼만 걸면 되기 때문에 관리가 쉬워진다
- 일반적으로 DBMS는 각 트랜잭션에서 접근하는 튜플 수에 따라 자동적으로 로크 단위를 조정함
- trade-off
  - 로크 단위가 작을수록 로킹에 따른 오버헤드가 증가함
  - 로크 단위가 작을수록 동시성의 정도는 증가함

## 11. 팬텀 문제(phantom problem)

![Untitled 9](https://user-images.githubusercontent.com/48282185/188043225-90e3bd85-2561-4cea-ac0e-1fde23a40456.png)

- 두 개의 트랜잭션 T1과 T2가 EMPLOYEE 릴레이션에 대해서 아래와 같은 순서대로 수행된다고 가정
- T1은 EMPLOYEE 릴레이션에서 1번 부서에 근무하는 사원들의 이름을 검색하는 동일한 SELECT문을 두 개 포함하고, T2는 1번 부서에 근무하는 사원 튜플을 한 개 삽입하는 INSERT문을 포함
- T1이 시간 1에 수행된 것과 시간 2때 수행된 결과가 다르고, 이 때 시간 2와 시간 1에 나타나는 결과의 차집합을 phantom tuple이라고 부름
- 팬텀 문제의 해결
  - 데이터에 lock을 거는것으로는 팬텀문제를 해결할 수 없음, 시간 1에서 DNO=1인 것들에 대해서는 lock을 걸 수 있지만 새로 들어오는 건 어쩔 수 없다
  - DNO = 1인 튜플들에 락을 거는게 아닌, DNO를 키로하는 인덱스에 lock을 걸어버리면 삽입을 막을 수 있어서 팬텀 문제를 해결할 수 있음
  - 근데 DNO index가 없다면 어떻게 함?? → 이거 조사해보기

## 12. 회복(recovery)

- 회복의 필요성
  - 어떤 트랜잭션 T를 수행하는 도중에 시스템이 다운되었을 때, T의 수행효과가 디스크의 데이터베이스에 일부 반영되었을 수 있음
    - 어떻게 T의 수행을 취소하여 원자성을 보장할 것인가?
  - T가 완료된 직후에 시스템이 다운되면 T의 모든 갱신 효과가 주기억 장치로부터 디스크에 기록되지 않을 수 있음
    - 어떻게 T의 수행 결과가 데이터베이스에 완전하게 반영되도록 하여 지속성을 보장할 것인가?
  - 디스크의 헤드 등이 고장나서 디스크의 데이터베이스를 접근할 수 없다면 어떻게 할 것인가?
- 회복의 개요
  - 여러 응용이 주기억 장치 버퍼 내의 동일한 데이터베이스 항목을 갱신한 후에 디스크에 기록함으로서 성능을 향상시키는 것이 중요함
  - 버퍼의 내용을 디스크에 기록하는 것을 가능하면 최대로 줄이는 것이 일반적
    - 예시로 버퍼가 꽉 찼을 때 또는 트랜잭션이 완료될 때 버퍼의 내용이 디스크에 기록될 수 있음
  - 트랜잭션이 버퍼에는 갱신 사항을 반영했지만 버퍼의 내용이 디스크에 기록되기 전에 고장이 발생할 수 있음
  - 핵심 매커니즘은 로그이다
  - 고장이 발생하기 전에 트랜잭션이 완료 명령을 수행했다면 리커버리 모듈은 이 트랜잭션의 갱신 사항을 재수행(redo)하여 트랜잭션의 갱신이 지속성을 갖도록 해야 함 → 로그 필요
  - 고장이 발생하기 전에 트랜잭션이 완료 명령을 수행하지 못했다면 원자성을 보장하기 위해서 이 트랜잭션이 데이터베이스에 반영했을 가능성이 있는 갱신 사항을 취소(undo)해야 함 → 로그 필요
- 저장 장치의 유형

  ![Untitled 10](https://user-images.githubusercontent.com/48282185/188043229-c235a1e4-c22f-4aad-8842-872eba7d2262.png)

  - 안전 저장 장치(stable storage)는 모든 유형의 고장을 견딜 수 있는 저장 장치를 의미
  - 두 개 이상의 비휘발성 장치가 동시에 고장날 가능성이 매우 낮으므로 비휘발성 저장 장치에 두 개 이상의 사본을 중복(redundancy)해서 저장함으로서 안전 저장 장치를 구현함
  - RAID구조에서 전체 디스크들 중 일부는 데이터 저장 용도말고 백업 용도로 사용됨
  - 데이터가 들어있는 디스크가 고장날 경우를 대비해 로그가 들어있는 디스크는 다른 디스크로 하는 것이 좋다
  - 위 그림에서는 로그가 하나있는데 여러 개 중복으로 저장한다

- 재해적 고장과 비재해적 고장
  - 재해적 고장
    - 디스크가 손상을 입어서 데이터베이스를 읽을 수 없는 고장
    - 백업에서 복구하는 거 외에는 방법 없음
  - 비재해적 고장
    - 디스크 손상을 제외한 고장
    - 대부분의 회복 알고리즘들은 비재해적 고장에 적용
    - 로그를 기반으로 한 즉시 갱신(업데이트 즉시, 디스크에 씀), 로그를 기반으로 한 지연 갱신(업데이트 이후 일정 시간 후에 디스크에 씀, 업데이트 중간에 장애나더라도 디스크에 안썼으니 undo필요 없음), 그림자 페이징(shadow paging, 업데이트를 항상 새블록에 해주는 방법, 업데이트가 발생하는 도중 장애가 발생했을 때 유용) 등 여러 알고리즘이 있음
    - 대부분의 상용 DBMS에서 로그를 기반으로 한 즉시 갱신 방식을 사용
      - 모든 트랜잭션이 정상 commit할거라는 낙관적인 입장과 실제로 그렇기도 하기 때문에 그냥 바로 써도 취소할 가능성이 적기 때문에 상관 없음
      - rollback은 매우 드물게 발생한다
- 로그를 사용한 즉시 갱신
  - 즉시 갱신에서는 트랜잭션이 데이터베이스를 갱신한 사항이 주기억 장치의 버퍼에 유지되다가 트랜잭션이 완료되기 전이라도 디스크의 데이터베이스에 기록될 수 있음
  - 데이터베이스에는 완료된 트랜잭션의 수행 결과뿐만 아니라 철회된 트랜잭션의 수행 결과도 반영될 수 있음
  - 트랜잭션의 원자성과 지속성을 보장하기 위해 DBMS는 로그라고 부르는 특별한 파일을 유지함
  - 데이터베이스의 항목에 영향을 미치는 모든 트랜잭션의 연산들에 대해서 로그 레코드를 기록함
  - 각 로그 레코드는 로그 순서 번호(LSN, Log Sequence Number)로 식별됨

> 출처
>
> - 이화여대 용환승 교수님 강의
> - 홍의경 저, MS SQL Server 기반 데이터베이스 배움터, 생능출판사, 2012년,
> - 이석호 저, 데이터베이스 시스템, 정익사, 2009.
> - A. Silberschatz, H. Korth, S. Sudarshan, "Database System Concepts," 6th Ed., McGraw-Hill, 2010.
> - Elmasri and Navathe, “ Fundamentals of Database Systems”, 6th ed. Addison-Wesley, 2010.
> - C.J.Date, “An Introduction to Database Systems (8th Edition)”, Addison-Wesley, 2003.
> - Won Kim, "Modern Database Systems," ACM Press, 1994
