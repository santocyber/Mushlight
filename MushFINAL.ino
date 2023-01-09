//Codigo disponivel no github desenvolvido por @SantoCyber
//#define _DISABLE_TLS_
#include <WiFi.h>
#include "esp_camera.h"
#include "img_converters.h"
#include "Arduino.h"
#include "soc/soc.h"           // Disable brownour problems
#include "soc/rtc_cntl_reg.h"  // Disable brownour problems
#include "driver/rtc_io.h"
#include <StringArray.h>
#include <Wire.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
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
#include "bitmap9.h"
#include <Adafruit_BMP280.h>
#include <Adafruit_AHTX0.h>
#include <FS.h>
#include <WiFiClientSecure.h>
#include <time.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "EEPROM.h"




//#####Configura data logger
String    time_now;
#define TZone -3

//####Configura contador
unsigned long cSegundos = 0;
unsigned int segundos = 0;
byte minutos = 0;



//###########Configura Crypto

const int httpsPort = 443;



//##########Configura cam
boolean takeNewPhoto = false;
//boolean dataAvailable = false;

// Keep track of number of pictures
unsigned int pictureNumber = 0;
String path = "";
String lastPhoto = "";
String cam;

camera_fb_t *fb = NULL;



const long gmtOffset_sec = 3600 * 10;
const int daylightOffset_sec = 3600;
// NTP Server - this is to put a time stanp in the file name
const char* ntpServer = "pool.ntp.org";


//CAMERA_MODEL_AI_THINKER
#define PWDN_GPIO_NUM    -1
#define RESET_GPIO_NUM   -1
#define XCLK_GPIO_NUM    21
#define SIOD_GPIO_NUM    26
#define SIOC_GPIO_NUM    27

#define Y9_GPIO_NUM      35
#define Y8_GPIO_NUM      34
#define Y7_GPIO_NUM      39
#define Y6_GPIO_NUM      36
#define Y5_GPIO_NUM      19
#define Y4_GPIO_NUM      18
#define Y3_GPIO_NUM       5
#define Y2_GPIO_NUM       4
#define VSYNC_GPIO_NUM   25
#define HREF_GPIO_NUM    23
#define PCLK_GPIO_NUM    22



// Photo File Name to save in SPIFFS
//#define FILE_PHOTO "/photo.jpg"


//##########CONFIG TEXTO
String runningText = "MushLight";


//#############Configura GPI0


#define LED_PIN 32
#define MQ_analog 33
const int vibra = 34;//pino 34


//config BMP

Adafruit_AHTX0 aht;
Adafruit_BMP280 bmp; // I2C


int valoratm = 0;//Declara a variável valorldr como inteiro
int valortemp = 0;//Declara a variável valorldr como inteiro
int valorhumi = 0;//Declara a variável valorldr como inteiro
int valorco2 = analogRead(MQ_analog);;//Declara a variável valorldr como inteiro
int valorvibra;

String ledState;
boolean led_state = true;
int clap_counter = 0;
int color_counter = 0;



 
// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

AsyncEventSource events("/events");


// Search for parameter in HTTP POST request
const char* PARAM_INPUT_1 = "ssid";
const char* PARAM_INPUT_2 = "pass";
const char* PARAM_INPUT_5 = "tokentelegram";
const char* PARAM_INPUT_11 = "clima";
const char* PARAM_INPUT_12 = "logger";
const char* PARAM_INPUT_6 = "txt";
const char* PARAM_INPUT_7 = "cor";
const char* PARAM_INPUT_8 = "text";
const char* PARAM_INPUT_9 = "velocidade";
const char* PARAM_INPUT_10 = "dimmer";
const char* PARAM_INPUT_13 = "sleeptime";
const char* PARAM_INPUT_14 = "nomedobot";

