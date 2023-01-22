

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



void readTel() {
  //Serial.println("handleNewMessages");
  //Serial.println(String(numNewMessages));

  Serial.print("Task Tele running on core ");
   Serial.println(xPortGetCoreID());
  
   int newmsg = bot.getUpdates(bot.last_message_received + 1);
//   int msgtxt = bot.getUpdates(bot.last_message_received);
 //  String msgtele = bot.messages[1].text;

   for (int i = 0; i < newmsg; i++)//Caso haja X mensagens novas, fara este loop X Vezes.
   {
    
    chat_id = String(bot.messages[i].chat_id);
    String text = bot.messages[i].text;
    String from_name = bot.messages[i].from_name;
 
    Serial.printf("\nPeguei a mensagem %s\n", text);

   

   
    if (from_name == "") from_name = "Cade o nick?";

    String hi = nomedobot.c_str(); 
    hi += ":";
    hi += "Vc disse isso mesmo?";
    hi += "\n";
    hi += text;
    hi += "\n";
    hi += "Vou executar!";
      
    bot.sendMessage(chat_id, hi, "Markdown");




    if (text.indexOf("/flash") > -1)//Caso o texto recebido contenha  blablabla      
    {
      flashState = !flashState;
      digitalWrite(FLASH_LED_PIN, flashState);
    }

    else if (text.indexOf("/status") > -1)//Caso o texto recebido contenha  blablabla      
    {
      String stat = "Device: " + devstr + "\nVer: " + String(vernum) + "\nRssi: " + String(WiFi.RSSI()) + "\nip: " +  WiFi.localIP().toString() + "\nEnabled: " + pir_enabled + "\nAvi Enabled: " + avi_enabled;
      if (frame_interval == 0) {
        stat = stat + "\nFast 3 sec";
      } else if (frame_interval == 125) {
        stat = stat + "\nMed 10 sec";
      } else {
        stat = stat + "\nSlow 40 sec";
      }
      stat = stat + "\nQuality: " + quality;

      bot.sendMessage(chat_id, stat, "");
    }

    else if (text.indexOf("/reboot") > -1)//Caso o texto recebido contenha  blablabla  
    {
      reboot_request = true;
    }

    else if (text.indexOf("/enable") > -1)//Caso o texto recebido contenha  blablabla       
    {
      pir_enabled = true;
    }

    else if (text.indexOf("/disable") > -1){
      pir_enabled = false;
    }

    else if (text.indexOf("/enavi") > -1){
      avi_enabled = true;
    }

    else if (text.indexOf("/disavi") > -1){
      avi_enabled = false;
    }

    else if (text.indexOf("/fast") > -1){
      max_frames = 150;
      frame_interval = 0;
      speed_up_factor = 0.5;
      pir_enabled = true;
      avi_enabled = true;
    }

    else if (text.indexOf("/med") > -1){
      max_frames = 150;
      frame_interval = 125;
      speed_up_factor = 1;
      pir_enabled = true;
      avi_enabled = true;
    }

    else if (text.indexOf("/slow") > -1){
      max_frames = 150;
      frame_interval = 500;
      speed_up_factor = 5;
      pir_enabled = true;
      avi_enabled = true;
    }

    /*
    if (fb) {
      esp_camera_fb_return(fb);
      Serial.println("Return an fb ???");
      if (fb) {
        esp_camera_fb_return(fb);
        Serial.println("Return another fb ?");
      }
    }
    */


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


       digitalWrite(FLASH_LED_PIN, HIGH);

      fb = esp_camera_fb_get();
      esp_camera_fb_return(fb);
      delay(100);

      // Take Picture with Camera
      fb = esp_camera_fb_get();
      if (!fb) {
        Serial.println("Camera capture failed");
        bot.sendMessage(chat_id, "Camera capture failed", "");
        return;
      }

       digitalWrite(FLASH_LED_PIN, LOW);

      currentByte = 0;
      fb_length = fb->len;
      fb_buffer = fb->buf;

      Serial.println("\n>>>>> Sending, bytes=  " + String(fb_length));

        bot.sendPhotoByBinary(chat_id, "image/jpeg", fb_length,
                              isMoreDataAvailable, getNextByte,
                              nullptr, nullptr);

        dataAvailable = true;

        Serial.println("done!");
      
      esp_camera_fb_return(fb);
     }



      

      else if (text.indexOf("/caption") > -1){

      fb = NULL;

      // Take Picture with Camera
      fb = esp_camera_fb_get();
      if (!fb) {
        Serial.println("Camera capture failed");
        bot.sendMessage(chat_id, "Camera capture failed", "");
        return;
      }

      currentByte = 0;
      fb_length = fb->len;
      fb_buffer = fb->buf;

        
        Serial.println("\n>>>>> Sending with a caption, bytes=  " + String(fb_length));

        String sent = bot.sendMultipartFormDataToTelegramWithCaption("sendPhoto", "photo", "img.jpg",
                      "image/jpeg", "Your photo", chat_id, fb_length,
                      isMoreDataAvailable, getNextByte, nullptr, nullptr);

        Serial.println("done!");

      } 

      

  
    

    else if (text.indexOf("/vga" ) > -1){

      fb = NULL;

      sensor_t * s = esp_camera_sensor_get();
      s->set_framesize(s, FRAMESIZE_HD);

      Serial.println("\n\n\nSending VGA");

      // Take Picture with Camera
      fb = esp_camera_fb_get();
      if (!fb) {
        Serial.println("Camera capture failed");
        bot.sendMessage(chat_id, "Camera capture failed", "");
        return;
      }

      currentByte = 0;
      fb_length = fb->len;
      fb_buffer = fb->buf;

      Serial.println("\n>>>>> Sending as 512 byte blocks, with jzdelay of 0, bytes=  " + String(fb_length));

      bot.sendPhotoByBinary(chat_id, "image/jpeg", fb_length,
                            isMoreDataAvailable, getNextByte,
                            nullptr, nullptr);

      esp_camera_fb_return(fb);
    }


    else if (text.indexOf("/clip") > -1){
      
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

    else if (text.indexOf("/server") > -1){

    if (camserver == "on"){
      camserver = "off";
 //stopCameraServer();
  
    bot.sendMessage(chat_id, "Servidor off", "Markdown");  
 
      
    }else{
camserver = "on";

    String welcome = "MushCam bot.\n\n";
      
    welcome += "Acesse o ip http://";
    welcome +=  WiFi.localIP().toString(); 
    welcome += "\n";
    welcome += "servidor local,Sorria!";
    bot.sendMessage(chat_id, welcome, "Markdown");  

 bot.sendMessage(chat_id, "Servidor on", "Markdown");  
     
     
client.flush();
//client.stop();
 //startCameraServer();
   
      
      
    }
    
    

      
    }

    

    else if (text.indexOf("/start") > -1){
      String welcome = "MushCam bot.\n\n";
      welcome += "/foto: take a picture\n";
      welcome += "/flash: toggle flash LED\n";
      welcome += "/caption: photo with caption\n";
      welcome += "/clip: short video clip\n";
      welcome += "\n Configure the clip\n";
      welcome += "/enable: enable pir\n";
      welcome += "/disable: disable pir\n";
      welcome += "/enavi: enable avi\n";
      welcome += "/disavi: disable avi\n";
      welcome += "\n/fast: 25 fps - 3  sec - play .5x speed\n";
      welcome += "/med: 8  fps - 10 sec - play 1x speed\n";
      welcome += "/slow: 2  fps - 40 sec - play 5x speed\n";
      welcome += "\n Acesse localmente\n";
      welcome += "/server: Server on Server off \n";
      welcome += "Acesse o ip http://";
      welcome +=  WiFi.localIP().toString(); 
      welcome += "\n/status: status\n";
      welcome += "/reboot: reboot\n";
      welcome += "/start: start\n";
      bot.sendMessage(chat_id, welcome, "Markdown");
    }
  }
}
