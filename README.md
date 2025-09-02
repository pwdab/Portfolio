# 김윤성의 포트폴리오

<table>
  <tr>
    <td>
      <a id="index"></a> 목차 <br><br>
      &nbsp;&nbsp;<a href="#introduction"> 1. Introduction </a><br>
      &nbsp;&nbsp;<a href="#technical-stacks"> 2. Technical Stacks </a><br>
      &nbsp;&nbsp;<a href="#team-projects"> 3. Team Projects </a><br>
      &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="#project-hal"> 3-1. Project_HAL </a><br>
      &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="#The First Foreigner"> 3-2. The First Foreignere </a><br>
      &nbsp;&nbsp;<a href="#contact"> 3. Contact </a><br>
    </td>
  </tr>
</table>
<br>

# <a id="introduction"> [1. 👋 Introduction](#index)
안녕하세요, 게임 클라이언트 프로그래머를 지망하고 있는 김윤성이라고 합니다.   

어릴 적부터 게임은 제 삶의 큰 즐거움이자 영감을 주는 원천이었습니다.   
게임이 선사하는 행복과 아름다움에 매료되어 자연스럽게 게임 개발이라는 길을 선택하게 되었습니다.   
아직 부족한 점도 많지만, 끊임없이 배우고 성장하며 더 많은 사람들에게 기쁨과 감동을 전할 수 있는 게임을 만들고 싶습니다.  
## 🏷️ 이름
김윤성 (Kim Youn Sung)   

## 💼 직무
C++ 게임 클라이언트 프로그래밍
  
## 🎓 학력   
2019.02 ~ 2026.02   
서강대학교 컴퓨터공학과 졸업예정   

2014.02 ~ 2017.02   
영동일고등학교 졸업   

## 🏃 활동   
2025.02.07 ~ 2025.02.10   
메이플 대학생 게임 커리어 캠프, 메토링 2기 수료   

<br>

# <a id="technical-stacks"> [2. 💻 Technical Stacks](#index) 
### <img src="images/unreal_logo(w).png" width="20" align="absmiddle"/> Unreal Engine 5

- Listen Server 기반 3D 멀티플레이 게임 개발 및 출시
- Unreal Gameplay Framework 이해 및 C++ API 활용
- Replication·RPC를 활용한 네트워크 동기화 및 클라이언트·서버 상호작용 구현   
- Animation Blueprint·State Machine·Montage를 활용한 애니메이션 제어
- Actor Component·Delegate·Notify를 활용한 모듈형·이벤트 중심 시스템 설계
- UMG를 활용한 동적 UI 제작 및 데이터 연동
- Steamworks API·OnlineSubsystem를 활용한 세션 생성 및 관리
### <img src="https://cdn.sanity.io/images/fuvbjjlp/production/aa5ce86e0add266e09ffa222609af4cc94befc7b-250x282.png" width="20" align="absmiddle"/> Unity 
- 2D 싱글플레이 게임 개발 경험
- Unity Entity-Component 구조 이해
- Sprite·Animator Controller를 활용한 애니메이션 전환 구현
- Collider2D를 활용한 캐릭터·오브젝트 상호작용 처리
- Canvas·UI를 활용한 동적 UI 및 인벤토리 시스템 구현
### <img src="images/C++_logo.png" width="20" align="absmiddle"/> C/C++   
- OpenGL API를 활용한 3D Camera Viewing System 구현
- PintOS 환경에서 User Stack 구조 및 Thread Scheduling 구현
- Linux 환경에서 프로세스 간 통신 기반 주식 거래 시뮬레이터 개발 및 동시성 문제 해결

<br>

# <a id="team-projects"></a> [3. 👥 Team Projects](#index)
# <a id="project-hal"></a> [3-1. Project_HAL](#index)  
<div align="center">
  <img src="images/Project_HAL.png" alt="Project_HAL" width="75%">
</div>

<p align="center">
    <img src="images/Project_HAL/features4.gif" width="32%">
    <img src="images/Project_HAL/features5.gif" width="32%">
    <img src="images/Project_HAL/features6.gif" width="32%">
  </p>

<br>

## 📌 프로젝트 소개
- **개발 기간**   
  2024.03.09 ~ 2024.06.24
- **개발 상태**   
  데모 빌드 배포 완료 (개발 종료)   
- **개발 환경**   
  Unity 2022.3.21f   
  Windows 10 (64bit)
