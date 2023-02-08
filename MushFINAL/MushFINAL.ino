//Codigo disponivel no github desenvolvido por @SantoCyber
#include <WiFi.h>
#include "Arduino.h"
#include <StringArray.h>
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
#include <FS.h>
#include <SD_MMC.h>
#include <SD.h>
#include <WiFiClientSecure.h>
#include <time.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "EEPROM.h"
#include <ESPmDNS.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_AHTX0.h>


//#######################################ATIVA FUNCOES

#define USETOUCH 1
#define GRAVALOG 1
#define CAMERA 1
#define SENSORES 1
#define BLEX 1

//##############################################ATIVA O bluetooth

#include <esp_attr.h>
//#include "soc/rtc_wdt.h"


#if (BLEX == 1)

#include <BleSerial.h>

BleSerial ble;
#endif
//################## IWDT
#include <ESP32Ping.h>

#include <esp_task_wdt.h>
#include "soc/rtc_cntl_reg.h"  // Disable brownour problems
#include "soc/soc.h"           // Disable brownour problems
#include "sdkconfig.h"
#include "driver/rtc_io.h"
#include <esp_system.h>
#include <nvs_flash.h>

#if (CAMERA ==1)
#include "esp_camera.h"
#endif

#define MBEDTLS_ERR_NET_RECV_FAILED                       -0x004C 



//################################################################################Task handle 
#include <stdio.h>
#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "freertos/queue.h"

TaskHandle_t teletask = NULL;
TaskHandle_t verificatask = NULL;
TaskHandle_t cameratask = NULL;
TaskHandle_t toquetask = NULL;


//void toque (void* pvParameters);

static void IRAM_ATTR PIR_ISR(void* arg) ;




//#############################################################################Configura GPIO
#define LEDBUILTIN  48 
#define LED_PIN  42 //2
#define co2pin   1
#define vibrapin 15
#define touchpin 14
//#define pinoLed 2 //led onboard built-in

int PIRpin = 19;

#define I2C_SDA 20
#define I2C_SCL 21



#define SD_MMC_CMD 38 //Please do not modify it.
#define SD_MMC_CLK 39 //Please do not modify it. 
#define SD_MMC_D0  40 //Please do not modify it.




//TwoWire I2C = TwoWire(1);
//Wire.begin(I2C_SDA, I2C_SCL);

//config BMP AHT I2C
Adafruit_AHTX0 aht;
Adafruit_BMP280 bmp; 







//############################################ Create AsyncWebServer object on port 80
AsyncWebServer server(80);

AsyncEventSource events("/events");

//###########Configura Crypto

const int httpsPort = 443;
HTTPClient http;



//##########Configura cam
boolean takeNewPhoto = false;


const long gmtOffset_sec = 3600 * 10;
const int daylightOffset_sec = 3600;
// NTP Server - this is to put a time stanp in the file name
const char* ntpServer = "pool.ntp.org";


static const char vernum[] = "MushLightCAM0.1V";


#if (CAMERA == 1)
int max_frames = 150;
framesize_t configframesize = FRAMESIZE_VGA; // FRAMESIZE_ + QVGA|CIF|VGA|SVGA|XGA|SXGA|UXGA
int frame_interval = 0;          // 0 = record at full speed, 100 = 100 ms delay between frames
float speed_up_factor = 0.5;          // 1 = play at realtime, 0.5 = slow motion, 10 = speedup 10x
int framesize = FRAMESIZE_VGA; //FRAMESIZE_HD;
int quality = 10;
int qualityconfig = 4;

struct tm timeinfo;
time_t now;



camera_fb_t * fb = NULL;
camera_fb_t * vid_fb = NULL;

TaskHandle_t the_camera_loop_task;
void the_camera_loop(void* pvParameter) ;
TaskHandle_t savesdtask;
void savesd(void* pvParameter) ;



bool video_ready = false;
bool picture_ready = false;
bool active_interupt = false;
bool pir_enabled = true;
bool avi_enabled = true;

int avi_buf_size = 0;
int idx_buf_size = 0;

bool isMoreDataAvailable();


////////////////////////////////  send photo as 512 byte blocks or jzblocksize 
int currentByte;
uint8_t* fb_buffer;
size_t fb_length;

