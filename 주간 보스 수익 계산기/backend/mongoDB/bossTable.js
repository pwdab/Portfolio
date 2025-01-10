const mongoose = require('mongoose');

const bossSchema = new mongoose.Schema({
    name: { type: String, required: true },
    difficulty: { type: String, required: true },
    battle_power: { type: Number, required: true },
    cost: { type: Number, required: true },
    type: { type: String, required: true }, // 'weekly' 또는 'monthly'
});

const Boss = mongoose.model('bossTable', bossSchema, 'bossTable');

module.exports = Boss;