

#if (USEPIR ==1)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// setup some interupts during reboot
//
//  int read13 = digitalRead(13); -- pir for video


static void setupinterrupts() {

  pinMode(PIRpin, INPUT_PULLDOWN) ; //INPUT_PULLDOWN);

  Serial.print("Setup PIRpin = ");
  for (int i = 0; i < 5; i++) {
    Serial.print( digitalRead(PIRpin) ); Serial.print(", ");
  }
 
  
  Serial.println(" ");
 //install gpio isr service
    gpio_install_isr_service(0);
    
  esp_err_t err = gpio_isr_handler_add((gpio_num_t)PIRpin, &PIR_ISR, NULL);

  if (err != ESP_OK) Serial.printf("gpio_isr_handler_add failed (%x)", err);
  gpio_set_intr_type((gpio_num_t)PIRpin, GPIO_INTR_POSEDGE); 


}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//  PIR_ISR - interupt handler for PIR  - starts or extends a video
//
static void IRAM_ATTR PIR_ISR(void* arg) {

  int PIRstatus = digitalRead(PIRpin) + digitalRead(PIRpin) + digitalRead(PIRpin) ;
  if (PIRstatus == 3) {
    Serial.print("PIR Interupt>> "); Serial.println(PIRstatus);
      active_interupt = false;


    if (!active_interupt && pir_enabled) {
      
   Serial.print("Task PIR running on core ");
   Serial.println(xPortGetCoreID());
      active_interupt = true;
      Serial.print("PIR Interupt ... start recording ... ");

     // xTaskCreate(the_camera_loop,"FOTO", 20000, NULL, 0, NULL);     
   //   xTaskCreatePinnedToCore( the_camera_loop, "the_camera_loop", 20000, NULL, 1, &the_camera_loop_task, 1);

      //xTaskCreatePinnedToCore( the_camera_loop, "the_camera_loop", 20000, NULL, 1, &the_camera_loop_task, 1);
      //xTaskCreatePinnedToCore( the_camera_loop, "the_camera_loop", 10000, NULL, 1, &the_camera_loop_task, 0);  //v8.5
      xTaskCreatePinnedToCore(foto, "fototask", 20000, NULL, 1, NULL, 1);
  //    xTaskCreatePinnedToCore(savesd, "sdtask", 20000, NULL, 1, &savesdtask, 1);


      if ( the_camera_loop == NULL ) {
     ///\\\\\\\\\\\\\\\\\\\\\\\\\\\/\\\\    Serial.printf("do_the_steaming_task failed to start! %d\n", the_camera_loop);
      }

    }
  }
}


#endif

#if (BLEX == 1)

void BLUETASK( void * pvParameters ){

    configASSERT( ( uint32_t ) pvParameters == 1UL );
    
  // Block for 500ms.
const TickType_t xDelay = 5000 / portTICK_PERIOD_MS;

  for( ;; )
  {
      // Simply toggle the LED every 500ms, blocking between each toggle.
    bluetoothvoid();
    vTaskDelay( xDelay );
  }

  }





void bluetoothvoid(){
 Serial.println("1- Download and install an BLE scanner app in your phone");
  Serial.println("2- Scan for BLE devices in the app");
  Serial.println("3- Connect to MyESP32");
  Serial.println("4- Go to CUSTOM CHARACTERISTIC in CUSTOM SERVICE and write something");
  Serial.println("5- See the magic =)");

  BLEDevice::init("MushLight::mirako.org");
  BLEServer *pServer = BLEDevice::createServer();

  BLEService *pService = pServer->createService(SERVICE_UUID);

  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE |
                                         BLECharacteristic::PROPERTY_NOTIFY
                                       );

  pCharacteristic->setCallbacks(new MyCallbacks());
    pCharacteristic->addDescriptor(new BLE2902());

  pCharacteristic->setValue("Hello World");
  pService->start();

   BLEAdvertising *pAdvertising = pServer->getAdvertising();  // this still is working for backward compatibility
  //BLEAdvertising *pAdvertising = pServer->getAdvertising();
   BLEBeacon myBeacon;
  myBeacon.setManufacturerId(0x4c00);
  myBeacon.setMajor(5);
  myBeacon.setMinor(88);
  myBeacon.setSignalPower(0xc5);
  myBeacon.setProximityUUID(BLEUUID(BEACON_UUID_REV));

  BLEAdvertisementData advertisementData;
  advertisementData.setFlags(0x1A);
  advertisementData.setManufacturerData(myBeacon.getData());
  pAdvertising->setAdvertisementData(advertisementData);

  pAdvertising->start();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  
  Serial.println("Characteristic defined! Now you can read it in your phone!");


  }


