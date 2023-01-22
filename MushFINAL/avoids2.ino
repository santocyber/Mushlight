

void tele(void * parameter){
    readTel();
    vTaskDelete(NULL);
}


void connect()//Funçao para Conectar ao wifi e verificar à conexao.
{
   if (WiFi.status() != WL_CONNECTED)//Caso nao esteja conectado ao WiFi, Ira conectarse
   {
    WiFi.begin(ssid.c_str(), pass.c_str());
    //WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    client.setHandshakeTimeout(120000);

    delay(10);
   }
}




void pingando(){

   Serial.print("Task2 running on core ");
   Serial.println(xPortGetCoreID());


//#################################################################PING WDT
    if(Ping.ping("google.com", 1)) {
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


void readTel()//Funçao que faz a leitura do Telegram.
{  


   Serial.print("Task1 running on core ");
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
         bot.sendMessage(id, "LED ON", "");//Envia uma Mensagem para a pessoa que enviou o Comando.
      }

      else if (text.indexOf("ledoff") > -1)//Caso o texto recebido contenha "OFF"
      {
      //  NeoFade(100);
        ledoff();
        bot.sendMessage(id, "LED OFF", "");//Envia uma Mensagem para a pessoa que enviou o Comando.
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

            else if (text.indexOf("foto") > -1)//Caso o texto recebido contenha "OFF"
      {
       // camtelegram();
       
        bot.sendMessage(id, "Sorria!!", "");//Envia uma Mensagem para a pessoa que enviou o Comando.
      }
                  else if (text.indexOf("sd") > -1)//Caso o texto recebido contenha "OFF"
      {
  //      cam = "on";
  //      sendPhotoTelegram();
       

//        sendfoto();
        bot.sendMessage(id, "foto sd", "");//Envia uma Mensagem para a pessoa que enviou o Comando.
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


}
