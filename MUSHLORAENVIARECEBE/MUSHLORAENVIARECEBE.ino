
//#include <StringArray.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <WiFiClientSecure.h>
#include <ESPmDNS.h>



#include <ESP32Ping.h>
#include <esp_task_wdt.h>
#include "soc/rtc_cntl_reg.h"  // Disable brownour problems
#include "soc/soc.h"           // Disable brownour problems
#include "sdkconfig.h"
#include "driver/rtc_io.h"
#include <esp_system.h>
#include <nvs_flash.h>


//################################################################################Task handle 
#include <stdio.h>
#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_attr.h>


TaskHandle_t xHandle = NULL;


#define STACK_SIZE 10000

 // Structure that will hold the TCB of the task being created.
 StaticTask_t xTaskBuffer;

 // Buffer that the task being created will use as its stack.  Note this is
 // an array of StackType_t variables.  The size of StackType_t is dependent on
 // the RTOS port.
// StackType_t *xStack;

 StackType_t xStack[ STACK_SIZE ];


//############################################ Create AsyncWebServer object on port 80
AsyncWebServer server(80);


//Variables to save values from HTML form
String ssid;
String pass;
String nomedobot;
String tokentelegram;

String id, welcome, from_name, text;//Váriaveis para armazenamento do ID e TEXTO gerado pelo Usuario


uint32_t notConnectedCounter = 0;
uint32_t ConnectedCounter = 0;


// Search for parameter in HTTP POST request
const char* PARAM_INPUT_1 = "ssid";
const char* PARAM_INPUT_2 = "pass";
const char* PARAM_INPUT_5 = "tokentelegram";
const char* PARAM_INPUT_14 = "nomedobot";

// File paths to save input values permanently
const char* ssidPath = "/ssid.txt";
const char* passPath = "/pass.txt";
const char* nomedobotPath = "/nomedobot.txt";
const char* tokentelegramPath = "/tokentelegram.txt";
//##configura o millis
const long interval = 10000;  // interval to wait for Wi-Fi connection (milliseconds)
unsigned long previousMillis = 0;
unsigned long tempoping = millis();
unsigned long tempotelegram = millis();


//##################### Configura IP

IPAddress localIP;
//IPAddress localIP(192, 168, 1, 200); // hardcoded

// Set your Gateway IP address
IPAddress localGateway;
//IPAddress localGateway(192, 168, 1, 1); //hardcoded
IPAddress subnet(255, 255, 0, 0);
IPAddress primaryDNS(8, 8, 8, 8); //nao nao eh opcional nao, eh obrigatorio demorei mt tempo pra perceber q o relogio e o telegram n tava funcionando por causa do dns




//############################################################### Arquivos 
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
    Serial.println(" SSID indefindo");
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











#include "Arduino.h"
#include "WiFi.h"
//#include "images.h"
#include "LoRaWan_APP.h"
#include <Wire.h>  
#include "HT_SSD1306Wire.h"



SSD1306Wire  factory_display(0x3c, 500000, SDA_OLED, SCL_OLED, GEOMETRY_128_64, RST_OLED); // addr , freq , i2c group , resolution , rst





//#AT+CHKEY=15C351E8617FAC8B1A5EF211E83B62BD
//#AT+CHKEY=16EF513D71596C1E30623C4A98D262B7

#define RF_FREQUENCY                                915000000 // Hz

#define TX_OUTPUT_POWER                             20        // dBm

#define LORA_BANDWIDTH                              0         // [0: 125 kHz,
                                                              //  1: 250 kHz,
                                                              //  2: 500 kHz,
                                                              //  3: Reserved]
#define LORA_SPREADING_FACTOR                       7         // [SF7..SF12]
#define LORA_CODINGRATE                             1         // [1: 4/5,
                                                              //  2: 4/6,
                                                              //  3: 4/7,
                                                              //  4: 4/8]
#define LORA_PREAMBLE_LENGTH                        8         // Same for Tx and Rx
#define LORA_SYMBOL_TIMEOUT                         0         // Symbols
#define LORA_FIX_LENGTH_PAYLOAD_ON                  false
#define LORA_IQ_INVERSION_ON                        false


#define RX_TIMEOUT_VALUE                            1000
#define BUFFER_SIZE                                 200 // Define the payload size here


char txpacket[BUFFER_SIZE];
char rxpacket[BUFFER_SIZE];

static RadioEvents_t RadioEvents;
void OnTxDone( void );
void OnTxTimeout( void );
void OnRxDone( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr );

typedef enum
{
    LOWPOWER,
    STATE_RX,
    STATE_TX
}States_t;

int16_t txNumber;
int16_t rxNumber;
States_t state;
bool sleepMode = false;
int16_t Rssi,rxSize;

String rssi = "RSSI --";
String packSize = "--";
String packet;
String send_num;
String show_lora = "lora data show";

unsigned int counter = 0;
bool receiveflag = false; // software flag for LoRa receiver, received data makes it true.
long lastSendTime = 0;        // last send time
uint64_t chipid;
int16_t RssiDetection = 0;


void OnTxDone( void )
{
  Serial.print("TX done......");
  state=STATE_RX;

}

void OnTxTimeout( void )
{
  Radio.Sleep( );
  Serial.print("TX Timeout......");
  state=STATE_TX;
}



