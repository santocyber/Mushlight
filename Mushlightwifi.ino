#include <WiFi.h>
#include <Wire.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"
#include "FastLED.h"
#include <NTPClient.h>
#include "fonts.h"
#include "timerMinim.h"
#include <WiFiUdp.h>
#include "bitmap1.h"
#include "bitmap2.h"
#include "bitmap3.h"
#include "bitmap4.h"
#include "bitmap5.h"
#include "bitmap6.h"
#include "bitmap7.h"
#include "bitmap8.h"
#include <DHT.h>
#include <FS.h>
#include "esp_camera.h"
#include <WiFiClientSecure.h>
#include <time.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "EEPROM.h"


//#####Configura data logger

String    webpage,time_now,log_time,lastcall,time_str, DataFile = "datalog.txt";
#define TZone -3

//####Configura contador
unsigned long cSegundos = 0;
unsigned int segundos = 0;
byte minutos = 0;



//###########Configura Crypto

const int httpsPort = 443;



//##########Configura cam
boolean takeNewPhoto = false;
boolean dataAvailable = false;


//CAMERA_MODEL_AI_THINKER
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27

#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

// Photo File Name to save in SPIFFS
#define FILE_PHOTO "/photo.jpg"

// Check if photo capture was successful
bool checkPhoto( fs::FS &fs ) {
  File f_pic = fs.open( FILE_PHOTO );
  unsigned int pic_sz = f_pic.size();
  return ( pic_sz > 100 );

}
 


//##########CONFIG TEXTO
String runningText = "MushLight";


//#############Configura LED GPI0
#define LED_PIN 5
#define MQ_analog 2
#define vibra 4

int valoratm = 0;//Declara a variável valorldr como inteiro
int valortemp = 0;//Declara a variável valorldr como inteiro
int valorhumi = 0;//Declara a variável valorldr como inteiro
int valorco2 = analogRead(MQ_analog);;//Declara a variável valorldr como inteiro
int valorvibra = analogRead(vibra);
int valor_analog = analogRead(MQ_analog);

String ledState;
boolean led_state = true;
int clap_counter = 0;
int color_counter = 0;


//############CONFIG TEMP HUMI PRESSAO
#define DHTPIN 15     // Digital pin connected to the DHT sensor
#define DHTTYPE    DHT11     // DHT 11
DHT dht(DHTPIN, DHTTYPE);



//############Configura led
//http://fastled.io/docs/3.1/struct_c_r_g_b.html


#define WIDTH 16
#define HEIGHT 16
#define SEGMENTS 1



#define NUM_LEDS WIDTH * HEIGHT * SEGMENTS
#define CHIPSET WS2811
CRGB leds[NUM_LEDS];



// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

AsyncEventSource events("/events");


// Search for parameter in HTTP POST request
const char* PARAM_INPUT_1 = "ssid";
const char* PARAM_INPUT_2 = "pass";
const char* PARAM_INPUT_3 = "ip";
const char* PARAM_INPUT_4 = "gateway";
const char* PARAM_INPUT_5 = "tokentelegram";
const char* PARAM_INPUT_11 = "clima";
const char* PARAM_INPUT_12 = "logger";
const char* PARAM_INPUT_6 = "txt";
const char* PARAM_INPUT_7 = "cor";
const char* PARAM_INPUT_8 = "text";
const char* PARAM_INPUT_9 = "velocidade";
const char* PARAM_INPUT_10 = "dimmer";

//Variables to save values from HTML form
String ssid;
String pass;
String ip;
String gateway;
String tokentelegram;
String clima;
String txt = runningText;
String cor = "0xFFFFFF";
String text;
String velocidade = "60";
String dimmer = "250";
//String bot;
String id, welcome, from_name;//Váriaveis para armazenamento do ID e TEXTO gerado pelo Usuario
String msg;
String readings;
String logger;


// File paths to save input values permanently
const char* ssidPath = "/ssid.txt";
const char* passPath = "/pass.txt";
const char* ipPath = "/ip.txt";
const char* gatewayPath = "/gateway.txt";
const char* tokentelegramPath = "/tokentelegram.txt";
const char* climaPath = "/clima.txt";
const char* loggerPath = "/log.txt";



String velovar;
String veloframe;
String dimmervar;





WiFiClientSecure client;
HTTPClient http;


//############CONFIG TIMER
#define UDP_PACKET_MAX_SIZE 1024
char incomeBuffer[UDP_PACKET_MAX_SIZE]; 

#define NTP_OFFSET  -3  * 60 * 60 // In seconds
#define NTP_INTERVAL 60 * 1000    // In miliseconds
#define NTP_ADDRESS  "0.pool.ntp.org"
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, NTP_ADDRESS, NTP_OFFSET, NTP_INTERVAL);
const int udpPort = 3333;



//##configura o millis
unsigned long tempo = millis();
unsigned long tempo2 = millis();
unsigned long tempo3 = millis();
unsigned long tempo4 = millis();
unsigned long tempo5 = millis();
unsigned long tempo6 = millis();
unsigned long tempo7 = millis();
unsigned long tempo8 = millis();

boolean AUTOPLAY = 1;        
#define AUTOPLAY_PERIOD 30    
#define IDLE_TIME 10 
#define D_TEXT_SPEED 50      
#define D_TEXT_SPEED_MIN 10
#define D_TEXT_SPEED_MAX 255

#define D_EFFECT_SPEED 80    
#define D_EFFECT_SPEED_MIN 0
#define D_EFFECT_SPEED_MAX 255
#define DEMO_GAME_SPEED 60    // скорость игр в демо режиме (мс)
#define D_GIF_SPEED 100   
#define USE_SNAKE 1        
#define USE_TETRIS 1 
#define USE_ARKAN  1 

int scrollSpeed = D_TEXT_SPEED;    // скорость прокрутки текста бегущей строки
int gameSpeed = DEMO_GAME_SPEED;   // скорость в играх
int effectSpeed = D_EFFECT_SPEED; 

uint32_t idleTime = ((long)IDLE_TIME * 60 * 1000);      // минуты -> миллисек
uint32_t autoplayTime = ((long)AUTOPLAY_PERIOD * 1000); // секунды -> миллисек
uint32_t autoplayTimer;


//##################### Configura voids

#define SCORE_SIZE 0  
#define FASTLED_INTERRUPT_RETRY_COUNT 0
#define FASTLED_ALLOW_INTERRUPTS 0
#define BRIGHTNESS 255
#define CURRENT_LIMIT 2000

#define WIDTH 16
#define HEIGHT 16
#define SEGMENTS 1

#define COLOR_ORDER GRB

