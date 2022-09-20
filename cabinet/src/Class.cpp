#include <Arduino.h>
#include <math.h>

#include "Class.h"
#include "pinMap.h"
#include "NewPing.h"
#include "Arduino_FreeRTOS.h"
/*
 *implementasi DHT pada Class.h
 * */

//void Dht::init(int pin, dht *dhtSensor)
//{
//    this->pin = pin;
//    this->dhtSensor = dhtSensor;
//    pinMode(this->pin, INPUT);
//}

void Dht::update()
{
    this->dhtSensor.read22(this->pin);
    this->suhu = this->dhtSensor.temperature; // member variable are double
    this->humidity = this->dhtSensor.humidity;
}

float Dht::getSuhu(){
	Dht::update();
	return this->suhu;
}

float Dht::getHumidity(){
	Dht::update();
	return this->humidity;
}

/*
 *implementasi LUX/sensor cahaya pada Class.h
 * */
//void Lux::init(int pin)
//{
//    this->pin = pin;
//    pinMode(this->pin, INPUT);
//}
void Lux::update()
{
    float adc = analogRead(this->pin);
    float volt = (float)5 * (adc/1024);

    double temp = (float)4.098 - (1.084 * volt);
    double tempLux = (double)pow(10, temp);
    this->lux = (int)tempLux;
}

float Lux::getLux(){
	Lux::update();
	return (float) this->lux;
}


/*
 *implementasi TDS pada Class.h
 * */
//void Tds::init(int dataPin, int powerPin)
//{
//    this->dataPin = dataPin;
//    this->powerPin = powerPin;
//    pinMode(this->dataPin, INPUT);
//    pinMode(this->powerPin, OUTPUT);
//}
void Tds::update(int suhu){

	/**
	 *  FUNCTION MEMBER INI AKAN MENGHITUNG DAN MENGUBAH NILAI PEMBACAAN
	 *  ADC KE PPM DALAM STANDAR USA.
	 *
	 *  ALGORITMA PENGHITUNGAN YANG DIGUNAKAN PADA FUNCTION INI
	 *  BERDASARKAN POST HACKADAY "MICHAEL RATCLIFFE" PADA TAHUN 2015(DI UPDATE PADA 2021)
	 *  UNTUK INFO LEBIH LANJUT LIHAT LINK:
	 *  	https://hackaday.io/project/7008-hacking-the-way-to-growing-food/log/24646-three-dollar-ec-ppm-meter-arduino
	 *
	 * */

	float ra = 25;
	float r1 = 1000 + ra; //resistor yang dipakai + 25
	float rc = 0;
	float ec = 0;
	float ec25 =0;

	// standar usa menggunakan 1ms/cm = 500ppm
	// standar eropa menggunakan 1ms/cm = 640ppm
	// standar australia menggunakan 1ms/cm = 700ppm
	// tds meter yang ada di kantor menggunakan standar usa
    float ppmConversion = 500;

    float tempCoef = 0.19;
//    float cellConstant = 0.04; // tune this variable
    float adc =0;
    float vdrop = 0;
    float k=0;
    float kmixer=1.58; // konstanta probe tds mixer
    float k1 = 2.497; //konstanta probe tds rak 1
    float k24=1.0523; // konstanta probe tds pada rak 2-4

    // berdasarkan hasil kalibrasi, probe tds rak 1 paling berbeda
    // dibandingkan rak 2,3,4. nilai K rak1 = 2.497, dan nilai k rak2-4 = 1.0523.
    // menurut : https://how2electronics.com/tds-sensor-arduino-interfacing-water-quality-monitoring/
    // probe tds memiliki akurasi sebesar +-10%
    // dan hasil pembacaan tds menggunakan konstanta yang telah dikalibrasi
    // masih masuk dalam rentang error +-10%
    //
    // hasil pengukuran yang dilakukan masih belum
    // memasukan toleransi suhu (suhu masih dianggap 25*C)

    //jika tds rak1, gunakan konstanta k1
    if(this->dataPin == RAK1_TDS) k=k1;

    // konstanta tds pada mixer
    else if(this->dataPin == RAK0_TDSMIXER) k=kmixer;

    // selain rak satu pakai k24
    else k=k24;

    digitalWrite(this->powerPin, HIGH);
    adc = analogRead(this->dataPin);
    adc = analogRead(this->dataPin); // this is not a mistake first reading will low
    digitalWrite(this->powerPin, LOW);

    vdrop = (5*adc)/1024;
    rc = (vdrop*r1)/(5-vdrop);
    rc -= ra;
    ec = 1000/(rc * k);

    // ganti angka 25 pertama untuk kompensasi suhu dari dht
    ec25 = ec / (1+tempCoef*(25-25));
    this->ppm = ec25 * (ppmConversion);
//    this->ppm = vdrop;
//    this->ppm = adc;
//    this->ppm = ec;

}

