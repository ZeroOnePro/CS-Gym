# 2. Network edge

<!--ts-->

- [2. Network edge](#2-network-edge)
  - [1. A closer look at network structure](#1-a-closer-look-at-network-structure)
  - [2. Access networks and physical media](#2-access-networks-and-physical-media)

<!-- Created by https://github.com/ekalinin/github-markdown-toc -->
<!-- Added by: sungminyou, at: 2022년 7월  2일 토요일 01시 44분 39초 KST -->

<!--te-->

## 1. A closer look at network structure

- Network edge
  - Hosts: client, server
  - server는 주로 데이터 센터에 위치
- Access Network(네트워크에 접속할 수 있는 방법), physical media
  - wired, wireless commnunication link
- Network core(메시지를 전달하는 핵심 개체들)
  - 상호 연결된 라우터들
  - 네트워크의 네트워크

## 2. Access networks and physical media

- 어떻게 엔드시스템들과 엣지 라우터(다른 네트워크 그룹과 경계선에 위치한 즉 다른 네트워크와 연결 고리)를 연결하는가?
  - 모바일 엑세스 환경(셀룰러, 와이파이)
  - 기관 엑세스 환경(학교, 병원, 기관 등)
  - 주거 엑세스 환경(집 내의 네트워크 환경)
- 고려해 볼 것
  - 엑세스 네트워크의 bandwidth(bits / second)
  - 공유되느냐 전용으로 사용되느냐?
