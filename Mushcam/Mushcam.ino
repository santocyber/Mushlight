/*******************************************************************

 MushCAM

  This program records an mjpeg avi video in the psram of a ESP32-CAM, and sends a jpeg and a avi video to Telegram.

  https://github.com/santocyber/MushCAM licensed under the
    GNU General Public License v3.0

  por SantoCyber
  santocyber@gmail.com


*******************************************************************/

// ----------------------------
// Standard Libraries - Already Installed if you have ESP32 set up
// ----------------------------

#include <WiFi.h>
#include <WiFiClientSecure.h>
//#include <WiFiClient.h>

#include "esp_camera.h"
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include <Arduino.h>
#include <ArduinoJson.h>

#include <soc/rtc_wdt.h>
#include <esp_task_wdt.h>



#include "esp_wifi.h"
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include <ESPmDNS.h>




//By SantoCyber Captiveportal
//#############################################################
#include <SPIFFS.h>
#include <ESPAsyncWebServer.h>

void startCameraServer();
void stopCameraServer();
String camserver = "off";
long timecam;   
long timefoto;
long tempotelegram;
long tempostream;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Search for parameter in HTTP POST request
const char* PARAM_INPUT_1 = "ssid";
const char* PARAM_INPUT_2 = "pass";
const char* PARAM_INPUT_5 = "tokentelegram";
const char* PARAM_INPUT_14 = "nomedobot";


//Variables to save values from HTML form
String ssid;
String pass;
String tokentelegram;
String nomedobot;


// File paths to save input values permanently
const char* ssidPath = "/ssid.txt";
const char* passPath = "/pass.txt";
const char* nomedobotPath = "/nomedobot.txt";
const char* tokentelegramPath = "/tokentelegram.txt";
const char* FILE_PHOTO = "/photo.jpg";


//#####################################################################Config streming


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
//##########################################################################


//#####INICIA TELEGRAM TOKEN via web
String TOKEN2(){
  initSPIFFS();
    tokentelegram = readFile (SPIFFS, tokentelegramPath);

String TTT = tokentelegram.c_str();
return TTT; 
}
String TOKEN = TOKEN2();

#define BOTtoken TOKEN

//#include <UniversalTelegramBot.h>
#include "UniversalTelegramBot.h"  // use local library which is a modified copy of an old version

static const char vernum[] = "MushCAM0.1V";
String devstr =  "mushcambysantocyber";
int max_frames = 500;
int frame_interval = 500;          // 0 = record at full speed, 100 = 100 ms delay between frames
float speed_up_factor = 1;          // 1 = play at realtime, 0.5 = slow motion, 10 = speedup 10x

framesize_t configframesize = FRAMESIZE_VGA; // FRAMESIZE_ + QVGA|CIF|VGA|SVGA|XGA|SXGA|UXGA
int framesize = FRAMESIZE_VGA; //FRAMESIZE_HD;
int quality = 0.5;
int qualityconfig = 10;


struct tm timeinfo;
time_t now;

// Initialize Wifi connection to the router and Telegram BOT

//char ssid[] = "InternetSA";     // your network SSID (name)
//char password[] = "cadebabaca"; // your network key
// https://sites.google.com/a/usapiens.com/opnode/time-zones  -- find your timezone here
String TIMEZONE = "GMT0BST,M3.5.0/01,M10.5.0/02";

// you can enter your home chat_id, so the device can send you a reboot message, otherwise it responds to the chat_id talking to telegram

String chat_id;


bool reboot_request = false;

#define CAMERA_MODEL_AI_THINKER
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

#include "esp_system.h"

bool setupCamera() {
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
  //init with high specs to pre-allocate larger buffers
  if (psramFound()) {
      config.fb_location = CAMERA_FB_IN_PSRAM;
    config.frame_size = configframesize;
    config.jpeg_quality = qualityconfig;
    config.fb_count = 4; 
        config.grab_mode = CAMERA_GRAB_LATEST;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
      config.fb_location = CAMERA_FB_IN_DRAM;
  }

  //Serial.printf("Internal Total heap %d, internal Free Heap %d\n", ESP.getHeapSize(), ESP.getFreeHeap());
  //Serial.printf("SPIRam Total heap   %d, SPIRam Free Heap   %d\n", ESP.getPsramSize(), ESP.getFreePsram());

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
  //Serial.printf("Internal Total heap %d, internal Free Heap %d\n", ESP.getHeapSize(), ESP.getFreeHeap());
  //Serial.printf("SPIRam Total heap   %d, SPIRam Free Heap   %d\n", ESP.getPsramSize(), ESP.getFreePsram());

 // sensor_t * s = esp_camera_sensor_get();

  //  drop down frame size for higher initial frame rate
 // s->set_framesize(s, (framesize_t)framesize);
 // s->set_quality(s, quality);
  delay(200);


  //  cam.init(config);

  
  return true;
}