bool isMoreDataAvailable() {
  return (fb_length - currentByte);
}

uint8_t getNextByte() {
  currentByte++;
  return (fb_buffer[currentByte - 1]);
}

////////////////////////////////  send avi as 512 byte blocks or jzblocksize 
int avi_ptr;
uint8_t* avi_buf;
size_t avi_len;

bool avi_more() {
  return (avi_len - avi_ptr);
}

uint8_t avi_next() {
  avi_ptr++;
  return (avi_buf[avi_ptr - 1]);
}

bool dataAvailable = false;


///////////////////////////////

uint8_t * psram_avi_buf = NULL;
uint8_t * psram_idx_buf = NULL;
uint8_t * psram_avi_ptr = 0;
uint8_t * psram_idx_ptr = 0;
char strftime_buf[64];





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


#endif

//##########CONFIG TEXTO
String runningText = "MushLight";

//####################################################Configura variaveis

int valoratm = 0;//Declara a variável valorldr como inteiro
int valortemp = 0;//Declara a variável valorldr como inteiro
int valorhumi = 0;//Declara a variável valorldr como inteiro
int valorvibra;


String ledStateCAM;
String ledState;
boolean led_state = true;
int clap_counter = 0;
int color_counter = 0;
int touchcounter = 0;

uint32_t notConnectedCounter = 0;
uint32_t ConnectedCounter = 0;

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




//##configura o millis
const long interval = 10000;  // interval to wait for Wi-Fi connection (milliseconds)
unsigned long previousMillis = 0;
unsigned long tempo = millis();
unsigned long tempoverifica = millis();
unsigned long tempo3 = millis();
unsigned long tempo4 = millis();
unsigned long tempo5 = millis();
unsigned long tempo6 = millis();
unsigned long tempo7 = millis();
unsigned long tempo8 = millis();
unsigned long tempo9 = millis();
unsigned long tempo10 = millis();
unsigned long tempoping = millis();
unsigned long tempotelegram = millis();

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




//#############################################################################CONFIGURA LED
//###########################################################################
//############################http://fastled.io/docs/3.1/struct_c_r_g_b.html
//############################CONFIGURA MATRIZ DE LED

#define WIDTH 16
#define HEIGHT 16
#define SEGMENTS 1

#define COLOR_ORDER GRB


//configura texto
#define TEXT_HEIGHT 0     
#define WIDTHX WIDTH       //configura texto pixels
#define HEIGHTX HEIGHT

#define LET_WIDTH 5       // ширина буквы шрифта
#define LET_HEIGHT 7      // высота буквы шрифта
#define SPACE 0           // пробел


//configura gif
#define FRAME_WIDTH WIDTH 
#define FRAME_HEIGHT HEIGHT


//##Configura effeito newone
#define NUM_ROWS HEIGHT
#define NUM_COLS WIDTH
//#define NUM_LEDS NUM_ROWS * NUM_COLS
byte eff = 0;
//#define WIDTH NUM_COLS
//#define HEIGHT NUM_ROWS
//#define speed (100/(HEIGHT-4))
//////////////////

#define NUM_LEDS WIDTH * HEIGHT * SEGMENTS
#define CHIPSET WS2811
CRGB leds[NUM_LEDS];
CRGB ledflash[1];



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

#define SCORE_SIZE 0  
//#define FASTLED_INTERRUPT_RETRY_COUNT 0
//#define FASTLED_ALLOW_INTERRUPTS 0
#define BRIGHTNESS 255
#define CURRENT_LIMIT 2000





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



//############CONFIG TIMER
#define UDP_PACKET_MAX_SIZE 1024
char incomeBuffer[UDP_PACKET_MAX_SIZE]; 

#define NTP_OFFSET  -3  * 60 * 60 // In seconds
#define NTP_INTERVAL 60 * 1000    // In miliseconds
#define NTP_ADDRESS  "0.pool.ntp.org"
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, NTP_ADDRESS, NTP_OFFSET, NTP_INTERVAL);
const int udpPort = 3333;



//#####Configura data logger
String    time_now;
#define TZone -3

//####Configura contador
unsigned long cSegundos = 0;
unsigned int segundos = 0;
byte minutos = 0;