#endif

void verifica1(void * parameter){
   verifica();
     Serial.print("Task VERIFICA running on core ");
   Serial.println(xPortGetCoreID());
    vTaskDelete(NULL);
}




void toque(){


  //if (digitalRead(touchpin) == HIGH){ //SE A LEITURA DO PINO FOR IGUAL A HIGH, FAZ
  
  //digitalWrite(pinoLed,HIGH); //liga O LED onboard
     Serial.print("Task TOQUE running on core ");
     Serial.println(xPortGetCoreID());

         xTaskCreatePinnedToCore(savesd, "sdtask", 20000, NULL, 1, &savesdtask, 1);


ledStateCAM = "flash";
delay(300);
ledStateCAM = "flashoff";



if (led_state) {
touchcounter++;
led_state = false;
  Serial.print("Contador de toque: ");
     Serial.println(touchcounter);
                
if(touchcounter > 11){ touchcounter = 0;}
        
if (touchcounter == 0)
  { 
    ledState = "matrixon";
    }

if (touchcounter == 1)
  {

   
   ledState = "ip";
   
    welcome = "ME TOCARAM!!!\n";
    welcome += "Nome do bot:\n";
    welcome +=  nomedobot.c_str();
    welcome += "\n";
    welcome += "MAC endereço:\n";
    welcome += WiFi.macAddress();
    welcome += "\n";
    welcome += "Nivel de sinal WIFI: ";
    welcome += WiFi.RSSI();
    welcome += "\n";
    welcome += "IP Local: ";
    welcome += WiFi.localIP().toString();
    welcome += "\n";
    bot.sendMessage(id, welcome, "Markdown"); 

   
  }
  if (touchcounter == 2)
  { 
    ledState = "green";
    }
   if (touchcounter == 3)
  { 
      ledState = "balls";
    }
   if (touchcounter == 4)
  { 
      ledState = "tetris";
    }
   if (touchcounter == 5)
  { 
      ledState = "rainbow2";
    }
  if (touchcounter == 6)
  { 
    ledState = "zebra";

    }  
 if (touchcounter == 7)
  { 
 ledState = "clock";
}
 if (touchcounter == 8)
  { 
 ledState = "ltc";
}
if (touchcounter == 9)
  { 
 ledState = "blue";
}
 if (touchcounter == 10)
  { 
 ledState = "fire2";
}
if (touchcounter == 11)
  { 
    ledState = "gif6";
    }
if (touchcounter == 12)
  { 
    ledState = "pride";
    }
  
  }
  else{ //SENÃO, FAZ
   
        led_state = true;
        ledState = "ledoff";

      Serial.println("Toque off");
      delay(1000);
     
     
     } 
  
   // vTaskDelay(1000 / portTICK_PERIOD_MS);
  //  Serial.println("Deleta toque task");
  delay(500);
     //vTaskDelete(toquetask);
  
  

  }

  


void verifica(){
time_t now = time(nullptr);
 time_now = String(ctime(&now)).substring(0,24);
//addFile(SPIFFS, loggerPath, msg.c_str());

DynamicJsonDocument root(200);
  //JsonObject root = jsonBuffer.as<JsonObject>();
//JsonArray& arr = jb.createArray();
  root["Hora"] = time_now;
  root["Temperatura"] = readDHTTemperature();
  root["Umidade"] = readDHTHumidity();
  root["Pressao"] = readDHTPressao();
  root["CO2"] = readCO2();
 
 String output;
    
  output = ",";
serializeJson(root, output);
     output += "";

 

appendFileSD(SD_MMC, climaPath, output.c_str());
//bot.sendMessage(id, output, "");


}





