#include <Stepper.h>
#include <time.h>
#include <WiFi.h>
#include <WiFiManager.h>
#include <ESP_Mail_Client.h>  

#include <ESP_Mail_Client.h>
#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT 465
#define EMAIL_REMETENTE "giromedalerta@gmail.com"
#define SENHA_APP "aohq whpv sxot yvyx " 
SMTPSession smtp;

String nurseEmail = "giovanibelliniestudos@gmail.com";

void sendEmail(String assunto, String corpoMensagem) {
  
  Serial.println("Conectando ao servidor do Google...");

  ESP_Mail_Session session;
  session.server.host_name = SMTP_HOST;
  session.server.port = SMTP_PORT;
  session.login.email = EMAIL_REMETENTE;
  session.login.password = SENHA_APP;
  session.login.user_domain = "";


  SMTP_Message message;
  message.sender.name = "Máquina GiroMed";
  message.sender.email = EMAIL_REMETENTE;
  
  message.addRecipient("Cuidador Responsável", nurseEmail);
  
  message.subject = assunto;
  message.text.content = corpoMensagem;

  if (!smtp.connect(&session)) {
    Serial.println("Erro de conexão SMTP!");
    return;
  }

  if (!MailClient.sendMail(&smtp, &message)) {
    Serial.print("Erro ao enviar o e-mail: "); 
    Serial.println(smtp.errorReason());
  } else {
    Serial.println(">>> E-MAIL ENVIADO COM SUCESSO! <<<");
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

    Serial.printf("\n>>> NOVO REMEDIO RECEBIDO VIA WIFI <<<\n");
    Serial.printf("Gaveta %d para as %02d:%02d no dia %d\n", newSlot, newHour, newMinute, newDay);

    server.send(200, "text/html", sucesso_html);
    
  } else {
    server.send(400, "text/plain", "Erro: Dados incompletos");
  }
}

void handleList() {
  String html = String(lista_topo);
  
  const char* daysOfTheWeek[] = {"Dom", "Seg", "Ter", "Qua", "Qui", "Sex", "Sab"};

  if (medsProgramTotalCount == 0) {
    html += "<tr><td colspan='3'>Nenhuma programação encontrada.</td></tr>";
  } else {
    for (int i = 0; i < medsProgramTotalCount; i++) {
      html += "<tr>";
      
      html += "<td>";
      html += medsProgramCalender[i].slot; 
      html += "</td>";
      
      char bufferHora[10];
      sprintf(bufferHora, "%02d:%02d", medsProgramCalender[i].hour, medsProgramCalender[i].minutes);
      html += "<td>";
      html += bufferHora;
      html += "</td>";
      
      html += "<td>";
      html += daysOfTheWeek[medsProgramCalender[i].weekDay]; 
      html += "</td>";
      
      html += "</tr>";
    }
  }

  html += String(lista_fim);
  server.send(200, "text/html", html);
}

void handleSavingEmail() {
if (server.hasArg("endereco_email")) {
  
  nurseEmail = server.arg("endereco_email");
  
  Serial.print("\n>>> NOVO E-MAIL DE ALERTA CADASTRADO <<<\n");
  Serial.print("Destino: ");
  Serial.println(nurseEmail);

  String htmlResposta = "<!DOCTYPE html><html><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'><title>Sucesso</title><style>body { font-family: Arial; text-align: center; margin-top: 50px; background-color: #f4f4f9;} .btn { padding: 12px 24px; background-color: #6c757d; color: white; text-decoration: none; border-radius: 5px; font-weight: bold;}</style></head><body><h2 style='color: #28a745;'>E-mail Atualizado! ✅</h2><p>Os alertas de dispensação agora serão enviados para:<br><b>" + nurseEmail + "</b></p><br><br><a href='/' class='btn'>Voltar ao Menu</a></body></html>";
  
  server.send(200, "text/html", htmlResposta);
  
  } else {
    server.send(400, "text/plain", "Erro: E-mail não recebido.");
  }
}

