#include <Arduino.h>
#include <math.h>
#include <Wire.h>
#include <TimerOne.h>
#include <DS3231.h>
#include <dht.h>
#include <stdlib.h>
#include <String.h>

#include "Model.h"
#include "Function.h"
#include "Class.h"

#include "Arduino_FreeRTOS.h"

#define SDA 20
#define SCL 21

ModelKontrol cabinetKontrol;
ModelMonitor cabinetMonitor;

DS3231 rtc(SDA, SCL);
extern Cabinet cabinet;


Time rtcTime;
int jam=0, menit=0, detik=0;

/**
 *
 * PENJELASAN ALUR PROGRAM:
 * 	1. 	PROGRAM AKAN MEMBUAT 4 TASK DENGAN PRIORITY-NYA MASING-MASING
 * 	2. 	SETELAH TASK DIBUAT, PROGRAM AKAN MEMULAI RTC,
 * 		LOAD KONTROL DARI EEPROM KE MEMORI DAN MEMULAI FUNGSI vTaskStartScheduler
 * 	3.	SETELAH vTaskStartScheduler DIMULAI PROGRAM AKAN SEPENUHNYA DIKENDALIKAN OLEH FREERTOS
 * 		BERDASARKAN PRIORITY-NYA
 *
 * 	INFO LEBIH LANJUT LIHAT:
 * 		https://create.arduino.cc/projecthub/feilipu/using-freertos-multi-tasking-in-arduino-ebc3cc
 *
 * */

void taskKontrolRack( void *pvParameters){

	while(1){
		// update lampu dan aeartor
		cabinet.rackController(detik);


		vTaskDelay(500/ portTICK_PERIOD_MS);
	}

}

void taskPantauRTC(void *pvParameters){
	while(1){

		jam = rtc.getTime().hour;
		menit = rtc.getTime().min;
		detik = rtc.getTime().sec;



		vTaskDelay(800/portTICK_PERIOD_MS);
	}
}