void verifica2(){
  
time_t now = time(nullptr);
 time_now = String(ctime(&now)).substring(0,24);
//addFile(SPIFFS, loggerPath, msg.c_str());

DynamicJsonDocument root(200);
  //JsonObject root = jsonBuffer.as<JsonObject>();
//JsonArray& arr = jb.createArray();

  root["Temperatura"] = readDHTTemperature();
  root["Umidade"] = readDHTHumidity();
  root["Pressao"] = readDHTPressao();
  root["CO2"] = readCO2();
  root["Hora"] = time_now;


 
 String output;
    
  output = ",";
serializeJson(root, output);
     output += "";

 

 //addFile(SPIFFS, loggerPath, output.c_str());
appendFileSD(SD_MMC, loggerPath, output.c_str());



 //##Obrigado santocyber por essa gambiarra aqui manda um pix rastanerdi@gmail.com , acabei usando tambem JSON.stringfy no proprio javascript
//return output;  

}



    
    
    
    
    void StartTime(){






      
  // Note: The ESP8266 Time Zone does not function e.g. ,0,"time.nist.gov"
  configTime(TZone * 3600, 0, "pool.ntp.org", "time.nist.gov");
  // Change this line to suit your time zone, e.g. USA EST configTime(-5 * 3600, 0, "pool.ntp.org", "time.nist.gov");
  // Change this line to suit your time zone, e.g. AUS configTime(8 * 3600, 0, "pool.ntp.org", "time.nist.gov");
  Serial.println(F("\nWaiting for time"));
  time(nullptr);
  
  Serial.println("Time set");

    }

#if (SENSORVIBRA == 1)

// Sound sensor code
void readVibra(){
   valorvibra = digitalRead(vibrapin);
//Serial.println(valorvibra);
//Serial.println("||");
//Serial.println(analogRead(vibra));


 if (valorvibra == 0) {
  
    clap_counter++;
 if (clap_counter > 0) { 
    
    
     if (led_state) {
      led_state = false;
      color_counter++;// LED was on, now off
      if(color_counter > 10){ color_counter = 0;}
      changeColor();
      //clap_counter = 0;
  //    sound_value = 0;
  Serial.println("Clap on");
  Serial.println(color_counter);

    delay(2000);

      
   }
    else {
        led_state = true;
        ledoff();
        ledState = "ledoff";

      Serial.println("Clap off");
      delay(1000);
    }}
  delay(500);
 
  Serial.println(valorvibra);
  Serial.println("||");  
  Serial.println(color_counter);
  Serial.println("||");
  Serial.println(clap_counter);

    }}


    void changeColor(){
   
  bot.sendMessage(id, "Alguem bateu em mim, acendendo a luz.", "");//Envia uma Mensagem para a pessoa que enviou o Comando.
  Serial.println("Luz acionada vibrando");

//muda de cor
  if (color_counter == 1)
  {
    
   ledState = "ip";
  }
  if (color_counter == 2)
  { 
    ledState = "green";
    }
   if (color_counter == 3)
  { 
      ledState = "balls";
    }
   if (color_counter == 4)
  { 
      ledState = "tetris";
    }
   if (color_counter == 5)
  { 
      ledState = "rainbow2";
    }
  if (color_counter == 6)
  { 
    ledState = "zebra";

    }  
 if (color_counter == 7)
  { 
 ledState = "clock";
}
 if (color_counter == 8)
  { 
 ledState = "ltc";
}
if (color_counter == 9)
  { 
 ledState = "blue";
}
 if (color_counter == 10)
  { 
 ledState = "fire2";
}

delay(1000);
  //sendPhotoTelegram();
  takeNewPhoto = true; 

}



#endif










String crono(){
String segundos;
cSegundos++;
segundos = cSegundos / 100;
delay(1000);
return segundos;
  
}





