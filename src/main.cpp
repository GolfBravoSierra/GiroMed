#include <Stepper.h>
#include <time.h>
#include <WiFi.h>
#include <WiFiManager.h>
#include <ESP_Mail_Client.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcdMonitor(0x27, 16, 2);

void updateLCDMonitor(int messageId, int param1 = 0, int param2 = 0, int param3 = 0, int param4 = 0, String textParam = "") {
  lcdMonitor.clear();
  lcdMonitor.setCursor(0, 0);

  char screenBuffer[17]; 

  switch(messageId) {
    case 1:
      lcdMonitor.print("Conectando SMTP");
      lcdMonitor.setCursor(0, 1);
      lcdMonitor.print("Google...");
      break;
    case 2:
      lcdMonitor.print("Erro de Conexao");
      lcdMonitor.setCursor(0, 1);
      lcdMonitor.print("Falha no SMTP!");
      break;
    case 3:
      lcdMonitor.print("Erro de E-mail");
      lcdMonitor.setCursor(0, 1);
      lcdMonitor.print(textParam.substring(0, 16));
      break;
    case 4:
      lcdMonitor.print("E-mail Enviado!");
      lcdMonitor.setCursor(0, 1);
      lcdMonitor.print("Sucesso");
      break;
    case 5:
      lcdMonitor.print("Novo Remedio RX!");
      lcdMonitor.setCursor(0, 1);
      snprintf(screenBuffer, sizeof(screenBuffer), "G:%d %02d:%02d D:%d", param1, param2, param3, param4);
      lcdMonitor.print(screenBuffer);
      break;
    case 6:
      lcdMonitor.print("Novo Email Dest.");
      lcdMonitor.setCursor(0, 1);
      lcdMonitor.print(textParam.substring(0, 16));
      break;
    case 7:
      lcdMonitor.print("Erro no Relogio");
      lcdMonitor.setCursor(0, 1);
      lcdMonitor.print("NTP Falhou");
      break;
    case 8:
      lcdMonitor.print("Alinhando...");
      lcdMonitor.setCursor(0, 1);
      lcdMonitor.print("Buscando Gav. 0");
      break;
    case 9:
      lcdMonitor.print("Alinhamento OK!");
      lcdMonitor.setCursor(0, 1);
      lcdMonitor.print("Gaveta 0 Pronta");
      break;
    case 10:
      lcdMonitor.print("Entrega Iniciada");
      lcdMonitor.setCursor(0, 1);
      snprintf(screenBuffer, sizeof(screenBuffer), "Gav:%d -> Gav:%d", param1, param2);
      lcdMonitor.print(screenBuffer);
      break;
    case 11:
      lcdMonitor.print("Girando Motor...");
      lcdMonitor.setCursor(0, 1);
      snprintf(screenBuffer, sizeof(screenBuffer), "Passos: %d", param1);
      lcdMonitor.print(screenBuffer);
      break;
    case 12:
      lcdMonitor.print("Remedio Ejetado!");
      lcdMonitor.setCursor(0, 1);
      lcdMonitor.print("Abra a Porta...");
      break;
    case 13:
      lcdMonitor.print("Porta Aberta");
      lcdMonitor.setCursor(0, 1);
      lcdMonitor.print("Retire o Remedio");
      break;
    case 14:
      lcdMonitor.print("Porta Fechada");
      lcdMonitor.setCursor(0, 1);
      lcdMonitor.print("Realinhando...");
      break;
    case 15:
      lcdMonitor.print("Iniciando...");
      lcdMonitor.setCursor(0, 1);
      lcdMonitor.print("Buscando Wi-Fi");
      break;
    case 16:
      lcdMonitor.print("Erro de Wi-Fi");
      lcdMonitor.setCursor(0, 1);
      lcdMonitor.print("Reiniciando...");
      break;
    case 17:
      lcdMonitor.print("Wi-Fi Conectado!");
      lcdMonitor.setCursor(0, 1);
      lcdMonitor.print("Rede OK");
      break;
    case 18:
      lcdMonitor.print("Acesse na Rede:");
      lcdMonitor.setCursor(0, 1);
      lcdMonitor.print("giromed.local");
      break;
    case 19:
      lcdMonitor.print("Teste de E-mail");
      lcdMonitor.setCursor(0, 1);
      lcdMonitor.print("Enviando agora");
      break;
    case 20:
      lcdMonitor.print("Hora do Remedio!");
      lcdMonitor.setCursor(0, 1);
      snprintf(screenBuffer, sizeof(screenBuffer), "Gaveta Alvo: %d", param1);
      lcdMonitor.print(screenBuffer);
      break;
    case 21:
      lcdMonitor.print("Dia de resabastecimento");
      lcdMonitor.setCursor(0, 1);
      lcdMonitor.print("Configurado");
      break;
    default:
      lcdMonitor.print("GiroMed Ativo");
      break;
  }
}

