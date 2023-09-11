#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include "Arduino.h"
#include <HTTPClient.h>
#include "time.h"
#include "sntp.h"
#include <ArduinoJson.h>
#include <SPIFFS.h>

#include <base64.h> // Certifique-se de incluir uma biblioteca de codificação base64 no seu projeto


//#define _DISABLE_TLS_
WiFiClientSecure client;
// Cria um novo objeto WebServer
WebServer server(80);




//###########################################CONFIGURA O LOOP DE UPDATE NO SERVDIOR SQL
unsigned long previousMillis = 0;
const long interval = 30000; // Intervalo de 30 segundos


//############################################CONFIG SENSOR TEMPERATURA PRESSAO
#define SENSORES 0
 
#include <Adafruit_BMP280.h>
#include <Adafruit_AHTX0.h>
//#define I2C_SDA 20
//#define I2C_SCL 21
//TwoWire I2C = TwoWire(1);
//Wire.begin(I2C_SDA, I2C_SCL);
//config BMP AHT I2C
Adafruit_AHTX0 aht;
Adafruit_BMP280 bmp; 


int valoratm = 0;//Declara a variável valorldr como inteiro
int valortemp = 0;//Declara a variável valorldr como inteiro
int valorhumi = 0;//Declara a variável valorldr como inteiro




#if (SENSORES == 1)



String readDHTTemperature() {
  sensors_event_t humidity, temp;
  aht.getEvent(&humidity, &temp);
  
  float t = temp.temperature;

  if (isnan(t)) {    
   // Serial.println("Failed to read from DHT sensor!");
    return "--";
  }
  else {
  //  Serial.println(t);
    return String(t);
  }
}

String readDHTHumidity() {
    sensors_event_t humidity, temp;
  aht.getEvent(&humidity, &temp);
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = humidity.relative_humidity;
  if (isnan(h)) {
    //Serial.println("Failed to read from DHT sensor!");
    return "--";
  }
  else {
   // Serial.println(h);
    return String(h);
  }
}

String readDHTPressao() {
  float p = bmp.readPressure();

      return String(p);
}
#endif

#if (SENSORES == 0)
String readDHTTemperature() {
  return "desativado";
}
String readDHTHumidity() {
  return "desativado";
}
String readDHTPressao() {
  return "desativado";
}
#endif

//####################CONFIG CAMERA

#include "soc/rtc_cntl_reg.h"  // Disable brownour problems
#include "soc/soc.h"           // Disable brownour problems
#include "sdkconfig.h"
#include "driver/rtc_io.h"
#include <esp_system.h>
#include <nvs_flash.h>
#include "esp_camera.h"

// Configuração da câmera
camera_fb_t * fb = NULL;
bool dataAvailable = false;

int max_frames = 20;
framesize_t configframesize = FRAMESIZE_VGA; // FRAMESIZE_ + QVGA|CIF|VGA|SVGA|XGA|SXGA|UXGA
int frame_interval = 500;          // 0 = record at full speed, 100 = 100 ms delay between frames
float speed_up_factor = 1;          // 1 = play at realtime, 0.5 = slow motion, 10 = speedup 10x
int framesize = FRAMESIZE_VGA; //FRAMESIZE_HD;
int quality = 8;
int qualityconfig = 4;



/// defined(CAMERA_MODEL_ESP32S3_EYE)
#define PWDN_GPIO_NUM -1
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM 15
#define SIOD_GPIO_NUM 4
#define SIOC_GPIO_NUM 5

#define Y2_GPIO_NUM 11
#define Y3_GPIO_NUM 9
#define Y4_GPIO_NUM 8
#define Y5_GPIO_NUM 10
#define Y6_GPIO_NUM 12
#define Y7_GPIO_NUM 18
#define Y8_GPIO_NUM 17
#define Y9_GPIO_NUM 16

#define VSYNC_GPIO_NUM 6
#define HREF_GPIO_NUM 7
#define PCLK_GPIO_NUM 13


