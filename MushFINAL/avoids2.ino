


void connect()//Funçao para Conectar ao wifi e verificar à conexao.
{
   if (WiFi.status() != WL_CONNECTED)//Caso nao esteja conectado ao WiFi, Ira conectarse
   {
   Serial.print("Task CONNECT running on core ");
   Serial.println(xPortGetCoreID());
   
    WiFi.begin(ssid.c_str(), pass.c_str());
    //WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
     delay(400);
    client.setInsecure();
     delay(400);
    client.setHandshakeTimeout(120000);
   // client.setTimeout(3000);

    delay(10);
   }
}

//#################################################################PING WDT

void pingando(){

   Serial.print("Task PING running on core ");
   Serial.println(xPortGetCoreID());

    if(Ping.ping("google.com", 1)) {
      ConnectedCounter++;
 Serial.println("Ping pong OK contando....");
    Serial.print(Ping.averageTime());
    Serial.println(" ms");
    Serial.printf("\n%lu: Remaining free mem: %u\n", millis(), ESP.getFreeHeap());

      }

else{ 
      notConnectedCounter++;
          Serial.println("Falha do ping da internet, contando...");
          Serial.println(notConnectedCounter);

    if(notConnectedCounter > 1000) { // Reset a placa apos 50 erros    
    Serial.println("Reiniciando esp por falha na internet");
    
          ESP.restart();
     
   }
   }

}

//##############VOID TELEGRAM