- **프로젝트 개요**   
  탑뷰 2D 싱글플레이 어드벤처 게임   
  플레이어는 기본 공격과 카드 드로우 기반 스킬로 몬스터를 처치하고 스테이지를 클리어
- **프로젝트 목적**   
  Unity 프레임워크 학습 및 2D 게임 개발 워크플로우 이해  
- **멤버 구성**   
  프로그래밍 4명

<br>

## 🎯 담당 업무
- Sprite Atlas 기반 2D 애니메이션 제작 및 Animator Controller 상태 전환 구현
- Canvas·UI 컴포넌트를 활용한 동적 UI 구현 및 동기화
- Collider2D 기반 캐릭터-오브젝트 상호작용 처리
- 아이템 획득·폐기·이동이 가능한 인벤토리 시스템 구현

<br>

## 🛠 문제 해결 사례   
### 캐릭터 애니메이션 전환 로직 개선   
- **문제**   
  간헐적으로 캐릭터의 애니메이션(ex. 공격)이 재생되지 않는 현상 발생
  <div align="center">
    <img src="images/Project_HAL/issues1.gif" width="25%">   
  </div>
  
- **원인**   
  루프로 설정된 Idle 애니메이션의 진행도(normalizedTime) 값이 1.0f를 넘어 누적   
  Animator의 상태가 실제로 전환되기도 전에 애니메이션 종료 조건이 먼저 충족되어 다음 루틴이 실행되지 않음
```C#
// Before
public IEnumerator Attack(Animator animator)
{
  ︙
    // 캐릭터의 상태를 "Attack"으로 바꿈
    if (!animator.GetCurrentAnimatorStateInfo(0).IsName("2_Attack_Bow"))
    {
        animator.SetInteger(animationState, (int)AnimationStateEnum.attack);
    }

    // state가 아직 "Attack"으로 바뀌지 않았음에도 이전에 재생 중이던 Idle의 normalizedTime이 1.0f을 넘으면 아래의 코드가 작동하지 않음
    while (animator.GetCurrentAnimatorStateInfo(0).normalizedTime < 1.0f)
    {
        // 현재 애니메이션이 종료될 때까지 기다림
    }
  ︙
}
```
- **해결**   
  Animator의 상태가 확실히 바뀌도록 대기하는 로직을 추가
```C#
// After
public IEnumerator Attack(Animator animator)
{
  ︙
    // 캐릭터의 상태를 확실히 "Attack"으로 바꿀 때까지 기다림
    while (!animator.GetCurrentAnimatorStateInfo(0).IsName("2_Attack_Bow"))
    {
        animator.SetInteger(animationState, (int)AnimationStateEnum.attack);
        yield return null;
    }

    // state가 확실히 "Attack"으로 바뀌었으므로 normalizedTime은 0.0f부터 시작
    // 따라서 아래의 코드가 정상적으로 작동함
    while (animator.GetCurrentAnimatorStateInfo(0).normalizedTime < 1.0f)
    {
        // 현재 애니메이션이 종료될 때까지 기다림
    }
  ︙
}
```
- **결과**   
  애니메이션이 정상적으로 재생되지 않던 문제 해결   
  애니메이션 상태 전환에서 안정성을 확보함으로써 추후 애니메이션의 후딜레이를 캔슬하는 기능으로 확장해 조작감과 전투 흐름을 개선
  <div align="center">
    <img src="images/Project_HAL/issues3.gif" width="25%">   
  </div>

<br>