bool setupCamera()
{
  camera_config_t config;
 config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
 // config.frame_size = FRAMESIZE_QVGA;
  config.pixel_format = PIXFORMAT_JPEG; // for streaming
  //config.pixel_format = PIXFORMAT_RGB565; // for face detection/recognition
  config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
  config.fb_location = CAMERA_FB_IN_PSRAM;
//  config.jpeg_quality = 12;
//  config.fb_count = 1;
  
  //init with high specs to pre-allocate larger buffers
  if (psramFound()) {
    config.frame_size = configframesize;
    config.jpeg_quality = qualityconfig;
    config.fb_count = 4; 
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

  Serial.printf("Internal Total heap %d, internal Free Heap %d\n", ESP.getHeapSize(), ESP.getFreeHeap());
  Serial.printf("SPIRam Total heap   %d, SPIRam Free Heap   %d\n", ESP.getPsramSize(), ESP.getFreePsram());

  static char * memtmp = (char *) malloc(32 * 1024);
  static char * memtmp2 = (char *) malloc(32 * 1024); //32767

  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return false;
  }
 
  free(memtmp2);
  memtmp2 = NULL;
  free(memtmp);
  memtmp = NULL;


  sensor_t * s = esp_camera_sensor_get();

  //  drop down frame size for higher initial frame rate
  s->set_framesize(s, (framesize_t)framesize);
  s->set_quality(s, quality);
  delay(200);
  return true;
}



//################CONFIGURA TASKS

TaskHandle_t xHandle = NULL;
#define STACK_SIZE 10000
StaticTask_t xTaskBuffer;
StackType_t xStack[ STACK_SIZE ];



// File paths to save input values permanently
const char* wifiPath = "/wifi.txt";
const char* estadoPath = "/estado.txt";


String clima;
String cor = "0xFFFFFF";
String text;
String velocidade = "60";
String dimmer = "250";
String velovar;
String veloframe;
String dimmervar;



bool wifiConnected = false;
String ssid;
String password;
String nomedobot;
String geo;
String estado;
String mensagem;
String txt = mensagem.c_str();
void saveWifiCredentials(const String &ssid, const String &password, const String &nomedobot, const String &geo);
void loadWifiCredentials(String &ssid, String &password, String &nomedobot, String &geo);
void savefile(const String &estado, const String &mensagem);
void loadfile(String &estado, String &mensagem);



//##################### Configura IP

IPAddress localIP;
//IPAddress localIP(192, 168, 1, 200); // hardcoded

// Set your Gateway IP address
IPAddress localGateway;
//IPAddress localGateway(192, 168, 1, 1); //hardcoded
IPAddress subnet(255, 255, 0, 0);
IPAddress primaryDNS(8, 8, 8, 8); //nao nao eh opcional nao, eh obrigatorio demorei mt tempo pra perceber q o relogio e o telegram n tava funcionando por causa do dns