#define MATRIX_TYPE 0 
#define CONNECTION_ANGLE 0    
#define STRIP_DIRECTION 0 
#define SMOOTH_CHANGE 1
#define _WIDTH WIDTH
#define THIS_X x
#define THIS_Y y
#define SNOW_DENSE 20
#define HUE_ADD 0
#define SPARKLES 50
#define COOLING  120

// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more flickery fire.
// Default 120, suggested range 50-200.
#define SPARKING 200
#define FRAMES_PER_SECOND velovar
bool gReverseDirection = false;
unsigned char matrixValue[8][16];
unsigned char line[WIDTH];
int pcnt = 30;

byte dawnDuration = 0;            
byte realDawnDuration = 0;   

timerMinim effectTimer(D_EFFECT_SPEED); 
timerMinim gifTimer(D_GIF_SPEED);
  timerMinim gameTimer(DEMO_GAME_SPEED);                  
timerMinim scrollTimer(D_TEXT_SPEED);                  
timerMinim changeTimer(70);                             
timerMinim halfsecTimer(500);                           
timerMinim idleTimer(idleTime);                        
timerMinim alarmSoundTimer(4294967295);                 
timerMinim fadeSoundTimer(4294967295);                  
timerMinim autoBrightnessTimer(500);                    
timerMinim saveSettingsTimer(15000);                    
timerMinim dawnTimer(4294967295);                       


#define GLOBAL_COLOR_1 CRGB::Green    
#define GLOBAL_COLOR_2 CRGB::Orange   



IPAddress localIP;
//IPAddress localIP(192, 168, 1, 200); // hardcoded

// Set your Gateway IP address
IPAddress localGateway;
//IPAddress localGateway(192, 168, 1, 1); //hardcoded
IPAddress subnet(255, 255, 0, 0);
IPAddress primaryDNS(8, 8, 8, 8); //nao nao eh opcional nao, eh obrigatorio demorei mt tempo pra perceber q o relogio e o telegram n tava funcionando por causa do dns

// Timer variables
unsigned long previousMillis = 0;
const long interval = 10000;  // interval to wait for Wi-Fi connection (milliseconds)


//###################Configura animation
boolean loadingFlag = true;      
boolean runningFlag; 
byte frameNum;  
boolean fullTextFlag = false; 
byte globalBrightness = BRIGHTNESS;
byte breathBrightness;             
uint32_t globalColor = 0xffffff;
byte modeCode;                     
byte thisMode = 0;
#define EFFECT_FIRE 5
boolean gameDemo = true;
byte buttons = 4;
boolean checkButtons() {
  if (buttons != 4) return true;
  return false;
}





//################### Arquivos 
// Initialize SPIFFS
void initSPIFFS() {
  if (!SPIFFS.begin(true)) {
    Serial.println("An error has occurred while mounting SPIFFS");
  }
  Serial.println("SPIFFS mounted successfully");
}



// Read  TOTAL File from SPIFFS
String readTotal(fs::FS &fs, const char * path){
  Serial.printf("Reading file: %s\r\n", path);

  File file = fs.open(path);
  if(!file || file.isDirectory()){
    Serial.println("- failed to open file for reading");
    return String();
  }
  
  String fileContent;
  while(file.available()){
    fileContent = file.readStringUntil('}');
    break;     
  }
  return fileContent;
}



// Read File from SPIFFS
String readFile(fs::FS &fs, const char * path){
  Serial.printf("Reading file: %s\r\n", path);

  File file = fs.open(path);
  if(!file || file.isDirectory()){
    Serial.println("- failed to open file for reading");
    return String();
  }
  
  String fileContent;
  while(file.available()){
    fileContent = file.readStringUntil('\n');
    break;     
  }
  return fileContent;
}

// Write file to SPIFFS
void writeFile(fs::FS &fs, const char * path, const char * message){
  Serial.printf("Writing file: %s\r\n", path);

  File file = fs.open(path, FILE_WRITE);
  if(!file){
    Serial.println("- failed to open file for writing");
    return;
  }
  if(file.print(message)){
    Serial.println("- file written");
  } else {
    Serial.println("- frite failed");
  }
}


// Adicinando no arquivo append file to SPIFFS
void addFile(fs::FS &fs, const char * path, const char * message){
  Serial.printf("ADD file: %s\r\n", path);

  File file = fs.open(path, FILE_APPEND);
  if(!file){
    Serial.println("- failed to open file for writing");
    return;
  }
  if(file.print(message)){
    Serial.println("- file adicionado");
  } else {
    Serial.println("- frite failed");
  }
}





//#####INICIA TELEGRAM TOKEN via web
String TOKEN2(){
  initSPIFFS();
    tokentelegram = readFile (SPIFFS, tokentelegramPath);

String TTT = tokentelegram.c_str();
return TTT; 
}
String TOKEN = TOKEN2();
#include <UniversalTelegramBot.h>
#define tokentele TOKEN
UniversalTelegramBot bot(tokentele, client);









// Initialize WiFi
bool initWiFi() {
  if(ssid=="" || ip==""){
    Serial.println("Undefined SSID or IP address.");
    return false;
  }

  WiFi.mode(WIFI_STA);
  localIP.fromString(ip.c_str());
  localGateway.fromString(gateway.c_str());


  if (!WiFi.config(localIP, localGateway, subnet,primaryDNS)){
    Serial.println("STA Failed to configure");
    return false;
  }
  WiFi.begin(ssid.c_str(), pass.c_str());
  
  //##################Configura servicoes q precisa de conexao
  
 // UniversalTelegramBot bot(tokentelegram.c_str(), client);
    timeClient.begin();
    timeClient.update();
    readTel();
    verifica();
    verifica2();
    StartTime();


  
  Serial.println("Connecting to WiFi...");

  unsigned long currentMillis = millis();
  previousMillis = currentMillis;

  while(WiFi.status() != WL_CONNECTED) {
    currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      Serial.println("Failed to connect.");
      return false;
    }
  }


  Serial.println(WiFi.localIP());
  return true;
}


 String readCO2() {

  float t = valor_analog;

  if (isnan(t)) {    
    Serial.println("Failed to read from CO2 sensor!");
    return "--";
  }
  else {
  //  Serial.println(t);
    return String(t);
  }
}

String readDHTTemperature() {
  float t = dht.readTemperature();

  if (isnan(t)) {    
    Serial.println("Failed to read from DHT sensor!");
    return "--";
  }
  else {
  //  Serial.println(t);
    return String(t);
  }
}