#define GLOBAL_COLOR_1 CRGB::Green    
#define GLOBAL_COLOR_2 CRGB::Orange   

//##################### Configura IP

IPAddress localIP;
//IPAddress localIP(192, 168, 1, 200); // hardcoded

// Set your Gateway IP address
IPAddress localGateway;
//IPAddress localGateway(192, 168, 1, 1); //hardcoded
IPAddress subnet(255, 255, 0, 0);
IPAddress primaryDNS(8, 8, 8, 8); //nao nao eh opcional nao, eh obrigatorio demorei mt tempo pra perceber q o relogio e o telegram n tava funcionando por causa do dns



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



//####################################### Configura voids do cartao SD

String readFileSD(fs::FS &fs, const char * path){
  Serial.printf("Reading file: %s\r\n", path);
    File root = SD_MMC.open(path);

  //File root = fs.open(path);
  if(!root || root.isDirectory()){
    Serial.println("- failed to open file for reading");
    return String();
  }
  
  String fileContent;
  while(root.available()){
    fileContent = root.readString();
    break;     
  }
  fileContent.trim();
  fileContent.length();
  return fileContent;
}


void deleteFile(fs::FS &fs, const char * path){
    Serial.printf("Deleting file: %s\n", path);
    if(fs.remove(path)){
        Serial.println("File deleted");
    } else {
        Serial.println("Delete failed");
    }
}


void writeFileSD(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Writing file: %s\n", path);
    //File root = SD_MMC.open(path, FILE_WRITE);

    File root = fs.open(path, FILE_WRITE);
    if(!root){
        Serial.println("Failed to open file for writing");
        return;
    }
   
}


//###########################################################INICIA TELEGRAM TOKEN via web
String TOKEN(){
  initSPIFFS();
    tokentelegram = readFile (SPIFFS, tokentelegramPath);

String TTT = tokentelegram.c_str();
return TTT; 
}
//#include <UniversalTelegramBot.h>
#include "UniversalTelegramBot.h"
#define tokentele TOKEN()

WiFiClientSecure client;

UniversalTelegramBot bot(tokentele, client);



//######################################################################## Initialize WiFi
bool initWiFi() {
  if(ssid==""){
    Serial.println("Undefined SSID");
    return false;
  }

  WiFi.mode(WIFI_STA);
  WiFi.setHostname(nomedobot.c_str());
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
  //  readTel(void * pvParameters );
  //  verifica();
  //  verifica2();
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

      
  Serial.println(WiFi.localIP());
  return true;
}








////#############################################################################Le sensores