// Configuração do certificado CA (Certificado de Autoridade) para confiar no servidor remoto
const char* caCert = \
 "-----BEGIN CERTIFICATE-----\n"
  "MIICtjCCAZ4CAQAwFTETMBEGA1UEAwwKdGVzLm9ibS5wdDCCASIwDQYJKoZIhvcN\n"
  "AQEBBQADggEPADCCAQoCggEBANvuYarL3NB+2S4uGc8IftHLctFT8qKAzAcJt3yJ\n"
  "iFa4dixMgxNMnNZ8t1iAMEIlm0exzQ9Jbhi0i27RGwXnUDU5sS9RESqc02ca3HSd\n"
  "DBWCc3RYxetHJTJWAIam7D7qlsHTdLBAKUKKhytHD+uFYcC0D44IHfUmXmoQva9c\n"
  "QNSWeFZJQOywcclQyaThjSpg9xbg09Nx1/JLRV4ocKMnSm1oR9EkGu1oI8ppgeQv\n"
  "wrEyPYniRlNY0uT7K+NvCKZ46J1naPrnWwleqxp/JGfykH63u6s4VLZEQ7ibmAI3\n"
  "WX3hXTcGYxcz3LvwZNjpRslAX3iFLr9docf0HjSbF70zcv0CAwEAAaBcMFoGCSqG\n"
  "SIb3DQEJDjFNMEswCQYDVR0TBAIwADALBgNVHQ8EBAMCBeAwMQYDVR0RBCowKIIK\n"
  "dGVzLm9ibS5wdIIMKi50ZXMub2JtLnB0ggwqLnRlcy5vYm0ucHQwDQYJKoZIhvcN\n"
  "AQELBQADggEBALYRG8PoLow3R6Z7X9ZntRvHYksRhaUsgmOqgqB/H2SjEFZ0kTvY\n"
  "0BIG98FFfHRG8omo/jlsPBBD/el8WzrBNRWvennpvWRteBheM5eSruHjZO5tOwHd\n"
  "b+A6i8GmMpQVAzx31oM+fNXUzUDUGtltIxxIQMsclkqiGWuT56QZ2oZEtVTOmQP9\n"
  "NPsmirCe0CfN47LgHz73X1wjp6nd09yloR/pFO4H+gruyh+NwnKp9wP4Pq/3i6g7\n"
  "hl0mVBvlvMs/7Jt3guvcdSHPc2yyEiarRUPUxCzrjG8RRr7HpY7xkKYRI06ROPsj\n"
  "LOYRaRnaPslLfcSXH1MsIjXRgq077tzeHdo=\n"
  "-----END CERTIFICATE-----\n";


//##############CONFIG DOS CONTADORES
uint32_t read_nvs_data();
uint32_t contagrana_data();
uint32_t contaobj_data();

void save_nvs_data(uint32_t data);
void contagrana_data(uint32_t data);
void contaobj_data(uint32_t data);

uint32_t last_id;
uint32_t contagrana;
uint32_t contaobj;

const char* host = "santocyber.helioho.st";
String url = "https://santocyber.helioho.st/mushlightgw/gravatele.php";
String url6 = "https://santocyber.helioho.st/mushlightgw/verificaacao.php";
String url7 = "https://santocyber.helioho.st/mushlightgw/api.php";
String url8 = "https://santocyber.helioho.st/mushlightgw/salvafoto.php";


String State = "menu";
String StateUpdate = "desativado";
String payload = "";

int pagamento = 0;
int ordervalue = 1;


// Variáveis para rastrear o estado do botão e os cliques
int buttonState = 0;       // Variável para armazenar o estado do botão

int contacendled = 0;
int conta = 1;
unsigned long intervalo = 0;


//###########################PCONFIGURACAO DAS PINAGENS
      int Button1 = 0;  // Pino do botão
const int ledPin = 48;     // Pino do LED
const int relayPin = 2; // Pino GPIO para o relé


//########################## Setting clock just to be sure...

const char* ntpServer1 = "pool.ntp.org";
const char* ntpServer2 = "time.nist.gov";
const long  gmtOffset_sec = -10800;
const int   daylightOffset_sec = 0;

const char* time_zone = "CET-3CEST,M3.5.0,M10.5.0/3";  // TimeZone rule for Europe/Rome including daylight adjustment rules (optional)


String obterHoraAtual() {

    time_t agora = time(nullptr); // Obtemos a hora atual
    struct tm infoTempo;
    localtime_r(&agora, &infoTempo); // Converte o tempo em uma estrutura tm

    char buffer[20]; // Buffer para armazenar a hora formatada (HH:MM:SS\0)
    snprintf(buffer, sizeof(buffer), "%02d:%02d:%02d_%02d/%02d/%04d", infoTempo.tm_hour, infoTempo.tm_min, infoTempo.tm_sec, infoTempo.tm_mday, infoTempo.tm_mon + 1, infoTempo.tm_year + 1900);
             
    return String(buffer);
}

void setClock() {

  sntp_servermode_dhcp(1);    // (optional)
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer1, ntpServer2);
}




//#########################################Cria paginas WEB

