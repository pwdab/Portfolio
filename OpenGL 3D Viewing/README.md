# OpenGL 3D Viewing

<br>

## 프로젝트 소개
- **개발 기간**   
  2023.05.15 ~ 2023.05.22   
- **개발 환경**   
  Visual Studio 2022   
  OpenGL 4.6   
- **프로젝트 목적**   
    서강대학교의 기초컴퓨터그래픽스 과목을 수강하며 학습한 내용을 바탕으로, OpenGL에서 3D Viewing을 진행해봄으로써 래스터화 기반의 실시간 랜더링 파이프라인에 관한 이해도를 높인다.
- **주요 기술 및 도구**
  - OpenGL 4.6
  - BistroExterior Texture PS GLSL (3D 장면 데이터)
- **개발 내용**   
    CallBack.cpp, Camera.cpp, DrawScene.cpp 작성


<br>

---

<br>

## 프로젝트 구조
```plaintext
├── data/
│   ├── dynamic_objects/    # 동적 3D 오브젝트들을 저장
│   ├── static_objects/     # 정적 3D 오브젝트들을 저장
│   ├── Data.h
│   ├── Data.cpp            # 해당 objects를 읽어오는 read_geometry() 함수 정의
├── geometry/               # BistroExterior Texture PS GLSL 3D 장면 데이터
├── src/
│   ├── CallBack.h
│   ├── CallBack.cpp        # 콜백 함수 정의
│   ├── Camera.h
│   ├── Camera.cpp          # 카메라 이동 및 회전 관련 함수 정의
│   ├── DrawScene.h
│   ├── DrawScene.cpp       # OpenGL 초기화 관련 함수 정의
│   ├── Geometry.h
│   ├── Geometry.cpp        # Geometry 관련 행렬과 변수 선언
│   ├── LoadScene.h
│   ├── LoadScene.cpp       # Scene을 불러오거나 메모리를 해제
│   ├── main.cpp            # LoadScene을 이용해 Scene을 불러옴
│   ├── Shader.h
│   ├── Shader.cpp          # 쉐이더 관련 함수 정의
│   ├── ShadingInfo.h       # 빛과 머티리얼 관련 구조체 변수 선언
```

<br>

---

<br>

## 주요 기능 및 구조도
### 1. 물체의 배치 및 움직임   
  ![기능 1 이미지1](images/features1.png)
  <p align="center">
    <img src="images/features1-1.gif" alt="기능 1 이미지2" width="49%">
    <img src="images/features1-2.gif" alt="기능 1 이미지3" width="49%">
  </p>
  
- **설명**:   
  맵에 tiger, ben, wolf 총 3개의 동적 오브젝트와 bike, bus, cow , dragon, dogzilla 총 5개의 정적 오브젝트가 배치되어 있다. 동적 오브젝트는 각자 고유한 경로를 따라 움직인다. 그 중 호랑이는 키보드 'r' 키를 눌러 움직임과 멈춤을 조절할 수 있다. 
- **주요 기술**: 기능 1 주요 기술
- **구조도**:
  ![기능 1 구조도](features-structure1.png)

### 2. 가상 카메라의 배치 및 조절 기능   
  <p align="center">
    <img src="images/features2-1.gif" alt="기능 2 이미지1" width="49%">
    <img src="images/features2-2.gif" alt="기능 2 이미지2" width="49%">
  </p>
  
- **설명**:   
  세상을 관찰할 수 있는 총 4개의 고정된 가상 카메라가 맵에 배치되어 있다. 각각의 가상 카메라는 키보드 'u', 'i', 'o', 'p'를 눌러 전환할 수 있다. 이때 키보드 'CTRL' 키를 누른 상태에서 마우스 스크롤 휠을 이용해 줌-인과 줌-아웃할 수 있다.
  ![기능 2 이미지1](images/features2-3.gif)
- **설명**:   
  세상을 자유롭게 이동할 수 있는 1개의 가상 카메라가 맵에 배치되어 있다. 이 가상 카메라는 키보드 'a'키를 눌러 전환할 수 있다. 이때 키보드 좌/우 키를 누르면 카메라가 좌/우로 이동한다. 또한 키보드 상/하 키를 누르면 카메라가 전/후로 이동한다. 마지막으로 키보드 'z'/'c' 키를 누르면 카메라가 상/하로 이동한다.
  ![기능 2 이미지2](images/features2-4.gif)
- **설명**:   
  세상을 자유롭게 이동할 수 있는 가상 카메라는 회전도 가능하다. 키보드 'shift' 키를 누른 상태에서 좌/우 키를 누르면 카메라가 좌/우(Yaw)로 회전한다. 또한 키보드 'shift' 키를 누른 상태에서 상/하 키를 누르면 카메라가 전/후(Pitch)로 회전한다. 마지막으로 키보드 'shift'키를 누른 상태에서 'z'/'c' 키를 누르면 카메라가 좌/우(Pitch)로 회전한다.
  ![기능 2 이미지3](images/features2-5.gif)
- **설명**:   
  호랑이의 눈에 배치된 가상 카메라와 호랑이의 뒤에 배치된 가상 카메라가 맵에 배치되어 호랑이의 움직임을 따라다니고 있다. 각각의 가상 카메라는 키보드 't'와 'g'를 눌러 전환할 수 있다. 
- **주요 기술**: 기능 2 주요 기술
- **구조도**:
  ![기능 2 구조도](features-structure2.png)

<br>

---

<br>

## 이슈 및 해결 과정
### 1. 이슈 1   
  ![이슈 1 이미지](issues1.png)
- **문제**: 이슈 1 설명
- **해결 과정**:
  1. `file.cpp` 과정 1
  2. `file.cpp` 과정 2
     ```c++
     // this is c++ code.
     ```

### 2. 이슈 2
  ![이슈 2 이미지](issues2.png)
- **문제**: 이슈 2 설명
- **해결 과정**:
  1. `file.cpp` 과정 1
  2. `file.cpp` 과정 2
     ```c++
     // this is c++ code.
     ```

### 3. 이슈 3
  ![이슈 3 이미지](issues3.png)
- **문제**: 이슈 3 설명
- **해결 과정**:
  1. `file.cpp` 과정 1
  2. `file.cpp` 과정 2
     ```c++
     // this is c++ code.
     ```

<br>

---

<br>

## 데모
- **YouTube 링크**: [프로젝트 데모 보기](https://www.youtube.com/watch?v=video-id)
- **스크린샷**:
  ![데모 스크린샷](link-to-screenshot.png)
