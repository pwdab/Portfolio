import api from './api';

// 모든 보스 데이터 가져오기
export const fetchAllBosses = async () => {
  try {
    const response = await api.get('/bosses');
    return response.data;
  } catch (error) {
    console.error('Error fetching bosses:', error);
    throw error;
  }
};

// 특정 전투력 이하의 보스 데이터 가져오기
export const fetchBossesByBattlePower = async (battlePower) => {
  try {
    const response = await api.get(`/bosses/cost`, {
      params: { battlePower },
    });
    return response.data;
  } catch (error) {
    console.error('Error fetching bosses by battle power:', error);
    throw error;
  }
};