float Tds::getPpm(){

	/**
	 * 	FUNCTION INI SAMA DENGAN TDS::UPDATE NAMUN FUNGSI INI MEMILIKI RETURN FLOAT
	 *
	 *  FUNCTION MEMBER INI AKAN MENGHITUNG DAN MENGUBAH NILAI PEMBACAAN
	 *  ADC KE PPM DALAM STANDAR USA.
	 *
	 *  ALGORITMA PENGHITUNGAN YANG DIGUNAKAN PADA FUNCTION INI
	 *  BERDASARKAN POST HACKADAY "MICHAEL RATCLIFFE" PADA TAHUN 2015(DI UPDATE PADA 2021)
	 *  UNTUK INFO LEBIH LANJUT LIHAT LINK:
	 *  	https://hackaday.io/project/7008-hacking-the-way-to-growing-food/log/24646-three-dollar-ec-ppm-meter-arduino
	 *
	 * */

	float ra = 25;
	float r1 = 1000 + ra; //resistor yang dipakai + 25
	float rc = 0;
	float ec = 0;
	float ec25 =0;

	// standar usa menggunakan 1ms/cm = 500ppm
	// standar eropa menggunakan 1ms/cm = 640ppm
	// standar australia menggunakan 1ms/cm = 700ppm
	// tds meter yang ada di kantor menggunakan standar usa
    float ppmConversion = 500;

    float tempCoef = 0.19;
//    float cellConstant = 0.04; // tune this variable
    float adc =0;
    float vdrop = 0;
    float k=0;
    float kmixer=1.58; // konstanta probe tds mixer
    float k1 = 2.497; //konstanta probe tds rak 1
    float k24=1.0523; // konstanta probe tds pada rak 2-4

    // berdasarkan hasil kalibrasi, probe tds rak 1 paling berbeda
    // dibandingkan rak 2,3,4. nilai K rak1 = 2.497, dan nilai k rak2-4 = 1.0523.
    // menurut : https://how2electronics.com/tds-sensor-arduino-interfacing-water-quality-monitoring/
    // probe tds memiliki akurasi sebesar +-10%
    // dan hasil pembacaan tds menggunakan konstanta yang telah dikalibrasi
    // masih masuk dalam rentang error +-10%
    //
    // hasil pengukuran yang dilakukan masih belum
    // memasukan toleransi suhu (suhu masih dianggap 25*C)

    //jika tds rak1, gunakan konstanta k1
    if(this->dataPin == RAK1_TDS) k=k1;

    // konstanta tds pada mixer
    else if(this->dataPin == RAK0_TDSMIXER) k=kmixer;

    // selain rak satu pakai k24
    else k=k24;

    digitalWrite(this->powerPin, HIGH);
    adc = analogRead(this->dataPin);
    adc = analogRead(this->dataPin); // this is not a mistake first reading will low
    digitalWrite(this->powerPin, LOW);

    vdrop = (5*adc)/1024;
    rc = (vdrop*r1)/(5-vdrop);
    rc -= ra;
    ec = 1000/(rc * k);

    // ganti angka 25 pertama untuk kompensasi suhu dari dht
    ec25 = ec / (1+tempCoef*(25-25));
    this->ppm = ec25 * (ppmConversion);

	return this->ppm;
}