String shiba(){
//const String site = "https://min-api.cryptocompare.com/data/pricemultifull?fsyms=ETH&tsyms=USD&e=Coinbase";
const String site = "https://www.bitstamp.net/api/v2/ticker/shibusd";
    http.begin(site);
     int httpCode = http.GET(); 
     Serial.println(site);                                                                                                                                                                                                                                              //Get crypto price from API

DynamicJsonDocument doc4(2000);
    deserializeJson(doc4, http.getString());
    //  JsonObject obj = doc4.as<JsonObject>();

    
        Serial.print("HTTP Status Code: ");
        Serial.println(httpCode);

        String SHIBAUSDPrice = doc4["last"].as<String>();
               SHIBAUSDPrice += ":24h:";
               SHIBAUSDPrice += doc4["percent_change_24"].as<String>();
               
        http.end();
    
        Serial.print("SHIBAUSD Price: ");
 
        Serial.println(SHIBAUSDPrice.toDouble());
  
        
        http.end();               
        return SHIBAUSDPrice;
        http.end();  
        delay(5000);                                   
}







String ltc(){
//const String site = "https://min-api.cryptocompare.com/data/pricemultifull?fsyms=ETH&tsyms=USD&e=Coinbase";
const String site = "https://www.bitstamp.net/api/v2/ticker/ltcusd";
       http.begin(site);
     int httpCode = http.GET(); 
     Serial.println(site);                                                                                                                                                                                                                                              //Get crypto price from API

DynamicJsonDocument doc3(400);
    deserializeJson(doc3, http.getString());
   //   JsonObject obj = doc3.as<JsonObject>();

    
        Serial.print("HTTP Status Code: ");
        Serial.println(httpCode);
        String LTCUSDPrice = doc3["last"].as<String>();
               LTCUSDPrice += ":24h:";
               LTCUSDPrice += doc3["percent_change_24"].as<String>();
               

        http.end();
    
        Serial.print("LTCUSD Price: ");
 
        Serial.println(LTCUSDPrice.toDouble());
        Serial.println(LTCUSDPrice);
        
        http.end();               
 return LTCUSDPrice;
        delay(5000);                                   
}










String eth(){
//const String site = "https://min-api.cryptocompare.com/data/pricemultifull?fsyms=ETH&tsyms=USD&e=Coinbase";
const String site = "https://www.bitstamp.net/api/v2/ticker/ethusd";
   
     http.begin(site);
     int httpCode = http.GET(); 
     Serial.println(site);                                                                                                                                                                                                                                              //Get crypto price from API

DynamicJsonDocument doc2(2000);
    deserializeJson(doc2, http.getString());
      JsonObject obj = doc2.as<JsonObject>();

    
        Serial.print("HTTP Status Code: ");
        Serial.println(httpCode);
        String ETHUSDPrice = obj["last"].as<String>();
               ETHUSDPrice += ":24h:";
               ETHUSDPrice += obj["percent_change_24"].as<String>();
               
        http.end();
    
        Serial.print("ETHUSD Price: ");
Serial.println(ETHUSDPrice.toDouble());
  
        
        http.end();               
        return ETHUSDPrice;
        http.end();  
        delay(5000);                                   
}



    




String getSensorReadings(){
DynamicJsonDocument jsonBuffer(2000);

//####Serializacao dos sensores OK
  jsonBuffer["Temperatura"] = readDHTTemperature();
  jsonBuffer["Umidade"] = readDHTHumidity();
  jsonBuffer["Pressao"] = readDHTPressao();
  jsonBuffer["CO2"] = readCO2();

 String output2; 
 serializeJson(jsonBuffer, output2);
 return output2;  
 
}


void configureEvents() {
  events.onConnect([](AsyncEventSourceClient *client){
    if(client->lastId()){
      Serial.printf("Client connections. Id: %u\n", client->lastId());
    }
    // and set reconnect delay to 1 second
    client->send("Ola da MushLight , um alo para o SantoCyber tambem",NULL,millis(),1000);
  });
  server.addHandler(&events);
}



