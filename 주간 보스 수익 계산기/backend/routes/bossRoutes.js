const express = require('express');
const { getAllBosses, getBossByName } = require('../controllers/bossController');

const router = express.Router();

// 모든 보스 가져오기
router.get('/', getAllBosses);

// 특정 보스 가져오기
router.get('/:name', getBossByName);

module.exports = router;