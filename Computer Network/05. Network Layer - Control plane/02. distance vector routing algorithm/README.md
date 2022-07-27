# 2. distance vector routing algorithm

<!--ts-->

- [2. distance vector routing algorithm](#2-distance-vector-routing-algorithm)
  - [1. Bellman-Ford equation](#1-bellman-ford-equation)
  - [2. Distance vector](#2-distance-vector)
  - [3. Distance vector example](#3-distance-vector-example)

<!-- Created by https://github.com/ekalinin/github-markdown-toc -->
<!-- Added by: sungminyou, at: 2022년 7월 27일 수요일 18시 29분 12초 KST -->

<!--te-->

## 1. Bellman-Ford equation

- $d_{x}(y)$를 x에서 y로 가는 최소 비용으로 정의
- c(x, y)는 x에서 y로 가는 비용
- v를 x의 모든 이웃들
- $d_{x}(y) = min(c(x,v) + d_{v}(y))$로 정의한다

## 2. Distance vector

- x 라우터의 distance vector를 $D_{x}$로 정의할 때 정의는 아래와 같다
- $D_{x} = [\ D_{x}(y): y \in N\ ]$(N은 네트워크의 라우터 집합)
- 하나의 라우터는 이웃들로 가는 비용들을 알고 있어야하고, 이웃들의 distance vector를 관리해야함
- 하나의 라우터가 메시지를 받으면, 이웃들에게 메시지를 전달하는 방식
  - 전체 네트워크를 파악하기 위한 정보를 획득하는데 시간이 많이 걸린다
    - link state도 마찬 가지로 정보를 얻는데는 이웃을 통해서 받겠지만, 한 번에 topology에 대한 정보를 받을 수 있어서 이웃들만으로 topology를 파악해 나가야하는 것이기 때문에 전체를 파악하는데는 오래 걸림
  - link state에 비해 정보량이 적음
  - 소규모 네트워크에서는 distance vector algorithm이 OSPF보다 옵션일 수 있다
- 알고리즘 trigger
  - Local link의 cost가 변경되었을 때
  - 이웃이 자신의 distance vector가 변경되었음을 알려올 때
    - 서로 변경될 때마다 트리거 되는 거면 무한 루프 돌지 않기 위해 어떤 장치가 필요할 듯

## 3. Distance vector example

![1](https://user-images.githubusercontent.com/48282185/181213247-25169952-95eb-4569-b029-6e86d0c4583e.png)