void handleMain() {
  String html = "<html><head>";
  html += "<style>";
  html += "body { font-family: Arial, sans-serif; margin: 20px; }";
  html += "h1 { color: #333; }";
  html += "h2 { color: #666; }";
  html += "p { margin: 10px 0; }";
  html += "</style>";
  html += "</head><body>";
  html += "<h1>GRANANET WEBPAGE</h1>";
  html += "<h2>NOMEBOT:</h2>";
  html += "<p class='data'>" + nomedobot + "</p>";
  html += "<h2>GEO:</h2>";
  html += "<p class='data'>" + geo + "</p>";
  html += "<h2>SSID:</h2>";
  html += "<p class='data'>" + ssid + "</p>";
  html += "<h2>MENSAGEM:</h2>";
  html += "<p class='data'>" + String(mensagem) + "</p>";
  html += "<h2>ESTADO:</h2>";
  html += "<p class='data'>" + String(estado) + "</p>";
  html += "</body></html>";

  server.send(200, "text/html", html);
}

void handleRoot() {
  loadWifiCredentials(ssid, password, nomedobot,geo);

  String html = "<html><head>";
  html += "<style>";
  html += "body { font-family: Arial, sans-serif; margin: 20px; }";
  html += "h1 { color: #333; }";
  html += "h2 { color: #666; }";
  html += "p { margin: 10px 0; }";
  html += ".data { font-weight: bold; }";
  html += ".button { background-color: #007bff; color: #fff; padding: 10px 20px; border: none; cursor: pointer; }";
  html += "</style>";
  html += "</head><body>";
  html += "<h1>CONFIG GRANANET</h1>";
  html += "<h2>NOMEBOT:</h2>";
  html += "<p class='data'>" + nomedobot + "</p>";
  html += "<h2>GEO:</h2>";
  html += "<p class='data'>" + geo + "</p>";
  html += "<h2>SSID:</h2>";
  html += "<p class='data'>" + ssid + "</p>";
  html += "<h2>password:</h2>";
  html += "<p class='data'>" + password + "</p>";
  html += "<form method='get' action='/scan'>";
  html += "<input type='submit' class='button' value='Escanear Redes'>";
  html += "</form>";
  html += "</body></html>";

  server.send(200, "text/html", html);
}

void handleScan() {
  Serial.println("SCAN");
  // Send an HTTP response
  String html = "<html><head><style>";
  html += "body { font-family: Arial, Helvetica, sans-serif; background-color: #f4f4f4; text-align: center; }";
  html += "h1 { color: #333; }";
  html += "ul { list-style-type: none; padding: 0; }";
  html += "li { margin: 10px 0; }";
  html += "label { font-weight: bold; }";
  html += "input[type='text'], input[type='password'] { width: 100%; padding: 10px; margin: 5px 0; }";
  html += "input[type='submit'], input[type='button'] { background-color: #4CAF50; color: white; padding: 10px 20px; border: none; cursor: pointer; }";
  html += "a { text-decoration: none; }";
  html += "</style></head><body>";
  html += "<h1>Selecione Wi-Fi Network:</h1>";
  html += "<form method='post' action='/connect'>";
  html += "<ul>";
  WiFi.disconnect();

  // Scan for Wi-Fi networks
   int n = WiFi.scanNetworks();
    Serial.println("Scan done");
    if (n == 0) {
        html += "<h1>No Wi-Fi Network:</h1>";
        Serial.println("no networks found");
    } else {
  for (int i = 0; i < n; i++) {
    // Add each network to the HTML list
    String ssid = WiFi.SSID(i);
    int32_t rssi = WiFi.RSSI(i);
    html += "<li><input type='radio' name='ssid' value='" + ssid + "'>" + ssid + " (RSSI: " + String(rssi) + ")</li>";
  }}

  html += "</ul>";
  html += "<label for='nomedobot'>NOMEDOBOT:</label><input type='text' name='nomedobot'><br>";
  html += "<label for='geo'>GEOLOCALIZACAO:</label><input type='text' name='geo'><br>";
  html += "<label for='password'>Password:</label><input type='password' name='password'><br>";
  html += "<input type='submit' value='CONECTAR'>";
  html += "</form>";
  html += "<p><br><br><br><a href='/'>Back</a></p>";
  html += "<input type='button' value='PESQUISAR NOVAMENTE' onclick='location.reload()'>";
  html += "</body></html>";

  // Send the HTML response
  server.send(200, "text/html", html);
}