#if (SENSORES == 1)


 String readCO2() {

  float c = analogRead(co2pin);

    return String(c);

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
#endif
#if (SENSORES == 0)
 String readCO2() {
  return "desativado";
}
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


#if (CAMERA == 1)

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

#endif

//######################## SETUP

void setup() {
    Serial.begin(115200);
    Serial.setDebugOutput(true);
    Serial.println();

    pinMode(LEDBUILTIN, OUTPUT);


    SD_MMC.setPins(SD_MMC_CLK, SD_MMC_CMD, SD_MMC_D0);
    if (!SD_MMC.begin("/sdcard", true, true, SDMMC_FREQ_DEFAULT, 5)) {
      Serial.println("Card Mount Failed");
      return;
    }
    
    uint8_t cardType = SD_MMC.cardType();
    if(cardType == CARD_NONE){
        Serial.println("No SD_MMC card attached");
        return;
    }
  

    Serial.print("SD_MMC Card Type: ");
    if(cardType == CARD_MMC){
        Serial.println("MMC");
    } else if(cardType == CARD_SD){
        Serial.println("SDSC");
    } else if(cardType == CARD_SDHC){
        Serial.println("SDHC");
    } else {
        Serial.println("UNKNOWN");
    }



    Serial.printf("Total space: %lluMB\r\n", SD_MMC.totalBytes() / (1024 * 1024));
    Serial.printf("Used space: %lluMB\r\n", SD_MMC.usedBytes() / (1024 * 1024));


  int avail_psram = ESP.getFreePsram();
  Serial.print("PSRAM size to store the video "); Serial.println(avail_psram);
  idx_buf_size = max_frames * 10 + 20;
//  if (hdcam) {
    avi_buf_size = avail_psram - 900 * 1024; //900 for hd, 500 for vga
    Serial.println("Camera to HD ");
  Serial.print("try to allocate "); Serial.println(avi_buf_size);
  psram_avi_buf = (uint8_t*)ps_malloc(avi_buf_size);
  if (psram_avi_buf == 0) Serial.printf("psram_avi allocation failed\n");
  psram_idx_buf = (uint8_t*)ps_malloc(idx_buf_size); // save file in psram
  if (psram_idx_buf == 0) Serial.printf("psram_idx allocation failed\n");






    

  //desahabilita o watchdog configurando o timeout para 40 segundos

  esp_task_wdt_init(60, false);

//xTaskCreate(fast,"FAST LED", 1000, NULL, 1, NULL );     
// xTaskCreate(tele,"READ TEL", 45000, NULL, 0, NULL);     
//xTaskCreatePinnedToCore(tele, "READTEL", 20000, NULL , 10, &teletask, 1);     




//inicia PIR
setupinterrupts();


  //Disable watchdog timers
  disableCore0WDT();
 // disableCore1WDT();
  disableLoopWDT();
  esp_task_wdt_delete(NULL);
  //rtc_wdt_protect_off();
  //rtc_wdt_disable();

  //Start tasks
 //// xTaskCreate(ReadSerialTask, "ReadSerialTask", 10240, NULL, 1, NULL);
  //xTaskCreate(ReadBtTask, "ReadBtTask", 10240, NULL, 1, NULL);

#if (BLEX == 1)
//#####################################Configura BLEX

  ble.begin("MushLight");

  BLESecurity *pSecurity = new BLESecurity();
  pSecurity->setStaticPIN(123456); 
  pSecurity->setAuthenticationMode(ESP_LE_AUTH_REQ_SC_BOND);
  
  //set static passkey
  Serial.println("Characteristic defined! Now you can read it in your phone!");
 #endif
 

    EEPROM.begin(512);

// Turn-off the 'brownout detector'
 // WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);



//#########################################################inicia sensores tem umi pressao




#if (SENSORES == 1)
  //I2C.begin(I2C_SDA, I2C_SCL, 100000);
Wire.begin(I2C_SDA, I2C_SCL);
aht.begin();
bmp.begin();
#endif


#if (CAMERA == 1)
  // Camera init
if (!setupCamera())
  {
    Serial.println("Camera Setup Failed!");
  }

#endif
 
//########### LED

    FastLED.addLeds<WS2812, LEDBUILTIN, GRB>(ledflash, 1).setCorrection(TypicalSMD5050);

FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);
  FastLED.setBrightness( BRIGHTNESS );

 // if (CURRENT_LIMIT > 0) FastLED.setMaxPowerInVoltsAndMilliamps(5, CURRENT_LIMIT);
  FastLED.clear();
  FastLED.show();
  //randomSeed(analogRead(0) + analogRead(1));




 //########################Le arquivos gravados             
//ja foi iniciado no telegram

  //initSPIFFS();

  
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

delay(200);
//client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
 client.setTimeout(2000);

 client.setInsecure();
 //xTaskCreatePinnedToCore(tele, "READTEL", configMINIMAL_STACK_SIZE + 20000, NULL , 1, &teletask, 0);   
 //xTaskCreate(tele,"READTEL", configMINIMAL_STACK_SIZE + 20000, NULL, 0, &teletask);     
 //xTaskCreate(toque,"Toque", configMINIMAL_STACK_SIZE + 2000, NULL, 0, &toquetask);     

delay(500);

delay(200);
       

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


server.on("/lersd", HTTP_GET, listFilesOnWebPage);
server.on("/lersdx/*", HTTP_GET, handleFile);
server.on("/apagarsd", HTTP_GET, apagarsd);
 


       
    // Route to set GPIO state to HIGH
    server.on("/log", HTTP_GET, [](AsyncWebServerRequest *request) {
   logger = "{\"sensores\":[{\"Temperatura\":\"22.00\",\"Umidade\":\"45.00\",\"Pressao\":\"45.00\",\"CO2\":\"198.00\",\"Hora\":\"22:00\"}";
  
    logger += readFileSD(SD_MMC, loggerPath); 
   logger += "]}";
  //###Q gambiarra pra funcionar o JSON
    request->send(200, "application/json", logger);
   request->send_P(200, "text/plain", logger.c_str());
   logger = String();
  });

  // Route to set GPIO state to HIGH
    server.on("/climalog", HTTP_GET, [](AsyncWebServerRequest *request) {
    logger = "{\"clima\":[{";
    logger += readFileSD(SD_MMC, climaPath); 
    logger += "]}}";
    request->send(200, "application/json", logger);
   request->send_P(200, "text/plain", logger.c_str());
   logger = String();
  });

    
      // Route to set GPIO state to HIGH
    server.on("/loggrafico", HTTP_GET, [](AsyncWebServerRequest *request) {
     verifica2();
     verifica();
 
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
      writeFileSD(SPIFFS, climaPath, "");
        writeFileSD(SPIFFS, loggerPath, "");
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
      ledState = "blue";
      request->send(SPIFFS, "/index.html", "text/html", false);
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
    Serial.println("TASK capture");
    takeNewPhoto = true;
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
delay(300);


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



      
      server.on("/capture", HTTP_GET, [](AsyncWebServerRequest * request) {
    takeNewPhoto = true;
//    request->send_P(200, "text/plain", "Taking Photo");
    request->send(SPIFFS, "/wifimanager.html", "text/html", false, processor);
  });

  server.on("/saved-photo", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, FILE_PHOTO, "image/jpg", false);
  });



    
    
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


String configg = "Prontinho. MushLight reiniciando, conecte no seu WIFI e clique no endereco abaixo";
       configg += "<br><br>";
       configg += "CLIQUE -> VVVVVVVV";
       configg += "<br><br>";
       configg += "<a href='";
       configg += "http://";       
       configg += nomedobot.c_str();
       configg += ".local";
       configg += "'>";
       configg += nomedobot.c_str();
       configg += ".local";             
       configg += "</a>";

      
      request->send(200, "text/html", configg);
      delay(3000);
      ESP.restart();
    });
    server.begin();
  }


}





