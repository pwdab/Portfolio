const Boss = require('../mongoDB/bossTable');

// 모든 보스 데이터 가져오기
const getAllBosses = async (req, res) => {
    try {
        console.log('Fetching bosses from MongoDB...');
        const bosses = await Boss.find(); // 모든 데이터 조회
        console.log('Fetched bosses:', bosses); // 데이터 확인
        res.status(200).json(bosses);
    } catch (error) {
        console.error('Error fetching bosses:', error.message);
        res.status(500).json({ error: 'Failed to fetch bosses' });
    }
};

// 특정 보스 데이터 가져오기
const getBossByName = async (req, res) => {
    const bossName = req.params.name; // URL에서 보스 이름 가져오기
    try {
      const boss = await Boss.findOne({ boss: bossName }); // 보스 이름으로 검색
      if (!boss) {
        return res.status(404).json({ error: 'Boss not found' });
      }
      res.status(200).json(boss);
    } catch (error) {
      console.error(error);
      res.status(500).json({ error: 'Failed to fetch boss' });
    }
};

module.exports = {
    getAllBosses,
    getBossByName,
};