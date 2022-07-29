# 2. error detection

## 1. Error detection

![Untitled](https://s3-us-west-2.amazonaws.com/secure.notion-static.com/1adc433b-976b-474d-9e30-4240870271c7/Untitled.png)

- 100% 완벽하게 검출할 수 있는 것은 아님
- error를 놓칠 수 있으나 그럴 확률이 낮다
- 큰 EDC(Error Detection and Correction bits)일 수록 detection, 옳게된 correction 확률이 높아진다
  - 근데 너무 크면 배보다 배꼽이다
  - 상황에 따른 trade-off가 필요
    - 한 번 보내는데 오래 걸린다, 다시 보내고 싶지 않다, 오류 교정까지 시킬 수 있게 든든하게 실어서 보냄
    - 가까운 거리다, 다시 보내는 거 상관없다, 오류에 관대함

## 2. Parity checking

![Untitled](https://s3-us-west-2.amazonaws.com/secure.notion-static.com/d33e77af-4851-40ca-a792-8a3117e792c0/Untitled.png)

- 데이터를 이루는 비트들 0이나 1의 그룹과 parity bit를 합쳐 그룹의 1이나 0의 수를 짝수 개 혹은 홀수개로 맞춘다, 만약 가다가 중간에 데이터가 변질되면, 서로 합의했던 규칙에 어긋나므로 오류를 검출할 수 있다
- 예시로 1의 개수를 짝수개로 맞추도록 합의하고 데이터를 이루는 비트들에서 1의 개수가 홀수 개였다면 parity bit를 1로 둬서 받는 쪽에서도 1의 개수가 짝수 개인지를 확인하는 방법임, 근데 두 개 bit망가지면 짝수라 검출못함
- parity bit가 망가질 수도 있지만 확률이 낫기 때문에 안 망가졌다고 믿는다, 완벽할 수는 없다
- 2차원 parity-bit로 어디가 망가졌는지 확인할 수 있다
- 오류 검출을 적용하기에 앞서 신뢰성이 중요한지, 단순히 빨리 보내고 적게 보내는게 중요한지 trade-off를 따져볼 필요가 있다

## 3. Cyclic redundancy check(CRC)

![Untitled](https://s3-us-west-2.amazonaws.com/secure.notion-static.com/8c37ead8-a481-4525-8c03-3ef5ece89902/Untitled.png)

- d bits로 이뤄진 데이터 D
- r+1 bits로 이뤄진 비트 패턴 G(Generator)
- r bits의 CRC bits를 만들어낸다(R)
  - <D, R>은 G로 modulo 2연산을 하면 나머지가 0이다, 만약 0이 아니면 error라고 판단
  - r+1 bits보다 적은 오류난 비트를 검출할 수 있다
- 이더넷, wifi, ATM에서 사용됨
- D를 r bits만큼 shifting한다(D \* $2^r$)그리고 만들어낸 R을 쉬프팅한 자리에 복사(XOR)한다
- 식 정리
  ![Untitled](https://s3-us-west-2.amazonaws.com/secure.notion-static.com/cbe7dbfc-3037-43d6-a780-fe255a734720/Untitled.png)
  - $D * 2^r\ XOR\ R = nG$는 $D * 2^r = nG\ XOR\ R$과 동치이다
  - $D * 2^r$을 G로 나누었을 때 n이 몫이고, R이 나머지다라는 뜻임
  - $x\ XOR\ x = 0$
  - 나누기를 구현할 때 xor연산을 사용한 것임
- 예시
  - data - 1001
  - G - 1011
  - r = 3
  - 전송되는 데이터는 1001(D) | 110(R)
  - 오류가 없으면 받는 쪽에서 1001110을 1011(G)로 나누면 나머지가 0이다
  ![Untitled](https://s3-us-west-2.amazonaws.com/secure.notion-static.com/08f994cb-3737-4d2a-83e3-bdfc7f464349/Untitled.png)
  ![Untitled](https://s3-us-west-2.amazonaws.com/secure.notion-static.com/6aaeac6a-6bda-4056-a88e-ad06a62899d3/Untitled.png)
  - 주의할 점은 여기서 이진수 나눗셈을 하는게 아니라 2로 나눈 나머지를 구하는 것이다, 지금 위의 예시는 72를 11로 나눈 나머지를 구하는 것이지 72를 11로 나누는게 아니라는 말임(몫부터가 10이 나온다)
  - 기존에 알던 이진수 나눗셈 과정이 아니라 오로지 나머지를 구하기 위한 기법이라고 생각하자
  - 근데 과정이 나눗셈과 비슷해서 헷갈릴 수 있음
  - 각 스텝에서 나누려고 하는 수의 MSB가 1이면 몫이 1, MSB가 0이면 몫도 0, 첫 번째 스텝을 보자 1001을 1011로 나누려고 하면 1001의 MSB는 1이기에 몫도 1이되고, 두 번째 스텝에서 나누려고하는 수는 0100, MSB가 0이므로 몫도 0이 나온다 이런식으로 계속 끝날때까지 반복하고 최종 스텝에서 나온 두 수(0110, 0000)를 XOR한 결과가 72를 11로 나눈 나머지 6이 나온다
- 수신쪽의 복호화 과정
  ![Untitled](https://s3-us-west-2.amazonaws.com/secure.notion-static.com/10d70c3a-289e-41f0-800a-903f84a165e1/Untitled.png)
  - 전달받은 수를 정해진 G로 나누기한다
  - 나머지가 0이 나오면 정상이고, 0이 아니면 데이터가 잘못된 것이다
- hardware로 간단히 할 수 있는 검출법이기에 link layer에서는 대부분 checksum대신 CRC를 사용한다