/*
 *implementasi Relay pada Class.h
 * */



Relay::Relay(int pin){
	// modul relay yang digunakan adalah active LOW,
    this->pin = pin;
    pinMode(this->pin, OUTPUT);
    digitalWrite(this->pin, HIGH);
    this->state = !digitalRead(this->pin);
}
void Relay::setOff()
{
    digitalWrite(this->pin, HIGH);
    this->state = false;
}
void Relay::setOn()
{
    digitalWrite(this->pin, LOW);
    this->state = true;
}
void Relay::change(int &jamSekarang)
{
	if(jamSekarang - this->lastJam <0 ) this->lastJam-=59;


	if(jamSekarang - this->lastJam > this->jamSettingOff && this->state){
		this->state = !this->state;
		digitalWrite(this->pin, this->state);
		this->lastJam=jamSekarang;
	}
	else if(jamSekarang - this->lastJam > this->jamSettingOn && !this->state){
		this->state = !this->state;
		digitalWrite(this->pin, this->state);
		this->lastJam=jamSekarang;
	}


}

int Relay::getState(){
	return this->state;
}

void Relay::setJamOn(int jam){
	this->jamSettingOn=jam;
}

void Relay::setJamOff(int jam){
	this->jamSettingOff=jam;
}

int Relay::getJamSeting(){
	return this->jamSettingOn;
}

int Relay::getLastJam(){
	return this->lastJam;
}
/*
 *implementasi Sensor Ultrasonic pada Class.h
 * */
//Srf::Srf(int trig, int echo, double maxHeight, double width, double length)
//{
////    pinMode(this->trig, OUTPUT);
////    pinMode(this->echo, INPUT);
//    this->trig = trig;
//    this->echo = echo;
//    this->maxHeight = maxHeight;
//    this->length = length;
//    this->width = width;
//    sonar(trig, echo, (int)maxHeight);
//}



void Srf::update()
{

    this->range = (double)	this->getPingCm();
    this->volume = ((maxHeight - range) * length * width) / 1000;

}

double Srf::getVolume(){
	return this->volume;
}

double Srf::getRange(){
	return this->range;
}

double Srf::getPingCm(){
	return Srf::sonar.ping_cm(maxHeight);
}

/*
 *implementasi sensor debit / waterflow pada Class.h
 * */
Waterflow::Waterflow(int pin, float flow)
{
    this->pin = pin;
    this->flow = flow;
    this->totalVolume=0;
    this->debit=0;
    this->avgFreq=0;
    pinMode(this->pin, INPUT);

}
void Waterflow::update()
{
    double acc = 0;
    for (int i = 9; i > 0; i--)
    {
        freqArray[i] = freqArray[i - 1];
        acc += freqArray[i];
    }
    freqArray[0] = freq;
    acc += freq;
    avgFreq = (double)acc / 10;
    totalVolume += avgFreq * flow;
    debit = avgFreq * flow;
    freq = 0;



}
void Waterflow::pulse()
{
    freq++;
}

double Waterflow::getDebit(){
	return this->debit;
}

double Waterflow::getVolume(){
	return this->totalVolume;
}

int Waterflow::getPulse(){
	return this->freq;
}


void Waterflow::setInterrupt(void *interrupt){

	attachInterrupt(digitalPinToInterrupt(this->pin), interrupt, RISING);
}

/*
 * implementasi RACK pada Class.h
 * setiap Rack ada sensor ultrasonic, LUX, aerator, tds, Lampu, pompa filler, pompa drainer
 * */