String btc(){

const String url = "http://api.coindesk.com/v1/bpi/currentprice/BTC.json";

    
        http.begin(url);
        int httpCode = http.GET();                                                                                                                                                                                                                                                //Get crypto price from API
        StaticJsonDocument<2000> doc;
        DeserializationError error = deserializeJson(doc, http.getString());
    
        if (error)                                                                                                                                                                                                                                                                                                                //Display error message if unsuccessful
        {
                Serial.print(F("deserializeJson Failed"));
                Serial.println(error.f_str());
                delay(2500);
//                return;
        }
    
        Serial.print("HTTP Status Code: ");
        Serial.println(httpCode);
    
        String BTCUSDPrice = doc["bpi"]["USD"]["rate_float"].as<String>(); 
  
        http.end();
    
        Serial.print("BTCUSD Price: ");                                                       //Display current price on serial monitor
        Serial.println(BTCUSDPrice.toDouble());

        http.end();               
        return String(BTCUSDPrice.toDouble());
        delay(5000);                                   //Sleep for 15 minutes
}

String doge(){

const String url = "https://api.bitfinex.com/v1/pubticker/doge:usd";


  //Create Json Document



  
  //Put data here
  //Serialize it
  //char * buffer = (char *) ps_malloc (20000 * sizeof (char));
  //size_t n = serializeJson(doc,buffer,20000);

    
        http.begin(url);
        int httpCode = http.GET();                                                                                                                                                                                                                                                //Get crypto price from API
        SpiRamJsonDocument doc (200);
        DeserializationError error = deserializeJson(doc, http.getString());
    
        if (error)                                                                                                                                                                                                                                                                                                                //Display error message if unsuccessful
        {
                Serial.print(F("deserializeJson Failed"));
                Serial.println(error.f_str());
                delay(2500);
//                return;
        }
    
        Serial.print("HTTP Status Code: ");
        Serial.println(httpCode);
    
        String DOGEPrice = doc["last_price"].as<String>(); 
  
        http.end();
    
        Serial.print("Doge Price: ");                                                       //Display current price on serial monitor
        Serial.println(DOGEPrice.toDouble());

        http.end();               
        return DOGEPrice;
        delay(5000);                                   //Sleep for 15 minutes
}

String xmr(){

const String url = "https://api.bitfinex.com/v1/pubticker/xmrusd";

    
        http.begin(url);
        int httpCode = http.GET();                                                                                                                                                                                                                                                //Get crypto price from API
        StaticJsonDocument<400> doc;
        DeserializationError error = deserializeJson(doc, http.getString());
    
        if (error)                                                                                                                                                                                                                                                                                                                //Display error message if unsuccessful
        {
                Serial.print(F("deserializeJson Failed"));
                Serial.println(error.f_str());
                delay(2500);
//                return;
        }
    
        Serial.print("HTTP Status Code: ");
        Serial.println(httpCode);
    
        String XMRPrice = doc["last_price"].as<String>(); 
  
        http.end();
    
        Serial.print("XMR Price: ");                                                       //Display current price on serial monitor
        Serial.println(XMRPrice.toDouble());

        http.end();  
        return String(XMRPrice.toDouble());
        delay(5000);                                   //Sleep for 15 minutes
}

String dolar(){

const String url = "https://economia.awesomeapi.com.br/json/last/USD";

    
        http.begin(url);
        int httpCode = http.GET();                                                                                                                                                                                                                                                //Get crypto price from API
        StaticJsonDocument<2000> doc;
        DeserializationError error = deserializeJson(doc, http.getString());
    
        if (error)                                                                                                                                                                                                                                                                                                                //Display error message if unsuccessful
        {
                Serial.print(F("deserializeJson Failed"));
                Serial.println(error.f_str());
                delay(2500);
//                return;
        }
    
        Serial.print("HTTP Status Code: ");
        Serial.println(httpCode);
    
        String dolarPrice = doc["USDBRL"]["bid"].as<String>(); 
               dolarPrice += ":24h:";
               dolarPrice += doc["pctChange"].as<String>();
                
  
        http.end();
    
        Serial.print("Dolar Price: ");                                                       //Display current price on serial monitor
        Serial.println(dolarPrice.toDouble());

        http.end();               
        return String(dolarPrice.toDouble());
        delay(5000);                                   //Sleep for 15 minutes
}
