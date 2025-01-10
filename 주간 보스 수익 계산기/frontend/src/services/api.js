import axios from 'axios';

// 백엔드 API URL (Ngrok 또는 로컬 서버 URL 사용)
const API_BASE_URL = 'https://e85c-163-239-4-185.ngrok-free.app/api';

const api = axios.create({
  baseURL: API_BASE_URL,
});

export default api;