String readDHTHumidity() {
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  if (isnan(h)) {
    Serial.println("Failed to read from DHT sensor!");
    return "--";
  }
  else {
   // Serial.println(h);
    return String(h);
  }
}

String readDHTPressao() {
  float p = dht.readHumidity();
  if (isnan(p)) {
    Serial.println("Failed to read from DHT sensor!");
    return "--";
  }
  else {
  //  Serial.println(p);
    return String(p);
  }
}

  

          



String processor(const String& var){
 // Serial.println(var);
  if(var == "STATE"){
 return ledState;
  }
  else if(var == "TEMPERATURE"){
    return readDHTTemperature();
  }
  else if(var == "HUMIDITY"){
    return readDHTHumidity();
  
  }
  else if(var == "CO2"){

    return readCO2();  
  }
    else if(var == "PRESSAO"){
    return readDHTPressao();
  
  }
  return String();
}


//######################## SETUP


void setup() {
  // Serial port for debugging purposes

   pinMode(MQ_analog, INPUT);
   pinMode(vibra, INPUT);
   pinMode(DHTPIN, INPUT);

   Serial.begin(115200);
   dht.begin();

  EEPROM.begin(512);
    
 
//########### LED

FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);
  FastLED.setBrightness( BRIGHTNESS );

  if (CURRENT_LIMIT > 0) FastLED.setMaxPowerInVoltsAndMilliamps(5, CURRENT_LIMIT);
  FastLED.clear();
  FastLED.show();
  randomSeed(analogRead(0) + analogRead(1));


   

  initSPIFFS();

  
  // Load values saved in SPIFFS
  ssid = readFile(SPIFFS, ssidPath);
  pass = readFile(SPIFFS, passPath);
  ip = readFile(SPIFFS, ipPath);
  gateway = readFile (SPIFFS, gatewayPath);
  tokentelegram = readFile (SPIFFS, tokentelegramPath);
  clima = readFile (SPIFFS, climaPath);
  logger = readFile (SPIFFS, loggerPath);

  Serial.println(ssid);
  Serial.println(pass);
  Serial.println(ip);
  Serial.println(gateway);
  Serial.println(tokentelegram);
  Serial.println(clima);
  Serial.println(logger);


