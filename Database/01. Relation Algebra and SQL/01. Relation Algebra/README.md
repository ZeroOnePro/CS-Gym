# 1. 관계대수와 SQL 1

<!--ts-->

- [1. 관계대수와 SQL 1](#1-관계대수와-sql-1)
  - [1. 관계 대수와 SQL](#1-관계-대수와-sql)
  - [2. 관계 대수](#2-관계-대수)
  - [3. 관계 대수 연산](#3-관계-대수-연산)

<!-- Created by https://github.com/ekalinin/github-markdown-toc -->
<!-- Added by: sungminyou, at: 2022년 8월 27일 토요일 22시 18분 41초 KST -->

<!--te-->

- 여기서 사용되는 용어들 정리

  [용어 정리](https://jhnyang.tistory.com/108)

- cardinality는 여러 의미로 쓰이는데 릴레이션 하나에 대해서는 행의 개수를 의미하고(카티션 곱에서 사용됨) 두 개의 릴레이션에서의 카디날리티는 전체 행에 대한 특정 컬럼의 중복 수치를 나타내는 지표로 같은 조건에 부합하는 행들만 간추려졌으므로 중복이 많아진다(selection에서 사용됨)

## 1. 관계 대수와 SQL

- 관계 데이터 모델에서 지원되는 두 가지 정형적인 언어
  - 관계 해석(relational calculus)
    - 원하는 데이터만 명시하고 질의를 어떻게 수행할 것인가는 명시하지 않는 선언적인 언어
    - what에 대한 것
  - 관계 대수(relational algebra)
    - 어떻게 질의를 수행할 것인가를 명시하는 절차적 언어
    - 관계 대수는 상용 relational DBMS에서 널리 사용되는 SQL의 이론적인 기초가 됨
    - 관계 대수는 SQL을 구현하고 최적화하기 위해 DBMS의 내부 언어로서도 사용됨
    - what과 how에 대한 것
- SQL
  - 상용 relational DBMS들의 사실상의 표준 질의어인 SQL을 이해하고 사용할 수 있는 능력은 매우 중요함
  - 사용자는 SQL을 사용하여 관계 데이터베이스에 릴레이션을 정의하고, 관계 데이터베이스에서 정보를 검색하고, 관계 데이터베이스를 갱신하며, 여러 가지 무결성 제약조건들을 명시할 수 있음

## 2. 관계 대수

![1](https://user-images.githubusercontent.com/48282185/186122669-c45a1a10-194e-4cf7-b903-5a0ce66d6985.png)

- 여기서 릴레이션은 테이블의 의미함
- 기존의 릴레이션들로부터 새로운 릴레이션을 생성함
- 릴레이션이나 관계 대수식(이것의 결과도 릴레이션임)에 연산자들을 적용하여 보다 복잡한 관계 대수식을 점차적으로 만들 수 있음
- 기본적인 연산자들의 집합으로 이루어짐
- 산술 연산자와 유사하게 단일 릴레이션이나 두 개의 릴레이션을 입력으로 받아 하나의 결과 릴레이션을 생성함
- 결과 릴레이션은 또 다른 관계 연산자의 입력으로 사용될 수 있음

## 3. 관계 대수 연산

- selection
  - 한 릴레이션에서 selection condition을 만족하는 튜플들의 부분집합을 생성함
  - 단항 연산자
  - 결과 릴레이션의 차수는 입력 릴레이션의 차수와 같음
  - 결과 릴레이션의 카디날리티는 항상 원래 릴레이션의 카디날리티보다 작거나 같음(카디널리티는 전체 행에 대한 특정 컬럼의 중복 수치를 나타내는 지표로 같은 조건에 부합하는 행들만 간추려졌으므로 중복이 많아진다)
  - selection condition을 predicate라고도 함
  - selection condition은 일반적으로 릴레이션의 임의의 attribute와 상수, =, <>, ≤, ≥, <, > 등의 비교 연산자, AND, OR, NOT 등의 boolean 연산자를 포함할 수 있다
- projection
  - 한 릴레이션의 attribute들의 부분집합을 생성함
  - 결과로 생성되는 릴레이션은 attribute list에 명시된 attribute들만 갖는다
  - selection의 결과 릴레이션에는 중복 tuple이 존재할 수 없지만(릴레이션의 기본 원칙에 따라 중복된 행은 존재할 수 없음), projection 결과 릴레이션에는 중복된 tuple이 존재할 수 있음, 따라서 결과 릴레이션은 엄밀히 말하면 중복이 있기때문에 릴레이션이라고 할 수 없다(중복제거는 시간이 오래 걸리는 연산임)
- 집합 연산자

  - 릴레이션이 튜플들의 집합이기 때문에 기존의 집합 연산이 릴레이션에 적용됨
  - 세 가지 집합 연산자
    - 합집합, 교집합, 차집합 연산자
  - 집합 연산자의 입력으로 사용되는 두 개의 릴에시녀은 합집합 호환(union compatible)이어야 함
  - 이항 연산자
  - 합집합 호환
    - 합집합할 수 있는 조건
    - 두 릴레이션 R1(A1, A2, …, An)과 R2(B1, B2, …, Bm)이 합집합 호환일 필요 충분 조건은 n=m이고(애트리뷰트 수가 동일), 모든 1 ≤ i ≤ n에 대해 domain(Ai) = domain(Bi), 같은 위치의 애트리뷰트 끼리는 동일한 의미를 가져야한다, 하나는 이름이고 하나는 주민등록번호고 그러면 안된다는 의미임
      - 여기서 Ai와 Bi는 i번째 attribute를 의미
    - employee relation하고 department relation하고 합집합해라? → 말도 안되고 의미 없음
  - 합집합 연산자
    - 두 릴레이션 R과 S의 합집합 R U S는 R 또는 S에 있거나 R과 S 모두에 속한 튜플들로 이루어진 릴레이션
    - 결과 릴레이션에서 중복된 튜플들은 제외됨
    - 결과 릴레이션에서 차수는 R 또는 S의 차수와 같으며, 결과 릴레이션의 attribute 이름들은 R의 attribute들의 이름과 같거나 S의 attribute들의 이름과 같음
  - 교집합 연산자
    - 두 릴레이션 R과 S의 교집합 R $\cap$ S 는 R과 S 모두에 속한 튜플들로 이루어진 릴레이션
    - 결과 릴레이션에서 차수는 R 또는 S의 차수와 같으며, 결과 릴레이션의 attribute 이름들은 R의 attribute들의 이름과 같거나 S의 attribute들의 이름과 같음
  - 차집합 연산자
    - 두 릴레이션 R과 S의 차집합 R - S는 R에는 속하지만 S에는 속하지 않은 튜플들로 이루어진 릴레이션
    - 결과 릴레이션에서 차수는 R 또는 S의 차수와 같으며, 결과 릴레이션의 attribute 이름들은 R의 attribute들의 이름과 같거나 S의 attribute들의 이름과 같음
  - 카티션 프로덕트(카티션 곱) 연산자

    ![2](https://user-images.githubusercontent.com/48282185/186122666-7107a7ef-d449-4f67-a172-e5d9b7467fee.png)

    - 카디날리티가 i인 릴레이션 R(A1, A2, …, An)과 카디날리티가 j인 릴레이션 S(B1, B2, …, Bm)의 카티션 곱 R x S는 차수가 n + m이고, 카디날리티가 i \* j이고, 애트리뷰트가 (A1, A2, …, An, B1, B2, …, Bm)이며, R과 S의 튜플들의 모든 가능한 조합으로 이루어진 릴레이션
    - 카티션 곱의 결과 릴레이션은 크기가 매우 클 수 있으며, 사용자가 실제로 원하는 것은 카티션 곱의 결과 릴레이션의 일부인 경우가 대부분이므로 카티션 곱 자체는 유용한 연산자가 아님
    - 차수가 n+m인 릴레이션 R(A1, A2, …, An, B1, B2, …, Bm)과 차수가 m인 릴레이션 S
    - join을 위해 고안된 개념

  - 관계 대수의 완전성
    - selection, projection, union, difference, cartesian product은 관계 대수의 필수적인 연산자
    - 다른 관계 연산자들은(join, 교집합 등등)은 필수적인 관계 연산자를 두 개 이상 조합하여 표현할 수 있음
    - 임의의 질의어가 적어도 필수적인 관계 대수 연산자들만큼의 표현력을 갖고 있으면 관계적으로 완전(relationally complete)하다고 말함, 관계 언어의 선택 기준이 되기도 했음, SQL이 관계 대수를 잘 표현해서 표준으로 채택된 것임
  - 조인 연산자

    - 두 개의 릴레이션으로부터 연관된 튜플들을 겹합하는 연산자
    - 관계 데이터베이스에서 두 개 이상의 릴레이션들의 관계를 다루는데 매우 중요한 연산자
    - 세타 조인(theta join), 동등 조인(equijoin)

      ![3](https://user-images.githubusercontent.com/48282185/186122660-305ace6e-1dcf-4743-a20f-e42d92ba60e4.png)

      - 두 릴레이션 R(A1, A2, …, An)과 S(B1, B2, …, Bm)의 세타 조인 결과는 차수가 n+m이고 애트리뷰트가 (A1, A2, …, An, B1, B2, …, Bm)이며, 조인 조건을 만족하는 튜플들로 이루어진 릴레이션 → 카티시안 프로덕트에서 파생
      - theta는 {=, <>($\ne$), ≤, ≥, <, >} 중의 하나를 의미
      - 동등 조인은 세타 조인 중에서 비교연산자가 =인 조인
      - 두 릴레이션의 공통 attribute를 가지고 join을 해야 의미가 있겠지

    - 자연 조인(natural join)

      ![4](https://user-images.githubusercontent.com/48282185/186122652-4c45674f-5197-4d7e-ab45-f8bb78c6098b.png)

      - 동등 조인의 결과 릴레이션에서 조인할 때 사용한 attribute를 제외한 조인
      - 여러 가지 조인 연산자들 중에서 가장 자주 사용됨
      - 실제로 관계 데이터베이스에서 대부분의 질의는 selection, projection, natural join으로 표현 가능

    - 외부 조인(outer join)
    - 세미 조인(semi join)

  - 디비젼 연산자

    ![5](https://user-images.githubusercontent.com/48282185/186122651-b253a9e4-dc14-4ee6-b25e-55c33697b9ed.png)

    - 차수가 n+m인 릴레이션 R(A1, A2, …, An, B1, B2, …, Bm)과 차수가 m인 릴레이션 S(B1, B2, …, Bm)의 디비젼 R $\div$ S는 차수가 n이, S에 속하는 모든 튜플 u에 대해 튜플 tu(튜플 t와 튜플 u를 결합한 것)가 R에 존재하는 튜플 t의 집합이다, 디비젼을 하기 위한 조건으로 R이 S의 attribute를 모두 포함하고 있어야 한다
    - 수강 신청 테이블에서 3학년 전공 교과목을 모두 수강하고 있는 학생들을 검색해라가 디비젼의 예시가 될 수 있음

  - 관계 대수의 한계
    - 위에서 살펴봤던 관계 대수만으로는 한계가 있더라
    - 관계 대수는 산술 연산을 할 수 없음
    - 집단 함수(aggregate function)를 지원하지 않음
      - 통계같은 거
    - 정렬을 나타낼 수 없음
    - 데이터베이스를 수정할 수 없음
    - 프로젝션 연산의 결과에 중복된 튜플을 나타내는 것이 필요할 때가 있는데 이를 명시하지 못함
  - 추가된 관계 대수 연산자

    - aggregate function(집단 함수)
      - 통계, 그룹화
    - outer join(외부 조인)

      - 상대 릴레이션에서 대응되는 튜플을 갖지 못하는 튜플이나 조인 애트리뷰트에 null값이 들어 있는 튜플을 다루기 위해서 조인 연산을 확장한 조인
      - 두 릴레시연에서 대응되는 튜플들을 결합하면서, 대응되는 튜플을 갖지 않는 튜플과 조인 애트리뷰트에 null값을 갖는 튜플도 결과에 포함시킴
      - 왼쪽 외부 조인(left outer join), 오른쪽 외부 조인(right outer join), 완전 외부 조인(full outer join)이 있다
      - natural join

        ![6](https://user-images.githubusercontent.com/48282185/186122644-04fd9ea2-4aed-4373-934e-c906ba9d153b.png)

      - right outer join

        ![7](https://user-images.githubusercontent.com/48282185/186122640-bab3dbfe-8a19-4e57-b756-6dc9778e6856.png)

        - 릴레이션 R과 S의 오른쪽 외부 조인 연산은 S의 모든 튜플들을 결과에 포함시키고, 만일 릴레이션 R에 관련된 튜플이 없으면, 결과 릴레이션에서 R의 애트리뷰트들은 null값으로 채운다

      - left outer join

        ![8](https://user-images.githubusercontent.com/48282185/186122638-8f24ec0e-5ed9-472a-ae09-007dc849835b.png)

        - 릴레이션 R과 S의 왼쪽 외부 조인 연산은 R의 모든 튜플들을 결과에 포함시키고, 만일 릴레이션 S에 관련된 튜플이 없으면, 결과 릴레이션에서 S의 애트리뷰트들은 null값으로 채운다

      - full outer join

        ![9](https://user-images.githubusercontent.com/48282185/186122627-bdcf9ed6-5641-4fc8-b452-b4b3c835e670.png)

        - 릴레이션 R와 S의 완전 외부 조인 연산은 R과 S의 모든 튜플들을 결과에 포함시키고, 만일 상대 릴레이션에 관련된 튜플이 없으면 결과 릴레이션에서 상대 릴레이션의 애트리뷰트들은 null 값으로 채운다

> 출처
>
> - 이화여대 용환승 교수님 강의
> - 홍의경 저, MS SQL Server 기반 데이터베이스 배움터, 생능출판사, 2012년,
> - 이석호 저, 데이터베이스 시스템, 정익사, 2009.
> - A. Silberschatz, H. Korth, S. Sudarshan, "Database System Concepts," 6th Ed., McGraw-Hill, 2010.
> - Elmasri and Navathe, “ Fundamentals of Database Systems”, 6th ed. Addison-Wesley, 2010.
> - C.J.Date, “An Introduction to Database Systems (8th Edition)”, Addison-Wesley, 2003.
> - Won Kim, "Modern Database Systems," ACM Press, 1994