#define FLASH_LED_PIN 4

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

int Bot_mtbs = 5000; //mean time between scan messages
long Bot_lasttime;   //last time messages' scan has been done

bool flashState = LOW;

camera_fb_t * fb = NULL;
camera_fb_t * vid_fb = NULL;

//TaskHandle_t the_camera_loop_task;
//void the_camera_loop (void* pvParameter) ;
static void IRAM_ATTR PIR_ISR(void* arg) ;

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





unsigned long previousMillis = 0;
const long interval = 10000;  // interval to wait for Wi-Fi connection (milliseconds)

IPAddress localIP;
//IPAddress localIP(192, 168, 1, 200); // hardcoded

// Set your Gateway IP address
IPAddress localGateway;
//IPAddress localGateway(192, 168, 1, 1); //hardcoded
IPAddress subnet(255, 255, 0, 0);
IPAddress primaryDNS(8, 8, 8, 8); //nao nao eh opcional nao, eh obrigatorio demorei mt tempo pra perceber q o relogio e o telegram n tava funcionando por causa do dns



bool init_wifi() {
  uint32_t brown_reg_temp = READ_PERI_REG(RTC_CNTL_BROWN_OUT_REG);
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
  if(ssid==""){
    Serial.println("Undefined SSID or IP address.");
    return false;
  }
  
  // Attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);

  // Set WiFi to station mode and disconnect from an AP if it was Previously connected
  //devstr.toCharArray(devname, devstr.length() + 1);        // name of your camera for mDNS, Router, and filenames
  WiFi.mode(WIFI_STA);
  WiFi.setHostname(nomedobot.c_str());

    WiFi.begin(ssid.c_str(), pass.c_str());

  
  
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


  //wifi_ps_type_t the_type;

  //esp_err_t get_ps = esp_wifi_get_ps(&the_type);

  //esp_err_t set_ps = esp_wifi_set_ps(WIFI_PS_NONE);

  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, brown_reg_temp);

  configTime(0, 0, "pool.ntp.org");
  char tzchar[80];
  TIMEZONE.toCharArray(tzchar, TIMEZONE.length());          // name of your camera for mDNS, Router, and filenames
  setenv("TZ", tzchar, 1);  // mountain time zone from #define at top
  tzset();

  if (!MDNS.begin(nomedobot.c_str())) {
    Serial.println("Error setting up MDNS responder!");
    return false;
  } else {
    Serial.printf("mDNS responder started '%s'\n", nomedobot.c_str());
  }
  time(&now);

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  return true;
}


//////////////////////////////////////////////////////////////////////////////////////

