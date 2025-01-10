import React, { useState } from "react";

const formatNumber = (num) => {
  if (num >= 100000000) {
      const 억 = Math.floor(num / 100000000);
      const 만 = Math.floor((num % 100000000) / 10000);
      const 나머지 = num % 10000;
      return `${억}억${만 > 0 ? ` ${만}만` : ""}${나머지 > 0 ? ` ${나머지}` : ""}`;
  } else if (num >= 10000) {
      const 만 = Math.floor(num / 10000);
      const 나머지 = num % 10000;
      return `${만}만${나머지 > 0 ? ` ${나머지}` : ""}`;
  } else {
      return `${num}`;
  }
};

const Home = () => {
  const [nickname, setNickname] = useState(""); // 닉네임 입력값 상태
  const [battlePower, setBattlePower] = useState(null); // 전투력 상태
  const [bossEarnings, setBossEarnings] = useState(null); // 보스 수익 상태
  const [selectedBosses, setSelectedBosses] = useState([]); // 계산된 보스 목록
  const [error, setError] = useState(null); // 에러 메시지 상태

  // 서버 API 호출
  const fetchCharacterData = async () => {
    setError(null); // 기존 에러 초기화
    setBattlePower(null);
    setBossEarnings(null);
    setSelectedBosses([]);

    try {
      // 캐릭터 정보 API 호출
      const response = await fetch(`https://e85c-163-239-4-185.ngrok-free.app/api/character/${nickname}`);
      if (!response.ok) {
        throw new Error(`캐릭터를 찾을 수 없습니다.<br>2023년 12월 21일 이후의 접속 기록이 없거나, 정지된 캐릭터일 수 있습니다.`);
      }
      
      const data = await response.json();
      setBattlePower(data.battlePower);
      //console.log("캐릭터 정보 조회");
      //console.log(data);
      //console.log(battlePower);

      setBossEarnings(data.totalCost);
      //console.log("보스 수익 조회");
      //console.log(bossEarnings);

      setSelectedBosses(data.selectedBosses);

    } catch (err) {
      setError(err.message);
    }
  };

  return (
    <div style={{ textAlign: "center", marginTop: "50px" }}>
      <h1>주간 보스 수익 계산기</h1>
      <p>솔로 플레이만 가정, 월간 보스 미포함, 현재 전투력 기준 상위 12개의 보스만 측정</p>
      <input
        type="text"
        value={nickname}
        onChange={(e) => setNickname(e.target.value)}
        placeholder="닉네임을 입력하세요"
        style={{ padding: "10px", width: "300px" }}
      />
      <button
        onClick={fetchCharacterData}
        style={{
          marginLeft: "10px",
          padding: "10px 20px",
          backgroundColor: "#4CAF50",
          color: "white",
          border: "none",
          cursor: "pointer",
        }}
      >
        Calculate
      </button>

      {error && (
          <p
              style={{ color: "red", marginTop: "20px" }}
              dangerouslySetInnerHTML={{
                  __html: error.replace(/\n/g, "<br>"), // \n을 <br>로 변환
              }}
          />
      )}

      {battlePower !== null && (
        <div style={{ marginTop: "20px", display: "flex", justifyContent: "center", alignItems: "center", gap: "50px" }}>
            <div style={{ textAlign: "center" }}>
                <h2>현재 전투력</h2>
                <p>{battlePower !== null ? formatNumber(battlePower) : "N/A"}</p>
            </div>
            <div style={{ textAlign: "center" }}>
                <h2>예상 최대 주간 보스 수익</h2>
                {/*<p>{bossEarnings !== null ? `${formatNumber(bossEarnings)} 메소` : "N/A"}</p>*/}
                <p>{bossEarnings !== null ? `${bossEarnings.toLocaleString()} 메소` : "N/A"}</p>
            </div>
        </div>
      )}

      {bossEarnings !== null && (
        <div style={{ marginTop: "20px" }}>
          <h3>예상 격파 보스 목록</h3>
          {selectedBosses.length === 0 ? (
            <p style={{ textAlign: "center"}}>격파 가능한 보스가 없습니다.</p>
          ) : (
            <div
              style={{
                display: "flex",
                flexDirection: "column",
                gap: "10px",
                maxWidth: "500px", // 최대 너비 설정
                margin: "0 auto", // 중앙 정렬
              }}
            >
              {selectedBosses.map((boss, index) => (
                <div
                  key={boss._id}
                  style={{
                    display: "flex",
                    justifyContent: "space-between",
                    borderBottom: "1px solid #ccc",
                    padding: "5px 0",
                  }}
                >
                  <span
                    style={{
                      flex: "1",
                      textAlign: "left",
                      paddingRight: "10px",
                    }}
                  >
                    {index + 1}. {boss.name} ({boss.difficulty})
                  </span>
                  <span
                    style={{
                      flex: "1",
                      textAlign: "right",
                      paddingLeft: "10px",
                    }}
                  >
                    {boss.cost.toLocaleString()} 메소
                  </span>
                </div>
              ))}
            </div>
          )}
        </div>
      )}
    </div>
  );
};

export default Home;
