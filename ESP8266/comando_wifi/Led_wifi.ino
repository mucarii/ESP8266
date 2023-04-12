
/*
Para acessar os comando versao web:
- Cheque o ip que esta conectado
- No navgador digite \\"endereco de ip"
- De enter
*/

// Inclusão das bibliotecas
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// Configração do WiFi
const char* ssid = "rede Wifi";  // SSID Wifi - insira sua rede wifi
const char* password = "senha wifi";  // Senha Wifi - insira a senha do wifi

// Variáveis de Server e Status do LED
ESP8266WebServer server(80);
bool LEDstatus = LOW;

void setup() {
  // Inicia Serial e LED
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);

  // Inicia Conexão WiFi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Aguarda Conexão e Informa IP
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Rede WiFi: ");
  Serial.println(ssid);
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());
  delay(100);

  // Configura Handles do Server e Inicia Server
  server.on("/", handle_OnConnect);
  server.on("/ledon", handle_ledon);
  server.on("/ledoff", handle_ledoff);
  server.onNotFound(handle_NotFound);
  server.begin();
  Serial.println("Servidor HTTP iniciado!");

}

void loop() {
  server.handleClient();    // Faz o Handle
  if (LEDstatus)            // Checa se LED deve acender
    digitalWrite(LED_BUILTIN, HIGH);  
  else
    digitalWrite(LED_BUILTIN, LOW);
}

// FUNÇÕES HANDLE PARA HTML SERVER

void handle_OnConnect() {
  LEDstatus = LOW;
  server.send(200, "text/html", SendHTML(false));
}

void handle_ledon() {
  LEDstatus = HIGH;
  server.send(200, "text/html", SendHTML(true));
}

void handle_ledoff() {
  LEDstatus = LOW;
  server.send(200, "text/html", SendHTML(false));
}

void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}

String SendHTML(uint8_t led) {
  String ptr = "<!DOCTYPE html>\n";
  ptr += "<html>\n";
  ptr += "<head>\n";
  ptr += "<title>Controle do LED</title>\n";
  ptr += "</head>\n";
  ptr += "<body>\n";
  ptr += "<h1>LED</h1>\n";
  ptr += "<p>Clique para Ligar e Desligar o LED.</p>\n";
  ptr += "<form method=\"get\">\n";
  if (led)
    ptr += "<input type=\"button\" value=\"LED Desligado\" onclick=\"window.location.href='/ledoff'\">\n";
  else
    ptr += "<input type=\"button\" value=\"LED Ligado\" onclick=\"window.location.href='/ledon'\">\n";
  ptr += "</form>\n";
  ptr += "</body>\n";
  ptr += "</html>\n";
  return ptr;
}
