//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// setup some interupts during reboot
//
//  int read13 = digitalRead(13); -- pir for video

int PIRpin = 13;

static void setupinterrupts() {

  pinMode(PIRpin, INPUT_PULLDOWN) ; //INPUT_PULLDOWN);

  Serial.print("Setup PIRpin = ");
  for (int i = 0; i < 5; i++) {
    Serial.print( digitalRead(PIRpin) ); Serial.print(", ");
  }
  Serial.println(" ");

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


    if (!active_interupt && pir_enabled) {
      active_interupt = true;
      digitalWrite(33, HIGH);
      Serial.print("PIR Interupt ... start recording ... ");

     // xTaskCreate(the_camera_loop,"FOTO", 20000, NULL, 0, NULL);     

      xTaskCreatePinnedToCore( the_camera_loop, "the_camera_loop", 20000, NULL, 1, &the_camera_loop_task, 1);
      //xTaskCreatePinnedToCore( the_camera_loop, "the_camera_loop", 10000, NULL, 1, &the_camera_loop_task, 0);  //v8.5
      xTaskCreatePinnedToCore(foto, "fototask", 20000, NULL, 1, NULL, 1);
      xTaskCreatePinnedToCore(savesd, "sdtask", 20000, NULL, 1, &savesdtask, 1);


      if ( the_camera_loop == NULL ) {
     ///\\\\\\\\\\\\\\\\\\\\\\\\\\\/\\\\    Serial.printf("do_the_steaming_task failed to start! %d\n", the_camera_loop);
      }

    }
  }
}









void verifica(){

    //   takeNewPhoto = true;
    //   send_the_picture(); 
      
        time_t now = time(nullptr);
          time_now = String(ctime(&now)).substring(0,24);

          String msg = nomedobot.c_str(); 
          msg += ",";          
          msg += "\n";
          msg += "Hora:"; 
          msg += time_now;
          msg += ",";
          msg += "\n";
          msg += "Temperatura:";
          msg += msg.concat(readDHTTemperature());
          msg += "C,";
          msg += "\n";
          msg += "Umidade:";
          msg += msg.concat(readDHTHumidity());
          msg += "%,"; 
          msg += "\n";
          msg += "Pressao:";
          msg += msg.concat(readDHTPressao());
          msg += " Pa,"; 
          msg += "\n";
          msg += "CO2:";
          msg += msg.concat(readCO2());
          msg += " PPM,"; 
          msg += "\n";
          bot.sendMessage(id, msg, "");
          
          addFile(SPIFFS, climaPath, msg.c_str());

 
          
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



// Sound sensor code
void readVibra(){
   valorvibra = digitalRead(vibra);
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
  //send_the_picture(); 
  takeNewPhoto = true; 

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

 

 addFile(SPIFFS, loggerPath, output.c_str());


 //##Obrigado santocyber por essa gambiarra aqui manda um pix rastanerdi@gmail.com , acabei usando tambem JSON.stringfy no proprio javascript
//return output;  

}











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

DynamicJsonDocument doc3(2000);
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
