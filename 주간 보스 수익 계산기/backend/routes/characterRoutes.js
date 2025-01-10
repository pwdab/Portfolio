const express = require('express');
const { getCharacterData } = require('../NexonOpenAPI/NexonOpenAPI');

const router = express.Router();

// GET /api/character/:nickname
router.get('/:characterName', getCharacterData);

module.exports = router;