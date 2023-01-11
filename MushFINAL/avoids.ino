    void StartTime(){
  // Note: The ESP8266 Time Zone does not function e.g. ,0,"time.nist.gov"
  configTime(TZone * 3600, 0, "pool.ntp.org", "time.nist.gov");
  // Change this line to suit your time zone, e.g. USA EST configTime(-5 * 3600, 0, "pool.ntp.org", "time.nist.gov");
  // Change this line to suit your time zone, e.g. AUS configTime(8 * 3600, 0, "pool.ntp.org", "time.nist.gov");
  Serial.println(F("\nWaiting for time"));
  time(nullptr);
  
  Serial.println("Time set");

    }





//tAKE A PICTURIII


// Check if photo capture was successful
bool checkPhoto( fs::FS &fs ) {
  File f_pic = fs.open( FILE_PHOTO );
  unsigned int pic_sz = f_pic.size();
  return ( pic_sz > 100 );

}
 

// Capture Photo and Save it to SPIFFS
void capturePhotoSaveSpiffs( void ) {
  //camera_fb_t * fb = NULL; // pointer
  bool ok = 0; // Boolean indicating if the picture has been taken correctly

  do {
    // Take a photo with the camera
    Serial.println("Taking a photo...");

//digitalWrite(LED_BUILTIN, LOW);
Serial.println("LED turned ON");


fb = esp_camera_fb_get();
delay(500);

//digitalWrite(LED_BUILTIN, HIGH);
Serial.println("LED turned OFF");
    if (!fb) {
      Serial.println("Camera capture failed");
      return;
    }

    // Photo file name
    Serial.printf("Picture file name: %s\n", FILE_PHOTO);
    File file = SPIFFS.open(FILE_PHOTO, FILE_WRITE);

    // Insert the data in the photo file
    if (!file) {
      Serial.println("Failed to open file in writing mode");
    }
    else {
      file.write(fb->buf, fb->len); // payload (image), payload length
      Serial.print("The picture has been saved in ");
      Serial.print(FILE_PHOTO);
      Serial.print(" - Size: ");
      Serial.print(file.size());
      Serial.println(" bytes");
    }
    // Close the file
    file.close();
    delay(300);
    esp_camera_fb_return(fb);

    // check if file has been correctly saved in SPIFFS
    ok = checkPhoto(SPIFFS);
  } while ( !ok );
}



//################Send foto telegram
bool isMoreDataAvailable();
byte getNextByte();
File filey = SPIFFS.open(FILE_PHOTO, "r");

//################Send foto telegram
////////////////////////////////  manda foto usando SPIFFS

bool isMoreDataAvailable()
{
  return filey.available();
}

byte getNextByte()
{  return  filey.read();
}
///////////////////////////////




void  sendPhotoTelegram()
{
 
         File filey = SPIFFS.open(FILE_PHOTO, "r");

    int newmsgX = bot.getUpdates(bot.last_message_received );
    idX = bot.messages[0].chat_id;//Armazenara o ID do Usuario à Váriavel.



 if (filey)
  {
    Serial.println(FILE_PHOTO);
    Serial.println("....");
    Serial.println(filey.read());
    Serial.println(filey.size());
    
    

    //Content type for PNG image/png
 String sent =     bot.sendPhotoByBinary(idX, "image/jpeg", filey.size(),
                                        isMoreDataAvailable,
                                        getNextByte, nullptr, nullptr);

  if (sent)
    {
      Serial.println("foto enviada ao telegram");
        bot.sendMessage(id, "Take a picture", ""); 
         filey.close();

    }
    else
    {
      Serial.println("n enviado");
    }


  }
  else
  {
    // if the file didn't open, print an error:
    Serial.println("error opening photo");
                bot.sendMessage(id, "n fique triste tente denovo erro ao acessar o arquivo", "");      
 }

      
    filey.close();
      Serial.println("done funcao , ultima foto enviada do SPIFFS");
 
}








//////Send camera to telegram


bool isMoreDataAvailablex();
byte *getNextBuffer();
int getNextBufferLen();

bool dataAvailable = false;
void camtelegram( void ){
  camera_fb_t * fb = NULL; // pointer

      // Take Picture with Camera

      fb = esp_camera_fb_get();


      if (!fb)
      {
        Serial.println("Camera capture failed");
        bot.sendMessage(id, "Camera capture failed", "");
        return;
      }
      dataAvailable = true;
      Serial.println("Sending cam");
      bot.sendPhotoByBinary(id, "image/jpeg", fb->len,
                            isMoreDataAvailablex, nullptr,
                            getNextBuffer, getNextBufferLen);

      Serial.println("mais nada com a cam!");

      esp_camera_fb_return(fb);


}


bool isMoreDataAvailablex()
{
  if (dataAvailable)
  {
    dataAvailable = false;
    return true;
  }
  else
  {
    return false;
  }
}

byte *getNextBuffer()
{ 
  if (fb)
  {
    return fb->buf;
  }
  else
  {
    return nullptr;
  }
}

int getNextBufferLen()
{
  if (fb)
  {
    return fb->len;
  }
  else
  {
    return 0;
  }
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
  //sendPhotoTelegram();
  takeNewPhoto = true; 

}




void verifica(){
       takeNewPhoto = true;
    //   sendPhotoTelegram();
      
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







String verifica2(){
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
return output;  
}








void connect()//Funçao para Conectar ao wifi e verificar à conexao.
{
   if (WiFi.status() != WL_CONNECTED)//Caso nao esteja conectado ao WiFi, Ira conectarse
   {
    WiFi.begin(ssid.c_str(), pass.c_str());
    //WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    delay(10);
   }
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