bool isSunday(){
  
  if (!getLocalTime(&currentTime)) {
    return false; 
  } 
  
  if (currentTime.tm_wday == 0) {
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

  int theMedForThisHour;
  
  for(int i = 0; i < 21; i++){
    if(currentTime.tm_hour == medsProgramCalender[i].hour && currentTime.tm_min == medsProgramCalender[i].minutes){
      return true;
    }
  }

  return false;
}

int findSlotsForThisHour(){
  
  if(!getLocalTime(&currentTime)){
    Serial.printf("erro na hora\n");
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
#define IN1 19
#define IN2 18
#define IN3 5
#define IN4 17
#define limitSwitch 22
Stepper myStepper(stepsPerRevolution, IN1, IN3, IN2, IN4);

void motorAus() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void alignCarousel(){
  pinMode(limitSwitch, INPUT_PULLUP);

  while(digitalRead(limitSwitch)== HIGH){
    myStepper.step(-10);
    delay(5);
  }

  motorAus();

  currentSlot = 0;
}


void rotateMotorTo(int targetSlot){

  alignCarousel();
  
  Serial.printf("\n--- INICIANDO ENTREGA ---\n");
  Serial.printf("Carrossel atual: Gaveta %d | Destino: Gaveta %d\n", currentSlot, targetSlot);

  float currentAngle = storageCarouselSlots[currentSlot];
  float targetAngle = storageCarouselSlots[targetSlot];

  float differenceBetweenCurrentAndTargetAngle = targetAngle - currentAngle;

  long stepsToWalk = (differenceBetweenCurrentAndTargetAngle / 360.0) * stepsPerRevolution;

  Serial.printf("Girando o motor em %ld passos...\n", stepsToWalk);

  myStepper.step(stepsToWalk);

  motorAus();

  Serial.println(">>> REMEDIO DISPENSADO COM SUCESSO! <<<");

  currentSlot = targetSlot;

  return;
}

const char* ssid = ""; 
const char* password = "";
const char* ntpServer = "pool.ntp.org";
const long  brasiliaTime = -3 * 3600;
const int   summerTimeOffset = 0;

void setup() {
  Serial.begin(115200);
  
  alignCarousel();

  WiFiManager wm;

  Serial.printf("waiting for wifi connectioni\n");
  bool connected = wm.autoConnect("giromed", "27042026");

  if (!connected) {
    Serial.println("ERRO ESP32 was unable to connect to the Wi-Fi.\n");
    delay(3000);
    ESP.restart();
  }

  Serial.println("\nWifi -> OK");
  configTime(brasiliaTime, summerTimeOffset, ntpServer);
  
  myStepper.setSpeed(5);
  
  if (MDNS.begin("giromed")) {
    Serial.println("MDNS iniciado com sucesso!");
    Serial.println("Acesse no navegador: http://giromed.local");
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
    Serial.println("Executando teste de e-mail solicitado via Web...");
    sendEmail("Teste de Funcionamento", "Olá! Este é um e-mail de teste enviado pelo GiroMed. O sistema de alertas está operacional.");
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

server.handleClient();

if (currentTime.tm_min != lastVerifiedMinute && thereAreProgramsForToday()) {

  if(thereAreProgramsForToday()){

    if(isSunday()){
      // aqui vai ter uma parte de notificação 
    }
    
    if (isTimeForMeds()){
      int thisHourSlot = findSlotsForThisHour();
      Serial.printf("/n O slot para esse hora é: %d",thisHourSlot);
      rotateMotorTo(thisHourSlot);

      char msg[100]; 
      sprintf(msg, "Aviso: O remedio da gaveta %d acabou de ser dispensado!", thisHourSlot); 

      sendEmail("Remedio dispensado", msg);

    }

  }else{
    delay(10000);
    // alguma coisa pra ele dormir aumentar o delay do loop para ele não ficar atualizando de 15 em 15 min
  }

  lastVerifiedMinute = currentTime.tm_min;
}


}