# Project_HAL

<br>

## 프로젝트 소개
- **개발 기간**   
  2024.03.09 ~ 2024.06.24
- **개발 환경**   
  Unity 2022.3.21f
- **프로젝트 목적**   
  Unity Game Engine을 이용하여 간단한 탑뷰 2D 게임을 제작함으로써 게임 개발에 필요한 지식을 이해하는 것 목적으로 한다.
- **주요 기술 및 도구**   
  - Framework: Unity (C#)
  - Design Pattern : Entity-Component, SingleTone
- **멤버 구성**:
  개발 4명
- **기여 내용**:
  - 역할: PlayerEntity, EnemyEntity, VirtualCameraManager, UI, PickableObjects 설계 및 작성
  - 기여율: 25%

<br>

---

<br>

## 프로젝트 구조
```plaintext
├── Resources/
│   ├── Prefabs/
│   │   ├── UI/
│   │   │   ├── HPBar/
│   │   │   │   ├── BossHPBar.prefab                # 보스 몬스터의 HP Bar 프리팹
│   │   │   │   ├── EnemyHPBarUI.prefab             # 일반 몬스터의 HP Bar 프리팹
│   │   │   │   ├── PlayerHPBarUI.prefab            # 플레이어의 HP Bar 프리팹
│   │   │   ├── Inventory/
│   │   │   │   ├── InventoryUI.prefab              # 인벤토리 UI 프리팹
│   │   │   │   ├── SlotUI.prefab                   # 인벤토리 슬롯 UI 프리팹
│   │   ├── CoinObject.prefab                       # 맵에 드랍할 수 있는 Coin 프리팹
│   │   ├── Dummy.prefab                            # 더미(일반 몬스터) 프리팹
│   │   ├── HeartObject.prefab                      # 맵에 드랍할 수 있는 Heart 프리팹
│   │   ├── GameManager.prefab                      # GameManager 프리팹
│   │   ├── PlayerObject.prefab                     # 플레이어 객체 프리팹
│   ├── ScriptableObjects/
│   │   ├── Coin.asset                              # Item을 상속받아 구현한 Coin Item
│   │   ├── DummyHPManager.asset                    # StatManager를 상속받아 Dummy의 Stat을 구현한 StatManager
│   │   ├── Heart.asset                             # Item을 상속받아 구현한 Heart Item
│   │   ├── Item.cs                                 # Item을 정의한 ScriptableObject
│   │   ├── StatManager.asset                       # StatManager를 상속받아 플레이어의 Stat을 구현한 StatManager
│   │   ├── StatManager.cs                          # Entity Stat을 정의한 ScriptableObject
├── Scripts/
│   ├── Entity/
│   │   ├── Entity.cs                               # Entity의 최상위 클래스
│   │   ├── EnemyEntity.cs                          # Entity를 상속받아 적 몬스터의 초기화, 피격 및 공격 등에 관련한 코드
│   │   ├── PlayerEntity.cs                         # Entity를 상속받아 플레이어의 초기화, 이동, 애니메이션 재생 등에 관련한 코드
│   ├── Manager/
│   │   ├── GameManager.cs                          # 게임 매니저 관련 코드 (싱글톤 패턴)
│   │   ├── VirtualCameraManager.cs                 # 가상 카메라와 관련 변수와 함수들에 관련한 코드
│   ├── UI/
│   │   ├── HPBarUI.cs                              # 플레이어의 화면에 표시되는 HP UI
│   │   ├── InventorySlotUI.cs                      # 인벤토리 슬롯 하나에 해당하는 UI
│   │   ├── InventoryUI.cs                          # InventorySlotUI 여러개를 모은 하나의 Inventory UI. Inventory 상 아이템의 습득, 폐기, 이동에 관련한 코드
│   ├── PickableObjects.cs                          # 필드에 드랍할 수 있는 Item에 관련한 코드
├── SPUM/
│   ├── Res/
│   │   ├── Animation/
│   │   │   ├── Clip/
│   │   │   │   ├── *.anim                           # 캐릭터의 애니메이션 파일들
│   │   │   ├── AnimatinoNewController.controller    # 캐릭터의 애니메이션을 상태에 따라 제어

```

<br>

---

<br>

## 주요 기능 및 구조도
### 1. 캐릭터 이동 및 캔슬  
  ![기능 1 이미지](images/features1.gif)
- **설명**: 기능 1 설명
- **주요 기술**: 기능 1 주요 기술
- **구조도**:
  ![기능 1 구조도](images/features-flowchart1.png)

### 2. 상호작용   
  ![기능 2 이미지](images/features2.gif)
- **설명**: 기능 2 설명
- **주요 기술**: 기능 2 주요 기술
- **구조도**:
  ![기능 2 구조도](images/features-flowchart2.png)

### 3. 아이템 습득, 폐기 및 이동   
  ![기능 3 이미지](images/features3.gif)
- **설명**: 기능 3 설명
- **주요 기술**: 기능 3 주요 기술
- **구조도**:
  ![기능 3 구조도](images/features-flowchart3.png)

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
- **YouTube 링크**:   
  [![Project HAL Demo 영상](https://img.youtube.com/vi/RwQ4D90kwPk/0.jpg)](https://www.youtube.com/watch?v=RwQ4D90kwPk)
