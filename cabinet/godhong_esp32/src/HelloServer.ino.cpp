# 1 "C:\\Users\\caasper\\AppData\\Local\\Temp\\tmpyb_im435"
#include <Arduino.h>
# 1 "D:/savePCB/2022Project/GODHONG_ReDesign/Software/github/godhong_esp32/src/HelloServer.ino"
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <string.h>
# 18 "D:/savePCB/2022Project/GODHONG_ReDesign/Software/github/godhong_esp32/src/HelloServer.ino"
unsigned long int interval=5000;
unsigned long int lastTime =0;

AsyncWebServer server(80);

const char* ssid = "Dua Musim";
const char* password = "HALLO2musim";

String data = "<h1>data not transfered</h1>";
void notFound(AsyncWebServerRequest *request);
void setup();
void loop();
#line 28 "D:/savePCB/2022Project/GODHONG_ReDesign/Software/github/godhong_esp32/src/HelloServer.ino"
void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}

void setup() {

    Serial.begin(115200);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
        Serial.printf("WiFi Failed!\n");
        return;
    }

    Serial.print("IP Address: ");
    Serial.print(WiFi.localIP());

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/html", data);
    });

    server.onNotFound(notFound);

    server.begin();
}

void loop() {
 unsigned long int currentTime = millis();

 while(Serial.available()>0){
  data = Serial.readString();
  Serial.print("\nIP Address: ");
  Serial.print(WiFi.localIP());
  Serial.print("\n");
 }

 if((currentTime-lastTime) >= interval){
  Serial.print("1");
  lastTime = currentTime;
 }


}