//Variables to save values from HTML form
String ssid;
String pass;
String nomedobot;
String tokentelegram;
String clima;
String txt = runningText;
String cor = "0xFFFFFF";
String text;
String velocidade = "60";
String dimmer = "250";
//String bot;
String id,idX, welcome, from_name;//Váriaveis para armazenamento do ID e TEXTO gerado pelo Usuario
String msg;
String readings;
String logger;
String photo;


// File paths to save input values permanently
const char* ssidPath = "/ssid.txt";
const char* passPath = "/pass.txt";
const char* nomedobotPath = "/nomedobot.txt";
const char* tokentelegramPath = "/tokentelegram.txt";
const char* climaPath = "/clima.txt";
const char* loggerPath = "/data.txt";
const char* FILE_PHOTO = "/photo.jpg";



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
unsigned long tempo9 = millis();
unsigned long tempo10 = millis();

//######Config sleep
unsigned long temposleep0;
unsigned long temposleep1;
long settemposleep = 600000;
String sleeptime;
String State;


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
#define CAM 1


int scrollSpeed = D_TEXT_SPEED;    // скорость прокрутки текста бегущей строки
int gameSpeed = DEMO_GAME_SPEED;   // скорость в играх
int effectSpeed = D_EFFECT_SPEED; 

uint32_t idleTime = ((long)IDLE_TIME * 60 * 1000);      // минуты -> миллисек
uint32_t autoplayTime = ((long)AUTOPLAY_PERIOD * 1000); // секунды -> миллисек
uint32_t autoplayTimer;


//##################### Configura voids

#define SCORE_SIZE 0  
//#define FASTLED_INTERRUPT_RETRY_COUNT 0
//#define FASTLED_ALLOW_INTERRUPTS 0
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
//#define SNOW_DENSE 20
#define HUE_ADD 0
//#define SPARKLES 50
#define COOLING  120

// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more flickery fire.
// Default 120, suggested range 50-200.
#define SPARKING 200
#define FRAMES_PER_SECOND velovar



//############Configura led
//http://fastled.io/docs/3.1/struct_c_r_g_b.html


#define NUM_LEDS WIDTH * HEIGHT * SEGMENTS
#define CHIPSET WS2811
CRGB leds[NUM_LEDS];


//##Configura effeito newone
#define NUM_ROWS 8
#define NUM_COLS 8
//#define NUM_LEDS NUM_ROWS * NUM_COLS
byte eff = 0;
//#define WIDTH NUM_COLS
//#define HEIGHT NUM_ROWS
//#define speed (100/(HEIGHT-4))







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
    fileContent = file.readString();
    break;     
  }
    fileContent.trim();
 // fileContent.length() - 1;

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
  fileContent.trim();
  fileContent.length();
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
    return;
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






//######################################################################## Initialize WiFi
bool initWiFi() {
  if(ssid==""){
    Serial.println("Undefined SSID");
    return false;
  }

  WiFi.mode(WIFI_STA);
  localIP;
  localGateway;


  if (!WiFi.config(localIP, localGateway, subnet,primaryDNS)){
    Serial.println("STA Failed to configure");
    return false;
  }
  WiFi.begin(ssid.c_str(), pass.c_str());
  
  //##################Configura servicoes q precisa de conexao
  
    timeClient.begin();
    timeClient.update();
    //readTel();
    //verifica();
    //verifica2();
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



////############################Le sensores


 String readCO2() {

  float t = valorco2;

    return String(t);
}



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

  
////###############################################################
          



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
  config.pixel_format = PIXFORMAT_JPEG;
  
  config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
config.fb_location = CAMERA_FB_IN_PSRAM;

const int pwmfreq = 50000;     // 50K pwm frequency
const int pwmresolution = 9;   // duty cycle bit range
ledcSetup(config.ledc_channel, pwmfreq, pwmresolution);  // configure LED PWM channel
ledcAttachPin(4, config.ledc_channel);     


  //init with high specs to pre-allocate larger buffers
  if (psramFound())
  {
    config.frame_size = FRAMESIZE_CIF; // FRAMESIZE_ + QVGA|CIF|VGA|SVGA|XGA|SXGA|UXGA
    config.jpeg_quality = 10;
    config.fb_count = 2;
  }
  else
  {
    config.frame_size = FRAMESIZE_QVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

#if defined(CAMERA_MODEL_ESP_EYE)
  pinMode(13, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);
#endif

  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK)
  {
    Serial.printf("Camera init failed with error 0x%x", err);
    return false;
  }

  sensor_t *s = esp_camera_sensor_get();
  //initial sensors are flipped vertically and colors are a bit saturated
  if (s->id.PID == OV3660_PID)
  {
    s->set_vflip(s, 1);       //flip it back
    s->set_brightness(s, 1);  //up the blightness just a bit
    s->set_saturation(s, -2); //lower the saturation
  }
  //drop down frame size for higher initial frame rate
  s->set_framesize(s, FRAMESIZE_QVGA);

#if defined(CAMERA_MODEL_M5STACK_WIDE)
  s->set_vflip(s, 1);
  s->set_hmirror(s, 1);
#endif

  return true;
}