Rack::Rack(int rak):
	srf(rak==0?RAK1_TRIG:rak==1?RAK2_TRIG:rak==2?RAK3_TRIG:RAK4_TRIG,
		rak==0?RAK1_ECHO:rak==1?RAK2_ECHO:rak==2?RAK3_ECHO:RAK4_ECHO,
		MAX_HEIGHT, RAK_WIDTH, RAK_LENGTH),

	lux(rak==0?RAK1_LUX:rak==1?RAK2_LUX:rak==2?RAK3_LUX:RAK4_LUX),

	tds(rak==0?RAK1_TDS:rak==1?RAK2_TDS:rak==2?RAK3_TDS:RAK4_TDS,
		rak==0?RAK1_TDSP:rak==1?RAK2_TDSP:rak==2?RAK3_TDSP:RAK4_TDSP),

	aerator(rak==0?AERATORSL1:rak==1?AERATORSL2:rak==2?AERATORSL3:AERATORSL4),

	lamp(rak==0?SSRLAMP1:rak==1?SSRLAMP2:rak==2?SSRLAMP3:SSRLAMP4),

	fill(rak==0?FILLERSL1:rak==1?FILLERSL2:rak==2?FILLERSL3:FILLERSL4),

	drain(rak==0?DRAINERSL1:rak==1?DRAINERSL2:rak==2?DRAINERSL3:DRAINERSL4)
{
	this->waktuAeratorOn=0;
	this->waktuAeratorOff=0;
	this->waktuLampOn=0;
	this->waktuLampOff=0;
	this->kadarPpm=0;

}



void Rack::getInfo(){

	String tmp="";

	tmp = this->aerator.getState() ? "Menyala" : "Mati";
	Serial.print("1.Aerator: ");
	Serial.println(tmp);

	tmp = this->lamp.getState() ? "Menyala" : "Mati";
	Serial.print("2.Lamp: ");
	Serial.println(tmp);

	tmp = this->fill.getState() ? "Menyala" : "Mati";
	Serial.print("3.Fill: ");
	Serial.println(tmp);

	tmp = this->drain.getState() ? "Menyala" : "Mati";
	Serial.print("4.Drain: ");
	Serial.println(tmp);

	Serial.print("5.Lux: ");
	Serial.println(this->lux.getLux());

	Serial.print("6. Srf: ");
	Serial.println(this->srf.getPingCm());

	this->tds.update(this->dht.getSuhu());
	Serial.print("7. TDS: ");
	Serial.println(this->tds.getPpm());

}


void Rack::setWaktuAerator(int waktuOn, int waktuOff){
	this->waktuAeratorOn=waktuOn;
	this->waktuAeratorOff=waktuOff;

	this->aerator.setJamOn(waktuOn);
	this->aerator.setJamOff(waktuOff);
}

void Rack::setWaktuLamp(int waktuOn, int waktuOff){
	this->waktuLampOn=waktuOn;
	this->waktuLampOff=waktuOff;

	this->lamp.setJamOn(waktuOn);
	this->lamp.setJamOff(waktuOff);
}

void Rack::setKadarPpm(int kadar){
	this->kadarPpm=kadar;
}

void Rack::setDrainOn(){
	this->drain.setOn();
}

void Rack::setDrainOff(){
	this->drain.setOff();
}

void Rack::setFillOn(){
	this->fill.setOn();
}

void Rack::setFillOff(){
	this->fill.setOff();
}

void Rack::update(int &jamSekarang){
	this->lamp.change(jamSekarang);
	this->aerator.change(jamSekarang);
}

void Rack::getStatusLampAerator(){
	Serial.print("Aerator/Lamp/Fill/Drain = ");
	Serial.print(this->aerator.getState()==0?"On":"Off");
	Serial.print("/");
	Serial.print(this->lamp.getState()==0?"On":"Off");
	Serial.print("/");
	Serial.print(this->fill.getState()==1?"On":"Off");
	Serial.print("/");
	Serial.println(this->drain.getState()==1?"On":"Off");

}