client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org



  if(initWiFi()) {

 
       

    // Route for root / web page
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
      request->send(SPIFFS, "/index.html", "text/html", false);
    });
    server.serveStatic("/", SPIFFS, "/");


    server.on("/", HTTP_POST, [](AsyncWebServerRequest *request) {
     
 int params = request->params();
      for(int i=0;i<params;i++){
        AsyncWebParameter* p = request->getParam(i);
        if(p->isPost()){

          // HTTP POST ssid value
          if (p->name() == PARAM_INPUT_11) {
            clima = p->value().c_str();
            Serial.print("txt set to: ");
            Serial.println(clima);
            // Write file to save value
            // request->send(200, "text/html", "<a href='../runningtext' Executar</a><br><br>Mensagem enviada:" + txt );
          
          }
          // HTTP POST ssid value
          if (p->name() == PARAM_INPUT_6) {
            txt = p->value().c_str();
            Serial.print("txt set to: ");
            Serial.println(txt);
            // Write file to save value
            // request->send(200, "text/html", "<a href='../runningtext' Executar</a><br><br>Mensagem enviada:" + txt );
          
          }

          // HTTP POST ssid value
          if (p->name() == PARAM_INPUT_7) {
            cor = p->value().c_str();
            Serial.print("cor set to: ");
            Serial.println(cor);
            // Write file to save value
            // request->send(200, "text/html", "<a href='../runningtext' Executar</a><br><br>Mensagem enviada:" + txt );
          
          }

          
          // HTTP POST ssid value
          if (p->name() == PARAM_INPUT_8) {
            text = p->value().c_str();
            Serial.print("cor set to: ");
            Serial.println(text);
            // Write file to save value
            // request->send(200, "text/html", "<a href='../runningtext' Executar</a><br><br>Mensagem enviada:" + txt );
          
          }
               
          // HTTP POST ssid value
          if (p->name() == PARAM_INPUT_9) {
            velocidade = p->value().c_str();
            Serial.print("velocidade set to: ");
            Serial.println(velocidade);
            // Write file to save value
            // request->send(200, "text/html", "<a href='../runningtext' Executar</a><br><br>Mensagem enviada:" + txt );
                }
  
          // HTTP POST ssid value
          if (p->name() == PARAM_INPUT_10) {
            dimmer = p->value().c_str();
            Serial.print("dimmer set to: ");
            Serial.println(dimmer);
            // Write file to save value
            // request->send(200, "text/html", "<a href='../runningtext' Executar</a><br><br>Mensagem enviada:" + txt );
    }}}
      
uint32_t dimmervar = strtoul(dimmer.c_str(), NULL, 16);
uint32_t corvar = strtoul(cor.c_str(), NULL, 16);
uint32_t velovar = strtoul(velocidade.c_str(), NULL, 16);
FastLED.setBrightness(dimmervar);
FastLED.delay(2000 / velovar);

    scrollTimer.setInterval(velovar);//reseta a velocidade
    effectTimer.setInterval(velovar);
    gameTimer.setInterval(velovar);
    gifTimer.setInterval(velovar);
    

    
             request->send(SPIFFS, "/index.html", "text/html", false, processor); 
      });


       
    // Route to set GPIO state to HIGH
    server.on("/log", HTTP_GET, [](AsyncWebServerRequest *request) {
      ledState = "log";
      verifica();
      request->send(SPIFFS, "/clima.txt", "text/html", false, processor);
    });
      // Route to set GPIO state to HIGH
    server.on("/loggrafico", HTTP_GET, [](AsyncWebServerRequest *request) {
      ledState = "loggrafico";
      
 
        request->send(SPIFFS, "/log.html", "text/html", false);
 //     request->send(SPIFFS, "/clima.txt", "text/json", false, processor);
//        request->send_P(200, "text/plain", getSensorReadings().c_str());

//   String json = getSensorReadings();
  //  request->send(200, "application/json", json);

    });

  // Request for the latest sensor readings
  server.on("/readings", HTTP_GET, [](AsyncWebServerRequest *request){
    String json = getSensorReadings();
    request->send(200, "application/json", json);
    request->send_P(200, "text/plain", getSensorReadings().c_str());

    json = String();
  });

    

        // Route to set GPIO state to HIGH
    server.on("/deletelog", HTTP_GET, [](AsyncWebServerRequest *request) {
      ledState = "deletelog";
      writeFile(SPIFFS, climaPath, "");
      writeFile(SPIFFS, loggerPath, "");

      request->send(SPIFFS, "/index.html", "text/html", false);
    });

    

    server.on("/ledon", HTTP_GET, [](AsyncWebServerRequest *request) {
    FastLED.setBrightness(255);

          ledState = "ledon";
      request->send(SPIFFS, "/index.html", "text/html", false, processor);
    });
    

    // Route to set GPIO state to HIGH
    server.on("/co2", HTTP_GET, [](AsyncWebServerRequest *request) {
      ledState = "co2";
      request->send(SPIFFS, "/index.html", "text/html", false, processor);
    });  
    // Route to set GPIO state to HIGH
    server.on("/humi", HTTP_GET, [](AsyncWebServerRequest *request) {
      ledState = "humi";
      request->send(SPIFFS, "/index.html", "text/html", false, processor);
    });  
    // Route to set GPIO state to HIGH
    server.on("/temp", HTTP_GET, [](AsyncWebServerRequest *request) {
      ledState = "temp";
      request->send(SPIFFS, "/index.html", "text/html", false, processor);
    });  
        // Route to set GPIO state to HIGH
    server.on("/pressao", HTTP_GET, [](AsyncWebServerRequest *request) {
      ledState = "pressao";
      request->send(SPIFFS, "/index.html", "text/html", false, processor);
    });  
    // Route to set GPIO state to HIGH
    server.on("/clima", HTTP_GET, [](AsyncWebServerRequest *request) {
      ledState = "clima";
      request->send(SPIFFS, "/index.html", "text/html", false, processor);
    });  
        // Route to set GPIO state to HIGH
    server.on("/dimmer1", HTTP_GET, [](AsyncWebServerRequest *request) {
      dimmer1();
      request->send(SPIFFS, "/index.html", "text/html", false, processor);
    });
            // Route to set GPIO state to HIGH
    server.on("/dimmer2", HTTP_GET, [](AsyncWebServerRequest *request) {
      dimmer2();
      request->send(SPIFFS, "/index.html", "text/html", false, processor);
    });
               // Route to set GPIO state to HIGH
    server.on("/dimmer3", HTTP_GET, [](AsyncWebServerRequest *request) {
      dimmer3();
      request->send(SPIFFS, "/index.html", "text/html", false, processor);
    });
                   // Route to set GPIO state to HIGH
    server.on("/dimmer4", HTTP_GET, [](AsyncWebServerRequest *request) {
      dimmer4();
      request->send(SPIFFS, "/index.html", "text/html", false, processor);
    });
    // Route to set GPIO state to HIGH
    server.on("/green", HTTP_GET, [](AsyncWebServerRequest *request) {
     
      FastLED.clear();
 
         fill_solid( leds, NUM_LEDS, CRGB::Green);
      ledState = "green";
      request->send(SPIFFS, "/index.html", "text/html", false, processor);
    });   
    
    // Route to set GPIO state to HIGH
    server.on("/blue", HTTP_GET, [](AsyncWebServerRequest *request) {
      blue();
      ledState = "blue";
      request->send(SPIFFS, "/index.html", "text/html", false, processor);
    });  
    // Route to set GPIO state to HIGH
    server.on("/red", HTTP_GET, [](AsyncWebServerRequest *request) {
      red();
      ledState = "red";
      request->send(SPIFFS, "/index.html", "text/html", false, processor);
    });
        // Route to set GPIO state to HIGH
    server.on("/amarelo", HTTP_GET, [](AsyncWebServerRequest *request) {
      amarelo();
        
      ledState = "amarelo";
      request->send(SPIFFS, "/index.html", "text/html", false, processor);
    });

           // Route to set GPIO state to HIGH
    server.on("/colors", HTTP_GET, [](AsyncWebServerRequest *request) {
      ledState = "colors";
      request->send(SPIFFS, "/index.html", "text/html", false, processor);
    });
    
    // Route to set GPIO state to HIGH
    server.on("/pisca", HTTP_GET, [](AsyncWebServerRequest *request) {
      pisca();
      ledState = "pisca";
      request->send(SPIFFS, "/index.html", "text/html", false, processor);
    });
        // Route to set GPIO state to HIGH
    server.on("/piscalento", HTTP_GET, [](AsyncWebServerRequest *request) {
      pisca();
      ledState = "piscalento";
      request->send(SPIFFS, "/index.html", "text/html", false, processor);
    });
    // Route to set GPIO state to HIGH
    server.on("/rainbowon", HTTP_GET, [](AsyncWebServerRequest *request) {
      rainbowon();
      ledState = "rainbowon";
      request->send(SPIFFS, "/index.html", "text/html", false, processor);
    });
        // Route to set GPIO state to HIGH
    server.on("/light", HTTP_GET, [](AsyncWebServerRequest *request) {  
      ledState = "light";
      request->send(SPIFFS, "/index.html", "text/html", false, processor);
    });     
    // Route to set GPIO state to HIGH
    server.on("/bpm", HTTP_GET, [](AsyncWebServerRequest *request) {   
      ledState = "bpm";
      request->send(SPIFFS, "/index.html", "text/html", false, processor);
    });
        // Route to set GPIO state to HIGH
    server.on("/sinelon", HTTP_GET, [](AsyncWebServerRequest *request) {   
      ledState = "sinelon";
      request->send(SPIFFS, "/index.html", "text/html", false, processor);
    });
        // Route to set GPIO state to HIGH
    server.on("/confetti", HTTP_GET, [](AsyncWebServerRequest *request) {   
      ledState = "confetti";
      request->send(SPIFFS, "/index.html", "text/html", false, processor);
    });
        // Route to set GPIO state to HIGH
    server.on("/rainbowgliter", HTTP_GET, [](AsyncWebServerRequest *request) {   
      ledState = "rainbowgliter";
      request->send(SPIFFS, "/index.html", "text/html", false, processor);
    });
            // Route to set GPIO state to HIGH
    server.on("/juggle", HTTP_GET, [](AsyncWebServerRequest *request) {   
      ledState = "juggle";
      request->send(SPIFFS, "/index.html", "text/html", false, processor);
    });

        // Route to set GPIO state to HIGH
    server.on("/runner", HTTP_GET, [](AsyncWebServerRequest *request) {
     // Fire2012WithPalette();   
      ledState = "runner";
      request->send(SPIFFS, "/index.html", "text/html", false, processor);
    });
    
    // Route to set GPIO state to HIGH
    server.on("/matrixon", HTTP_GET, [](AsyncWebServerRequest *request) {
          matrixRoutine();
          ledState = "matrixon";
      request->send(SPIFFS, "/index.html", "text/html", false, processor);
    });   
    
    // Route to set GPIO state to HIGH
    server.on("/rainbowstrip", HTTP_GET, [](AsyncWebServerRequest *request) {
          rainbowStripeNoise();
          ledState = "rainbowstrip";
      request->send(SPIFFS, "/index.html", "text/html", false, processor);
    });
    

    // Route to set GPIO state to LOW
    server.on("/off", HTTP_GET, [](AsyncWebServerRequest *request) {
      ledoff();
      ledState = "ledoff";
      request->send(SPIFFS, "/index.html", "text/html", false, processor);
    });
 // Route to set GPIO state to HIGH
    server.on("/pride", HTTP_GET, [](AsyncWebServerRequest *request) {
      pride();
        
      ledState = "pride";
      request->send(SPIFFS, "/index.html", "text/html", false, processor);
    });

            // Route to set GPIO state to HIGH
    server.on("/fire2", HTTP_GET, [](AsyncWebServerRequest *request) {
      fireRoutine();
        
      ledState = "fire2";
      request->send(SPIFFS, "/index.html", "text/html", false, processor);
    });
            // Route to set GPIO state to HIGH
    server.on("/snowon", HTTP_GET, [](AsyncWebServerRequest *request) {
      snowRoutine();
          ledState = "snowon";
      request->send(SPIFFS, "/index.html", "text/html", false, processor);
    });
            // Route to set GPIO state to HIGH
    server.on("/clock", HTTP_GET, [](AsyncWebServerRequest *request) {
  timeClient.update();
   fillString(timeClient.getFormattedTime(), 1);
          ledState = "clock";
      request->send(SPIFFS, "/index.html", "text/html", false, processor);
    });
                // Route to set GPIO state to HIGH
    server.on("/dia", HTTP_GET, [](AsyncWebServerRequest *request) {
  timeClient.update();
   fillString(timeClient.getFormattedTime(), 1);
          ledState = "dia";
      request->send(SPIFFS, "/index.html", "text/html", false, processor);
    });
                // Route to set GPIO state to HIGH
    server.on("/data", HTTP_GET, [](AsyncWebServerRequest *request) {
  timeClient.update();
   fillString(timeClient.getFormattedTime(), 1);
          ledState = "data";
      request->send(SPIFFS, "/index.html", "text/html", false, processor);
    });

    
            // Route to set GPIO state to HIGH
    server.on("/cronometro", HTTP_GET, [](AsyncWebServerRequest *request) {
       cSegundos = 0;
          ledState = "cronometro";
      request->send(SPIFFS, "/index.html", "text/html", false, processor);
    });

             // Route to set GPIO state to HIGH
    server.on("/rainbow2", HTTP_GET, [](AsyncWebServerRequest *request) {
      rainbowRoutine();
          ledState = "rainbow2";
      request->send(SPIFFS, "/index.html", "text/html", false, processor);
    });

              // Route to set GPIO state to HIGH
    server.on("/zebra", HTTP_GET, [](AsyncWebServerRequest *request) {
      zebraNoise();
          ledState = "zebra";
      request->send(SPIFFS, "/index.html", "text/html", false, processor);
    });

              // Route to set GPIO state to HIGH
    server.on("/plasma", HTTP_GET, [](AsyncWebServerRequest *request) {
        plasmaNoise();
          ledState = "plasma";
      request->send(SPIFFS, "/index.html", "text/html", false, processor);
    });

                  // Route to set GPIO state to HIGH
    server.on("/gif", HTTP_GET, [](AsyncWebServerRequest *request) {
        animation(1);
          ledState = "gif";
      request->send(SPIFFS, "/index.html", "text/html", false, processor);
    });
                  // Route to set GPIO state to HIGH
    server.on("/gif2", HTTP_GET, [](AsyncWebServerRequest *request) {
        animation(2);
          ledState = "gif2";
      request->send(SPIFFS, "/index.html", "text/html", false, processor);
    });
                  // Route to set GPIO state to HIGH
    server.on("/gif3", HTTP_GET, [](AsyncWebServerRequest *request) {
        animation(3);
          ledState = "gif3";
      request->send(SPIFFS, "/index.html", "text/html", false, processor);
    });
                      // Route to set GPIO state to HIGH
    server.on("/gif4", HTTP_GET, [](AsyncWebServerRequest *request) {
        animation(4);
          ledState = "gif4";
      request->send(SPIFFS, "/index.html", "text/html", false, processor);
    });
                      // Route to set GPIO state to HIGH
    server.on("/gif5", HTTP_GET, [](AsyncWebServerRequest *request) {
        animation(5);
          ledState = "gif5";
      request->send(SPIFFS, "/index.html", "text/html", false, processor);
    });
                        // Route to set GPIO state to HIGH
    server.on("/gif6", HTTP_GET, [](AsyncWebServerRequest *request) {
        animation(6);
          ledState = "gif6";
      request->send(SPIFFS, "/index.html", "text/html", false, processor);
    });
                        // Route to set GPIO state to HIGH
    server.on("/gif7", HTTP_GET, [](AsyncWebServerRequest *request) {
        animation(7);
          ledState = "gif7";
      request->send(SPIFFS, "/index.html", "text/html", false, processor);
    });
                        // Route to set GPIO state to HIGH
    server.on("/gif8", HTTP_GET, [](AsyncWebServerRequest *request) {
        animation(8);
          ledState = "gif8";
      request->send(SPIFFS, "/index.html", "text/html", false, processor);
    });

                      // Route to set GPIO state to HIGH
    server.on("/oceano", HTTP_GET, [](AsyncWebServerRequest *request) {
     oceanNoise();
          ledState = "oceano";
      request->send(SPIFFS, "/index.html", "text/html", false, processor);
    });

                          // Route to set GPIO state to HIGH
    server.on("/forest", HTTP_GET, [](AsyncWebServerRequest *request) {
     forestNoise();
          ledState = "forest";
      request->send(SPIFFS, "/index.html", "text/html", false, processor);
    });

                   // Route to set GPIO state to HIGH
    server.on("/madness", HTTP_GET, [](AsyncWebServerRequest *request) {
     madnessNoise();
          ledState = "madness";
      request->send(SPIFFS, "/index.html", "text/html", false, processor);
    });

                   // Route to set GPIO state to HIGH
    server.on("/lava", HTTP_GET, [](AsyncWebServerRequest *request) {
     lavaNoise();
          ledState = "lava";
      request->send(SPIFFS, "/index.html", "text/html", false, processor);
    });


                   // Route to set GPIO state to HIGH
    server.on("/balls", HTTP_GET, [](AsyncWebServerRequest *request) {
     lightBallsRoutine();
          ledState = "balls";
      request->send(SPIFFS, "/index.html", "text/html", false, processor);
    });
                       // Route to set GPIO state to HIGH
    server.on("/swirl", HTTP_GET, [](AsyncWebServerRequest *request) {
 swirlRoutine();
          ledState = "swirl";
      request->send(SPIFFS, "/index.html", "text/html", false, processor);
    });
    
    // Route to set GPIO state to HIGH
    server.on("/star", HTTP_GET, [](AsyncWebServerRequest *request) {
          ledState = "star";
      request->send(SPIFFS, "/index.html", "text/html", false, processor);
    });

                       // Route to set GPIO state to HIGH
    server.on("/tetris", HTTP_GET, [](AsyncWebServerRequest *request) {

 tetrisRoutine();
          ledState = "tetris";
      request->send(SPIFFS, "/index.html", "text/html", false, processor);
    });
                           // Route to set GPIO state to HIGH
    server.on("/snake", HTTP_GET, [](AsyncWebServerRequest *request) {
 snakeRoutine();
          ledState = "snake";
      request->send(SPIFFS, "/index.html", "text/html", false, processor);
    });
    // Route to set GPIO state to HIGH
        server.on("/arkanoid", HTTP_GET, [](AsyncWebServerRequest *request) {
 arkanoidRoutine();
          ledState = "arkanoid";
      request->send(SPIFFS, "/index.html", "text/html", false, processor);
    });
    // Route to set GPIO state to HIGH
        server.on("/runningtext", HTTP_GET, [](AsyncWebServerRequest *request) {
 fillString("MushLight", 1);
 void reset();
          ledState = "runningtext";
      request->send(SPIFFS, "/index.html", "text/html", false, processor);
    });
       
                 // Route to set GPIO state to HIGH
        server.on("/pacifica", HTTP_GET, [](AsyncWebServerRequest *request) {
          ledState = "pacifiga";
          pacifica_loop();
          
      request->send(SPIFFS, "/index.html", "text/html", false, processor);
    });
        
        // Route to set GPIO state to HIGH
        server.on("/telegram", HTTP_GET, [](AsyncWebServerRequest *request) {
 
          ledState = "telegram";
      request->send(SPIFFS, "/index.html", "text/html", false, processor);
    });
         // Route to set GPIO state to HIGH
        server.on("/spiral", HTTP_GET, [](AsyncWebServerRequest *request) {
 
          ledState = "spiral";
      request->send(SPIFFS, "/index.html", "text/html", false, processor);
    });
             // Route to set GPIO state to HIGH
        server.on("/spark", HTTP_GET, [](AsyncWebServerRequest *request) {
 
          ledState = "spark";
      request->send(SPIFFS, "/index.html", "text/html", false, processor);
    });
       
                // Route to set GPIO state to HIGH
        server.on("/cronometro", HTTP_GET, [](AsyncWebServerRequest *request) {
          ledState = "cronometro";
      request->send(SPIFFS, "/index.html", "text/html", false, processor);
    });
           // Route to set GPIO state to HIGH
        server.on("/btc", HTTP_GET, [](AsyncWebServerRequest *request) {
          ledState = "btc";
      request->send(SPIFFS, "/index.html", "text/html", false, processor);
    });
    
     // Route to set GPIO state to HIGH
        server.on("/eth", HTTP_GET, [](AsyncWebServerRequest *request) {
          ledState = "eth";
      request->send(SPIFFS, "/index.html", "text/html", false, processor);
    });
    // Route to set GPIO state to HIGH
        server.on("/ltc", HTTP_GET, [](AsyncWebServerRequest *request) {
          ledState = "ltc";
      request->send(SPIFFS, "/index.html", "text/html", false, processor);
    });
          // Route to set GPIO state to HIGH
        server.on("/shiba", HTTP_GET, [](AsyncWebServerRequest *request) {
          ledState = "shiba";
      request->send(SPIFFS, "/index.html", "text/html", false, processor);
    });
      server.on("/capture", HTTP_GET, [](AsyncWebServerRequest * request) {
    takeNewPhoto = true;
//    request->send_P(200, "text/plain", "Taking Photo");
    request->send(SPIFFS, "/index.html", "text/html", false, processor);
  });

  server.on("/saved-photo", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, FILE_PHOTO, "image/jpg", false);
  });

    configureEvents();
    server.begin();
    //uint32_t dimmervar = strtoul(dimmer.c_str(), NULL, 16);
