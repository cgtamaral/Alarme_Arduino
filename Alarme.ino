#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <FirebaseArduino.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>

#define PIN_PIR D7
#define PIN_LED D6
#define PIN_BUZZER D5

#define FIREBASE_HOST "MEU_PROJETO.firebaseio.com"
#define FIREBASE_AUTH "MINHA_SENHA"

// Configuracao ID e senha da rede Wireless
#define WIFI_SSID        "NOME_DA_REDE"
#define WIFI_PASSWORD    "SENHA_DA_REDE"

void setup() 
{
  Serial.begin(115200);
  Serial.println();
  delay(100);
    
  pinMode(PIN_PIR, INPUT); 
  pinMode(PIN_LED, OUTPUT);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.printf("Conectando...");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.printf("Conectado com o IP: ");
  Serial.println(WiFi.localIP());
 
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void loop()
{
  long valorSensorPIR = digitalRead(PIN_PIR);
  if (valorSensorPIR == HIGH)
  {
    Serial.println("Presença detectada!");
    gravarDados(true);
    ligarAlarme();
  } 
  else 
  {
    Serial.println("Nenhuma presença detectada!");
    gravarDados(false);
    desligarAlarme();
  }
}


void gravarDados(bool presenca_detectada)
{
  Firebase.setBool("presenca-detectada", presenca_detectada);
  if (Firebase.failed())
  {
      Serial.printf("Erro ao gravar valor no firebase: ");
      Serial.println(Firebase.error());  
      return;
  } 
  else 
  {
      Serial.print("Gravou com sucesso o valor: ");
      Serial.println(presenca_detectada);
  }
}

void ligarAlarme()
{
  digitalWrite(PIN_LED, HIGH);
  tone(PIN_BUZZER,1500);
  delay(4000);
  desligarAlarme();
}
 
void desligarAlarme() 
{
  digitalWrite(PIN_LED, LOW);
  noTone(PIN_BUZZER);
}
