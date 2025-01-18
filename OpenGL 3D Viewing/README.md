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
  ![기능 1 이미지](images/features1.png)
- **설명**: 기능 1 설명
- **주요 기술**: 기능 1 주요 기술
- **구조도**:
  ![기능 1 구조도](features-structure1.png)

### 2. 가상 카메라의 배치 및 조절 기능   
  ![기능 2 이미지1](images/features2-1.gif)
  ![기능 2 이미지2](images/features2-2.gif)
  ![기능 2 이미지3](images/features2-3.gif)
  ![기능 2 이미지4](images/features2-4.gif)
- **설명**: 기능 2 설명
- **주요 기술**: 기능 2 주요 기술
- **구조도**:
  ![기능 2 구조도](features-structure2.png)

### 3. 기능 3   
  ![기능 3 이미지](features3.png)
- **설명**: 기능 3 설명
- **주요 기술**: 기능 3 주요 기술
- **구조도**:
  ![기능 3 구조도](features-structure3.png)

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