void loop() {

 //##Loop paralelo


#if (GRAVALOG == 1)
    

    if (millis() - tempo6 > 10000)//Faz a verificaçao das funçoes a cada 30min
   {
          Serial.println("EVENTS ping pagina");
   events.send("ping",NULL,millis());
   events.send(getSensorReadings().c_str(),"new_readings" ,millis());
 //  Serial.println(getSensorReadings().c_str());
   

        //   xTaskCreate(sensorread,"Envia sensorread", 10000, NULL, 0, NULL);     

       
      tempo6 = millis();
     
   }





if (millis() - tempo8 > 3600000)//Faz a verificaçao das funçoes a cada 60min
   {

          Serial.println("verifica2");

      //#### Grava o LOG em JSON
      verifica2(); 
      tempo8 = millis();
     
   }

   


//1200000
  if (millis() - tempoverifica > 1800000)//Faz a verificaçao das funçoes a cada 30min
   {   //tira foto e manda clima no telegram
          Serial.println("verifica1");

        connect();
      //  readTel();
        //xTaskCreate(readTel,"READ TEL", 50000, NULL, 0, NULL);     
    //    xTaskCreate(verifica1,"VERIFICA1", 10000, NULL, 0, NULL);     

      verifica();
      tempoverifica = millis();
     
   }


#endif

     if (millis() - tempoping > 100000)//Faz a verificaçao das funçoes a cada 30min
   {
          Serial.println("pingando");
      connect();
      pingando();

      //esp_wifi_stop(); 
    delay(200);
    WiFi.disconnect();
    delay(2000);
    connect();
      tempoping = millis();
     
   }



  if (millis() - tempotelegram > 20000)//Faz a verificaçao das funçoes a cada 30min
   {   //tira foto e manda clima no telegram
          Serial.println("ReadTel millis");

        connect();
        readTel();
#if (BLEX == 1)
        
        ble.println("Hello!");
#endif
        
      tempotelegram = millis();
     
   }



#if (USETOUCH ==1)


 if (digitalRead(touchpin) == HIGH){ //SE A LEITURA DO PINO FOR IGUAL A HIGH, FAZ
  toque();

    //  xTaskCreate(toque,"Toque", 20000, NULL, tskIDLE_PRIORITY + 1, &toquetask);     

  delay(1000);
 }

#endif

#if (BLEX == 1)

//####################################le bluetooth serial
    if (ble.available() > 0) {
    
    delay(200);
    
    String input = ble.readStringUntil('\n');
    Serial.println(input);


if (input.indexOf("ledon") > -1)//Caso o texto recebido contenha "ON"
      {
         Serial.println("led ON");
         ble.println("led ON");

         ledState = "ledon";
       //  bot.sendMessage(id, "LED ON VIA BLUETOOTH", "");//Envia uma Mensagem para a pessoa que enviou o Comando.
      }
if (input.indexOf("ledoff") > -1)//Caso o texto recebido contenha "ON"
      {
         Serial.println("led OFF");
         ledState = "ledoff";
         bot.sendMessage(id, "LED OFF VIA BLUETOOTH", "");//Envia uma Mensagem para a pessoa que enviou o Comando.
      }

if (input.indexOf("foto") > -1)//Caso o texto recebido contenha "ON"
      {
         Serial.println("SORRIA!");
         //ledon();

    send_the_picture();

           
        // bot.sendMessage(id, "FOTO VIA BLUETOOTH", "");//Envia uma Mensagem para a pessoa que enviou o Comando.
      }

      if (input.indexOf("sd") > -1)//Caso o texto recebido contenha "ON"
      {
         Serial.println("SORRIA!");
         ble.println("SORRIA!!!");
         //savesd();
      xTaskCreatePinnedToCore(savesd, "sdtask", 20000, NULL, 1, &savesdtask, 1);

        // bot.sendMessage(id, "FOTO VIA BLUETOOTH", "");//Envia uma Mensagem para a pessoa que enviou o Comando.
      }
  if (input.indexOf("reset") > -1)//Caso o texto recebido contenha "ON"
      {
         Serial.println("RESET!");
         ESP.restart();
       }
  if (input.indexOf("flash") > -1)//Caso o texto recebido contenha "ON"
      {

        if(ledStateCAM == "flash"){
         Serial.println("FLASHOFF");
         ledStateCAM = "flashoff";
        }
        else{
           Serial.println("FLASH");
          ledStateCAM = "flash";}
        
       }

       
      
    delay(300);
    }
    delay(20);
#endif

//############################## Le porta serial
  if (Serial.available() > 0) {
    
      String input = Serial.readStringUntil('\n');
      Serial.println(input); 
 #if (BLEX == 1)        
      ble.println(input);
#endif  
      delay(300);

      if (input.indexOf("sd") > -1)//Caso o texto recebido contenha "ON"
      {
         Serial.println("SORRIA!");
         //savesd();
      xTaskCreatePinnedToCore(savesd, "sdtask", 20000, NULL, 1, &savesdtask, 1);

         
        // bot.sendMessage(id, "FOTO VIA BLUETOOTH", "");//Envia uma Mensagem para a pessoa que enviou o Comando.
      }

  if (input.indexOf("reset") > -1)//Caso o texto recebido contenha "ON"
      {
         Serial.println("RESET!");
         ESP.restart();
       }
   if (input.indexOf("flash") > -1)//Caso o texto recebido contenha "ON"
      {

        if(ledStateCAM == "flash"){
         Serial.println("FLASHOFF");
         ledStateCAM = "flashoff";
        }
        else{
           Serial.println("FLASH");
          ledStateCAM = "flash";}
        
       }


  }




//#################################################TIRA FOTO
    if (takeNewPhoto) {
    delay(200);

     //   xTaskCreate(camera,"TAKEAPICTURE", 40000, NULL, 0, NULL);     
#if (CAMERA == 1)
//capturePhotoSaveSpiffs();
      xTaskCreatePinnedToCore(foto, "fototask", 20000, NULL, 1, NULL, 1);
      xTaskCreatePinnedToCore(savesd, "sdtask", 20000, NULL, 1, &savesdtask, 1);


    takeNewPhoto = false;
     delay(600);
  }
  delay(50);


  if (picture_ready) {
    picture_ready = false;
    send_the_picture();
  }

  if (video_ready) {
    
    video_ready = false;
    send_the_video();
  }

#endif


ifs();
}