//uint32_t velovar = strtoul(velocidade.c_str(), NULL, 16);
//FastLED.setBrightness(dimmervar);
//FastLED.delay(2000 / velovar);
//scrollTimer.setInterval(velovar);

uint32_t dimmervar = strtoul(dimmer.c_str(), NULL, 16);
uint32_t corvar = strtoul(cor.c_str(), NULL, 16);
uint32_t velovar = strtoul(velocidade.c_str(), NULL, 16);
if (millis() - tempo7 > 1000)//Faz a verificaçao das funçoes a cada 2 Segundos
    {
FastLED.setBrightness(dimmervar);
FastLED.delay(2000 / velovar);
      tempo7 = millis();//Reseta o tempo
     
   }

   
  
  }
  else {
    // Connect to Wi-Fi network with SSID and password
    Serial.println("Setting AP (Access Point)");
    // NULL sets an open Access Point
    WiFi.softAP("MushLight-config", NULL);

    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(IP); 

    // Web Server Root URL
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(SPIFFS, "/wifimanager.html", "text/html");
    });
    
    server.serveStatic("/", SPIFFS, "/");
    
    server.on("/", HTTP_POST, [](AsyncWebServerRequest *request) {
      int params = request->params();
      for(int i=0;i<params;i++){
        AsyncWebParameter* p = request->getParam(i);
        if(p->isPost()){
          // HTTP POST ssid value
          if (p->name() == PARAM_INPUT_1) {
            ssid = p->value().c_str();
            Serial.print("SSID set to: ");
            Serial.println(ssid);
            // Write file to save value
            writeFile(SPIFFS, ssidPath, ssid.c_str());
          }
          // HTTP POST pass value
          if (p->name() == PARAM_INPUT_2) {
            pass = p->value().c_str();
            Serial.print("Password set to: ");
            Serial.println(pass);
            // Write file to save value
            writeFile(SPIFFS, passPath, pass.c_str());
          }
          // HTTP POST ip value
          if (p->name() == PARAM_INPUT_3) {
            ip = p->value().c_str();
            Serial.print("IP Address set to: ");
            Serial.println(ip);
            // Write file to save value
            writeFile(SPIFFS, ipPath, ip.c_str());
          }
          // HTTP POST gateway value
          if (p->name() == PARAM_INPUT_4) {
            gateway = p->value().c_str();
            Serial.print("Gateway set to: ");
            Serial.println(gateway);
            // Write file to save value
            writeFile(SPIFFS, gatewayPath, gateway.c_str());
          }
          // HTTP POST tokentelegram value
          if (p->name() == PARAM_INPUT_5) {
            tokentelegram = p->value().c_str();
            Serial.print("Token Telgeram set to: ");
            Serial.println(tokentelegram);
            // Write file to save value
            writeFile(SPIFFS, tokentelegramPath, tokentelegram.c_str());
            //EEPROM.put(tokentelegram.c_str());
           
            
          }
          //Serial.printf("POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
        }
      }
      request->send(200, "text/plain", "Prontinho. MushLight reiniciando, conecte no seu roteador e acesse o IP :" + ip);
      delay(3000);
      ESP.restart();
    });
    server.begin();
  }


}





