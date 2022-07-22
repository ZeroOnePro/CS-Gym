# 08. TCP congestion control example

## 1. problem 1

![1](https://user-images.githubusercontent.com/48282185/180398086-d760676f-ff99-41a7-acae-b4d7e2f71e82.png)

- transmission delay가 무시할 정도로 작으니 RTT에서 고려할 필요 없다
- TCP buffer가 충분히 크므로 flow control에 고려할 필요 없다
- 원래는 delay ACK이지만 받을 때마다 ACK을 보낸다고 가정

1. 몇 개의 segment가 만들어지는가?
   1. 51200 bytes / 1024 bytes = 50개
2. slow start phase에는 몇 개의 segment를 전송할 수 있는가?
   1. ssthreshold가 8이므로, 1 + 2 + 4 + 8 = 15개 → 3RTT
3. congestion avoidance phase에는 몇 개의 segment를 전송할 수 있는가?
   1. 총 50개에서 15개를 제외한 35개가 남았고, 9 + 10 + 11 + 5 = 35이므로, 3RTT에 다 보낼 수 있다
4. 전송에 걸리는 총 시간은?
   1. 3RTT(slow start phase) + 3RTT(congestion avoidance phase) + 1RTT(남은 5개 보냄) = 7RTT이고, 데이터를 정상적으로 보낸것을 확인 받아야하므로 1RTT를 더 필요로 하게되어 총 8RTT가 지나야 전송이 완료됬다고 볼 수 있다

## 2. problem 2

![2](https://user-images.githubusercontent.com/48282185/180398082-cbabed2d-1dbf-448a-8ae1-451c57bc5213.png)

- sequence diagram을 그려본다

  ![3](https://user-images.githubusercontent.com/48282185/180398079-ac16e464-5bce-48d7-87a8-49ed427fbe71.png)

  - 임의로 초기 sn을 3001로 지정, mss = 1024bytes임
  - 4025가 버려졌기때문에 receiver는 5049를 받으면 4025를 ACK으로 보내고, base에 대한 ACK이 안왔기에 sender의 window는 sliding할 수 없다(데이터를 더 보낼 수 없다), 그래서 3 duplicated ACK이 발생할 수 없음, 결론적으로 손실 인지 event는 timer expiry라고 볼 수 있음

## 3. problem 3

![4](https://user-images.githubusercontent.com/48282185/180398075-9f3b55b5-5a1e-4db1-bdbe-d1f8ec3d014d.png)

- 마찬가지로 sequence diagram을 그려보자

  ![5](https://user-images.githubusercontent.com/48282185/180398070-ee11fb83-d22f-4d4b-8f3e-cb33f33d12ca.png)

  - sn=6073에 대한 응답 ack=7097이 잘 왔으므로 sender는 윈도우를 슬라이딩한다, ack하나 당 윈도우 크기가 2씩 늘어나므로 sn=10169, 11193 데이터가 보내지고 총 네 개의 duplicated ACK을 받는다, 그리고 윈도우가 슬라이딩 됬기에 ACK=7097을 처음받는 시점에 sn=7097에 대한 타이머가 새로 세팅되고, expiry timer는 5RTT이기에 dupliacted ACK이 먼저 생길 수 밖에 없다
  - 결론적으로 duplicated ACK event로 손실 감지

## 4. TCP congestion control summary

![6](https://user-images.githubusercontent.com/48282185/180398065-da25bb3f-4e42-41a3-80c6-08c82ee0b418.png)

- TCP Reno

## 5. TCP throughput

![7](https://user-images.githubusercontent.com/48282185/180398063-e9eb9f87-1f19-4310-a19e-3c01c00ed9c9.png)

- window size(W)에서 loss가 일어나는 특별한 상황을 가정
- 평균 window size는 ${{w + {w \over 2}} \over 2} = {3w\over4}$
- throughput은 단위 시간 당 데이터의 처리량(window size / RTT)이므로, 평균 throughput은 매 RTT 마다 3w / 4
- avg TCP throughput = $3w \over 4 \times RTT$bytes/sec이다

## 6. TCP over “long, fat pipes”

- 요즘과 같은 10Gbps 링크를 가진 환경에서 TCP Reno에서 사용하는 slow start와 congestion avoidance가 효용이 있을까?
- 만약 RTT가 100ms라고 가정하고 1500bytes의 세그먼트 크기를 갖는다고 할 때 window size는 83,333이 되야함 → 10Gbits / 1500 \* 8bits
- 손실률은 10억분의 2어서 매 50억개의 패킷들 중 1개가 버려지는 것, 시간으로 바꾸면 ${1}{2\over3}$시간 마다 1개가 버려짐 → 조금 더 aggressive하게 증가를 시켜야한다
- high-speed를 위한 TCP에서는 줄이는 건 조금, 늘리는 건 많이한다

## 7. TCP Faireness

![8](https://user-images.githubusercontent.com/48282185/180398039-1801d9b1-c79b-4754-993a-f8d2895c7a4e.png)

- 두 개의 TCP 연결 세션이 bottle neck link를 공유하고 있는 상황
- R은 bottle neck link의 처리 용량이므로 두 개의 세션의 처리량이 R을 넘게되면 손실이 발생(파란 직선)
- TCP Reno의 congestion avoidance가 서로 다른 두 개의 처리량으로 시작했던 연결의 처리량이 같아지게 수렴시켜주더라, 공평해진다

이런게 있다를 공부하지 말고, 이러이러한 특징이 있으니 현실 문제에 대입해보면 어떻게 동작할까?에 대해서 사고해보자
