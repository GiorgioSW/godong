#include <Arduino.h>
#include <DS3231.h>
#include <stdlib.h>
#include <dht.h>
#include "NewPing.h"
#include "pinMap.h"

#include "Model.h"
#ifndef Class
#define Class

#define MAX_HEIGHT 200
#define RAK_WIDTH 10
#define RAK_LENGTH 25
#define DEF_FLOW 2.5

class Dht{
	dht dhtSensor;
	int pin;
	float suhu;
	float humidity=0;

public:
	Dht():pin(PIN_DHT22), suhu(0), humidity(0){}
	Dht(int pin, float suhu):pin(pin), suhu(suhu) {}
    //void init(int pin, dht *dhtSensor);
    void update();
    float getSuhu();
    float getHumidity();


};

class Lux{
	int pin;
	int lux=0;

public:
	Lux(){}
	Lux(int pin):pin{pin}{}
//    void init(int pin);
    void update();
    float getLux();


};

class Tds{
	int dataPin,
	   powerPin;
   float	ppm=0;
public:
	Tds(){}
	Tds(int datapin, int powerpin):dataPin(datapin), powerPin(powerpin){
		pinMode(this->powerPin, OUTPUT);
		pinMode(this->dataPin, INPUT);
	}

//    void init(int dataPin, int powerPin);
    void update(int suhu);
    float getPpm();

};

class Relay{
    int pin;
	bool state = false;
	int lastJam=0;
	int jamSettingOn=0;
	int jamSettingOff=0;


public:
	Relay(){}
	Relay(int);
    void setOn();
    void setOff();
    void change(int &jamSekarang);
    int getState();
    void setJamOn(int jam);
    void setJamOff(int jam);
    int getJamSeting();
    int getLastJam();


};

class Srf{
	NewPing sonar;

	int trig,
		echo;
	double range,
		   volume,
		   length,
		   width,
		   maxHeight;
public:
	Srf():sonar(trig, echo, maxHeight){}
	Srf(int trig, int echo, double maxHeight, double width, double length)
	:sonar(trig, echo, maxHeight){
		this->trig=trig;
		this->echo=echo;
		this->maxHeight=maxHeight;
		this->width=width;
		this->length=length;
		volume=0;
		range=0;
	}
	Srf(int trig, int echo, int maxHeight, int width, int length)
		:sonar(trig, echo, maxHeight){
			this->trig=trig;
			this->echo=echo;
			this->maxHeight=maxHeight;
			this->width=width;
			this->length=length;
			volume=0;
			range=0;
		}

	void update();
	double getVolume();
	double getRange();
	double getPingCm();

};

class Waterflow{
	unsigned long int freqArray[10] = {0,0,0,0,0,0,0,0,0,0},
		pin,
		freq=0;
	double avgFreq,
		   debit,
		   totalVolume;
	float flow;
public:
	Waterflow(){}
	Waterflow(int pin, float flow);
	void update();
	void pulse();
	double getDebit();
	double getVolume();
	int getPulse();
	void setInterrupt(void *interrupt);


};

class Rack{
	Dht dht;
	Srf srf;
	Lux lux;
	Tds tds;
	Relay 	aerator,
			lamp, // barang fisiknya SSR bukan relay
			fill,
			drain;
	int waktuAeratorOn,
		waktuLampOn,
		waktuAeratorOff,
		waktuLampOff,
		kadarPpm;

public:
	Rack(){}
	Rack(int);
	void getInfo();
	void getInfoInline();
	int setRack();

	void setWaktuAerator(int waktuOn, int waktuOff);
	void setWaktuLamp(int waktuOn, int waktuOff);
	void setKadarPpm(int kadar);
	void setDrainOn();
	void setDrainOff();
	void setFillOn();
	void setFillOff();

	int getWaktuAerator();
	int getWaktuLamp();
	int getKadarPpm();
	int getSettingPpm();
	int getRange();
	void getInfoKomponen(int device);
	float getInfoNumKomponen(int device);
	void update(int &jamSekarang);
	void getStatusLampAerator();

//public:
//	Srf *srf = new Srf;
//	Lux *lux = new Lux;
//	Tds *tds = new Tds;
//	Relay *aerator = new Relay,
//		  *light = new Relay,
//		  *fill = new Relay,
//		  *drain = new Relay;
//	void init(int trigPin, int echoPin, int luxPin, int ecPin, int ecPower, int aeratorPin, int lightPin, int fillPin, int drainPin, double maxHeight, double width, double length);
        
};

class Cabinet{

	ModelKontrol kontrolData;
	ModelMonitor monitorData;
	Rack rack[4];
	Dht dhtCab;
	Srf srfBak,
		srfA,
		srfB,
		srfCampuran,
		srfMist;
	Tds tds;
	Relay Bak,
		  PpkA,
		  PpkB,
		  Fan,
		  Mist;
	Waterflow flowA,
			  flowB,
			  flowBak;
public:
	Cabinet();
//	void init(ModelKontrol *kontrolInput, dht *dhtSensor);
	int checkActuator(String device);
	void getInfoAll();
	void getInfoRack(int);
	void getInfoRackInline();
	void pulse(int num);
	void rackController(int &jamSekarang);
	void setFillerON();
	void setPpkAOn();
	void setPpkAOff();
	void setPpkBOn();
	void setPpkBOff();
	void setBakOn();
	void setBakOff();
	void setActuator(int rack, int device, bool state);
	void penstabilPpm(float &jarak1, float &jarak2,  float &jarak3, float &jarak4, float &ppm1, float &ppm2, float &ppm3, float &ppm4);
	void setRack(int rackNum, int waktuAeratorOn, int waktuAeratorOff, int waktuLampOn,  int waktuLampOff, int kadarPpm, int waktuMistOn, int waktuMistOff);
	void getInfoPerKomponen(int rack, int device);
	float getInfoNumPerKomponen(int rack, int device);
	float getPpm();
	float getSettingPpm(int i);
	void getWaterFlow();
	float getSuhu();
	float getHumidity();
	float getRangePpkA();
	float getRangePpkB();
	float getRangeMixer();
	float getRackPpm(int i);






};
#endif