void loop() {

uint32_t corvar = strtoul(cor.c_str(), NULL, 16);





//#####Configura  loop telegram 
   if (millis() - tempo > 30000)//Faz a verificaçao das funçoes a cada 2 Segundos
    {
     // connect();//Funçao para verificar se ainda há conexao
      readTel();//Funçao para ler o telegram
      tempo = millis();//Reseta o tempo
     
   }

  if (millis() - tempo2 > 1800000)//Faz a verificaçao das funçoes a cada 30min
   {   //tira foto e manda clima no telegram
      verifica();
      tempo2 = millis();
     
   }

     if (millis() - tempo3 > 500)//Faz a verificaçao das funçoes a cada 30min
   {
      //readSoundSensor();
      tempo3 = millis();
     
   }

if (ledState == "loggrafico") {


if (millis() - tempo8 > 30000)//Faz a verificaçao das funçoes a cada 30min
   {
    
//verifica2(); 
      tempo8 = millis();
     
   }

}

if (ledState == "ledoff") {

    ledoff();
        FastLED.show(); // display this frame
    }
      if (ledState == "pisca") {
    pisca();
    FastLED.show(); // display this frame
    }      
    if (ledState == "piscalento") {
    piscalento();
    FastLED.show(); // display this frame
    } 
  if (ledState == "rainbowon") {
    rainbowon();
    } 
       if (ledState == "rainbow2") {
    rainbowRoutine();
        FastLED.show(); // display this frame
    }
  
  if (ledState == "matrixon") {
    matrixRoutine();
        FastLED.show(); // display this frame
        //FastLED.delay(2000 / strtoul(velocidade.c_str(), NULL, 16));
    } 
  
  if (ledState == "pride") {
    pride();
     FastLED.show();
    } 

if (ledState == "green") {

           fill_solid( leds, NUM_LEDS, CRGB::Green);

       
        FastLED.show(); // display this frame
    }
    if (ledState == "blue") {
    blue();
        FastLED.show(); // display this frame
    }
        if (ledState == "red") {
    red();
        FastLED.show(); // display this frame
    }
 if (ledState == "amarelo") {
 
    fill_solid( leds, NUM_LEDS, 0xFFFF00);
        FastLED.show(); // display this frame
    }

    
 if (ledState == "colors") {
      // colorsRoutine();
// drawLetter(1, "o", 16, 0xF0F8FF);
    //  drawLetter(uint8_t index, uint8_t letter, int16_t offset, uint32_t color)
        FastLED.show(); // display this frame
    }

if (ledState == "ledon") {


    fill_solid( leds, NUM_LEDS, corvar);
//FastLED.setBrightness(dimmervar);
        FastLED.show(); // display this frame
    
    }

    if (ledState == "fire2") {
    fireRoutine();
        FastLED.show(); // display this frame
    }

          if (ledState == "snowon") {
            snowRoutine();
            FastLED.delay(2000 / strtoul(velocidade.c_str(), NULL, 16));
            FastLED.show(); // display this frame
            
    }
    
              if (ledState == "temp") {
    fillString(readDHTTemperature() + "C", 0x0000FF);
        FastLED.show(); // display this frame
    }
                 if (ledState == "co2") {
    fillString(readCO2()+ "PPM", 0x00FFFF);
        FastLED.show(); // display this frame
    }
                 if (ledState == "humi") {
    fillString(readDHTHumidity() + "%", 0xFFFF00);
        FastLED.show(); // display this frame
    }
        if (ledState == "pressao") {
    fillString(readDHTPressao(), 0xFF6347);
    FastLED.show(); // display this frame
    }
    if (ledState == "clima") {
    fillString("||T" + readDHTTemperature() + "C||CO2" + readCO2()+ "PPM||ATM" + readDHTPressao() + "Pa ||Umid" + readDHTHumidity() + "%", 1);
    FastLED.show(); // display this frame
    }
    
    if (ledState == "cronometro") {
   // cronometro();
    fillString(crono(), corvar);

    FastLED.show(); // display this frame
    //FastLED.setBrightness(dimmervar);
    }
    if (ledState == "clock") {

 
    fillString(timeClient.getFormattedTime(), corvar);
    FastLED.show(); // display this frame
    //FastLED.setBrightness(dimmervar);
    }
        if (ledState == "dia") {

 
    fillString(timeClient.getFormattedTime(), corvar);
    FastLED.show(); // display this frame
    //FastLED.setBrightness(dimmervar);
    }
        if (ledState == "data") {
time_t now = time(nullptr);
 time_now = String(ctime(&now)).substring(0,24);
    fillString(time_now, corvar);
    FastLED.show(); // display this frame
    //FastLED.setBrightness(dimmervar);
    }
 
 

   if (ledState == "zebra") {
    //    uint16_t veloframe = strtoul(velocidade.c_str(), NULL, 8);

    zebraNoise();
        FastLED.show(); // display this frame
          //FastLED.delay(2000 / strtoul(velocidade.c_str(), NULL, 16));
          //FastLED.setBrightness(dimmervar);
    }
    
   if (ledState == "plasma") {
  plasmaNoise();
        FastLED.show(); // display this frame
        //FastLED.delay(2000 / velovar);  
        //FastLED.setBrightness(dimmervar);
        }
       if (ledState == "gif") {
animation(1);
        FastLED.show(); // display this frame
    }
     if (ledState == "gif2") {
animation(2);
        FastLED.show(); // display this frame
    }
     if (ledState == "gif3") {
animation(3);
        FastLED.show(); // display this frame
    }

     if (ledState == "gif4") {
animation(4);
        FastLED.show(); // display this frame
    }
         if (ledState == "gif5") {
animation(5);
        FastLED.show(); // display this frame
    }
      if (ledState == "gif6") {
animation(6);
        FastLED.show(); // display this frame
    }
         if (ledState == "gif7") {
animation(7);
        FastLED.show(); // display this frame
    }
         if (ledState == "gif8") {
animation(8);
        FastLED.show(); // display this frame
    }
           if (ledState == "oceano") {
oceanNoise();
        FastLED.show(); // display this frame
    }

               if (ledState == "forest") {
forestNoise();
        FastLED.show(); // display this frame
    }
                   if (ledState == "madness") {
madnessNoise();
        FastLED.show(); // display this frame
    }
                   if (ledState == "lava") {
lavaNoise();
 //FastLED.delay(2000 / velovar);
 
        FastLED.show(); // display this frame
    }
                   if (ledState == "balls") {
lightBallsRoutine();
 //FastLED.delay(2000 / velovar);        
 FastLED.show(); // display this frame
    }
                   if (ledState == "swirl") {
swirlRoutine();
 //FastLED.delay(2000 / velovar);        
 FastLED.show(); // display this frame
    }
                   if (ledState == "tetris") {
tetrisRoutine();
        FastLED.show(); // display this frame
    }
                   if (ledState == "snake") {
snakeRoutine();
        FastLED.show(); // display this frame
    }
                       if (ledState == "arkanoid") {
arkanoidRoutine();
        FastLED.show(); // display this frame
    } 
    if (ledState == "runningtext") {


  //scrollTimer.setInterval(velovar);
 //FastLED.setBrightness(dimmervar);
 fillString(txt,corvar);

        FastLED.show(); // display this frame
    }
    if (ledState == "telegram") {
 
 fillString(text,2);
        FastLED.show(); // display this frame
    }
        if (ledState == "btc") {

  if (millis() - tempo5 > 10000)//Faz a verificaçao das funçoes a cada 30min
   {
      EEPROM.writeString(1, btc());
      tempo5 = millis();
     
   }

          
  //scrollTimer.setInterval(velovar);
  fillString(EEPROM.readString(1),corvar);

        FastLED.show(); // display this frame
     
   
// delay(2000);
    }

           
            if (ledState == "eth") {
    if (millis() - tempo5 > 10000)//Faz a verificaçao das funçoes a cada 30min
   {
      EEPROM.writeString(2, eth());
      tempo5 = millis();
     
   }


  //scrollTimer.setInterval(velovar);
    fillString(EEPROM.readString(2),corvar);

        FastLED.show(); // display this frame
     
   
 }

  if (ledState == "ltc") {
           
  if (millis() - tempo5 > 10000)//Faz a verificaçao das funçoes a cada 30min
   {
      EEPROM.writeString(3, ltc());
      tempo5 = millis();
     
   }


  //scrollTimer.setInterval(velovar);
    fillString(EEPROM.readString(3),corvar);

        FastLED.show(); // display this frame
     
   
// delay(2000);    
}
             if (ledState == "shiba") {
    if (millis() - tempo5 > 10000)//Faz a verificaçao das funçoes a cada 30min
   {
      EEPROM.writeString(4, shiba());
      tempo5 = millis();
     
   }

          

  //scrollTimer.setInterval(velovar);
    fillString(EEPROM.readString(4),corvar);

        FastLED.show(); // display this frame
     
   
 }

         if (ledState == "spark") {

      sparklesRoutine();
            //FastLED.delay(2000 / strtoul(velocidade.c_str(), NULL, 16));

FastLED.show(); // display this frame

    }

    
        if (ledState == "star") {
                 
  starfallRoutine();
               //FastLED.delay(2000 / strtoul(velocidade.c_str(), NULL, 16));

   FastLED.show(); // display this frame


    }

        if (ledState == "pacifica") {
pacifica_loop();
  FastLED.show(); // display this frame

    }
    
        if (ledState == "spiral") {
ballRoutine();
  FastLED.show(); // display this frame
  //      uint16_t veloframe = strtoul(velocidade.c_str(), NULL, 8);
 //FastLED.delay(2000 / veloframe);    
 }
        if (ledState == "rainbowstrip") {
rainbowStripeNoise();
   //     uint16_t veloframe = strtoul(velocidade.c_str(), NULL, 8);
 //FastLED.delay(2000 / veloframe);
 // FastLED.show(); // display this frame

    }
    
        if (ledState == "light") {
 lightersRoutine();
  FastLED.show(); // display this frame
   //     uint16_t veloframe = strtoul(velocidade.c_str(), NULL, 8);
 //FastLED.delay(2000 / veloframe);
   }
           if (ledState == "bpm") {
 bpm();
  FastLED.show(); // display this frame
   //     uint16_t veloframe = strtoul(velocidade.c_str(), NULL, 8);
 //FastLED.delay(2000 / veloframe);
   }
   if (ledState == "sinelon") {
 sinelon();
  FastLED.show(); // display this frame
   //     uint16_t veloframe = strtoul(velocidade.c_str(), NULL, 8);
 //FastLED.delay(2000 / veloframe);
   }
   if (ledState == "confetti") {
 confetti();
  FastLED.show(); // display this frame
   //     uint16_t veloframe = strtoul(velocidade.c_str(), NULL, 8);
 //FastLED.delay(2000 / veloframe);
   }
      if (ledState == "juggle") {
 juggle();
  FastLED.show(); // display this frame
   //     uint16_t veloframe = strtoul(velocidade.c_str(), NULL, 8);
 //FastLED.delay(2000 / veloframe);
   }
   if (ledState == "rainbowgliter") {
 rainbowWithGlitter();
  FastLED.show(); // display this frame
   //     uint16_t veloframe = strtoul(velocidade.c_str(), NULL, 8);
 //FastLED.delay(2000 / veloframe);
   }
   
        if (ledState == "runner") {
 runnerRoutine();
  FastLED.show(); // display this frame
   //     uint16_t veloframe = strtoul(velocidade.c_str(), NULL, 8);
 //FastLED.delay(2000 / veloframe);
   }

    
    if (takeNewPhoto) {
    capturePhotoSaveSpiffs();
    takeNewPhoto = false;
  }


 //##Loop paralelo
    if (millis() - tempo6 > 2000)//Faz a verificaçao das funçoes a cada 30min
   {
    
 events.send("ping",NULL,millis());
 events.send(getSensorReadings().c_str(),"new_readings" ,millis());
   Serial.print(getSensorReadings().c_str());
    
      
      tempo6 = millis();
     
   }




//delay(2000);
  
}
