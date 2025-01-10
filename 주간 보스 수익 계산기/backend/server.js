/*
const app = require('./app');
const mongoose = require('./mongoDB/mongoDB');

const PORT = process.env.PORT || 3000;

// Start server
app.listen(PORT, () => {
  console.log(`Server is running on http://localhost:${PORT}`);
});
*/

const app = require('./app');
const mongoose = require('./mongoDB/mongoDB');
const path = require('path'); // React 정적 파일 경로 처리를 위한 모듈
//const PORT = process.env.PORT || 3000;
const PORT = 3000;

// React 정적 파일 서빙
const buildPath = path.join(__dirname, '../frontend/build'); // React 빌드 파일 경로
app.use(require('express').static(buildPath)); // 정적 파일 서빙

// React 라우팅 처리
app.get('*', (req, res) => {
    res.sendFile(path.join(buildPath, 'index.html'));
});

// 서버 시작
app.listen(PORT, () => {
    console.log(`Server is running on http://localhost:${PORT}`);
});