void handleConnect() {
  Serial.println("CONECT");

  String ssid = server.arg("ssid");
  String password = server.arg("password");
  String nomedobot = server.arg("nomedobot");
  String geo = server.arg("geo");
saveWifiCredentials(ssid.c_str(), password.c_str(), nomedobot.c_str(), geo.c_str());

  // Verifique se o SSID e a senha não estão vazios
  if (ssid.length() > 0 && password.length() > 0) {
    WiFi.begin(ssid.c_str(), password.c_str());

    // Aguarde a conexão ser estabelecida
    int timeout = 10; // Tempo limite para a conexão em segundos
    while (WiFi.status() != WL_CONNECTED && timeout > 0) {
      delay(1000); // Aguarde 1 segundo
      timeout--;
    }

    if (WiFi.status() == WL_CONNECTED) {
      String configg = "Prontinho. GRANANET reiniciando, conecte no seu WIFI e clique no endereco abaixo IP: " + WiFi.localIP().toString();
      configg += "<br><br>";
      configg += "CLIQUE -> VVVVVVVV";
      configg += "<br><br>";
      configg += "<a href='http://";
      configg += WiFi.localIP().toString();
      configg += "'>";
      configg += nomedobot.c_str();
      configg += "</a>";
      server.send(200, "text/html", configg);
      delay(5000);
      ESP.restart();
    } else {
      server.send(200, "text/plain", "n conectado!  ");
    }
  }
}



bool initWiFi() {
  loadWifiCredentials(ssid, password, nomedobot,geo);
  ssid.trim();
  password.trim();
  nomedobot.trim();
  geo.trim();
  
  WiFi.mode(WIFI_STA);
  
  if(ssid==""){
    Serial.println("SSID indefindo");
    return false;
  }

 // Verifique se o SSID e a senha não estão vazios
  if (ssid.length() > 0 && password.length() > 0) {

    WiFi.begin(ssid, password);
    Serial.println("Connecting to WiFi...");
    Serial.println(ssid.c_str());
    Serial.println(password.c_str());
    Serial.println(nomedobot.c_str());
    Serial.println(geo.c_str());
    }
   delay(5000);
  if (WiFi.status() == WL_CONNECTED) {
  Serial.println("conectado");
  WiFi.setHostname(nomedobot.c_str());
  localIP;
  localGateway;
  Serial.println(WiFi.localIP());
  setClock();  

  StateUpdate = "ativo";


 if (!MDNS.begin(nomedobot.c_str())) {
    Serial.println("Error setting up MDNS responder!");
    return false;
  } else {
    MDNS.addService("http", "tcp", 80);
    Serial.printf("mDNS responder started '%s'\n", nomedobot.c_str());
    Serial.print("You can now connect to http://");
    Serial.print(nomedobot.c_str());
    Serial.println(".local");   

 }

    return true;
  
  if (!WiFi.config(localIP, localGateway, subnet,primaryDNS)){
    Serial.println("STA Failed to configure");
    return false;
  }
    } else {
    Serial.println("nao conectado");
   return false;

    }
  }


