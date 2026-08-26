#ifndef WEB_PAGES_H
#define WEB_PAGES_H

#include <Arduino.h>

// =========================================================================
// 1. TELA INICIAL (MENU PRINCIPAL) - A primeira que o usuário vê
// =========================================================================
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>GiroMed - Início</title>
  <style>
    body { font-family: Arial, sans-serif; text-align: center; margin: 20px; background-color: #f4f4f9;}
    .card { background: white; padding: 30px 20px; border-radius: 10px; box-shadow: 0 4px 8px rgba(0,0,0,0.1); max-width: 400px; margin: auto;}
    .btn { display: block; margin: 15px auto; padding: 15px; width: 85%; border-radius: 8px; text-decoration: none; color: white; font-weight: bold; font-size: 18px; box-sizing: border-box;}
    
    .btn-cadastrar { background-color: #28a745; }
    .btn-cadastrar:hover { background-color: #218838; }
    
    .btn-lista { background-color: #007bff; }
    .btn-lista:hover { background-color: #0069d9; }
    
    .btn-email { background-color: #ff9800; }
    .btn-email:hover { background-color: #e68a00; }

    .btn-reabastecer { background-color: #6f42c1; }
    .btn-reabastecer:hover { background-color: #59339d; }
    
    .logo-text { font-size: 28px; font-weight: bold; color: #333; margin-bottom: 5px; }
    .subtitle { font-size: 14px; color: #666; margin-bottom: 25px; }
  </style>
</head>
<body>
  <div class="card">
    <div class="logo-text">GiroMed</div>
    <div class="subtitle">Dispensador Inteligente de Medicamentos</div>
    
    <a href="/cadastrar" class="btn btn-cadastrar">➕ Adicionar Remédio</a>
    <a href="/lista" class="btn btn-lista">📋 Ver Programações</a>
    <a href="/reabastecer" class="btn btn-reabastecer">🔄 Dia de Reabastecimento</a>
    <a href="/email" class="btn btn-email">✉️ Configurar Alerta</a>
  </div>
</body>
</html>
)rawliteral";

// =========================================================================
// 2. TELA DE CADASTRO DE REMÉDIOS
// =========================================================================
const char cadastro_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>GiroMed - Configuração</title>
  <style>
    body { font-family: Arial, sans-serif; text-align: center; margin: 20px; background-color: #f4f4f9;}
    .card { background: white; padding: 20px; border-radius: 10px; box-shadow: 0 4px 8px rgba(0,0,0,0.1); max-width: 400px; margin: auto;}
    input, select, button { margin: 10px 0; padding: 10px; width: 90%; border-radius: 5px; border: 1px solid #ccc; box-sizing: border-box;}
    button { background-color: #28a745; color: white; border: none; cursor: pointer; font-size: 16px; font-weight: bold; }
    button:hover { background-color: #218838; }
    .btn-voltar { display: inline-block; margin-top: 15px; padding: 10px 20px; background-color: #6c757d; color: white; text-decoration: none; border-radius: 5px; }
  </style>
</head>
<body>
  <div class="card">
    <h2>Programar Remédio</h2>
    <form action="/salvar" method="GET">
      <label>Gaveta (Slot):</label>
      <input type="number" name="slot" min="0" max="20" required>

      <label>Hora (0-23):</label>
      <input type="number" name="hora" min="0" max="23" required>

      <label>Minutos (0 a 59):</label>
      <input type="number" name="minuto" min="0" max="59" required>

      <label>Dia da Semana:</label>
      <select name="dia">
        <option value="0">Domingo</option>
        <option value="1">Segunda</option>
        <option value="2">Terça</option>
        <option value="3">Quarta</option>
        <option value="4">Quinta</option>
        <option value="5">Sexta</option>
        <option value="6">Sábado</option>
      </select>

      <button type="submit">Salvar Programação</button>
    </form>
    <a href="/" class="btn-voltar">⬅ Voltar ao Menu</a>
  </div>
</body>
</html>
)rawliteral";


// =========================================================================
// 3. TELA DE SUCESSO APÓS SALVAR
// =========================================================================
const char sucesso_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>GiroMed - Sucesso</title>
  <style>
    body { font-family: Arial, sans-serif; text-align: center; margin: 20px; background-color: #f4f4f9;}
    .card { background: white; padding: 30px 20px; border-radius: 10px; box-shadow: 0 4px 8px rgba(0,0,0,0.1); max-width: 400px; margin: auto;}
    .btn { display: block; margin: 15px auto; padding: 12px; width: 85%; border-radius: 5px; text-decoration: none; color: white; font-weight: bold; font-size: 16px; box-sizing: border-box;}
    .btn-novo { background-color: #28a745; }
    .btn-novo:hover { background-color: #218838; }
    .btn-menu { background-color: #6c757d; }
    .btn-menu:hover { background-color: #5a6268; }
  </style>
</head>
<body>
  <div class="card">
    <h2 style="color: #28a745;">Programação Salva! ✅</h2>
    <p>A configuração foi registrada com sucesso.</p>

    <a href="/cadastrar" class="btn btn-novo">➕ Configurar Novo Slot</a>
    <a href="/" class="btn btn-menu">🏠 Voltar ao Menu</a>
  </div>
</body>
</html>
)rawliteral";

// =========================================================================
// 4. TOPO DA LISTA DE REMÉDIOS
// =========================================================================
const char lista_topo[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>GiroMed - Programações</title>
  <style>
    body { font-family: Arial, sans-serif; text-align: center; margin: 20px; background-color: #f4f4f9;}
    .card { background: white; padding: 20px; border-radius: 10px; box-shadow: 0 4px 8px rgba(0,0,0,0.1); max-width: 500px; margin: auto;}
    table { width: 100%; border-collapse: collapse; margin-top: 20px; }
    th, td { padding: 12px; border-bottom: 1px solid #eee; text-align: center; }
    th { background-color: #007bff; color: white; border-radius: 5px 5px 0 0; }
    .btn-voltar { display: inline-block; margin-top: 20px; padding: 10px 20px; background-color: #6c757d; color: white; text-decoration: none; border-radius: 5px; }
  </style>
</head>
<body>
  <div class="card">
    <h2>Remédios Agendados</h2>
    <table>
      <thead>
        <tr>
          <th>Gaveta</th>
          <th>Horário</th>
          <th>Dia</th>
        </tr>
      </thead>
      <tbody>
)rawliteral";

// =========================================================================
// 5. FIM DA LISTA DE REMÉDIOS
// =========================================================================
const char lista_fim[] PROGMEM = R"rawliteral(
      </tbody>
    </table>
    <a href="/" class="btn-voltar">⬅ Voltar ao Menu</a>
  </div>
</body>
</html>
)rawliteral";

// =========================================================================
// 6. TELA DE CONFIGURAÇÃO DE E-MAIL (COM BOTÃO DE TESTE INCLUSO)
// =========================================================================
const char email_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>GiroMed - E-mail Alerta</title>
  <style>
    body { font-family: Arial, sans-serif; text-align: center; margin: 20px; background-color: #f4f4f9;}
    .card { background: white; padding: 20px; border-radius: 10px; box-shadow: 0 4px 8px rgba(0,0,0,0.1); max-width: 400px; margin: auto;}
    input, button, .btn-teste { margin: 10px 0; padding: 10px; width: 90%; border-radius: 5px; border: 1px solid #ccc; box-sizing: border-box; display: inline-block;}
    button { background-color: #ff9800; color: white; border: none; cursor: pointer; font-size: 16px; font-weight: bold; }
    button:hover { background-color: #e68a00; }
    .btn-teste { background-color: #17a2b8; color: white; text-decoration: none; font-size: 16px; font-weight: bold; border: none;}
    .btn-teste:hover { background-color: #138496; }
    .btn-voltar { display: inline-block; margin-top: 15px; padding: 10px 20px; background-color: #6c757d; color: white; text-decoration: none; border-radius: 5px; }
  </style>
</head>
<body>
  <div class="card">
    <h2>Alerta para Cuidador</h2>
    <p style="font-size: 14px; color: #555;">Cadastre o e-mail que receberá os avisos de dispensação.</p>
    
    <form action="/salvarEmail" method="GET">
      <label>E-mail do Responsável:</label>
      <input type="email" name="endereco_email" placeholder="exemplo@gmail.com" required>
      <button type="submit">Salvar E-mail</button>
    </form>
    
    <a href="/testeEmail" class="btn-teste">📧 Testar Envio de Alerta</a>
    
    <br>
    <a href="/" class="btn-voltar">⬅ Voltar ao Menu</a>
  </div>
</body>
</html>
)rawliteral";

#endif