//######################## SETUP


void setup() {


  // Serial port for debugging purposes
  pinMode(33, OUTPUT);
   pinMode(MQ_analog, INPUT);
  // pinMode(vibra, INPUT);


   Serial.begin(115200);
 

  EEPROM.begin(512);




 
      // Turn off the flash
      pinMode(4, OUTPUT);
   //   digitalWrite(4, LOW);
      //rtc_gpio_hold_dis(GPIO_NUM_4);


Serial.setDebugOutput(true);
  Serial.println();

// Turn-off the 'brownout detector'
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);

 
  // Camera init
if (!setupCamera())
  {
    Serial.println("Camera Setup Failed!");
   
  }


 //init telegram cert

client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
//client->setTimeout(4000);

client.setTimeout(30000);

 
//########### LED

FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);
  FastLED.setBrightness( BRIGHTNESS );

 // if (CURRENT_LIMIT > 0) FastLED.setMaxPowerInVoltsAndMilliamps(5, CURRENT_LIMIT);
  FastLED.clear();
  FastLED.show();
  randomSeed(analogRead(0) + analogRead(1));


//inicia sensores tem umi pressao
  //I2CSensors.begin(I2C_SDA, I2C_SCL, 100000);


aht.begin();
bmp.begin();


bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */

 //########################Le arquivos gravados             

  initSPIFFS();

  
  // Load values saved in SPIFFS
  ssid = readFile(SPIFFS, ssidPath);
  pass = readFile(SPIFFS, passPath);
  nomedobot = readFile(SPIFFS, nomedobotPath);
  tokentelegram = readFile (SPIFFS, tokentelegramPath);
  clima = readFile (SPIFFS, climaPath);
  logger = readTotal (SPIFFS, loggerPath);
  photo = readFile (SPIFFS, FILE_PHOTO);

  Serial.println(ssid);
  Serial.println(pass);
  Serial.println(tokentelegram);
  //Serial.println(clima);
 // Serial.println(logger);





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
          if (p->name() == PARAM_INPUT_13) {
            sleeptime = p->value().c_str();
            Serial.print("Sleep set to: ");
            Serial.println(sleeptime);
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
//uint32_t corvar = strtoul(cor.c_str(), NULL, 16);
uint32_t velovar = strtoul(velocidade.c_str(), NULL, 16);
//long settemposleep = strtoul(sleeptime.c_str(),NULL, 1000);

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
   logger = "{\"sensores\":[{\"Temperatura\":\"22.00\",\"Umidade\":\"45.00\",\"Pressao\":\"45.00\",\"CO2\":\"198.00\",\"Hora\":\"22:00\"}";
   logger += readFile(SPIFFS, loggerPath); 
   logger += "]}";
  //###Q gambiarra pra funcionar o JSON
    request->send(200, "application/json", logger);
   request->send_P(200, "text/plain", logger.c_str());
   logger = String();
  });
    
      // Route to set GPIO state to HIGH
    server.on("/loggrafico", HTTP_GET, [](AsyncWebServerRequest *request) {
        verifica2();
        request->send(SPIFFS, "/log.html", "text/html", false);
 
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
     // ledState = "deletelog";
      writeFile(SPIFFS, climaPath, "");
        writeFile(SPIFFS, loggerPath, "");
     // writeFile(SPIFFS, loggerPath, "{\"Temperatura\":\"22.00\",\"Umidade\":\"45.00\",\"Pressao\":\"45.00\",\"CO2\":\"198.00\"}");
      
      request->send(SPIFFS, "/index.html", "text/html", false, processor);
    });

            // Route to set GPIO state to HIGH
    server.on("/apagaconfig", HTTP_GET, [](AsyncWebServerRequest *request) {
     // ledState = "deletelog";
      writeFile(SPIFFS, ssidPath, "");
      writeFile(SPIFFS, passPath, "");
      writeFile(SPIFFS, tokentelegramPath, "");
      request->send(SPIFFS, "/index.html", "text/html", false, processor);
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
    server.on("/newone", HTTP_GET, [](AsyncWebServerRequest *request) {
      newone();
            ledState = "newone";

      request->send(SPIFFS, "/index.html", "text/html", false);
    });
                       // Route to set GPIO state to HIGH
    server.on("/spiralone", HTTP_GET, [](AsyncWebServerRequest *request) {
                            FastLED.clear();

            ledState = "spiralone";

      request->send(SPIFFS, "/index.html", "text/html", false, processor);
    });
                           // Route to set GPIO state to HIGH
    server.on("/papainoel", HTTP_GET, [](AsyncWebServerRequest *request) {
                  FastLED.clear();

            ledState = "papainoel";

      request->send(SPIFFS, "/index.html", "text/html", false, processor);
    });
        server.on("/spiraltwo", HTTP_GET, [](AsyncWebServerRequest *request) {
                  FastLED.clear();

            ledState = "spiraltwo";

      request->send(SPIFFS, "/index.html", "text/html", false, processor);
    });

                               // Route to set GPIO state to HIGH
    server.on("/dots", HTTP_GET, [](AsyncWebServerRequest *request) {
                  FastLED.clear();

            ledState = "dots";

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
              FastLED.clear();

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
    server.on("/gif9", HTTP_GET, [](AsyncWebServerRequest *request) {
        animation(9);
          ledState = "gif9";
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
          ledState = "pacifica";
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
        server.on("/xmr", HTTP_GET, [](AsyncWebServerRequest *request) {
          ledState = "xmr";
      request->send(SPIFFS, "/index.html", "text/html", false, processor);
    });
               // Route to set GPIO state to HIGH
        server.on("/doge", HTTP_GET, [](AsyncWebServerRequest *request) {
          ledState = "doge";
      request->send(SPIFFS, "/index.html", "text/html", false, processor);
    });
               // Route to set GPIO state to HIGH
        server.on("/dolar", HTTP_GET, [](AsyncWebServerRequest *request) {
          ledState = "dolar";
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
          // Route to set GPIO state to HIGH
        server.on("/credito", HTTP_GET, [](AsyncWebServerRequest *request) {
          ledState = "credito";
    //  bot.sendMessage(id, "Aloouu @santocyber, obrigado pela MushLight", "");//Envia uma Mensagem para a pessoa que enviou o Comando.


          
      request->send(SPIFFS, "/index.html", "text/html", false, processor);
    });
            // Route to set GPIO state to HIGH
        server.on("/sleep", HTTP_GET, [](AsyncWebServerRequest *request) {
          
          State = "sleep";
         temposleep0 = millis();

          
      request->send(SPIFFS, "/index.html", "text/html", false, processor);
    });
      

            server.on("/restart", HTTP_GET, [](AsyncWebServerRequest * request) {
ESP.restart();
//    request->send_P(200, "text/plain", "Taking Photo");
   // request->send(SPIFFS, "/", "text/html", false, processor);
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
//FastLED.setBrightness(dimmervar);
//FastLED.delay(2000 / velovar);
//scrollTimer.setInterval(velovar);



//uint32_t dimmervar = strtoul(dimmer.c_str(), NULL, 16);
//uint32_t corvar = strtoul(cor.c_str(), NULL, 16);
uint32_t velovar = strtoul(velocidade.c_str(), NULL, 16);
if (millis() - tempo7 > 1000)//Faz a verificaçao das funçoes a cada 2 Segundos
    {
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

          // HTTP POST tokentelegram value
          if (p->name() == PARAM_INPUT_5) {
            tokentelegram = p->value().c_str();
            Serial.print("Token Telgeram set to: ");
            Serial.println(tokentelegram);
            // Write file to save value
            writeFile(SPIFFS, tokentelegramPath, tokentelegram.c_str());
            }
            
                  // HTTP POST tokentelegram value
          if (p->name() == PARAM_INPUT_14) {
            nomedobot = p->value().c_str();
            Serial.print("Nome do bot set to: ");
            Serial.println(nomedobot);
            // Write file to save value
            writeFile(SPIFFS, nomedobotPath, nomedobot.c_str());
          }

          
          
          
          
          
          //Serial.printf("POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
        }
      }
      request->send(200, "text/plain", "Prontinho. MushLight reiniciando, conecte no seu roteador e veja o ip na sua lista DHCP ou digita /start no telegram pra saber o ip");
      delay(3000);
      ESP.restart();
    });
    server.begin();
  }


}





void loop() {

uint32_t corvar = strtoul(cor.c_str(), NULL, 16);


    if (State == "sleep") {     
          temposleep1 = millis();

    if (temposleep1 - temposleep0 > settemposleep)//Faz a verificaçao das funçoes a cada 30min
   {
    ledState = "ledoff";
    FastLED.show(); // display this frame
     State = "acordado";     
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
    FastLED.delay(2000 / strtoul(velocidade.c_str(), NULL, 16));
    } 
       if (ledState == "rainbow2") {
    rainbowRoutine();
    FastLED.show(); // display this frame
    FastLED.delay(2000 / strtoul(velocidade.c_str(), NULL, 16));
    }
  
  if (ledState == "matrixon") {
    matrixRoutine();
    FastLED.show(); // display this frame
    FastLED.delay(2000 / strtoul(velocidade.c_str(), NULL, 16));
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
FastLED.clear();
   timeClient.update();

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
    zebraNoise();
    FastLED.show(); // display this frame
    FastLED.delay(2000 / strtoul(velocidade.c_str(), NULL, 16));
    }
    
   if (ledState == "plasma") {
    plasmaNoise();
    FastLED.show(); // display this frame
    FastLED.delay(2000 / strtoul(velocidade.c_str(), NULL, 16));
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
             if (ledState == "gif9") {
animation(9);
        FastLED.show(); // display this frame
    }
           if (ledState == "oceano") {
oceanNoise();
        FastLED.show(); // display this frame
    }

               if (ledState == "forest") {
    forestNoise();
    FastLED.show(); // display this frame
    FastLED.delay(2000 / strtoul(velocidade.c_str(), NULL, 16));

    }
                   if (ledState == "madness") {
    madnessNoise();
    FastLED.show(); // display this frame
    FastLED.delay(2000 / strtoul(velocidade.c_str(), NULL, 16));
    }
                   if (ledState == "lava") {
    lavaNoise();
    FastLED.show(); // display this frame
    FastLED.delay(2000 / strtoul(velocidade.c_str(), NULL, 16));

    }
                   if (ledState == "balls") {
    lightBallsRoutine();
    FastLED.show(); // display this frame
    FastLED.delay(2000 / strtoul(velocidade.c_str(), NULL, 16));

    }
                   if (ledState == "swirl") {
     swirlRoutine();
     FastLED.show(); // display this frame
     FastLED.delay(2000 / strtoul(velocidade.c_str(), NULL, 16));

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
    fillString(EEPROM.readString(1),corvar);
    FastLED.show(); // display this frame
    FastLED.delay(2000 / strtoul(velocidade.c_str(), NULL, 16));

    }
            if (ledState == "eth") {
    if (millis() - tempo5 > 10000)//Faz a verificaçao das funçoes a cada 30min
   {
      EEPROM.writeString(2, eth());
      tempo5 = millis();
     
   }
    fillString(EEPROM.readString(2),corvar);
    FastLED.show(); // display this frame
    FastLED.delay(2000 / strtoul(velocidade.c_str(), NULL, 16));
 }

  if (ledState == "ltc") {
           
  if (millis() - tempo5 > 10000)//Faz a verificaçao das funçoes a cada 30min
   {
      EEPROM.writeString(3, ltc());
      tempo5 = millis();
     
   }
   fillString(EEPROM.readString(3),corvar);
    FastLED.show(); // display this frame
}
             if (ledState == "shiba") {
    if (millis() - tempo5 > 10000)//Faz a verificaçao das funçoes a cada 30min
   {
      EEPROM.writeString(4, shiba());
      tempo5 = millis();
     
   }
   fillString(EEPROM.readString(4),corvar);
    FastLED.show(); // display this frame
    FastLED.delay(2000 / strtoul(velocidade.c_str(), NULL, 16));
    } 
                if (ledState == "xmr") {
    if (millis() - tempo5 > 10000)//Faz a verificaçao das funçoes a cada 30min
   {
      EEPROM.writeString(5, xmr());
      tempo5 = millis();
     
   }
   fillString(EEPROM.readString(5),corvar);
    FastLED.show(); // display this frame
    FastLED.delay(2000 / strtoul(velocidade.c_str(), NULL, 16));
    } 
                if (ledState == "doge") {
    if (millis() - tempo5 > 10000)//Faz a verificaçao das funçoes a cada 30min
   {
      EEPROM.writeString(6, doge());
      tempo5 = millis();
     
   }
   fillString(EEPROM.readString(6),corvar);
    FastLED.show(); // display this frame
    FastLED.delay(2000 / strtoul(velocidade.c_str(), NULL, 16));
    } 
                if (ledState == "dolar") {
    if (millis() - tempo5 > 10000)//Faz a verificaçao das funçoes a cada 30min
   {
      EEPROM.writeString(7, dolar());
      tempo5 = millis();
     
   }
   fillString(EEPROM.readString(7),corvar);
    FastLED.show(); // display this frame
    FastLED.delay(2000 / strtoul(velocidade.c_str(), NULL, 16));
    } 
        if (ledState == "spark") {
    sparklesRoutine();
    FastLED.delay(2000 / strtoul(velocidade.c_str(), NULL, 16));
    }
       if (ledState == "star") {                
   starfallRoutine();
   FastLED.delay(2000 / strtoul(velocidade.c_str(), NULL, 16));
   FastLED.show(); // display this frame
    }
        if (ledState == "pacifica") {
    pacifica_loop();
    FastLED.show(); // display this frame
    FastLED.delay(2000 / strtoul(velocidade.c_str(), NULL, 16));
    }
        if (ledState == "spiral") {
    ballRoutine();
    FastLED.show(); // display this frame
    FastLED.delay(2000 / strtoul(velocidade.c_str(), NULL, 16));
}
        if (ledState == "rainbowstrip") {
    rainbowStripeNoise();
    FastLED.delay(2000 / strtoul(velocidade.c_str(), NULL, 16));
   }
    
        if (ledState == "light") {
   lightersRoutine();
   FastLED.show(); // display this frame
   FastLED.delay(2000 / strtoul(velocidade.c_str(), NULL, 16));
  }
           if (ledState == "bpm") {
    bpm();
    FastLED.show(); // display this frame
    FastLED.delay(2000 / strtoul(velocidade.c_str(), NULL, 16));

   }
   if (ledState == "sinelon") {
    sinelon();
    FastLED.show(); // display this frame
    FastLED.delay(2000 / strtoul(velocidade.c_str(), NULL, 16));
   }
   if (ledState == "confetti") {
    confetti();
    FastLED.show(); // display this frame
    FastLED.delay(2000 / strtoul(velocidade.c_str(), NULL, 16));

   }
      if (ledState == "juggle") {
 juggle();
  FastLED.show(); // display this frame
             FastLED.delay(2000 / strtoul(velocidade.c_str(), NULL, 16));

   }
   if (ledState == "rainbowgliter") {
 rainbowWithGlitter();
  FastLED.show(); // display this frame
            FastLED.delay(2000 / strtoul(velocidade.c_str(), NULL, 16));

   }
   
        if (ledState == "runner") {
 runnerRoutine();
  FastLED.show(); // display this frame
 //FastLED.delay(2000 / veloframe);
   }
           if (ledState == "newone") {
 newone();
  FastLED.show(); // display this frame
            FastLED.delay(2000 / strtoul(velocidade.c_str(), NULL, 16));
   }
              if (ledState == "spiralone") {
   spiralone();
  FastLED.show(); // display this frame
            FastLED.delay(2000 / strtoul(velocidade.c_str(), NULL, 16));
   }
                 if (ledState == "spiraltwo") {
   spiraltwo();
  FastLED.show(); // display this frame
            FastLED.delay(2000 / strtoul(velocidade.c_str(), NULL, 16));
   }

              if (ledState == "papainoel") {
  draw(); 

    FastLED.show(); // display this frame

                if (millis() - tempo10 > 30000)//Faz a verificaçao das funçoes a cada 30min
   {

  draw();

   if (eff >= 3) {
      eff = 0;
    } else {
      eff += 1;
    }
    
  //static int frame = 0;
  //if (frame++ % 32 == 0)
  //  Serial.println(FastLED.getFPS());
  FastLED.show(); // display this frame

    tempo10 = millis();
     
   }

  }

              if (ledState == "dots") {
   dots();
  FastLED.show(); // display this frame
            FastLED.delay(2000 / strtoul(velocidade.c_str(), NULL, 16));
   }

    if (ledState == "credito") {
    fillString("Obrigado @SantoCyber faz um pix rastanerdi@gmail.com",1);
    FastLED.show(); // display this frame
    }

    


 //##Loop paralelo
    if (millis() - tempo6 > 5000)//Faz a verificaçao das funçoes a cada 30min
   {
    
 events.send("ping",NULL,millis());
 events.send(getSensorReadings().c_str(),"new_readings" ,millis());
   //Serial.print(getSensorReadings().c_str());
       
      tempo6 = millis();
     
   }


if (millis() - tempo8 > 3600000)//Faz a verificaçao das funçoes a cada 30min
   {
    //#### Grava o LOG em JSON
verifica2(); 
      tempo8 = millis();
     
   }


//#####Configura  loop telegram 
   if (millis() - tempo > 5000)//Faz a verificaçao das funçoes a cada 2 Segundos
    {
      connect();//Funçao para verificar se ainda há conexao
      readTel();//Funçao para ler o telegram
      tempo = millis();//Reseta o tempo
     
   }

  if (millis() - tempo2 > 1800000)//Faz a verificaçao das funçoes a cada 30min
   {   //tira foto e manda clima no telegram
      verifica();
      tempo2 = millis();
     
   }

     if (millis() - tempo3 > 100)//Faz a verificaçao das funçoes a cada 30min
   {
      readVibra();
      tempo3 = millis();
     
   }

  digitalWrite(33, LOW);
  
    if (takeNewPhoto) {
    delay(200);
    capturePhotoSaveSpiffs();

    takeNewPhoto = false;
     delay(600);
  }
  delay(50);




  
  
}
