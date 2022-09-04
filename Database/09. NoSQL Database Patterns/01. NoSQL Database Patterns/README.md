# 1. NoSQL Database Pattern

<!--ts-->

- [1. NoSQL Database Pattern](#1-nosql-database-pattern)
  - [1. NoSQL의 개요](#1-nosql의-개요)
  - [2. NoSQL 이전](#2-nosql-이전)
  - [3. NoSQL의 등장](#3-nosql의-등장)
  - [4. Data에 대한 요구사항](#4-data에-대한-요구사항)
  - [5. 다양한 NoSQL 솔루션](#5-다양한-nosql-솔루션)
  - [6. Key-Value Stores](#6-key-value-stores)
  - [7. Column-Family](#7-column-family)
  - [8. Graph Store](#8-graph-store)
  - [9. Document Store](#9-document-store)
  - [10. Hybrid architectures](#10-hybrid-architectures)

<!-- Created by https://github.com/ekalinin/github-markdown-toc -->
<!-- Added by: sungminyou, at: 2022년 9월  4일 일요일 12시 37분 00초 KST -->

<!--te-->

## 1. NoSQL의 개요

- No가 yes no할 때 no가 아니라 Not Only의 약자임
- 많은 양의 데이터를 다루기 위해서는 SQL로는 한계가 있다
- 데이터베이스의 발전 과정

  ![Untitled](https://user-images.githubusercontent.com/48282185/188296124-1be02460-644f-48a8-8dc1-c2a9080e4e30.png)

- RDB, NoSQL 모두 데이터베이스 시스템의 패턴이라고 할 수 있음
  - 관계형 데이터베이스에는 RDB 패턴이 적용되어야한다
  - 데이터베이스 시스템에 따라 적절한 패턴을 적용해야함

## 2. NoSQL 이전

- RDBMS(OLTP, Online Transaction Processing)는 트랜잭션을 위한 용도이고 Data Warehouse(OLAP, Online **Analytical** Processing)는 분석을 위한 용도이다

  ![Untitled 1](https://user-images.githubusercontent.com/48282185/188296111-9d9e0889-dec2-448c-bf96-df150edc7cbe.png)

  - 여기서 Online은 real-time을 의미, 굉장히 빠른 처리를 요구
  - RDB와 특히 Data warehouse의 경우 그 크기가 굉장히 커서 real-time으로 처리가 기술적으로 굉장히 어려웠고, 이 때문에 새로운 스타일의 데이터베이스 스키마를 고안하게 됨(star schema)
  - star schema는 릴레이션인데 모양이 별처럼 생겼다하여 붙은 이름(위에서 Analytical을 나타내는 그림)이고 가운데 중심 테이블을 fact table, 옆에 가지들을 dimension table이라고 한다
  - star schema는 특별한 인덱싱 구조를 사용하여 real-time을 지원함

- 하나의 RDBMS 아키텍처로 감당이 안됨

  ![Untitled 2](https://user-images.githubusercontent.com/48282185/188296112-f3a2e545-aa15-40ac-9f23-58bcbc5b679b.png)

  - agile schema free와 같이 스키마가 고정된게 아니라 빠르게 동적으로 바뀌는 데이터들이 점점 증가한다, 데이터가 정형화 돼있지 않음
  - document-data는 XML, json 같은 semi structured data
  - document-data가 real world에서 많았고 이것들을 대량으로 저장하고 관리하는 솔루션이 필요하게되었고, 결과적으로 NoSQL가 나왔다

## 3. NoSQL의 등장

- Key-Value, Column-Family, Graph, Document 등 다양한 구조의 NoSQL이 나오게 됨(표준화 되지 않음)

  ![Untitled 3](https://user-images.githubusercontent.com/48282185/188296113-d5945c31-d5cc-47a4-81d7-f2ec1143bdea.png)

- 위 네 가지 형태 모두 RDB로 매핑해서 쓸 수 있고, 지금까지는 그랬으나 매핑하지 말고 자료 구조 그대로 이용하자는 방향으로 발전하고 있음
- 응용의 데이터베이스 선택

  ![Untitled 4](https://user-images.githubusercontent.com/48282185/188296114-e5fbc0a0-8243-4e9b-96b4-16c1e08a433d.png)

## 4. Data에 대한 요구사항

- Transactions(OLTP)
- Analysis(OLAP)
- 검색과 찾을 수 있어야함
- 신속 대응이 가능해야함
- 속도가 빨라야하고 신뢰성 있어야한다
- knowledge discover를 할 수 있어야함
- consistency
  - 빅데이터로 가니 consistency가 그렇게 큰 문제가 아님
  - 2PC까지 필요없이 그것보다 완화된 실용적인 솔루션을 내놓게 됨

## 5. 다양한 NoSQL 솔루션

![Untitled 5](https://user-images.githubusercontent.com/48282185/188296115-d1bfe83c-5a6b-4f6e-99ed-bed501fad09b.png)

## 6. Key-Value Stores

- 키를 사용하여 구체적인 데이터 구조가 인터페이스에 정의되지 않은 데이터(opaque) blobs에 접근한다
- value는 어떤 타입의 데이터든지 포함할 수 있다(image, video 등등)
- 장점
  - scalable
  - simple API(put, get, delete)
- 단점
  - 데이터가 불투명하게 처리되기때문에 content를 가지고 쿼리를 날릴 방법이 없음
  - 키 값을 모르면 데이터를 못찾음, 어떤 값을 가진 정보를 찾아라 이런게 안된다
- Redis, DynamoDB, S3, Memcache, Berkley DB(B+ 트리 base), Riak 등이 대표적 솔루션
- real-time DB에서는 메모리에 DB를 올려놓고 사용함
  - web search engine, game 등 많은 사람들이 자주 접근하는 정보들은 key-value형태로 메모리에 올려놓고 사용한다
- 매우 빠르고 쉽게 스케일 가능하다(join이 없음)
- RDB vs Key-Value store

  ![Untitled 6](https://user-images.githubusercontent.com/48282185/188296116-99a8987f-e02f-4542-9b41-24857bb95547.png)

  - 결과
    - RDB는 튜플들의 집합이고, Key-Value는 single item
  - index
    - RDB는 대용량 데이터 집합에 대해 반드시 인덱싱되어야함, Key-Value는 key에 대한 index만 있으면 됨
  - column
    - RDB는 같은 컬럼이라면 반드시 같은 데이터 타입을 가져야하지만 Key-Value에서 value는 어떤 데이터 타입이 와도 상관없음

- Key-Value store의 종류
  - eventually-consistent key-value store
    - eventually consistent는 항상 consistent할 필요는 없고 eventually consistent하기만 하면 된다
    - eventually는 최종적이란 뜻으로 전통적인 RDB에서는 트랜잭션의 결과가 항상 일관성을 유지했어야됬는데, eventually consistent는 일정시간 후에 최종적으로 일관성을 유지하면 되는거고, 트랜잭션의 결과가 즉시 일관성을 유지할 필요가 없다라는 의미
  - hierarchical key-value store
  - key-value stores in RAM
    - 대표적인 솔루션으로 오픈소스인 memcached가 있음
    - 모든 애플리케이션이 DB에 가서 가져오면 DB의 로드가 너무 커짐
    - 많이 access하는 것들은 램에 놔두겠다는 목적
    - memcached
      - 분산 웹 서버들의 램 사용을 효율적으로 만들어준다
      - 데이터베이스 로드를 완화하여 동적 웹 애플리케이션 속도 향상
      - RAM에 상주하며 데이터베이스 호출, API 호출 또는 페이지 렌더링 결과에서 얻은 임의 데이터의 작은 청크를 저장한다
      - 많이 분산된 RAM cache들에 대한 간단한 인터페이스를 제공
      - 읽는데 일반적으로 30ms정도 걸림
      - **신속한 배포, 개발 용이성을 위해 설계됨**
      - 많은 언어들에 대한 API를 제공
    - redis
      - open source in-memory key-value store with optional durability(optional durability는 레벨에 따라 지속성 서포트 정도를 정할 수 있음)
      - 고속 쓰기와 읽기에 포커스를 맞춤
      - 간단한 lists, sets, hashes를 value내에 저장하고 조작하는 것을 허용한다
      - expiration, transactions, pub / sub, partioning 등 많은 특징이 있음
        - pub / sub 패턴은 publish는 작업 완료를 의미 subscribe는 구독을 의미하는데, 구독자들은 어떤 작업이 이뤄졌는지 확인하기 위해 구독을 해놓는다, publish는 작업 완료로 구독자들에게만 이 정보를 알린다
        - 정보 전달 시스템에서 많이 사용되는 패턴임(ex. kafka)
    - key-value stores in DISK
  - high availability key-value store
  - ordered key-value store
  - values that allow simple list operations(value가 list일 때 간단히 list에 대한 연산을 제공)
- replication

  - 지속성을 제공하기 위해 데이터를 복제해놓는 것으로서 replicatation factor(몇 개를 복제할 것인가, 보통 3)에 따라 분산 환경에 각각 따로 저장해놓음
  - 하나의 key-value store가 분산된 웹서버들 여러 곳에 저장
  - 부하를 줄이기 위한 용도로도 사용될 수 있네

    [Database의 리플리케이션(Replication)이란?](https://nesoy.github.io/articles/2018-02/Database-Replication)

- sharding

  - 릴레이션의 row들을 나눠서 분산환경에 각각 따로 저장하는 기법
  - 자세하게 나온 블로그가 있어 여기를 참고하면 좋을 것 같다

    [Database의 샤딩(Sharding)이란?](https://nesoy.github.io/articles/2018-05/Database-Shard)

- 많은 사람들에게 real-time으로 응답을 줘야하고, 빅데이터 같은 큰 데이터를 다루려면 기존의 DBMS로는 한계가 있어서 메모리 기반으로한 key-value store가 폭넓게 솔루션으로 활용됨
  - 네이버 메인화면같이 많은 사람들이 쓰는 것들은 메모리에 올려놓는 것이 성능상으로도 비용상으로도 좋을 것임

## 7. Column-Family

- key-value 모델의 확장, sparse 데이터가 많은 릴레이션으로도 볼 수 있음
- 키가 하나의 row, column family, column 이름을 포함한다
  - access 순서: row → column family → a column
- 하나의 거대한 테이블 내에 버전이 있는 blobs를 저장한다
- rows, column families(column들의 집합, 하나의 row에 다수 개의 column family가 대응되고, 애트리뷰트 값 하나를 찾기 위해 키의 구성이 세 가지 요소로 돼있음), column names에 대한 쿼리가 가능
- flexible한 스키마를 지원하기 위해 나옴
- 장점
  - scale out하기 좋다
  - 버전관리가 용이
- 단점
  - blob content에 대한 쿼리는 불가능
  - row와 column의 설계에 많은 비용이 들어간다
- 관계형 데이터베이스는 row oriented 설계였지만(계산 단위가 튜플), column-family는 column oriented 설계
- 대표적인 솔루션으로 cassandra, HBase, Hypertable, Apache Accumulo, Google Bigtable
- 빅데이터에서는 과거의 어떤 시점에 어떤 값을 가졌는지에 따라 변화를 추정하는 작업이 대부분이기에 데이터가 버전을 가져야하고, 이를 timestamp로 구현할 수 있음
- 어떤식으로 구성되는지에 대한 자세한 설명
  [[NoSQL] NoSQL 시작하기](https://12bme.tistory.com/323)
- 정리

  ![Untitled 7](https://user-images.githubusercontent.com/48282185/188296117-8f50b379-826a-4186-ad41-97fa5ce4c6af.png)

  - RDBMS와 유사한 테이블 구조를 보존
  - join연산에 최적화되어있지 않음(high cardinality, 애트리뷰트 단위의 중복이 많음, 키의 구성을 보면 왜 조인에 최적화 되지 않는지 알 수 있음)
  - 하나의 row가 수만 개이상의 column들을 가질 수 있지만 이 데이터는 매우 sparse하다(column family형태에서 RDBMS로 치환해보면, 왜 sparse하다는 건지 알 수 있음)
  - 높은 다양성을 갖는 데이터 셋에 이상적인 구조
  - alter table 연산 없이 새로운 column들을 삽입하는게 가능
  - 삽입이 된다는 것은 새로운 column의 추가를 의미하는 것이기도 함
  - 컬럼에 대한 쿼리 가능
  - column의 그룹 ⇒ column family
  - column family의 그룹 → super columns
  - column family와 super column을 가지고 column에 대해 쿼리 가능
  - 속도 향상을 위해 유사한 데이터들을 묶을 수 있음

- 대표 솔루션 - HBase
  - java로 쓴 MapReduce 알고리즘 구현체 오픈소스
  - column-oriented data store
  - java interface
  - HBase는 Hadoop위에서 돌아감
  - High-level query language(Pig) 지원
  - 빅데이터를 위한 거의 모든 솔루션을 제공
- 대표 솔루션 - Cassandra

  - DataStax이 지원하는 아파치재단의 coulmn family database 오픈소스
  - P2P 분산 모델
  - 초당 수만개의 write가 가능, linear scale out(가장 이상적인 형태의 scale out)을 위한 강력한 솔루션

    - scale up vs scale out
    - scale up은 용량이 부족해졌을 때 그 시스템 자체를 업그레이드하는 것 - CPU 클럭 수, 메모리 용량 등등
    - scale out은 용량이 부족해졌을 때 시스템을 다른 곳으로 복제하여 똑같은 것을 하나 만들어서 여러 대가 지원할 수 있게 만드는 것
    - 서버 대수를 늘림에 따라 성능도 linear하게 증가

      ![Untitled 8](https://user-images.githubusercontent.com/48282185/188296118-3f5f832e-8b30-488d-b3a1-15eeddd52f1d.png)

      - cf) 서버를 늘린다고 성능이 증가함을 보장할 수는 없으나 카산드라는 linear scale out으로 정평나있음

  - java로 쓰여졌고 HDFS(Hadoop File System)과 MapReduce기능들을 지원함

## 8. Graph Store

- 데이터가 그래프 자료구조형태로 저장되는 것
- query는 graph tranversals
  - load가 큰 연산
- 소셜 네트워킹같이 데이터 사이의 관계가 중요한 형태에서 사용하기 적절
- 장점
  - 빠르게 네트워크 서칭이 가능
  - public linked 데이터 셋들을 다룰 수 있음(open-linked data)
- 단점
  - 그래프가 RAM과 맞지 않을 때 확장성측면에서 안좋음
  - 특별한 query language가 필요(RDF가 사용하는 SPARQL 같은 것)
- RDF(Resource Discription Framework)은 웹 상의 자원의 정보를 표현하기 위한 규격, 메타데이터들을 표준화 시킨 형태

  ![Untitled 9](https://user-images.githubusercontent.com/48282185/188296119-e738b55a-de49-4838-bf41-447909284ce4.png)

  - 모든 정보를 triple형태(노드 - 관계 - 노드를 attribute 3개로 매핑한 레코드)로 표현
  - triple로 이뤄진 데이터들에 쿼리를 보내기 위한 language SPARQL이 있음
  - 노드들은 서로 join해서 graph를 만들어낸다
  - 노드의 식별을 위해 URI(Uniform Resource Identifier) 혹은 그 비슷한 것이 필요

- 대표 솔루션
  - Neo4j, AllegroGraph, BigData triple store, infiniteGraph, StarDog 등
- Neo4J
  - java 개발자가 사용하기 쉽게 설계된 Graph database
  - dual license, 유료버전과 무료버전이 있음
  - 애플리케이션 내에서 내장 java library 위에서 동작
  - disk-based(RAM도 있음)
  - full ACID

## 9. Document Store

- 데이터가 계층 구조로 저장되는 형태, 대표적으로 XML, json 등이 있음
- document 내의 어떤 데이터든지 쿼리 가능
- 장점
  - 릴레이션의 튜플 객체에 데이터를 매핑할 필요가 없어졌음(No object-relational mapping layer)
  - search에 이상적인 구조
- 단점
  - 구현이 복잡하고, SQL과 호환이 안됨
- 대표적인 솔루션들로 MongoDB, MarkLogic, Couchbase, CouchDB, eXist-db가 있음
- 제일 많이 쓰는 스토어
- 컴퓨터에서 읽을 수 있는 document를 single blob of data로 함께 저장
- 저장형태로 XML이나 JSON을 사용
- 여러면에서 “object store”과 유사
- No shredding
  - document를 RDB로 저장하기위해 분해해서 각 attribute에 맞게 매핑해주는 것을 Shredding이라고 함
- 계층 구조를 위한 새로운 형태의 query language가 필요하고 대표적으로 XML의 query language XQuery가 있음
- ACID 트랜잭션 기능이 지원됨
- object-relational mapping

  ![Untitled 10](https://user-images.githubusercontent.com/48282185/188296120-7ae2ad92-6b68-45f3-b323-7f65cc8b18fd.png)

  - HTML과 relational database 사이의 변환 과정
  - document의 개입

    ![Untitled 11](https://user-images.githubusercontent.com/48282185/188296122-a85e2b51-0b39-4c0c-aece-e2f51537767d.png)

  - object-relational mapping은 현재 application에서 가장 복잡한 문제
  - java hibernate framework, ruby on rails 등이 이 문제를 위한 솔루션으로 등장했지만 완전히 해결할 수는 없었다, 중간 변환과정이 여전히 필요하다

- document store는 object-relational mapping이 필요 없다
  - document 자체를 데이터베이스에 저장하고 application에서 사용한다
  - 중간에 변환이 없어도 된다
  - No shredding, No reassembly
  - 간단함
- schema free
  - RDB에서는 데이터를 저장하기 위해 모델링(스키마 생성)이 반드시 필요한 절차였음
  - 스키마 없이 데이터 그 자체를 그대로 저장, 형태가 달라져도 상관없음
  - system이 자동적으로 로드되는 데이터들에 대한 인덱싱을 결정할 수 있음
  - 자료 형태에 대한 사전 지식이 없어도 됨
  - 데이터 모델링이 필요 없음
  - 데이터 element level에서 무엇이 추가되던 삭제되던 신경쓸 필요 없음
  - 데이터가 자유분방한 만큼 자료의 형태를 찾기는 힘듦, 스키마가 통계로 결정됨 ⇒ 제일 많이 들어온 스키마가 이 데이터의 스키마겠구나 유추 정도
- 대표 솔루션 - mongoDB
  - json data store 오픈 소스
  - master-slave scale out model
  - 커뮤니티 활성화
  - sharding이 내장되있고 자동임
  - C++로 구현되었고, 수 많은 언어에 API를 제공

## 10. Hybrid architectures

- 대부분의 real world 구현체들은 용도에 따라 NoSQL 솔루션들의 조합을 사용함
- 한 시스템의 구성 예시
  - data를 저장하기 위해 document store를 사용
  - image / pdf / binary를 저장하기 위해 S3를 사용
  - document index를 저장하기 위해 apache lucene(검색엔진 오픈소스)을 사용
  - real-time index와 집계 생성 및 유지 관리를 위해서 MapReduce를 사용한다
  - 분석을 위해 OLAP을 사용함
- ATAM

  - 카네기멜론 대학에서 만든 적합한 데이터베이스 시스템 솔루션 선택 방법론

    ![Untitled 12](https://user-images.githubusercontent.com/48282185/188296123-b551e60f-166d-4ec4-8f78-a53927eaf9b8.png)

> 출처
>
> - 이화여대 용환승 교수님 강의
> - 홍의경 저, MS SQL Server 기반 데이터베이스 배움터, 생능출판사, 2012년,
> - 이석호 저, 데이터베이스 시스템, 정익사, 2009.
> - A. Silberschatz, H. Korth, S. Sudarshan, "Database System Concepts," 6th Ed., McGraw-Hill, 2010.
> - Elmasri and Navathe, “ Fundamentals of Database Systems”, 6th ed. Addison-Wesley, 2010.
> - C.J.Date, “An Introduction to Database Systems (8th Edition)”, Addison-Wesley, 2003.
> - Won Kim, "Modern Database Systems," ACM Press, 1994
