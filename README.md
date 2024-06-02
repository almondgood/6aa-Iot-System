# 육아 IoT 프로젝트

## 프로젝트 개요
- 강의명 : IoT 데이터 수집 변환
- 강의 기간 : 2024/03/15(금) ~ 2024/04/29(월)
- 프로젝트 이름 : 육아 IoT 프로젝트
- 프로젝트 기간 : 2024/04/15(월) ~ 2024/04/21(일), 7일 간 제작

<br>

### 목표
1. 센서 값에 따라 모듈들이 자동으로 상호작용하는 스마트 IoT 시스템 개발
    - 배변 가스를 감지하여 센서 값을 전달하는 스마트 기저귀
    - 배변 감지 시 자동으로 온수를 작동시키고 방 내부 온도를 조절하는 스마트 보일러
    - 욕조의 물 온도와 수위를 측정하는 센서
2. Client가 Server에 센서 값을 전달, Server는 다른 단말기들에게 메시지 중개
3. 배변 감지 -> 보일러 작동 -> 수온 조절 및 수위 측정 알림의 과정을 자동화하여 편리성 도모

  <br>


### 팀원
- 나준환
- 정우택


<br><br>

## 프로젝트 환경

![image](https://github.com/almondgood/6aa-Iot-System/assets/88735581/39b69f05-3a06-4800-9491-6fd2de454196)


<br>

### 모듈 별 역할
|MQ6|Water Level|DS18B20|
|:---:|:---:|:---:|
|![MQ-6](https://github.com/almondgood/6aa-Iot-System/assets/88735581/b8c7673c-f035-442e-b19d-4d50b1a1d68d)|![image](https://github.com/almondgood/6aa-Iot-System/assets/88735581/e360b985-f7d9-49b0-aca7-527558e526f8)|![image](https://github.com/almondgood/6aa-Iot-System/assets/88735581/eefde5bc-67dd-4986-b62e-fe136fea2041)|
|**가스 센서**|**수위 센서**|**수온 센서**|
||||
|DHT11|ESP8266|I2C LCD|
|![image](https://github.com/almondgood/6aa-Iot-System/assets/88735581/562348df-d72b-4c60-bda8-79a877f30bd8)|![image](https://github.com/almondgood/6aa-Iot-System/assets/88735581/b1dbdbbb-7459-4f63-adb7-e385eb4fc774)|![image](https://github.com/almondgood/6aa-Iot-System/assets/88735581/02c7baa4-be12-4e71-853d-36b455a20123)|
|**온습도 센서**|**와이파이 모듈**|**센서 값 출력 LCD**|

<br>
<br>



## 프로젝트 과정

![image](https://github.com/almondgood/6aa-Iot-System/assets/88735581/ae0b861d-3c92-4df1-a649-4a1abf233a0d)



## 기능

### 1. 스마트 기저귀
- 실시간으로 가스 값을 감지한 결과를 서버에 전송

|![image](https://github.com/almondgood/6aa-Iot-System/assets/88735581/80022280-1645-4cad-8e22-b5d0da86f0dd)|![image](https://github.com/almondgood/6aa-Iot-System/assets/88735581/d662cae5-a1f5-42e0-b2e6-78c4ef9c8d95)|
|:---:|:---:|
|**하드웨어 구성도**|**가스 센서 읽기 코드**|

<br>

### 2. 수위 및 수온 측정 센서
- 배변 활동 감지 시 센서 작동
- 추후 수도꼭지를 자동으로 작동시키는 기능 추가 예정

|![image](https://github.com/almondgood/6aa-Iot-System/assets/88735581/e3bb6dda-eaad-42d1-b16d-af93c7a21478)|![image](https://github.com/almondgood/6aa-Iot-System/assets/88735581/e4423a97-5770-4f73-94d4-511b073bd6b4)|
|:---:|:---:|
|**하드웨어 구성도**|**수위 및 수온 읽기 코드**|

<br>

### 3. 스마트 보일러
- 배변 활동 감지 시 보일러 작동 및 실내 온도 조절

|![Untitled Sketch_bb](https://github.com/almondgood/6aa-Iot-System/assets/88735581/184dda65-19b9-4a1c-af5e-a92919c20b18)|![image](https://github.com/almondgood/6aa-Iot-System/assets/88735581/d620440c-9cc9-4fc1-a263-67f0de33356b)![image](https://github.com/almondgood/6aa-Iot-System/assets/88735581/30d533c1-0a5b-4353-9b6a-f71d8a2dbddf)|
|:---:|:---:|
|**하드웨어 구성도**|**온습도 읽기 및 명령 수신부 코드**|

<br>

### 4. IoT 서버
- 서버는 각 모듈로부터 메시지를 수신받아 보내고자 하는 대상에게 메시지 전달
- SQL 클라이언트는 메시지를 송신자, 명령부, 인수부로 나누어 해당 모듈의 DB 테이블에 로그 저장

|![image](https://github.com/almondgood/6aa-Iot-System/assets/88735581/5d35923a-7bcf-491c-aa8c-edd9394e59ee)|![image](https://github.com/almondgood/6aa-Iot-System/assets/88735581/a21d29a8-db72-4b3b-851f-25fb3ba159ed)|
|:---:|:---:|
|**IoT Server**|**SQL Client**|

<br>

### 로그 저장

|![image](https://github.com/almondgood/6aa-Iot-System/assets/88735581/671952d3-ddb5-4354-8bd1-30902d89e91e)|![image](https://github.com/almondgood/6aa-Iot-System/assets/88735581/250ebf07-22e3-4ee5-9109-0d287b9f3623)|![image](https://github.com/almondgood/6aa-Iot-System/assets/88735581/0d51a648-baad-4b29-8c4d-00c282d3d334)|
|:---:|:---:|:---:|
|가스 센서 값|수온 및 수위 센서 값|보일러 온습도 값|

<br><br>

## 결론
### 기대 효과
- 방 온도 확인, 배변 감지, 목욕 물 관리로 육아에 대한 부담을 덜 것으로 기대
- 새로운 모듈을 추가할 수 있도록 확장성 유지
- 데이터를 이용 가능하도록 수신 데이터 로그 저장

<br>

### 추후 계획
- 기저귀에서 배변 감지 시 목욕 물을 자동으로 받고, 온도를 조절하도록 실제 작동
- 로그 데이터를 활용하여 사용자에게 최적화된 동작 패턴 제공
- 아이가 울 때 모빌을 자동으로 작동시키는 등 다양한 기능으로 확장 가능










