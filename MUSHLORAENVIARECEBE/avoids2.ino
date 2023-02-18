


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
    client.setHandshakeTimeout(1200000);
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

    if(notConnectedCounter > 50) { // Reset a placa apos 50 erros    
    Serial.println("Reiniciando esp por falha na internet");
    
          ESP.restart();
     
   }
   }

}

//##############VOID TELEGRAM

void TELE(void*p){
//portVALID_STACK_MEM(pxStackBuffer); 
    
    configASSERT( ( uint32_t ) p == 1UL );
    
  // Block for 500ms.
const TickType_t xDelay = 10000 / portTICK_PERIOD_MS;

  for( ;; )
  {
      // Simply toggle the LED every 500ms, blocking between each toggle.
  readTel();
      vTaskDelay( xDelay );
  }

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
    welcome += "Numero de PING na internet: ";
    welcome += ConnectedCounter;
    welcome += "\n";


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
      welcome += "/caption : Tira uma foto com legenda\n";
      welcome += "/video : Grava um mini video clip\n";
      welcome += "/flash : Liga e desliga o flash\n";
      welcome += "/pir : Ativa o sensor de presença\n";
      welcome += "/timelapse : Liga e desliga o timelapse\n";
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
//client.flush();
//client.stop();
esp_get_free_heap_size();

//  vTaskDelete(NULL);
       //   vTaskDelete(teletask);
   // vTaskSuspend(NULL);

}