void tele(void* pvParameters){
readTel();
            vTaskDelay(5000/ portTICK_RATE_MS);


}





  void readTel(){

  



   Serial.print("Task TELE running on core ");
   Serial.println(xPortGetCoreID());

  
   int newmsg = bot.getUpdates(bot.last_message_received + 1);
//   int msgtxt = bot.getUpdates(bot.last_message_received);
 //  String msgtele = bot.messages[1].text;

   for (int i = 0; i < newmsg; i++)//Caso haja X mensagens novas, fara este loop X Vezes.
   {
      id = bot.messages[i].chat_id;//Armazenara o ID do Usuario à Váriavel.
      text = bot.messages[i].text;//Armazenara o TEXTO do Usuario à Váriavel.
      //text.toUpperCase();//Converte a STRING_TEXT inteiramente em Maiuscúla.
      from_name = bot.messages[i].from_name;
      //bot.messages[i].type == "channel_post";

    Serial.printf("\nPeguei essa mensagem %s\n", text);

   
    if (from_name == "") from_name = "Cade o nick?";

    String hi = nomedobot.c_str(); 
    hi += ":";
    hi += "Vc disse isso mesmo?";
    hi += "\n";
    hi += text;
    hi += "\n";
    hi += "Vou executar!";
      
    bot.sendMessage(id, hi, "Markdown");


  
  
      if (text.indexOf("ledon") > -1)//Caso o texto recebido contenha "ON"
      {
         ledon();
         ledState = "ledon";
         bot.sendMessage(id, ledState, "");//Envia uma Mensagem para a pessoa que enviou o Comando.
      }
    if (text.indexOf("flash") > -1)//Caso o texto recebido contenha "ON"
      {        
  if(ledStateCAM == "flash"){
         Serial.println("FLASHOFF");
         bot.sendMessage(id, ledStateCAM, "");//Envia uma Mensagem para a pessoa que enviou o Comando.
     
         ledStateCAM = "flashoff";
        }
        else{
           Serial.println("FLASH");
      bot.sendMessage(id, ledStateCAM, "");//Envia uma Mensagem para a pessoa que enviou o Comando.
          ledStateCAM = "flash";}         
         }

      else if (text.indexOf("ledoff") > -1)//Caso o texto recebido contenha "OFF"
      {
      //  NeoFade(100);
        ledoff();
        ledState = "ledoff";
        bot.sendMessage(id, ledState, "");//Envia uma Mensagem para a pessoa que enviou o Comando.
      }
      else if (text.indexOf("clima") > -1)//Caso o texto recebido contenha "START"
      {
//          strobe(0, 0, 0xff, 20, 30, 10);
   ledState = "clima";
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
          msg += "PPM,"; 

          bot.sendMessage(id, msg, "");
          
       //   addFile(SPIFFS, climaPath, msg.c_str());

     }

              else if (text.indexOf("btc") > -1)//Caso o texto recebido contenha "OFF"
      {
        ledState = "btc";
         EEPROM.writeString(1, btc());
        
        bot.sendMessage(id,btc(), "");//Envia uma Mensagem para a pessoa que enviou o Comando.
      }  
      else if (text.indexOf("ltc") > -1)//Caso o texto recebido contenha "OFF"
      {
        ledState = "ltc";
         EEPROM.writeString(1, ltc());
        
        bot.sendMessage(id,ltc(), "");//Envia uma Mensagem para a pessoa que enviou o Comando.
      }  
//######Comando telegram
      else if (text.indexOf("xmr") > -1)//Caso o texto recebido contenha "OFF"
      {
       ledState = "xmr";
         EEPROM.writeString(1, xmr());
        
        bot.sendMessage(id,xmr(), "");//Envia uma Mensagem para a pessoa que enviou o Comando.
      }     
      
//######Comando telegram
      else if (text.indexOf("eth") > -1)//Caso o texto recebido contenha "OFF"
      {
       ledState = "eth";
         EEPROM.writeString(1, eth());
        
        bot.sendMessage(id,eth(), "");//Envia uma Mensagem para a pessoa que enviou o Comando.
      }     
      
//######Comando telegram
      else if (text.indexOf("doge") > -1)//Caso o texto recebido contenha "OFF"
      {
       ledState = "doge";
         EEPROM.writeString(1, doge());
        
        bot.sendMessage(id,eth(), "");//Envia uma Mensagem para a pessoa que enviou o Comando.
      }     
      
      //######Comando telegram
      else if (text.indexOf("clock") > -1)//Caso o texto recebido contenha "OFF"
      {
        ledState = "clock";
        
        bot.sendMessage(id,timeClient.getFormattedTime(), "");//Envia uma Mensagem para a pessoa que enviou o Comando.
      }     
      else if (text.indexOf("rainbow") > -1)//Caso o texto recebido contenha "OFF"
      {
        ledState = "rainbowon";
        
        bot.sendMessage(id,timeClient.getFormattedTime(), "");//Envia uma Mensagem para a pessoa que enviou o Comando.
      }

            else if (text.indexOf("telegram") > -1)//Caso o texto recebido contenha "OFF"
      {
        ledState = "telegram";
        bot.sendMessage(id, "Monitorando grupo do telegram, envie msg com / para aparecer na luminaria", "");//Envia uma Mensagem para a pessoa que enviou o Comando.
      }


    for (int j = 0; j < 2; j++) {
    camera_fb_t * newfb = esp_camera_fb_get();
    if (!newfb) {
      Serial.println("Camera Capture Failed");
    } else {
      //Serial.print("Pic, len="); Serial.print(newfb->len);
      //Serial.printf(", new fb %X\n", (long)newfb->buf);
      esp_camera_fb_return(newfb);
      delay(30);
    }
  }
  
     

      if (text.indexOf("/foto") > -1){

      fb = NULL;


    //   digitalWrite(FLASH_LED_PIN, HIGH);

      fb = esp_camera_fb_get();
      esp_camera_fb_return(fb);
      delay(100);

      // Take Picture with Camera
      fb = esp_camera_fb_get();
      if (!fb) {
        Serial.println("Camera capture failed");
        bot.sendMessage(id, "Camera capture failed", "");
        return;
      }

      // digitalWrite(FLASH_LED_PIN, LOW);

      currentByte = 0;
      fb_length = fb->len;
      fb_buffer = fb->buf;

      Serial.println("\n>>>>> Sending, bytes=  " + String(fb_length));

        bot.sendPhotoByBinary(id, "image/jpeg", fb_length,
                              isMoreDataAvailable, getNextByte,
                              nullptr, nullptr);

        dataAvailable = true;

        Serial.println("done!");
      
      esp_camera_fb_return(fb);
     }


                  else if (text.indexOf("sd") > -1)//Caso o texto recebido contenha "OFF"
      {
            takeNewPhoto = true;
           delay(300);

//        sendfoto();
        bot.sendMessage(id, "foto sd", "");//Envia uma Mensagem para a pessoa que enviou o Comando.
      }


 else if (text.indexOf("vga") > -1)//Caso o texto recebido contenha "OFF"
      {
 fb = NULL;

      sensor_t * s = esp_camera_sensor_get();
      s->set_framesize(s, FRAMESIZE_HD);

      Serial.println("\n\n\nSending VGA");

      // Take Picture with Camera
      fb = esp_camera_fb_get();
      if (!fb) {
        Serial.println("Camera capture failed");
        bot.sendMessage(id, "Camera capture failed", "");
        return;
      }

      currentByte = 0;
      fb_length = fb->len;
      fb_buffer = fb->buf;

      Serial.println("\n>>>>> Sending as 512 byte blocks, with jzdelay of 0, bytes=  " + String(fb_length));

      bot.sendPhotoByBinary(id, "image/jpeg", fb_length,
                            isMoreDataAvailable, getNextByte,
                            nullptr, nullptr);

      esp_camera_fb_return(fb);


      

      }




       else if (text.indexOf("clip") > -1)//Caso o texto recebido contenha "OFF"
      {


  avi_enabled = true;

      // record the video
      bot.longPoll =  0;
      xTaskCreate(the_camera_loop,"FOTO", 20000, NULL, 0, NULL);     

     // xTaskCreatePinnedToCore( the_camera_loop, "the_camera_loop", 10000, NULL, 1, &the_camera_loop_task, 1);
    //  xTaskCreatePinnedToCore( the_camera_loop, "the_camera_loop", 10000, NULL, 1, &the_camera_loop_task, 0);  //v8.5

      if ( the_camera_loop == NULL ) {
        //vTaskDelete( xHandle );
        Serial.printf("do_the_steaming_task failed to start! %d\n", the_camera_loop);
      }
        avi_enabled = false;

    }

    else if (text.indexOf("/video") > -1){
 //     xTaskCreate(the_camera_loop,"FOTO", 20000, NULL, 0, NULL);     

   // video_ready = true;
        
      }
      
           else if (text.indexOf("restart") > -1)//Caso o texto recebido contenha "OFF"
      {
        bot.sendMessage(id, "reiniciando esp", "");//Envia uma Mensagem para a pessoa que enviou o Comando.
        delay(500);
        ESP.restart();
      }

      
           else if (text.indexOf("deletalog") > -1)//Caso o texto recebido contenha "OFF"
      {
        bot.sendMessage(id, "deletando log", "");//Envia uma Mensagem para a pessoa que enviou o Comando.
        delay(500);
        writeFile(SPIFFS, climaPath, "");
        writeFile(SPIFFS, loggerPath, "");
        
      }

      

       else if (text.indexOf("status") > -1)//Caso o texto recebido contenha "OFF"
      {
    welcome = "Nome do bot:\n";
    welcome +=  nomedobot.c_str();
    welcome += "\n";
    welcome += "MAC endereço:\n";
    welcome += WiFi.macAddress();
    welcome += "\n";
    welcome += "CORE:";    
    welcome += xPortGetCoreID();
    welcome += "\n";
    welcome += "SSID:";    
    welcome += ssid.c_str();
    welcome += "\n";    
    welcome += "Tempo ping:";    
Ping.ping("google.com", 1);
    welcome +=  Ping.averageTime();
    welcome += "ms";
    welcome += "\n";
    welcome += "MMC Size:";
    welcome += SD_MMC.totalBytes();
    welcome += "\n"; 
    welcome += "MMC USADOSize:";
    welcome += SD_MMC.usedBytes();
    welcome += "\n";
    welcome += "HEAP Size:";
    welcome += ESP.getHeapSize();  
    welcome += "\nRemaining free mem:";
    welcome += ESP.getFreeHeap();
    welcome += "\n";
    welcome += "PSRAM Size:";
    welcome += ESP.getPsramSize();  
    welcome += "\nFree PSRAM:";
    welcome += ESP.getFreePsram();
    welcome += "\n";
    welcome += "Millis: ";
    welcome += millis();
    welcome += "\n";
    welcome += "Nivel de sinal WIFI: ";
    welcome += WiFi.RSSI();
    welcome += "\n";
    welcome += "IP Local: ";
    welcome += WiFi.localIP().toString();
    welcome += "\n";
    welcome += "Numero de Falhas na internet: ";
    welcome += notConnectedCounter;
    welcome += "\n";
    welcome += "Estado LED: ";
    welcome += ledState;

   
    

      bot.sendMessage(id, welcome, "Markdown");      
      }
          
      else if(text.indexOf("start") > -1)//Caso o texto recebido contenha "START"
      {
      if (from_name == "")
      from_name = "Guest";

      welcome = "Bem vindo, " + from_name + ".\n";
      welcome += "Essa eh a MushLight\n\n";
      welcome += "Agora se chama\n";
      welcome +=  nomedobot.c_str();
      welcome += "\n";
      welcome += "/foto : Tira uma foto\n";
      welcome += "/btc : Mostra o preco do btc \n";
      welcome += "/ltc : Mostra o preco do ltc \n";
      welcome += "/clock : Mostra o relogio na MushlLght \n";
      welcome += "/vermelho : Para ligar o LED \n";
      welcome += "/verde : Para ligar o LED verde\n";
      welcome += "/rainbow : Para ligar o LED \n";
      welcome += "/clima : Para verificar temperatura, humidade e pressao\n";
      welcome += "/telegram : Passa as msgs dos comandos do telegram\n";
      welcome += "/ledon: Liga o LED \n";
      welcome += "/ledoff: Para desligar o LED\n";
      welcome += "/status : STATUS\n";
      welcome += "/start : Abre esse menu\n";
      welcome += "/restart : Reinicia o Controlador\n";
      welcome += "/deletalog : Deleta o log\n";
      welcome += "Acesse o ip http://";
      welcome +=  WiFi.localIP().toString(); 
      welcome += "\n";
      welcome += "Para mais controles e vizualizar o historico dos sensores\n";
      welcome += "\n";
      welcome += "Codigo fonte em https://github.com/santocyber/MushLight\n";
     

      bot.sendMessage(id, welcome, "Markdown");      
      }

   }
//delay(200);
client.flush();
client.stop();
//  vTaskDelete(NULL);
       //   vTaskDelete(teletask);
   // vTaskSuspend(NULL);

}
