//feito pela AI

void listFilesOnWebPage(AsyncWebServerRequest *request) {
  File root = SD_MMC.open("/");

 String output = "<html><body>";
    output += "<h1>Arquivos no cartao SD</h1><br>";
    root.rewindDirectory();
    File file = root.openNextFile();
    while(file){
      if(!file.isDirectory()){
        String fileName = file.name();
        output += "<a href='/lersdx/";
        output += fileName;
        output += "'>";
        output += fileName;
        output += "</a><br>";
      }
      file = root.openNextFile();
    }
    output += "</body></html>";
    request->send(200, "text/html", output);
    
    }



void handleFile(AsyncWebServerRequest *request) {
File root = SD_MMC.open("/");

  String fileName = request->url();
    fileName.remove(0, 7);
    Serial.print(fileName);
    if(SD_MMC.exists(fileName)){
      File file = SD_MMC.open(fileName, FILE_READ);
      request->send(file, fileName, "text/plain");
    } else {
      request->send(404, "text/plain", "Arquivo nao encontrado");
    }
}