int Rack::getKadarPpm(){
	return this->tds.getPpm();

}

int Rack::getSettingPpm(){
	return this->kadarPpm;
}

int Rack::getRange(){
	return this->srf.getPingCm();
}

void Rack::getInfoKomponen(int device){

	if(device == 1){
		Serial.print("DHT suhu/humidity = ");
		Serial.print(this->dht.getSuhu());
		Serial.print("/");
		Serial.println(this->dht.getHumidity());
	}
	else if(device ==2){
		Serial.print("SRF = ");
		Serial.println(this->srf.getPingCm());
	}
	else if(device ==3){
		Serial.print("LUX = ");
		Serial.println(this->lux.getLux());
	}
	else if(device ==4){
//		float tds=1/3.f;
		Serial.print("TDS = ");
//		printFloat(this->tds.getPpm());
		Serial.println(this->tds.getPpm(), 3);
	}
	else if(device == 5){
		this->getStatusLampAerator();
	}
}

float Rack::getInfoNumKomponen(int device){

	if(device ==1)return this->srf.getPingCm();
	else if(device ==2)return this->lux.getLux();
	else if(device ==3)return this->tds.getPpm();


}


Cabinet::Cabinet():
	srfBak(RAK0_TBAKU, RAK0_EBAKU, MAX_HEIGHT, RAK_WIDTH, RAK_LENGTH),
	srfA(RAK0_TPPKA, RAK0_EPPKA, MAX_HEIGHT, RAK_WIDTH, RAK_LENGTH),
	srfB(RAK0_TPPKB, RAK0_EPPKB, MAX_HEIGHT, RAK_WIDTH, RAK_LENGTH),
	srfCampuran(RAK0_TMIXER, RAK0_EMIXER, MAX_HEIGHT, RAK_WIDTH, RAK_LENGTH),
	srfMist(RAK0_TMIST, RAK0_EMIST, MAX_HEIGHT, RAK_WIDTH, RAK_LENGTH),

	tds(RAK0_TDSMIXER, RAK0_TDSMIXERP),

	Bak(P_AIRBAKU),
	PpkA(P_PPKA),
	PpkB(P_PPKB),
	Fan(FANMIST),
	Mist(SSRMIST),
	flowA(RAK0_FLOWPPKA, DEF_FLOW),
	flowB(RAK0_FLOWPPKB, DEF_FLOW),
	flowBak(RAK0_FLOWBAKU, DEF_FLOW){


	for(int a=0; a<4; a++){
		rack[a]=Rack(a);
	}


}

void Cabinet::getInfoAll(){
	Serial.println("===================");
	Serial.print("TDS mixer: ");
	Serial.println(this->tds.getPpm());
	Serial.print("Level Air Mixer: ");
	Serial.println(this->srfCampuran.getRange());


	Serial.println("===================");
	for(int i=0; i<4; i++){

		rack[i].getInfo();
		Serial.print("===================");
		Serial.println("  ");
	}
}

void Cabinet::getInfoRack(int rackNum){
	Serial.print("RACK: ");
	Serial.println(rackNum);
	rack[rackNum-1].getInfo();
}

void Cabinet::setRack(int rackNum, int waktuAeratorOn, int waktuAeratorOff, int waktuLampOn, int waktuLampOff, int kadarPpm,  int waktuMistOn, int waktuMistOff){
	rackNum -=1;
	rack[rackNum].setWaktuAerator(waktuAeratorOn, waktuAeratorOff);
	rack[rackNum].setWaktuLamp(waktuLampOn, waktuLampOff);
	rack[rackNum].setKadarPpm(kadarPpm);
	this->Mist.setJamOn(waktuMistOn);
	this->Mist.setJamOff(waktuMistOff);

}

void Cabinet::getInfoPerKomponen(int rack, int device){

	this->rack[rack].getInfoKomponen(device);
}

