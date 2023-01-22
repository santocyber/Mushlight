//boolean dataAvailable = false;

// Keep track of number of pictures
unsigned int pictureNumber = 0;
String path = "";
String lastPhoto = "";
String cam;


void camera(void * parameter){
//capturePhotoSaveSpiffs();
    vTaskDelete(NULL);
}



/*


//tAKE A PICTURIII


// Check if photo capture was successful
bool checkPhoto( fs::FS &fs ) {
  File f_pic = fs.open( FILE_PHOTO );
  unsigned int pic_sz = f_pic.size();
  return ( pic_sz > 100 );

}
 

// Capture Photo and Save it to SPIFFS
void capturePhotoSaveSpiffs() {

  camera_fb_t * fb = NULL; // pointer
  bool ok = 0; // Boolean indicating if the picture has been taken correctly

  do {
    // Take a photo with the camera
    Serial.println("Taking a photo...");

//digitalWrite(LED_BUILTIN, LOW);
Serial.println("LED turned ON");
fill_solid( leds, NUM_LEDS, 0xFFFFFF);
FastLED.show(); // display this frame




    fb = esp_camera_fb_get();
    esp_camera_fb_return(fb);    
    delay(100);
    fb = esp_camera_fb_get();
    


//digitalWrite(LED_BUILTIN, HIGH);
Serial.println("LED turned OFF");
fill_solid( leds, NUM_LEDS, 0x000000);
FastLED.show(); // display this frame


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
 // Serial.println("Deleting the camera task");
  delay(100);
 //   vTaskDelete(Task5);

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
      Serial.println("Deleting the camera task");
  delay(100);
  // vTaskDelete(Task5);

}








//////Send camera to telegram
 camera_fb_t * fb = NULL; // pointer


bool isMoreDataAvailablex();
byte *getNextBuffer();
int getNextBufferLen();

bool dataAvailable = false;
void camtelegram( void ){
  //camera_fb_t *fb = NULL;

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



*/
