const express = require('express');
const cors = require('cors');
const morgan = require('morgan');
const dotenv = require('dotenv');
const characterRoutes = require('./routes/characterRoutes');
const bossRoutes = require('./routes/bossRoutes');

// Load environment variables
dotenv.config();

// Initialize Express app
const app = express();

// Middleware
app.use(cors());
app.use(morgan('dev'));
app.use(express.json());

// Routes
app.use('/api/character', characterRoutes);
app.use('/api/bosses', bossRoutes);

/*
// Basic route
app.get('/', (req, res) => {
  res.send('Welcome to Boss Calc Backend!');
});
*/

module.exports = app;