#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT 465
#define SENDER_EMAIL "giromedalerta@gmail.com"
#define APP_PASSWORD "aohq whpv sxot yvyx " 
SMTPSession smtp;

String nurseEmail = "giovanibelliniestudos@gmail.com";

void sendEmail(String emailSubject, String emailBody) {
  
  updateLCDMonitor(1); 

  ESP_Mail_Session session;
  session.server.host_name = SMTP_HOST;
  session.server.port = SMTP_PORT;
  session.login.email = SENDER_EMAIL;
  session.login.password = APP_PASSWORD;
  session.login.user_domain = "";

  SMTP_Message message;
  message.sender.name = "Máquina GiroMed";
  message.sender.email = SENDER_EMAIL;
  
  message.addRecipient("Cuidador Responsável", nurseEmail);
  
  message.subject = emailSubject;
  message.text.content = emailBody;

  if (!smtp.connect(&session)) {
    updateLCDMonitor(2); 
    return;
  }

  if (!MailClient.sendMail(&smtp, &message)) {
    updateLCDMonitor(3, 0, 0, 0, 0, smtp.errorReason().c_str()); 
  } else {
    updateLCDMonitor(4); 
  }

  smtp.sendingResult.clear();
}

const float storageCarouselSlots[21] = {
  0.0000,   17.1429,  34.2857,  51.4286,  68.5714,
  85.7143,  102.8571, 120.0000, 137.1429, 154.2857,
  171.4286, 188.5714, 205.7143, 222.8571, 240.0000,
  257.1429, 274.2857, 291.4286, 308.5714, 325.7143,
  342.8571
};

struct medicineTimeAndSlot {
  int weekDay;
  int hour;
  int minutes; 
  int slot;
};

medicineTimeAndSlot medsProgramCalender[21];
int medsProgramTotalCount = 0;

struct tm currentTime;

#include <ESPmDNS.h>
#include <WebServer.h>
WebServer server(80);
#include "web_page.h"

void handleSavingPrograms() {
  if (server.hasArg("slot") && server.hasArg("hora") && server.hasArg("minuto") && server.hasArg("dia")) {
    
    int newSlot = server.arg("slot").toInt();
    int newHour = server.arg("hora").toInt();
    int newMinute = server.arg("minuto").toInt();
    int newDay = server.arg("dia").toInt();

    medsProgramCalender[medsProgramTotalCount].slot = newSlot;
    medsProgramCalender[medsProgramTotalCount].hour = newHour;
    medsProgramCalender[medsProgramTotalCount].minutes = newMinute;
    medsProgramCalender[medsProgramTotalCount].weekDay = newDay;
    
    medsProgramTotalCount++;

    updateLCDMonitor(5, newSlot, newHour, newMinute, newDay);
    
    server.send(200, "text/html", sucesso_html);
  } else {
    server.send(400, "text/plain", "Erro: Dados incompletos");
  }
}