float Cabinet::getInfoNumPerKomponen(int rack, int device){
	return this->rack[rack].getInfoNumKomponen(device);
}

float Cabinet::getPpm(){
	return this->tds.getPpm();
}

float Cabinet::getSettingPpm(int i){
	return this->rack[i].getSettingPpm();
}

void Cabinet::setFillerON(){
	for(int i=0; i<4; i++){
		this->rack[i].setFillOn();
	}
}

void Cabinet::setPpkAOn(){
	this->PpkA.setOn();
}
void Cabinet::setPpkAOff(){
	this->PpkA.setOff();
}
void Cabinet::setPpkBOn(){
	this->PpkB.setOn();
}
void Cabinet::setPpkBOff(){
	this->PpkB.setOff();
}
void Cabinet::setBakOn(){
	this->Bak.setOn();
}
void Cabinet::setBakOff(){
	this->Bak.setOff();
}

void Cabinet::setActuator(int rack, int device, bool state){
	// 1. filler
	// 2. drainer
	if(device ==1 && state == 1) this->rack[rack].setFillOn();
	if(device ==1 && state == 0) this->rack[rack].setFillOff();

	if(device ==2 && state == 1) this->rack[rack].setDrainOn();
	if(device ==2 && state == 0) this->rack[rack].setDrainOff();
}

void Cabinet::getWaterFlow(){
	Serial.print("Pulse Baku: ");
	Serial.println(this->flowBak.getPulse());
	Serial.print("Pulse Pupuk A: ");
	Serial.println(this->flowA.getPulse());
	Serial.print("Pulse Pupuk B: ");
	Serial.println(this->flowB.getPulse());
}

float Cabinet::getSuhu(){
	return this->dhtCab.getSuhu();
}

float Cabinet::getHumidity(){
	return this->dhtCab.getHumidity();
}

void Cabinet::penstabilPpm(float &jarak1, float &jarak2,  float &jarak3, float &jarak4, float &ppm1, float &ppm2, float &ppm3, float &ppm4){
	/**
	 * FUNGSI INI TELAH DIGANTIKAN OLEH FUNGSI penstabilPPM PADA Function.cpp
	 * DIKARENAKAN KETIKA MEMAKAI FUNCTION MEMBER INI, FREERTOS TIDAK BISA BERGANTI
	 * KE TASK LAINNYA DAN HANYA STACK PADA FUNGSI PENSTABIL INI SAJA (HANG)
	 *
	 * TODO: UBAH FUNGSI penstabilPPM PADA Function.cpp MENJADI FUNCTION MEMBER CLASS CABINET
	 *
	 * */
	float jarak[4] = {jarak1, jarak2, jarak3, jarak4};
	float ppm[4] = {ppm1, ppm2, ppm3, ppm4};
	for(int i=0; i<4; i++){
		Serial.println(jarak[i]);
		Serial.println(ppm[i]);
	}

}

float Cabinet::getRangePpkA(){
	return this->srfA.getPingCm();
}

float Cabinet::getRangePpkB(){
	return this->srfB.getPingCm();
}

float Cabinet::getRangeMixer(){
	return this->srfCampuran.getPingCm();
}

float Cabinet::getRackPpm(int num){
	return this->rack[num].getKadarPpm();
}

/*
 * 1 = FlowppkA
 * 2 = FlowppkB
 * 3 = flowAirBaku
 * */

void Cabinet::pulse(int num){
	switch(num){
		case 1:
			this->flowA.pulse();
			break;
		case 2:
			this->flowB.pulse();
			break;
		case 3:
			this->flowBak.pulse();
			break;
		default:break;
	}
}

void Cabinet::rackController(int &jamSekarang){
	for(int i=0; i<4; i++){
		this->rack[i].update(jamSekarang);
	}
}

void Cabinet::getInfoRackInline(){

	this->rack[0].getInfo();
	this->rack[1].getInfo();
	this->rack[2].getInfo();
	this->rack[3].getInfo();
}