void lora_init(void)
{
  Mcu.begin();
  txNumber=0;
  Rssi=0;
  rxNumber = 0;
  RadioEvents.TxDone = OnTxDone;
  RadioEvents.TxTimeout = OnTxTimeout;
  RadioEvents.RxDone = OnRxDone;

  Radio.Init( &RadioEvents );
  Radio.SetChannel( RF_FREQUENCY );
  Radio.SetTxConfig( MODEM_LORA, TX_OUTPUT_POWER, 0, LORA_BANDWIDTH,
                                 LORA_SPREADING_FACTOR, LORA_CODINGRATE,
                                 LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON,
                                 true, 0, 0, LORA_IQ_INVERSION_ON, 3000 );

  Radio.SetRxConfig( MODEM_LORA, LORA_BANDWIDTH, LORA_SPREADING_FACTOR,
                                 LORA_CODINGRATE, 0, LORA_PREAMBLE_LENGTH,
                                 LORA_SYMBOL_TIMEOUT, LORA_FIX_LENGTH_PAYLOAD_ON,
                                 0, true, 0, 0, LORA_IQ_INVERSION_ON, true );
  state=STATE_TX;
}











void setup() {
    Serial.begin(115200);
  lora_init();

  factory_display.init();
  factory_display.clear();
  factory_display.display();
  //logo();
  delay(300);
  factory_display.clear();

  chipid=ESP.getEfuseMac();//The chip ID is essentially its MAC address(length: 6 bytes).
  Serial.printf("ESP32ChipID=%04X",(uint16_t)(chipid>>32));//print High 2 bytes
  Serial.printf("%08X\n",(uint32_t)chipid);//print Low 4bytes.



 //########################Le arquivos gravados             
  //ja foi iniciado no telegram
  //initSPIFFS();
  
  // Load values saved in SPIFFS
  ssid = readFile(SPIFFS, ssidPath);
  pass = readFile(SPIFFS, passPath);
  nomedobot = readFile(SPIFFS, nomedobotPath);
  tokentelegram = readFile (SPIFFS, tokentelegramPath);



  Serial.println(ssid);
  Serial.println(pass);
  Serial.println(tokentelegram);

 if(initWiFi()) {

   delay(200);
//client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
   client.setTimeout(2000);

   client.setInsecure();

delay(500);

delay(200);
       

    // Route for root / web page
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
      request->send(SPIFFS, "/index.html", "text/html", false);
    });
    server.serveStatic("/", SPIFFS, "/");
  //  server.on("/flashcam", HTTP_GET, flashcam);



}
  else {
    // Connect to Wi-Fi network with SSID and password
    Serial.println("Setting AP (Access Point)");
    // NULL sets an open Access Point
    WiFi.softAP("MushLORA-config", NULL);

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





  
//############################################ Configura LOOP TELEGRAM
 
 //xStack = (uint8_t*)heap_caps_calloc(1, 5000, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT | MALLOC_CAP_32BIT);
//pxStackBuffer = (uint8_t*)heap_caps_calloc(1, 5000, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT | MALLOC_CAP_32BIT);
  delay(200);


     // Create the task pinned to core 0 without using any dynamic memory allocation.
    //      xHandle = xTaskCreateStatic(
     xHandle = xTaskCreateStaticPinnedToCore(
                   TELE,       // Function that implements the task.
                   "TELE",          // Text name for the task.
                   STACK_SIZE,      // Stack size in bytes, not words.
                   ( void * ) 1,    // Parameter passed into the task.   ( void * ) 1
                   1,// Priority at which the task is created.
                   xStack,          // Array to use as the task's stack.
                   &xTaskBuffer,    // Variable to hold the task's data structure.
                   1 );     
   
   
   }



void loop()
{
	if(state == STATE_TX)
	{
    delay(1000);
		txNumber += 0.01;
    counter++;


    String msglora = "Contador:";
           msglora += counter;   
           msglora += "\n";
           msglora += "RSSI:";      
		       msglora += Rssi;  
           msglora += "\n";
           msglora += "Pacote Recebido:";      
           msglora += rxNumber;  
sprintf(txpacket,msglora.c_str(),txNumber,Rssi,rxNumber);  //start a package

    factory_display.clear();


factory_display.drawString(0, 0, "Enviando pacote:");
    factory_display.drawString(0, 10, msglora);



    
  //      factory_display.drawString(0, 50, Rssi);

    factory_display.display();
    delay(10);
    factory_display.clear();
   
		Serial.printf("\r\nsending packet \"%s\" , length %d\r\n",txpacket, strlen(txpacket));

		Radio.Send( (uint8_t *)txpacket, strlen(txpacket) ); //send the package out	
        //delay(4000);

 //RX


    

  //  lora_idle = false;

	}

 //RX


 if(state == STATE_RX){ 
 
      Serial.println("into RX mode");
      Radio.Rx( 0 );
      RadioEvents.RxDone = OnRxDone;

      delay(4000);
      state=STATE_TX;

 }

 
 
  Radio.IrqProcess( );
}



void OnRxDone( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr )
{
  rxNumber++;
  Rssi=rssi;
  receiveflag = true;
  
    Serial.println("RXDONE");

      
    rxSize=size;
    memcpy(rxpacket, payload, size );
    rxpacket[size]='\0';
   // Radio.Sleep( );
    factory_display.clear();
    factory_display.drawString(0, 0, "Recebendo pacote:");
    factory_display.drawString(0, 10, rxpacket);
    factory_display.display();
    delay(2000);
    
    Serial.printf("\r\nreceived packet \"%s\" with rssi %d , length %d\r\n",rxpacket,rssi,rxSize);
    state=STATE_TX;

  
}