void setup() {
  // Note the format for setting a serial port is as follows: Serial2.begin(baud-rate, protocol, RX pin, TX pin);
   Serial.begin(115200);
   //Serial1.begin(115200);
//   Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);
 
  
  // ################DEFINE PINAGENS
  pinMode(Button1, INPUT); // Define o pino do botão como entrada
  pinMode(ledPin, OUTPUT);   // Define o pino do LED como saída
  pinMode(relayPin, OUTPUT);
  
  digitalWrite(relayPin, LOW); // Inicialmente, o relé está desligado

if (!SPIFFS.begin(true)) {
    Serial.println("An error has occurred while mounting SPIFFS");
  }
  Serial.println("SPIFFS mounted successfully");



  if(initWiFi()) {
  server.on("/", handleMain);
 // server.on("/desligar", handleDesligar);
 // server.on("/ligar", handleLigar);
  server.begin();
  Serial.println("Servidor web iniciado");
  }

else {
// Iniciar o modo AP
  WiFi.mode(WIFI_AP);
  WiFi.softAP("GRANANET-Config"); // Nome da rede AP
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("Endereço IP do AP: ");
  Serial.println(myIP);
  // Inicializa a biblioteca WiFi
  WiFi.begin();

    // Inicializa o servidor web
  server.on("/", handleRoot);
  server.on("/scan", HTTP_GET, handleScan);
  server.on("/connect", handleConnect);
 // server.on("/desligar", handleDesligar);
 // server.on("/ligar", handleLigar);
  server.begin();
  Serial.println("Servidor web AP iniciado");

}



  loadfile(estado, mensagem);
  
  Serial.println("VARIAVEIS ARMAZENADA"); 
  Serial.println(estado);
  Serial.println(mensagem);

 // Camera init
if (!setupCamera())
  {
    Serial.println("Camera Setup Failed!");
  }else{    Serial.println("Camera INICIADA");
}

  // Serial.setDebugOutput(true);
//#####################################################wifi scan
}



void loop() {
  // Executa o servidor web
  server.handleClient();
//##################################################LOOP SE CONECTADO
 if (StateUpdate == "ativo") {

unsigned long currentMillis = millis();

  // Verificar se passaram 30 segundos
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // Obter e imprimir a hora atual
    time_t now = time(nullptr);
    struct tm timeinfo;
    gmtime_r(&now, &timeinfo);
    Serial.println("Iniciando rotina : ");
    Serial.print(interval);
    Serial.print(" segundos");
    Serial.println(" ");
    Serial.println("CAPTURA IMAGEM");
    capturaimagem();
    Serial.println(" ");

    Serial.print("Hora atual: ");
    Serial.printf("%02d:%02d:%02d\n", timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
    telemetria();
    verifyActionAndExecute();
  }
}


//###############INICIA LOOP MENU

 if (State == "menu") {
  menu();
 }
  
 if(State == "valor"){
    Serial.print("VALOR");

}
 if(State == "pix"){      
      Serial.print("PIX");
    State = "menu";

}
}
  
void menu(){
     // Serial.println("MENU");


  
    buttonState = digitalRead(Button1); // Lê o estado do botão

 if (buttonState == LOW) { // Se o botão estiver pressionado
      Serial.println("botao pressionado MENU");
      Serial.println(contacendled);

      digitalWrite(RGB_BUILTIN, HIGH);
      contacendled++;
      delay(500); 
      digitalWrite(RGB_BUILTIN, LOW);      
      intervalo = millis();}

  if(millis() > intervalo+4000){
  contacendled = 0;
}    
   if(millis() > intervalo+2000){

  if(contacendled == 1){
    //##########################################################################################
       Serial.println("INICIA VALOR");

       State = "valor";
    
  }
  if(contacendled == 2){
  //##########################################################################################

      neopixelWrite(RGB_BUILTIN,0,RGB_BRIGHTNESS,0); // Green
      Serial.println("botao 2");

  }
  if(contacendled == 3){
  neopixelWrite(RGB_BUILTIN,0,60,78); 
  Serial.println("botao 3 granaentrada");  

     State = "menu";


  }
   if(contacendled == 4){
  neopixelWrite(RGB_BUILTIN,0,60,78); 
  Serial.println("botao 4  objsaida");  


  delay(2000);
     State = "menu";     
  
  }

 if(contacendled == 10){
  neopixelWrite(RGB_BUILTIN,0,60,78); 
  Serial.println("botao 10  apaga granaentrada obdjsiada");  

  Serial.println("apagando grana entrada");  

  delay(2000);
     State = "menu";     
  
  }
   if(contacendled == 17){
  neopixelWrite(RGB_BUILTIN,0,60,78); 
  Serial.println("botao 11  apaga wifi");  
  deletewififile();
  Serial.println("apagando wifi");  

  delay(2000);
     State = "menu";     
  
  }
  }}