## 🎮 데모 플레이   
- **플레이 방법**   
  - <img src="https://upload.wikimedia.org/wikipedia/commons/1/12/Google_Drive_icon_%282020%29.svg" width="15" align="absmiddle"/> [구글 드라이브](https://drive.google.com/file/d/1hhAQobi0zfsc5SucmJjASzMnFuvB9EYg/view?usp=sharing)에서 Project_HAL_Demo.zip 파일을 다운로드 후 압축 해제   
  - HALNENG.exe 파일 실행   
- **플레이 영상**   
  - 아래 썸네일을 클릭하면 <img src="https://upload.wikimedia.org/wikipedia/commons/0/09/YouTube_full-color_icon_%282017%29.svg" width="15" align="absmiddle"/> YouTube로 이동합니다.
    <div align="center">
      <a href="https://youtu.be/5klQiKKPS54">
        <img src="https://img.youtube.com/vi/5klQiKKPS54/0.jpg" width="50%">
      </a>
    </div>
    
<br>

## 📖 참고 자료
- Halpern, J. (2016). *Learn Unity 2D Game Development with C#*. 김홍중 옮김. 서울: 에이콘. (2020).
- Unity Official Documentation (Unity Manual & Scripting API)  

<br>

## 🔗 추가 정보
- 상세 내용 및 구현 코드는 <img src="https://upload.wikimedia.org/wikipedia/commons/9/91/Octicons-mark-github.svg" width="15" /> [GitHub Repository](https://github.com/pwdab/Project_HAL)에서 확인 가능합니다.

<br>

# <a id="The First Foreigner"></a> [3-2. The First Foreigner](#index)     
<div align="center">
  <img src="images/The First Foreigner.png" alt="The First Foreigner" width="75%">
</div>

## 📌 프로젝트 소개
- **개발 기간**   
  2024.09.10 ~ 2024.12.06   
  2025.05.19 Steam 업로드 완료
- **개발 환경**   
  Unreal 5.2.1   
  Windows 10 (64bit)
- **프로젝트 개요** 
  3D 멀티플레이 캐주얼 게임
  두 명의 플레이어가 번갈아 제시어를 행동이나 사물로 표현하고, 상대가 이를 추리해 맞춤   
- **프로젝트 목적**   
  Unreal Engine 5 프레임워크 이해 및 클라이언트-서버 네트워크 게임 개발   
  게임 패키징·배포 및 유저 피드백 기반 개선 경험   
- **멤버 구성**
  - 기획 및 레벨 디자인 1명   
  - 프로그래밍 1명

- **담당 업무**
  - Replication 및 RPC를 통해 클라이언트-서버 간 데이터 동기화 및 명령 전파를 구현   
  - Animation Blueprint, Montage, AnimNotify를 사용하여 애니메이션 전환을 제어   
  - Interface 및 Actor Component를 활용하여 유연하고 재사용 가능한 시스템을 설계   
  - Widget Blueprint를 활용하여 게임 로직과 연동된 동적 UI 시스템을 구현
  - Steamworks API를 활용하여 Steam과 연동 및 게임 매칭 시스템 구현
- **주요 기술 및 도구**   
  - **Framework**   
    - Unreal (C++, Blueprint)
    - Steamworks
  - **Network**   
    - Replication, RPC, OnlineSubsystem   
  - **UI**   
    - UMG   

### **주요 기능 및 이미지**
  - 게임 Session 생성 및 참가   
    <img src="images/Squire/features1.gif" alt="Squire 이미지1" width="85%">
    
  - 캐릭터 시선 처리   
    <img src="images/Squire/features2.gif" alt="Squire 이미지1" width="85%">
    
  - 게임 흐름 제어와 데이터 동기화   
    <img src="images/Squire/features3.gif" alt="Squire 이미지1" width="85%">

## 🎮 데모 플레이
- **플레이 방법**
  - [Steam](https://store.steampowered.com/app/3634090/The_First_Foreigner/)에서 게임을 다운로드 받고, 실행합니다.
- **YouTube 링크**
  - 아래 썸네일을 클릭하면 YouTube 데모 영상으로 이동합니다.   
  [![The First Foreigner 데모 영상](https://img.youtube.com/vi/AIy8zwr5r8M/0.jpg)](https://www.youtube.com/watch?v=AIy8zwr5r8M)

### 참고 서적
- Multiplayer Game Development with Unreal Engine 5 – Maro Secchi  
- 이득우의 언리얼 C++ 게임 개발의 정석 – 이득우  

### More
  - 이 프로젝트에 대해 더 자세한 내용과 구현 방법은 [여기](https://github.com/pwdab/Squire)에서 보실 수 있습니다.

---

# <a id="contact"></a> [3. Contact](#index)
## &nbsp;&nbsp;E-Mail
&nbsp;&nbsp;&nbsp;&nbsp;soundno07@naver.com   
## &nbsp;&nbsp;Git
&nbsp;&nbsp;&nbsp;&nbsp;[Git](https://github.com/pwdab)   
## &nbsp;&nbsp;YouTube
&nbsp;&nbsp;&nbsp;&nbsp;[YouTube](https://www.youtube.com/@%ED%8F%89%EB%8B%A4%EB%B2%94)
