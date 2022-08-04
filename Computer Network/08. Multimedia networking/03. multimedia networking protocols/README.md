# 3. multimedia networking protocols

<!--ts-->

- [3. multimedia networking protocols](#3-multimedia-networking-protocols)
  - [1. Dimensioning best effort networks](#1-dimensioning-best-effort-networks)
  - [2. Providing multiple classes of services](#2-providing-multiple-classes-of-services)
  - [3. Classification, conditioning](#3-classification-conditioning)
  - [4. Scheduling policies](#4-scheduling-policies)

<!-- Created by https://github.com/ekalinin/github-markdown-toc -->
<!-- Added by: sungminyou, at: 2022년 8월  4일 목요일 13시 19분 40초 KST -->

<!--te-->

## 1. Dimensioning best effort networks

- best effort, 기회가 있을 때마다 보낸다
- delay나 loss없이 멀티미디어 트래픽을 전달하기 위해서 충분한 link 용량을 알아내는 것이 중요
- bandwidth를 높은 경우, best effort방식을 이용하게 되면 비용이 많이 들어가기 때문에 타당한지 검토해야함
- 도전 과제
  - 얼만큼의 bandwidth가 충분하다고 할 수 있는지 알아내는 것
  - network traffic을 예측한다고 했을 때 얼만큼의 bandwidth를 확보해야 끊김없이 데이터를 전송할 수 있는지 알아내는 것
  - 즉, 주어진 자원내에서 multimedia traffic을 전달하는 방법이 연구 대상

## 2. Providing multiple classes of services

- 여러 개의 클래스를 둬서 각 서비스에 맞게 데이터를 전달하는 방식
- 단순 best effort방식으로 하기에는 streaming과 VoIP 등 각각의 요구 사항에 맞추기 힘들다
- 특별한 처리가 필요한 트래픽들을 세세하게 나누지 말고 특별한 그룹으로 묶어서 각 그룹의 요구 사항에 맞게 데이터를 전송한다
  - 그룹이라 하면, delay가 중요시 되는 그룹, loss가 중요시 되는 그룹, bandwidth가 중요시 되는 그룹 등이 있을 수 있다
- 90년대 초 까지는 각 연결에 대해 차등화를 뒀는데, 너무 복잡해서 클래스로 나눠서 접근하게 됐음
- IP패킷 헤더에 클래스 정보를 싣는다
  - IPv4에서는 ToS필드에, IPv6에서는 Traffic class필드에 적는다
  - 빨리 보내려고 급한 트래픽으로 설정할 수 있는데, 아무나 못하게 막는 로직이 필요

## 3. Classification, conditioning

![1](https://user-images.githubusercontent.com/48282185/182762242-3b1f5591-f48c-48d9-8401-02c772679772.png)

- user는 ISP계약 시 traffic profile을 정한다
  - 단위 시간 당 얼마나 보낼 건지(rate), 한꺼번에 얼마 정도를 보낼 건지(burst size)
- classifier는 패킷의 헤더를 보고 class를 구분짓는다
- meter는 user가 traffic profile에 정의한대로 잘 보내는지 측정
- marker는 meter에서 잘 보낸 패킷인지 아닌 건지 확인해주면 그에 걸맞는 마킹
- shaper / dropper는 마킹을 보고 traffic profile에 맞게 잘 보낸 패킷은 그대로 내보내고 아닌 패킷을 profile에 맞춰서 보내게 변경하고 버릴 건 버린다
- 소프트웨어로 구현돼있다

## 4. Scheduling policies

![2](https://user-images.githubusercontent.com/48282185/182762228-d0c40417-9079-4715-bdc0-f699c39ba935.png)

- 라우터의 output queue에서 이뤄지는 클래스 별 스케줄 정책
- weighted fair queuing(WFQ)
  - 일반적으로는 라운드 로빈
  - 각 패킷은 클래스에 따라 서로 다른 가중치를 갖는 queue에 들어가고, 각 queue는 각 사이클 마다 스케줄되는 가중치가 다르다
  - 위의 예시에서 파란 패킷들이 담긴 큐는 각 사이클마다 세 개, 주황은 두 개, 연두는 하나를 보낼 수 있다
- WFQ이외에도 QoS를 제공하기 위한 기법은 다양하다
