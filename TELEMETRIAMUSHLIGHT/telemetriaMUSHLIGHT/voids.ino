
//########################################TELEMETRIA
     void telemetria(){
     client.setCACert(caCert);
     HTTPClient https;

  String horaAtual = obterHoraAtual();
    Serial.print("Hora atual:");
    Serial.println(horaAtual);
          
           String geoSemEspacos = "";
    // Remover espaços em branco da string 'geo'
for (int i = 0; i < geo.length(); i++) {
    if (geo[i] != ' ') {
        geoSemEspacos += geo[i];
    }
}

    String CHAVE2PHP =   url;
           CHAVE2PHP += "?&mac_value=";
           CHAVE2PHP +=  WiFi.macAddress();
           CHAVE2PHP += "&iplocal=";
           CHAVE2PHP +=  WiFi.localIP().toString();
           CHAVE2PHP += "&wifilocal=";
           CHAVE2PHP +=  WiFi.SSID();       
           CHAVE2PHP += "&timelocal=";
           CHAVE2PHP +=  horaAtual;
           CHAVE2PHP += "&nomedobot=";
           nomedobot.trim();
           CHAVE2PHP += nomedobot;
           CHAVE2PHP += "&geo=";
           geoSemEspacos.trim();
           CHAVE2PHP += geoSemEspacos;



           Serial.print("||T" + readDHTTemperature() + "PPM||ATM" + readDHTPressao() + "Pa ||Umid" + readDHTHumidity() + "%");

  neopixelWrite(RGB_BUILTIN,0,RGB_BRIGHTNESS,0); // Green

  if (https.begin(CHAVE2PHP)) 
  {

 Serial.println("[HTTPS] GET...\n");
        // start connection and send HTTP header
        int httpCode = https.GET();
        Serial.println(CHAVE2PHP);

  
        // httpCode will be negative on error
        if (httpCode > 0) {
          // HTTP header has been send and Server response header has been handled
            Serial.printf("[HTTPS] GET... code: %d\n", httpCode);
            String payload = https.getString();
            Serial.println(payload);
            }
            

  neopixelWrite(RGB_BUILTIN,0,0,RGB_BRIGHTNESS); // Blue

  delay(500);
 
  Serial.println("");
  Serial.println(F("Desconectando."));
  https.end();

  digitalWrite(RGB_BUILTIN, LOW);
  
  } 
   else 
    {
    Serial.println(F("connection failed")); 
    Serial.println();
    neopixelWrite(RGB_BUILTIN,RGB_BRIGHTNESS,0,0); // Red
    }


}


//###############################################TIRA FOTO ENVIA

void capturaimagem() {
       client.setCACert(caCert);

    // Obtém o frame buffer da câmera
    camera_fb_t* fb = esp_camera_fb_get();

    if (!fb) {
        Serial.println("Erro ao capturar a imagem");
        return; // Saia da função se a captura da imagem falhar
    }

    // Certifique-se de que o valor do MAC seja fornecido corretamente
    String urlx = url8 + "?mac=" + WiFi.macAddress();
    
    // Inicialize uma conexão HTTPClient
    HTTPClient http;
    http.begin(urlx);

    // Configurar o cabeçalho Content-Type para indicar que você está enviando dados binários (imagem)
    http.addHeader("Content-Type", "image/jpeg");

    // Envia os dados binários da imagem para o servidor PHP
    int httpResponseCode = http.POST(fb->buf, fb->len);

    if (httpResponseCode > 0) {
        Serial.printf("[HTTP] POST request done with status code %d\n", httpResponseCode);
        // Obtenha a resposta do servidor como uma string
        String response = http.getString();
        Serial.println("Resposta do servidor: " + response);
    } else {
        Serial.printf("[HTTP] POST request failed, error: %s\n", http.errorToString(httpResponseCode).c_str());
    }

    http.end();
    esp_camera_fb_return(fb);
}








//##########################################VERIFICA FUNCOES PENDENTES NO SQL

void verifyActionAndExecute() {
       client.setCACert(caCert);

  // Get the MAC address from the ESP32
  //uint8_t mac[6];
  //WiFi.macAddress(mac);
  // Convert the MAC address to a string
  //String macStr = String(mac[0], HEX) + ":" + String(mac[1], HEX) + ":" + String(mac[2], HEX) + ":" + String(mac[3], HEX) + ":" + String(mac[4], HEX) + ":" + String(mac[5], HEX);

  // Make a HTTP request to the database to get the action information
  HTTPClient httpClient;
  httpClient.begin(url6 + "?mac=" + WiFi.macAddress());
  int httpCode = httpClient.GET();

  // Check the HTTP status code
  if (httpCode == 200) {
    // The action was found
    String payload = httpClient.getString();
    Serial.println(payload);

    // Parse the JSON payload to get the action information
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, payload);

    String action = doc["acoes"];
    
    // Execute the action
    if (action == "ligado") {
      Serial.println("LIGADO PELO SQL");
      digitalWrite(relayPin, HIGH);


      
      neopixelWrite(RGB_BUILTIN,0,RGB_BRIGHTNESS,0); // Green
    } else if (action == "desligado") {
      Serial.println("DESLIGADO PELO SQL");
      
      neopixelWrite(RGB_BUILTIN, 0, 0, 0);
      digitalWrite(relayPin, LOW);
      
    } else if (action == "mensagem") {
      Serial.println("MENSAGEM PELO SQL");
     funcaolesql();
     
    neopixelWrite(RGB_BUILTIN,RGB_BRIGHTNESS,0,0); // Red
    delay(400);
    neopixelWrite(RGB_BUILTIN, 0, 0, 0);

       
    } else if (action == "rainbow") {
      Serial.println("ARCOIRIS PELO SQL");

     
      neopixelWrite(RGB_BUILTIN, 255, 255, 127); // Yellow    

    } else {
      Serial.println("Unknown action: " + action);
      Serial.println(WiFi.macAddress());

    }
  } else {
    // The action was not found
    Serial.println("Action not found: " + WiFi.macAddress());
  }

  httpClient.end();
}





//#######################################################

void funcaolesql() {
  //client.setCACert(caCert);

  HTTPClient http;

  // Fazer uma solicitação GET para a API PHP
  http.begin(url7 + "?mac=" + WiFi.macAddress());

  int httpResponseCode = http.GET();
  if (httpResponseCode == HTTP_CODE_OK) {
    String payload = http.getString();
    payload.trim();
    Serial.println("Resposta da API: " + payload);

    // Parse the JSON payload to get the action information
    DynamicJsonDocument doc(1024);
    DeserializationError error = deserializeJson(doc, payload);

    if (error) {
      Serial.println("Erro ao analisar JSON: " + String(error.c_str()));
    } else {
      String estado = doc["acoes"];
      String mensagem = doc["mensagem"];
      savefile(estado.c_str(), mensagem.c_str());


      Serial.println("Valor do campo 'mensagem' no JSON:");
      Serial.println(mensagem);
      Serial.println("mensagem: " + String(mensagem));  
      Serial.println("estado: " + String(estado));     
   
      } 
    
  } else {
    Serial.println("Erro na solicitação HTTP: " + http.errorToString(httpResponseCode));
  }

  http.end();
}