void handleList() {
  String htmlContent = String(lista_topo);
  
  const char* daysOfTheWeek[] = {"Dom", "Seg", "Ter", "Qua", "Qui", "Sex", "Sab"};

  if (medsProgramTotalCount == 0) {
    htmlContent += "<tr><td colspan='3'>Nenhuma programação encontrada.</td></tr>";
  } else {
    for (int i = 0; i < medsProgramTotalCount; i++) {
      htmlContent += "<tr>";
      
      htmlContent += "<td>";
      htmlContent += medsProgramCalender[i].slot; 
      htmlContent += "</td>";
      
      char hourBuffer[10];
      sprintf(hourBuffer, "%02d:%02d", medsProgramCalender[i].hour, medsProgramCalender[i].minutes);
      htmlContent += "<td>";
      htmlContent += hourBuffer;
      htmlContent += "</td>";
      
      htmlContent += "<td>";
      htmlContent += daysOfTheWeek[medsProgramCalender[i].weekDay]; 
      htmlContent += "</td>";
      
      htmlContent += "</tr>";
    }
  }

  htmlContent += String(lista_fim);
  server.send(200, "text/html", htmlContent);
}

void handleSavingEmail() {
  if (server.hasArg("endereco_email")) {
    
    nurseEmail = server.arg("endereco_email");
    
    updateLCDMonitor(6, 0, 0, 0, 0, nurseEmail);
  
    String responseHtml = "<!DOCTYPE html><html><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'><title>Sucesso</title><style>body { font-family: Arial; text-align: center; margin-top: 50px; background-color: #f4f4f9;} .btn { padding: 12px 24px; background-color: #6c757d; color: white; text-decoration: none; border-radius: 5px; font-weight: bold;}</style></head><body><h2 style='color: #28a745;'>E-mail Atualizado! ✅</h2><p>Os alertas de dispensação agora serão enviados para:<br><b>" + nurseEmail + "</b></p><br><br><a href='/' class='btn'>Voltar ao Menu</a></body></html>";
    
    server.send(200, "text/html", responseHtml);
    
  } else {
    server.send(400, "text/plain", "Erro: E-mail não recebido.");
  }
}

void handleSaveRestockDay(){
  
}

int RestockDay = 0;

bool isRestockDay(){
  if (!getLocalTime(&currentTime)) {
    return false; 
  } 
  if (currentTime.tm_wday == RestockDay) {
    return true; 
  } else {
    return false;
  }
}

bool thereAreProgramsForToday(){
  if (!getLocalTime(&currentTime)) {
    return false; 
  } 
  for(int i=0; i < 21 ;i++){
    if(currentTime.tm_wday == medsProgramCalender[i].weekDay){
      return true;
    }
  }
  return false;
}

bool isTimeForMeds(){
  if (!getLocalTime(&currentTime)) {
    return 0; 
  } 
  for(int i = 0; i < 21; i++){
    if(currentTime.tm_hour == medsProgramCalender[i].hour && currentTime.tm_min == medsProgramCalender[i].minutes){
      return true;
    }
  }
  return false;
}

int findSlotsForThisHour(){
  if(!getLocalTime(&currentTime)){
    updateLCDMonitor(7);
  }
  for(int i = 0; i < 21; i++){
    if (currentTime.tm_hour == medsProgramCalender[i].hour && currentTime.tm_min == medsProgramCalender[i].minutes   ){
      return medsProgramCalender[i].slot;
    }
  }
  return 0;
}

const int stepsPerRevolution = 2048;
int currentSlot = 0;

#define MOTOR_IN1 25
#define MOTOR_IN2 13
#define MOTOR_IN3 14
#define MOTOR_IN4 27

#define MAGNETIC_SENSOR_PIN 26 
#define DOOR_SENSOR_PIN 32     
#define BUZZER_PIN 33    

Stepper myStepper(stepsPerRevolution, MOTOR_IN1, MOTOR_IN3, MOTOR_IN2, MOTOR_IN4);

void turnOffMotor() {
  digitalWrite(MOTOR_IN1, LOW);
  digitalWrite(MOTOR_IN2, LOW);
  digitalWrite(MOTOR_IN3, LOW);
  digitalWrite(MOTOR_IN4, LOW);
}

