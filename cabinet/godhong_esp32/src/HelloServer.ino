#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <string.h>

/**
 * LOCAL SERVER SEDERHANA YANG AKAN MENAMPILKAN DATA DARI ARDUINO.
 * DATA YANG DIKIRIMKAN DALAM FORMAT HTML.
 *
 * IP YANG DIHASILKAN AKAN DIKIRIM KE ARDUINO, SEHINGGA USER DAPAT MELIHAT
 * IP ADDRESS PADA SERIAL MONITOR ARDUINO
 *
 * TODO: UBAH DARI LOCAL SERVER KE SERVER DARI GODONG
 *
 * */

unsigned long int interval=5000;
unsigned long int lastTime =0;

AsyncWebServer server(80);

const char* ssid = "Dua Musim";
const char* password = "HALLO2musim";

String data  = "<h1>data not transfered</h1>";

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