void taskPantauInputUser(void *pvParameters){
	while(1){

		if(Serial.available()>0){

			int command = 0;
			command = Serial.parseInt();
			menu(command, &cabinetKontrol);
		}



		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
}

void taskPenstabilPpm(void *pvParameters){

	float ppm[4]={0,0,0,0};
	float jarak[4]={0,0,0,0};
	float lux[4]={0,0,0,0};

	int a=0,b=1,c=2,d=3;
	float ppm1=0, ppm2=0, ppm3=0, ppm4=0;
	float jarak1=0, jarak2=0, jarak3=0, jarak4=0;
	float lux1=0, lux2=0, lux3=0, lux4=0;

	while(1){

		// SENAJA TIDAK PAKAI FOR DAN ARRAY,
		// ENTAH KENAPA KETIKA PAKAI ARRAY DAN FOR ERROR (BUG??)

		Serial.println("======================");

		// 1. SRF
		// 2. LUX
		// 3. TDS

		//=======

		// 1. DHT
		// 2. SRF
		// 3. LUX
		// 4. TDS
		// 5. LAMP

		jarak1	= cabinet.getInfoNumPerKomponen(a, 1);
		lux1 	= cabinet.getInfoNumPerKomponen(a, 2);
		ppm1	= cabinet.getInfoNumPerKomponen(a, 3);
		Serial.print("SRF: ");
		Serial.println(jarak1);

		Serial.print("LUX: ");
		Serial.println(lux1);

		Serial.print("TDS: ");
		Serial.println(ppm1);

		cabinet.getInfoPerKomponen(a, 5);

		Serial.print("Setting TDS: ");
		Serial.println(cabinet.getSettingPpm(a));

		Serial.println("***********************");

		//=====================================

		jarak2	= cabinet.getInfoNumPerKomponen(b, 1);
		lux2 	= cabinet.getInfoNumPerKomponen(b, 2);
		ppm2	= cabinet.getInfoNumPerKomponen(b, 3);
		Serial.print("SRF: ");
		Serial.println(jarak2);

		Serial.print("LUX: ");
		Serial.println(lux2);

		Serial.print("TDS: ");
		Serial.println(ppm2);

		cabinet.getInfoPerKomponen(b, 5);

		Serial.print("Setting TDS: ");
		Serial.println(cabinet.getSettingPpm(b));

		Serial.println("***********************");

		//=====================================

		jarak3	= cabinet.getInfoNumPerKomponen(c, 1);
		lux3 	= cabinet.getInfoNumPerKomponen(c, 2);
		ppm3	= cabinet.getInfoNumPerKomponen(c, 3);
		Serial.print("SRF: ");
		Serial.println(jarak3);

		Serial.print("LUX: ");
		Serial.println(lux3);

		Serial.print("TDS: ");
		Serial.println(ppm3);

		cabinet.getInfoPerKomponen(c, 5);

		Serial.print("Setting TDS: ");
		Serial.println(cabinet.getSettingPpm(c));

		Serial.println("***********************");

		//=====================================

		jarak4	= cabinet.getInfoNumPerKomponen(d, 1);
		lux4 	= cabinet.getInfoNumPerKomponen(d, 2);
		ppm4	= cabinet.getInfoNumPerKomponen(d, 3);
		Serial.print("SRF: ");
		Serial.println(jarak4);

		Serial.print("LUX: ");
		Serial.println(lux4);

		Serial.print("TDS: ");
		Serial.println(ppm4);

		cabinet.getInfoPerKomponen(d, 5);

		Serial.print("Setting TDS: ");
		Serial.println(cabinet.getSettingPpm(d));

		Serial.println("***********************");


		Serial.print("Suhu: ");
		Serial.println(cabinet.getSuhu());

		Serial.print("Humidity: ");
		Serial.println(cabinet.getHumidity());


		Serial.print("TDS MIXER: ");
		Serial.println(cabinet.getPpm());

		cabinet.getWaterFlow();
		float tdsCabinet= cabinet.getPpm();
		if(tdsCabinet < SET_PPM_MIXER){
			// apabila tds pada mixer kurang dari set
			// tambahkan pupuk B
			Serial.println(cabinet.getPpm());
			cabinet.setPpkBOn();
			cabinet.setBakOn();
		}


		if(tdsCabinet > SET_PPM_MIXER){
			// apabila sudah sesuai set matikan pupuk B
			cabinet.setPpkBOff();

//			for(int a=0; a<=3; a++){
//				penstabilPPM(jarak[a], ppm[a], a);
//			}
			penstabilPPM(jarak1, ppm1, 0);
			penstabilPPM(jarak2, ppm2, 1);
			penstabilPPM(jarak3, ppm3, 2);
//			penstabilPPM(jarak4, ppm4, 3);
		}

		if(Serial2.available()>0){
			String command = Serial2.readString();
			Serial.println("======================");
			Serial.println(command);
			Serial.println("======================");
//			int i= stoi(command);
			if(command == "1") sendToNode(&cabinetKontrol, &cabinetMonitor);
		}

		vTaskDelay(3000 / portTICK_PERIOD_MS);
	}
}

void setup()
{
  Serial.begin(115200);
  Serial2.begin(115200);

  xTaskCreate(taskPantauInputUser,
  		  	  "taskPantauInput",
  			  128,
  			  NULL,
  			  4,
  			  NULL);

  xTaskCreate(taskPantauRTC,
		  	  "taskPantauRTC",
			  128,
			  NULL,
			  3,
			  NULL);


  xTaskCreate(taskPenstabilPpm,
			  "taskPenstabilPpm",
			  128,
			  NULL,
			  2,
			  NULL);

  xTaskCreate(taskKontrolRack,
  		  	  "taskKontrolRack",
  			  128,
  			  NULL,
  			  1,
  			  NULL);

  attachInterrupt(digitalPinToInterrupt(RAK0_FLOWPPKA), interrupt1, RISING);
  attachInterrupt(digitalPinToInterrupt(RAK0_FLOWPPKB), interrupt2, RISING);
  attachInterrupt(digitalPinToInterrupt(RAK0_FLOWBAKU), interrupt3, RISING);
  sei();
  rtc.begin();
  rtc.enable32KHz(false);

//  cabinet->init(&cabinetKontrol, dhtSensor);
  loadKontrol(&cabinetKontrol, &cabinet);
  vTaskStartScheduler();
  
}

void loop(){}