void setup() {
  //desahabilita o watchdog configurando o timeout para 40 segundos

  esp_task_wdt_init(30, true);


  
  Serial.begin(115200);
  Serial.println("---------------------------------");
  Serial.printf("MushCAM bot %s\n", vernum);
  Serial.println("---------------------------------");

// in setup()
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);


  pinMode(FLASH_LED_PIN, OUTPUT);
  digitalWrite(FLASH_LED_PIN, flashState); //defaults to low

  pinMode(12, INPUT_PULLUP);        // pull this down to stop recording

  pinMode(33, OUTPUT);             // little red led on back of chip
  digitalWrite(33, LOW);           // turn on the red LED on the back of chip



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






  if (!setupCamera()) {
    Serial.println("Camera Setup Failed!");

  }

  for (int j = 0; j < 3; j++) {
    camera_fb_t * fb = esp_camera_fb_get();
    if (!fb) {
      Serial.println("Camera Capture Failed");
    } else {
      Serial.print("Pic, len="); Serial.print(fb->len);
      Serial.printf(", new fb %X\n", (long)fb->buf);
      esp_camera_fb_return(fb);
      delay(50);
    }
  }

 




  //initSPIFFS();

  
  // Load values saved in SPIFFS
  ssid = readFile(SPIFFS, ssidPath);
  pass = readFile(SPIFFS, passPath);
  nomedobot = readFile(SPIFFS, nomedobotPath);
  tokentelegram = readFile (SPIFFS, tokentelegramPath);


  Serial.println(ssid);
  Serial.println(pass);
  Serial.println(nomedobot);
  Serial.println(tokentelegram);




  
  if(init_wifi()) {


  client.setInsecure();




  // Route for root / web page
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
      request->send(SPIFFS, "/index.html", "text/html", false);
    });
    server.serveStatic("/", SPIFFS, "/");



      server.on("/server", HTTP_GET, [](AsyncWebServerRequest * request) {
  
    if (camserver == "on"){
      camserver = "off";     
    }else{
camserver = "on";
    }
    request->send(SPIFFS, "/index.html", "text/html", false);
  });

      server.on("/capture", HTTP_GET, [](AsyncWebServerRequest * request) {
      xTaskCreate(foto,"FOTO", 20000, NULL, 0, NULL);     

  
//  capturePhotoSaveSpiffs();
//    request->send_P(200, "text/plain", "Taking Photo");
    request->send(SPIFFS, "/index.html", "text/html", false);
  });

     server.on("/saved-photo", HTTP_GET, [](AsyncWebServerRequest * request) {


//    request->send_P(200, "text/plain", "Taking Photo");
//    request->send(SPIFFS, "/index.html", "text/html", false, processor);
       request->send(SPIFFS, FILE_PHOTO, "image/jpg", false);
  });


     

        
    
     server.begin();






    




    }
  else {
      client.setInsecure();

    // Connect to Wi-Fi network with SSID and password
    Serial.println("Setting AP (Access Point)");
    // NULL sets an open Access Point
    WiFi.softAP("MushCAM-config", NULL);

    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(IP); 

    // Web Server Root URL
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(SPIFFS, "/wifimanager.html", "text/html");
    });
    
    server.serveStatic("/", SPIFFS, "/");


      server.on("/capture", HTTP_GET, [](AsyncWebServerRequest * request) {
 // capturePhotoSaveSpiffs();
      xTaskCreate(foto,"FOTO", 20000, NULL, 0, NULL);     


//    request->send_P(200, "text/plain", "Taking Photo");
    request->send(SPIFFS, "/wifimanager.html", "text/html", false);
  });

     server.on("/saved-photo", HTTP_GET, [](AsyncWebServerRequest * request) {


//    request->send_P(200, "text/plain", "Taking Photo");
//    request->send(SPIFFS, "/index.html", "text/html", false, processor);
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
            //EEPROM.put(tokentelegram.c_str());
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
      request->send(200, "text/plain", "Prontinho. MushCAM reiniciando, digite /server no telegram para saber o ip");
      delay(3000);
      ESP.restart();
    });
    server.begin();
  }


  // Make the bot wait for a new message for up to 60seconds
  //bot.longPoll = 60;
  bot.longPoll = 5;

//  client.setInsecure();

  setupinterrupts();

  String stat = "Reboot\nDevice: " + devstr + "\nVer: " + String(vernum) + "\nRssi: " + String(WiFi.RSSI()) + "\nip: " +  WiFi.localIP().toString() + "\n/start";
  bot.sendMessage(chat_id, stat, "");

  pir_enabled = true;
  avi_enabled = true;
  digitalWrite(33, HIGH);
}

int loopcount = 0;


void loop() {
  loopcount++;

  client.setHandshakeTimeout(120000); // workaround for esp32-arduino 2.02 bug https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot/issues/270#issuecomment-1003795884

  if (reboot_request) {
    String stat = "Rebooting on request\nDevice: " + devstr + "\nVer: " + String(vernum) + "\nRssi: " + String(WiFi.RSSI()) + "\nip: " +  WiFi.localIP().toString() ;
    bot.sendMessage(chat_id, stat, "");
    delay(10000);
    ESP.restart();
  }

  if (picture_ready) {
    picture_ready = false;
    send_the_picture();
  }

  if (video_ready) {
    
    video_ready = false;
    send_the_video();
  }


    
      
  





     if (millis() - tempotelegram > 5000)//Faz a verificaçao das funçoes a cada 30min
   {
      connect();
       //  server.begin();
      readTel();
   // xTaskCreate(tele,"READ TEL", 50000, NULL, 0, NULL);     

    
      tempotelegram = millis();
     
   }

     if (millis() - timefoto > 1800000)//Faz a verificaçao das funçoes a cada 30min
   {
      send_the_picture();
      timefoto = millis();
     
   }


     if (millis() - tempostream > 100)//Faz a verificaçao das funçoes a cada 30min
   {

     if (camserver == "on"){

      

//inicia streming

 //  rtspServer.begin();


  //    stream();
   //  xTaskCreate(streamx,"VLC", 20000, NULL, 0, NULL);     
     }

      tempostream = millis();
     
   }


 


   

  
}
