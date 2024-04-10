const express = require('express');
const server = express();
const port = 3000;

// Middleware для обработки POST запросов
server.use(express.urlencoded({ extended: true }));

// Обработчик GET запроса на главную страницу
server.get('/', (req, res) => {
  res.sendFile(__dirname + '/index.html');
});

// Обработчик POST запроса для обработки данных и вызова функций
server.post('/process-data', (req, res) => {
  const userData = req.body;
  
  // Проверка привилегий пользователя
  if (userData.role === 'admin') {
    res.send('Вы вошли как администратор');
  } else {
    res.send('Вы вошли как обычный пользователь');
  }
});

// Запуск сервера
server.listen(port, () => {
  console.log(`Сервер запущен на http://localhost:${port}`);
});