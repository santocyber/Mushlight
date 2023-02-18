


void ifs(){

  

uint32_t corvar = strtoul(cor.c_str(), NULL, 16);


    if (State == "sleep") {     
          temposleep1 = millis();

    if (temposleep1 - temposleep0 > settemposleep)//Faz a verificaçao das funçoes a cada 30min
   {
    ledState = "ledoff";
    FastLED.show(); // display this frame
     State = "acordado";     
   }
   }

if (ledStateCAM == "flash") {

    flash();
    }
    if (ledStateCAM == "flashoff") {

    flashoff();
    }

if (ledState == "ledoff") {

    ledoff();
        FastLED.show(); // display this frame
    }
      if (ledState == "pisca") {
    pisca();
    FastLED.show(); // display this frame
    }      
    if (ledState == "piscalento") {
    piscalento();
    FastLED.show(); // display this frame
    } 
  if (ledState == "rainbowon") {
    rainbowon();
    FastLED.delay(2000 / strtoul(velocidade.c_str(), NULL, 16));
    } 
       if (ledState == "rainbow2") {
    rainbowRoutine();
    FastLED.show(); // display this frame
    FastLED.delay(2000 / strtoul(velocidade.c_str(), NULL, 16));
    }
  
  if (ledState == "matrixon") {
    matrixRoutine();
    FastLED.show(); // display this frame
    FastLED.delay(2000 / strtoul(velocidade.c_str(), NULL, 16));
} 
  
  if (ledState == "pride") {
    pride();
     FastLED.show();
    } 

if (ledState == "green") {

           fill_solid( leds, NUM_LEDS, CRGB::Green);

       
        FastLED.show(); // display this frame
    }
    if (ledState == "blue") {
    blue();
        FastLED.show(); // display this frame
    }
        if (ledState == "red") {
    red();
        FastLED.show(); // display this frame
    }
 if (ledState == "amarelo") {
 
    fill_solid( leds, NUM_LEDS, 0xFFFF00);
        FastLED.show(); // display this frame
    }

    
 if (ledState == "colors") {
      // colorsRoutine();
// drawLetter(1, "o", 16, 0xF0F8FF);
    //  drawLetter(uint8_t index, uint8_t letter, int16_t offset, uint32_t color)
        FastLED.show(); // display this frame
    }

if (ledState == "ledon") {


    fill_solid( leds, NUM_LEDS, corvar);
//FastLED.setBrightness(dimmervar);
        FastLED.show(); // display this frame
    
    }

    if (ledState == "fire2") {
    fireRoutine();
    FastLED.show(); // display this frame
    }

    if (ledState == "snowon") {
    snowRoutine();
    FastLED.delay(2000 / strtoul(velocidade.c_str(), NULL, 16));
    FastLED.show(); // display this frame         
    }
    
    if (ledState == "temp") {
    fillString(readDHTTemperature() + "C", 0x0000FF);
        FastLED.show(); // display this frame
    }
                 if (ledState == "co2") {
    fillString(readCO2()+ "PPM", 0x00FFFF);
        FastLED.show(); // display this frame
    }

                 if (ledState == "humi") {
    fillString(readDHTHumidity() + "%", 0xFFFF00);
        FastLED.show(); // display this frame
    }
        if (ledState == "pressao") {
    fillString(readDHTPressao(), 0xFF6347);
    FastLED.show(); // display this frame
    }
    if (ledState == "clima") {
    fillString("||T" + readDHTTemperature() + "C||CO2" + readCO2()+ "PPM||ATM" + readDHTPressao() + "Pa ||Umid" + readDHTHumidity() + "%", 1);
    FastLED.show(); // display this frame
    }
    
    if (ledState == "cronometro") {
   // cronometro();
    fillString(crono(), corvar);

    FastLED.show(); // display this frame
    //FastLED.setBrightness(dimmervar);
    }
    if (ledState == "clock") {
//FastLED.clear();
   timeClient.update();

    fillString(timeClient.getFormattedTime(), corvar);
    FastLED.show(); // display this frame
    //FastLED.setBrightness(dimmervar);
    }
        if (ledState == "dia") {

 
    fillString(timeClient.getFormattedTime(), corvar);
    FastLED.show(); // display this frame
    //FastLED.setBrightness(dimmervar);
    }
        if (ledState == "data") {
time_t now = time(nullptr);
 time_now = String(ctime(&now)).substring(0,24);
    fillString(time_now, corvar);
    FastLED.show(); // display this frame
    //FastLED.setBrightness(dimmervar);
    }
 
 

   if (ledState == "zebra") {
    zebraNoise();
    FastLED.show(); // display this frame
    FastLED.delay(2000 / strtoul(velocidade.c_str(), NULL, 16));
    }
    
   if (ledState == "plasma") {
    plasmaNoise();
    FastLED.show(); // display this frame
    FastLED.delay(2000 / strtoul(velocidade.c_str(), NULL, 16));
       }
       if (ledState == "gif") {
animation(1);
        FastLED.show(); // display this frame
    }
     if (ledState == "gif2") {
animation(2);
        FastLED.show(); // display this frame
    }
     if (ledState == "gif3") {
animation(3);
        FastLED.show(); // display this frame
    }

     if (ledState == "gif4") {
animation(4);
        FastLED.show(); // display this frame
    }
         if (ledState == "gif5") {
animation(5);
        FastLED.show(); // display this frame
    }
      if (ledState == "gif6") {
animation(6);
        FastLED.show(); // display this frame
    }
         if (ledState == "gif7") {
animation(7);
        FastLED.show(); // display this frame
    }
         if (ledState == "gif8") {
animation(8);
        FastLED.show(); // display this frame
    }
             if (ledState == "gif9") {
animation(9);
        FastLED.show(); // display this frame
    }
           if (ledState == "oceano") {
oceanNoise();
        FastLED.show(); // display this frame
    }

               if (ledState == "forest") {
    forestNoise();
    FastLED.show(); // display this frame
    FastLED.delay(2000 / strtoul(velocidade.c_str(), NULL, 16));

    }
                   if (ledState == "madness") {
    madnessNoise();
    FastLED.show(); // display this frame
    FastLED.delay(2000 / strtoul(velocidade.c_str(), NULL, 16));
    }
                   if (ledState == "lava") {
    lavaNoise();
    FastLED.show(); // display this frame
    FastLED.delay(2000 / strtoul(velocidade.c_str(), NULL, 16));

    }
                   if (ledState == "balls") {
    lightBallsRoutine();
    FastLED.show(); // display this frame
    FastLED.delay(2000 / strtoul(velocidade.c_str(), NULL, 16));

    }
                   if (ledState == "swirl") {
     swirlRoutine();
     FastLED.show(); // display this frame
     FastLED.delay(2000 / strtoul(velocidade.c_str(), NULL, 16));

    }
                   if (ledState == "tetris") {
    tetrisRoutine();
    FastLED.show(); // display this frame
    }
                   if (ledState == "snake") {
    snakeRoutine();
    FastLED.show(); // display this frame
    }
                       if (ledState == "arkanoid") {
    arkanoidRoutine();
    FastLED.show(); // display this frame
    } 
    if (ledState == "runningtext") {
    fillString(txt,corvar);
    FastLED.show(); // display this frame
  //  xTaskCreate(fast,"FAST LED", 1000, NULL, 1, NULL );     

    
    }
    if (ledState == "telegram") {
    fillString(text,2);
    FastLED.show(); // display this frame
    }
        if (ledState == "btc") {

  if (millis() - tempo5 > 60000)//Faz a verificaçao das funçoes a cada 30min
   {
      EEPROM.writeString(1, btc());
      tempo5 = millis();
   }
    fillString(EEPROM.readString(1),corvar);
    FastLED.show(); // display this frame
    FastLED.delay(2000 / strtoul(velocidade.c_str(), NULL, 16));

    }
            if (ledState == "eth") {
    if (millis() - tempo5 > 60000)//Faz a verificaçao das funçoes a cada 30min
   {
      EEPROM.writeString(2, eth());
      tempo5 = millis();
     
   }
    fillString(EEPROM.readString(2),corvar);
    FastLED.show(); // display this frame
    FastLED.delay(2000 / strtoul(velocidade.c_str(), NULL, 16));
 }

  if (ledState == "ltc") {
           
  if (millis() - tempo5 > 60000)//Faz a verificaçao das funçoes a cada 30min
   {
      EEPROM.writeString(3, ltc());
      tempo5 = millis();
     
   }
   fillString("LTC: " + EEPROM.readString(3),corvar);
    FastLED.show(); // display this frame
}
             if (ledState == "shiba") {
    if (millis() - tempo5 > 60000)//Faz a verificaçao das funçoes a cada 30min
   {
      EEPROM.writeString(4, shiba());
      tempo5 = millis();
     
   }
   fillString(EEPROM.readString(4),corvar);
    FastLED.show(); // display this frame
    FastLED.delay(2000 / strtoul(velocidade.c_str(), NULL, 16));
    } 
                if (ledState == "xmr") {
    if (millis() - tempo5 > 60000)//Faz a verificaçao das funçoes a cada 30min
   {
      EEPROM.writeString(5, xmr());
      tempo5 = millis();
     
   }
   fillString(EEPROM.readString(5),corvar);
    FastLED.show(); // display this frame
    FastLED.delay(2000 / strtoul(velocidade.c_str(), NULL, 16));
    } 
                if (ledState == "doge") {
    if (millis() - tempo5 > 60000)//Faz a verificaçao das funçoes a cada 30min
   {
      EEPROM.writeString(6, doge());
      tempo5 = millis();
     
   }
   fillString(EEPROM.readString(6),corvar);
    FastLED.show(); // display this frame
    FastLED.delay(2000 / strtoul(velocidade.c_str(), NULL, 16));
    } 
                if (ledState == "dolar") {
    if (millis() - tempo5 > 60000)//Faz a verificaçao das funçoes a cada 30min
   {
      EEPROM.writeString(7, dolar());
      tempo5 = millis();
     
   }
   fillString(EEPROM.readString(7),corvar);
    FastLED.show(); // display this frame
    FastLED.delay(2000 / strtoul(velocidade.c_str(), NULL, 16));
    } 
        if (ledState == "spark") {
    sparklesRoutine();
    FastLED.delay(2000 / strtoul(velocidade.c_str(), NULL, 16));
    }
       if (ledState == "star") {                
   starfallRoutine();
   FastLED.delay(2000 / strtoul(velocidade.c_str(), NULL, 16));
   FastLED.show(); // display this frame
    }
        if (ledState == "pacifica") {
    pacifica_loop();
    FastLED.show(); // display this frame
    FastLED.delay(2000 / strtoul(velocidade.c_str(), NULL, 16));
    }
        if (ledState == "spiral") {
    ballRoutine();
    FastLED.show(); // display this frame
    FastLED.delay(2000 / strtoul(velocidade.c_str(), NULL, 16));
}
        if (ledState == "rainbowstrip") {
    rainbowStripeNoise();
    FastLED.delay(2000 / strtoul(velocidade.c_str(), NULL, 16));
   }
    
        if (ledState == "light") {
   lightersRoutine();
   FastLED.show(); // display this frame
   FastLED.delay(2000 / strtoul(velocidade.c_str(), NULL, 16));
  }
           if (ledState == "bpm") {
    bpm();
    FastLED.show(); // display this frame
    FastLED.delay(2000 / strtoul(velocidade.c_str(), NULL, 16));

   }
   if (ledState == "sinelon") {
    sinelon();
    FastLED.show(); // display this frame
    FastLED.delay(2000 / strtoul(velocidade.c_str(), NULL, 16));
   }
   if (ledState == "confetti") {
    confetti();
    FastLED.show(); // display this frame
    FastLED.delay(2000 / strtoul(velocidade.c_str(), NULL, 16));

   }
      if (ledState == "juggle") {
 juggle();
  FastLED.show(); // display this frame
             FastLED.delay(2000 / strtoul(velocidade.c_str(), NULL, 16));

   }
   if (ledState == "rainbowgliter") {
 rainbowWithGlitter();
  FastLED.show(); // display this frame
            FastLED.delay(2000 / strtoul(velocidade.c_str(), NULL, 16));

   }
   
        if (ledState == "runner") {
 runnerRoutine();
  FastLED.show(); // display this frame
 //FastLED.delay(2000 / veloframe);
   }
           if (ledState == "newone") {
 newone();
  FastLED.show(); // display this frame
            FastLED.delay(2000 / strtoul(velocidade.c_str(), NULL, 16));
   }
              if (ledState == "spiralone") {
   spiralone();
  FastLED.show(); // display this frame
            FastLED.delay(2000 / strtoul(velocidade.c_str(), NULL, 16));
   }
                 if (ledState == "spiraltwo") {
   spiraltwo();
  FastLED.show(); // display this frame
            FastLED.delay(2000 / strtoul(velocidade.c_str(), NULL, 16));
   }

              if (ledState == "papainoel") {
  draw(); 

    FastLED.show(); // display this frame

                if (millis() - tempo10 > 30000)//Faz a verificaçao das funçoes a cada 30min
   {

  draw();

   if (eff >= 3) {
      eff = 0;
    } else {
      eff += 1;
    }
    
  //static int frame = 0;
  //if (frame++ % 32 == 0)
  //  Serial.println(FastLED.getFPS());
  FastLED.show(); // display this frame

    tempo10 = millis();
     
   }

  }

              if (ledState == "dots") {
   dots();
  FastLED.show(); // display this frame
            FastLED.delay(2000 / strtoul(velocidade.c_str(), NULL, 16));
   }

    if (ledState == "credito") {
    fillString("Obrigado @SantoCyber faz um pix rastanerdi@gmail.com",1);
    FastLED.show(); // display this frame
    }

    if (ledState == "ip") {
    fillString(WiFi.localIP().toString(), 0x0000FF);
        FastLED.show(); // display this frame
    }    


}