//################################ Função para salvar o SSID e a senha no SPIFFS
void saveWifiCredentials(const String &ssid, const String &password, const String &nomedobot, const String &geo) {
  File file = SPIFFS.open("/wifi_credentials.txt", "w");
  if (!file) {
    Serial.println("Erro ao abrir o arquivo para salvar as credenciais.");
    return;
  }

  file.println(ssid);
  file.println(password);
  file.println(nomedobot);
  file.println(geo);

  file.close();
}

// Função para carregar o SSID e a senha do SPIFFS
void  loadWifiCredentials(String &ssid, String &password, String &nomedobot, String &geo) {
  File file = SPIFFS.open("/wifi_credentials.txt", "r");
  if (!file) {
    Serial.println("Arquivo de credenciais não encontrado.");
    return;
  }

  ssid = file.readStringUntil('\n');
  password = file.readStringUntil('\n');
  nomedobot = file.readStringUntil('\n');
  geo = file.readStringUntil('\n');

  file.close();
}

  void deletewififile() {
    String ssid0 ="";
    String password0 ="";
    String nomedobot0 ="";
    String geo0 ="";
      saveWifiCredentials(ssid0,password0,nomedobot0,geo0);
      ESP.restart();
}

//###########################



//################################ Função para salvar no arquivo txt
void savefile(const String &estado, const String &mensagem) {
  File file = SPIFFS.open("/estado.txt", "w");
  if (!file) {
    Serial.println("Erro ao abrir o arquivo.");
    return;
  }

  file.println(estado);
  file.println(mensagem);
  file.close();
}

// Função para carregar o SSID e a senha do SPIFFS
void  loadfile(String &estado, String &mensagem) {
  File file = SPIFFS.open("/estado.txt", "r");
  if (!file) {
    Serial.println("Arquivo não encontrado.");
    return;
  }

  estado = file.readStringUntil('\n');
  mensagem = file.readStringUntil('\n');

  file.close();
}

  void deletefile() {
    String estado0 ="";
    String mensagem0 ="";
    savefile(estado0,mensagem0);
}












//#########################SALVANDO e apagando CONTADORES EM ARQUIVOS

void delete_contagrana_data_file() {
          contagrana = 0;
          contagrana_data(contagrana);
}
void delete_contaobj_data_file() {
          contaobj = 0;
          contaobj_data(contaobj);
}
uint32_t read_contagrana(){
uint32_t contagrana = 0;
  if (SPIFFS.begin(true))
  {
    File contagranaFile = SPIFFS.open("/contagrana.txt", "r");
    if (contagranaFile)
    {
      contagrana = contagranaFile.parseInt();
      contagranaFile.close();
    }
  }
  return contagrana;
}

void contagrana_data(uint32_t data){
  if (SPIFFS.begin(true))
  {
    File contagranaFile = SPIFFS.open("/contagrana.txt", "w");
    if (contagranaFile)
    {
      contagranaFile.print(data);
      contagranaFile.close();
    }
  }
}


uint32_t read_contaobj(){
uint32_t contaobj = 0;
  if (SPIFFS.begin(true))
  {
    File contaobjFile = SPIFFS.open("/contaobj.txt", "r");
    if (contaobjFile)
    {
      contaobj = contaobjFile.parseInt();
      contaobjFile.close();
    }
  }
  return contaobj;
}

void contaobj_data(uint32_t data){
  if (SPIFFS.begin(true))
  {
    File contaobjFile = SPIFFS.open("/contaobj.txt", "w");
    if (contaobjFile)
    {
      contaobjFile.print(data);
      contaobjFile.close();
    }
  }
}