void alignCarousel(){
  pinMode(MAGNETIC_SENSOR_PIN, INPUT_PULLUP);

  updateLCDMonitor(8); 

  while(digitalRead(MAGNETIC_SENSOR_PIN) == HIGH){
    myStepper.step(-1);
    delay(10);
  }

  turnOffMotor();
  currentSlot = 0;
  
  updateLCDMonitor(9); 
}

void rotateMotorTo(int targetSlot){
  
  updateLCDMonitor(10, currentSlot, targetSlot); 

  float currentAngle = storageCarouselSlots[currentSlot];
  float targetAngle = storageCarouselSlots[targetSlot];

  float differenceBetweenCurrentAndTargetAngle = targetAngle - currentAngle;
  long stepsToWalk = (differenceBetweenCurrentAndTargetAngle / 360.0) * stepsPerRevolution;

  updateLCDMonitor(11, stepsToWalk); 
  myStepper.step(stepsToWalk);
  
  turnOffMotor(); 

  updateLCDMonitor(12); 

  while(digitalRead(DOOR_SENSOR_PIN) == LOW){
    tone(BUZZER_PIN, 1000); 
    delay(300);
    tone(BUZZER_PIN, 1500); 
    delay(300);
  }

  noTone(BUZZER_PIN); 
  
  updateLCDMonitor(13); 

  while(digitalRead(DOOR_SENSOR_PIN) == HIGH){
    delay(100); 
  }

  updateLCDMonitor(14); 
  delay(1000); 

  alignCarousel();

  return;
}

const char* ssid = ""; 
const char* password = "";
const char* ntpServer = "pool.ntp.org";
const long  brasiliaTime = -3 * 3600;
const int   summerTimeOffset = 0;

void setup() {
  
  lcdMonitor.init();
  lcdMonitor.backlight();

  pinMode(DOOR_SENSOR_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);

  alignCarousel();
  
  WiFiManager wm;

  updateLCDMonitor(15); 
  bool connected = wm.autoConnect("giromed", "27042026");

  if (!connected) {
    updateLCDMonitor(16); 
    delay(3000);
    ESP.restart();
  }

  updateLCDMonitor(17); 
  delay(1500);
  
  configTime(brasiliaTime, summerTimeOffset, ntpServer);
  
  myStepper.setSpeed(5);
  
  if (MDNS.begin("giromed")) {
    updateLCDMonitor(18); 
  }

  server.on("/", []() {
    server.send(200, "text/html", index_html);
  });

  server.on("/cadastrar", []() {
    server.send(200, "text/html", cadastro_html);
  });

  server.on("/email", []() {
    server.send(200, "text/html", email_html);
  });
  
  server.on("/testeEmail", []() {
    updateLCDMonitor(19); 
    sendEmail("Teste de Funcionamento", "Olá! Este é um e-mail de teste enviado pelo GiroMed. O sistema de alertas está operacional.");
  });

  server.on("/restockDay", []() {
    updateLCDMonitor(21);
    handleSaveRestockDay(); 
  });
  
  server.on("/salvar", handleSavingPrograms); 
  server.on("/lista", handleList);
  server.on("/salvarEmail", handleSavingEmail);
  server.begin();
}

int lastVerifiedMinute = -1;

void loop() {

  if(!getLocalTime(&currentTime)){
    return;
  }

  char emailMessage[100]; 
  server.handleClient();

  if (currentTime.tm_min != lastVerifiedMinute && thereAreProgramsForToday()) {

    if(thereAreProgramsForToday()){

      if(isRestockDay()){
         sprintf(emailMessage, "Hoje é dia de reabastecer o GiroMed"); 
      }
      
      if (isTimeForMeds()){
        int thisHourSlot = findSlotsForThisHour();
        
        updateLCDMonitor(20, thisHourSlot); 
        delay(2000);
        
        rotateMotorTo(thisHourSlot);

        sprintf(emailMessage, "Aviso: O remedio da gaveta %d acabou de ser dispensado!", thisHourSlot); 

        sendEmail("Remedio dispensado", emailMessage);
      }

    }else{
      delay(10000);
    }

    lastVerifiedMinute = currentTime.tm_min;
  }
}
