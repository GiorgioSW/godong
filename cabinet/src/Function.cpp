#include <EEPROM.h>
#include <stdlib.h>
#include <String.h>
#include "Function.h"
#include "Class.h"

#ifndef FUNCTION
#define FUNCTION

Cabinet cabinet;

enum ControlAdress{
	aerator1WaktuOn,
	aerator1WaktuOff,
	aerator2WaktuOn,
	aerator2WaktuOff,
	aerator3WaktuOn,
	aerator3WaktuOff,
	aerator4WaktuOn,
	aerator4WaktuOff,

	light1WaktuOn,
	light1WaktuOff,
	light2WaktuOn,
	light2WaktuOff,
	light3WaktuOn,
	light3WaktuOff,
	light4WaktuOn,
	light4WaktuOff,

	ppm1,
	ppm2,
	ppm3,
	ppm4,

	waktuMistOn,
	waktuMistOff,

	pengalippm1,
	pengalippm2,
	pengalippm3,
	pengalippm4
};
ControlAdress controlAdress=aerator1WaktuOn;

void interrupt1() { cabinet.pulse(1); }
void interrupt2() { cabinet.pulse(2); }
void interrupt3() { cabinet.pulse(3); }

int stoi(String s)
{
    // Initialize a variable
    int num = 0;
    int n = s.length();

    // Iterate till length of the string
    for (int i = 0; i < n; i++)

        // Subtract 48 from the current digit
        num = num * 10 + (int(s[i]) - 48);

    // Print the answer
    return num;
}

void setVar(int address, int data){
    EEPROM.write(address, data);
}
int getVar(int address){
    return EEPROM.read(address);
}
int getVar2(int address1, int address2){
    return (EEPROM.read(address1)*255)+(EEPROM.read(address2));
}

void penstabilPPM(float jarak, float ppm, int rack){

	/**
	 * FUNGSI INI BERGUNA UNTUK MENSTABILKAN PPM PADA MASING MASING RAK,
	 * ARGUMENT 1 MERUPAKAN HASIL PEMBACAAN SRF,
	 * ARGUMENT 2 MERUPAKAN HASIL PEMBACAAN TDS
	 * ARGUMENT 3 MERUPAKAN RAK YANG AKAN DISTABILKAN
	 *
	 * PADA CLASS CABINET SEBENARNYA SUDAH DIBUAT FUNGSI UNTUK PENSTABIL PPM
	 * NAMUN ENTAH KENAPA (BUG), KETIKA MENGGUNAKAN FUNCTION MEMBER PADA CLASS
	 * CABINET TERSEBUT FREERTOS TIDAK BISA BERGANTI KE TASK LAINNYA,
	 * SEHINGGA PROGRAM HANYA STACK/BERHENTI PADA FUNCTION MEMBER TERSEBUT.
	 *
	 *
	 * */


	// apabila air rak i belum penuh nyalakan filler
	// atau, apabila air rak i belum penuh dan ppm pada rak i kurang dari set
	// nyalakan filler

	cabinet.setBakOff();
//		cabinet.setBakOn();
	// 1 filler
	// 2 drainer

	// apabila air rak i terlalu penuh nyalakan drainer
	if(jarak <= 2) {
		cabinet.setActuator(rack, 2, 1); // set drainer ON
		cabinet.setActuator(rack, 1, 0); // set filler OFF
	}

	// jika jarak > 4cm atau jarak>4 dan ppm rak i kurang dari setting
	else if(jarak > 4 || (jarak > 4 && ppm < cabinet.getSettingPpm(rack))){
		cabinet.setActuator(rack, 1, 1); // set filler ON
		cabinet.setActuator(rack, 2, 0); // set drainer OFF
	}

	// jika jarak 3cm hingga 4cm dan ppm rak i kurang dari setting
	else if(jarak >= 3 &&
		jarak <= 4 &&
		ppm < cabinet.getSettingPpm(rack)){

		cabinet.setActuator(rack, 1, 1); // set filler ON
		cabinet.setActuator(rack, 2, 1); // set drainer ON
	}

	// jika jarak 3cm hingga 4cm dan ppm rak i lebih besar sama dengan setting
	else if(jarak >= 3 &&
		jarak <= 4 &&
		ppm >= cabinet.getSettingPpm(rack)){

		cabinet.setActuator(rack, 1, 0); // set filler OFF
		cabinet.setActuator(rack, 2, 0); // set drainer OFF
	}

}

void menuInfo(){



	int jawaban=0;

	Serial.println("Rak Berapa? (1/2/3/4/0 untuk semua rak)");
	Serial.println("Jawaban:");
	while(Serial.available()==0){}// tunggu user memasukan input

	jawaban = Serial.parseInt();
	Serial.print("jawaban: ");
	Serial.println(jawaban);

	switch(jawaban){
		case 1:
			cabinet.getInfoRack(1);
			break;
		case 2:
			cabinet.getInfoRack(2);
			break;
		case 3:
			cabinet.getInfoRack(3);
			break;
		case 4:
			cabinet.getInfoRack(4);
			break;
		case 0:
			cabinet.getInfoAll();
			break;
		default:
			Serial.println("Jawaban salah! (1/2/3/4/0 untuk semua rak)");
			Serial.println("============================");
			menuInfo();
			break;
	}

}

void menuSettingRak(ModelKontrol *modelKontrol){

	int jawaban;
	int waktuAeratorOn=0, waktuAeratorOff=0, waktuLampOn=0,waktuLampOff=0, kadarPpm=0, waktuMistOn=0, waktuMistOff=0;

	// tanya rak berapa
	Serial.println("Setting rak berapa?? (1/2/3/4)");
	while(Serial.available()==0){} //tunggu user input jawaban
	jawaban = Serial.parseInt();

	if(jawaban <1 && jawaban >4){
		Serial.println("jawaban Salah!! (1/2/3/4)");
		menuSettingRak(modelKontrol);
	}

	Serial.println("=========================");
	Serial.print("Memulai Setting untuk RAK: ");
	Serial.println(jawaban);

	//tanya waktu aerator
	Serial.println("Masukan waktu Aerator menyala!! (jam)");
	while(Serial.available()==0){} //tunggu user input jawaban
	waktuAeratorOn = Serial.parseInt();
	modelKontrol->aerator[jawaban-1].waktuOn = waktuAeratorOn;

	Serial.println("Masukan waktu Aerator mati!! (jam)");
	while(Serial.available()==0){} //tunggu user input jawaban
	waktuAeratorOff = Serial.parseInt();
	modelKontrol->aerator[jawaban-1].waktuOff = waktuAeratorOff;

	//tanya waktu Lamp
	Serial.println("Masukan waktu Lampu menyala!! (jam)");
	while(Serial.available()==0){} //tunggu user input jawaban
	waktuLampOn = Serial.parseInt();
	modelKontrol->lamp[jawaban-1].waktuOn = waktuLampOn;

	Serial.println("Masukan waktu Lampu Mati!! (jam)");
	while(Serial.available()==0){} //tunggu user input jawaban
	waktuLampOff = Serial.parseInt();
	modelKontrol->lamp[jawaban-1].waktuOff = waktuLampOff;

	//tanya kadar ppm
	Serial.println("Masukan kadar PPM!! (ppm)");
	while(Serial.available()==0){} //tunggu user input jawaban
	kadarPpm = Serial.parseInt();
	modelKontrol->ppm[jawaban-1] = kadarPpm;

	Serial.println("Masukan kadar waktu Mist On!! (jam)");
	while(Serial.available()==0){} //tunggu user input jawaban
	waktuMistOn = Serial.parseInt();
	modelKontrol->waktuMistOn = waktuMistOn;

	Serial.println("Masukan kadar waktu Mist Off!! (jam)");
	while(Serial.available()==0){} //tunggu user input jawaban
	waktuMistOff = Serial.parseInt();
	modelKontrol->waktuMistOff = waktuMistOff;



	switch(jawaban){
		case 1:
			cabinet.setRack(1, waktuAeratorOn, waktuAeratorOff, waktuLampOn, waktuLampOff, kadarPpm, waktuMistOn, waktuMistOff);
			saveKontrol(modelKontrol);
			loadKontrol(modelKontrol, &cabinet);
			break;
		case 2:
			cabinet.setRack(2, waktuAeratorOn, waktuAeratorOff, waktuLampOn, waktuLampOff, kadarPpm, waktuMistOn, waktuMistOff);
			saveKontrol(modelKontrol);
			loadKontrol(modelKontrol, &cabinet);
			break;
		case 3:
			cabinet.setRack(3, waktuAeratorOn, waktuAeratorOff, waktuLampOn, waktuLampOff, kadarPpm, waktuMistOn, waktuMistOff);
			saveKontrol(modelKontrol);
			loadKontrol(modelKontrol, &cabinet);
			break;
		case 4:
			cabinet.setRack(4, waktuAeratorOn, waktuAeratorOff, waktuLampOn, waktuLampOff, kadarPpm, waktuMistOn, waktuMistOff);
			saveKontrol(modelKontrol);
			loadKontrol(modelKontrol, &cabinet);
			break;
		default:
			break;
	}
}
void menu(int command, ModelKontrol *modelKontrol){

	/**
	 * FUNGSI INI AKAN MENAMPILKAN MENU PADA SERIAL MONITOR PLATFORM IO MAUPUN
	 * ARDUINO IDE KETIKA SERIAL AVAILABLE.
	 *
	 * ADA 4 BUAH PILIHAN
	 * 	1. MENU INFO -> DAPAT DIGUNAKAN UNTUK MELIHAT STATUS MASING MASING RAK,
	 * 					UNTUK LEBIH LANJUT LIHAT FUNGSI VOID menuInfo()
	 *	2. GET INFO	-> 	PILIHAN INI MASIH BELUM DI REALISASIKAN, RENCANANYA
	 *					KETIKA PILIHAN INI DIPILIH ARDUINO AKAN MENGIRIMKAN SELURUH
	 *					DATA DALAM FORMAT JSON KE NODEMCU, LALU DARI NODE MCU
	 *					AKAN DIKIRIMKAN KE SERVER. (FUNGSI INI BELUM DI REALISASIKAN
	 *					KARENA WAKTU MAGANG YANG KURANG).
	 *	3.SETTING RAK->	PILIHAN INI DAPAT DIGUNAKAN UNTUK MENGATUR SETTING MASING-MASING RAK
	 *	4.SEE KONTROL->	PILIHAN INI DAPAT DIGUNAKAN UNTUK MELIHAT SETTING YANG TELAH TERSIMPAN PADA EEPROM
	 *
	 *
	 * */

	switch(command){
		case 1:
			menuInfo();
			break;
		case 2:
			Serial.println("TODO....");
			break;
		case 3:
			menuSettingRak(modelKontrol);
			break;
		case 4:
			seeKontrol(modelKontrol);
			break;
		default :
			Serial.println("========================================================");
			Serial.println("PENGGUNAAN MENU (masukan angka menu lalu enter)");
			Serial.println("1. info");
			Serial.println("2. get data");
			Serial.println("3. setting rak");
			Serial.println("4. lihat setting");
			Serial.println("contoh: 1 ");
			Serial.println("========================================================");
//			menu(command);
			break;
	}
}



void saveKontrol(ModelKontrol* modelKontrol){

	/**
	 * FUNGSI INI AKAN MENYIMPAN DATA SETTING YANG TELAH DIMASUKAN USER KE MEMORI
	 * EEPROM, SEHINGGA KETIKA POWER DIMATIKAN DATA SETTING TIDAK HILANG
	 *
	 * NOTE:
	 * 	SENAJA TIDAK MEMAKAI FUNGSI FOR, DIKARENAKAN BELUM TERBIASA MEMAKAI ENUM.
	 *
	 * */

	setVar(aerator1WaktuOn, 	modelKontrol->aerator[0].waktuOn);
	setVar(aerator1WaktuOff, 	modelKontrol->aerator[0].waktuOff);
	setVar(aerator2WaktuOn, 	modelKontrol->aerator[1].waktuOn);
	setVar(aerator2WaktuOff, 	modelKontrol->aerator[1].waktuOff);
	setVar(aerator3WaktuOn, 	modelKontrol->aerator[2].waktuOn);
	setVar(aerator3WaktuOff, 	modelKontrol->aerator[2].waktuOff);
	setVar(aerator4WaktuOn, 	modelKontrol->aerator[3].waktuOn);
	setVar(aerator4WaktuOff, 	modelKontrol->aerator[3].waktuOff);

	setVar(light1WaktuOn, 	modelKontrol->lamp[0].waktuOn);
	setVar(light1WaktuOff, 	modelKontrol->lamp[0].waktuOff);
	setVar(light2WaktuOn, 	modelKontrol->lamp[1].waktuOn);
	setVar(light2WaktuOff, 	modelKontrol->lamp[1].waktuOff);
	setVar(light3WaktuOn, 	modelKontrol->lamp[2].waktuOn);
	setVar(light3WaktuOff, 	modelKontrol->lamp[2].waktuOff);
	setVar(light4WaktuOn, 	modelKontrol->lamp[3].waktuOn);
	setVar(light4WaktuOff, 	modelKontrol->lamp[3].waktuOff);

	modelKontrol->ppm[0]>=1000 ? setVar(ppm1, modelKontrol->ppm[0]/100) :  setVar(ppm1, modelKontrol->ppm[0]/10);
	modelKontrol->ppm[1]>=1000 ? setVar(ppm2, modelKontrol->ppm[1]/100) :  setVar(ppm2, modelKontrol->ppm[1]/10);
	modelKontrol->ppm[2]>=1000 ? setVar(ppm3, modelKontrol->ppm[2]/100) :  setVar(ppm3, modelKontrol->ppm[2]/10);
	modelKontrol->ppm[3]>=1000 ? setVar(ppm4, modelKontrol->ppm[3]/100) :  setVar(ppm4, modelKontrol->ppm[3]/10);

	setVar(waktuMistOn, modelKontrol->waktuMistOn);
	setVar(waktuMistOff, modelKontrol->waktuMistOff);

	modelKontrol->ppm[0] >=1000 ? setVar(pengalippm1, 100) : setVar(pengalippm1, 10);
	modelKontrol->ppm[1] >=1000 ? setVar(pengalippm2, 100) : setVar(pengalippm2, 10);
	modelKontrol->ppm[2] >=1000 ? setVar(pengalippm3, 100) : setVar(pengalippm3, 10);
	modelKontrol->ppm[3] >=1000 ? setVar(pengalippm4, 100) : setVar(pengalippm4, 10);
}

void loadKontrol(ModelKontrol* modelKontrol, Cabinet *cabinet){

	/**
	 * FUNGSI INI AKAN MENGAMBIL DATA DARI EEPROM DAN DIMASUKAN KE
	 * VARIABLE modelKontrol dan cabinet, SEHINGGA SISTEM AKAN BERJALAN
	 * SESUAI DENGAN SETTING TERAKHIR YANG USER MASUKAN.
	 * */

	modelKontrol->aerator[0].waktuOn 	= getVar(aerator1WaktuOn);
	modelKontrol->aerator[0].waktuOff 	= getVar(aerator1WaktuOff);
	modelKontrol->aerator[1].waktuOn 	= getVar(aerator2WaktuOn);
	modelKontrol->aerator[1].waktuOff 	= getVar(aerator2WaktuOff);
	modelKontrol->aerator[2].waktuOn 	= getVar(aerator3WaktuOn);
	modelKontrol->aerator[2].waktuOff 	= getVar(aerator3WaktuOff);
	modelKontrol->aerator[3].waktuOn 	= getVar(aerator4WaktuOn);
	modelKontrol->aerator[3].waktuOff 	= getVar(aerator4WaktuOff);

	modelKontrol->lamp[0].waktuOn 	= getVar(light1WaktuOn);
	modelKontrol->lamp[0].waktuOff 	= getVar(light1WaktuOff);
	modelKontrol->lamp[1].waktuOn 	= getVar(light2WaktuOn);
	modelKontrol->lamp[1].waktuOff 	= getVar(light2WaktuOff);
	modelKontrol->lamp[2].waktuOn 	= getVar(light3WaktuOn);
	modelKontrol->lamp[2].waktuOff 	= getVar(light3WaktuOff);
	modelKontrol->lamp[3].waktuOn 	= getVar(light4WaktuOn);
	modelKontrol->lamp[3].waktuOff 	= getVar(light4WaktuOff);

	modelKontrol->waktuMistOn 	= getVar(waktuMistOn);
	modelKontrol->waktuMistOff 	= getVar(waktuMistOff);


	modelKontrol->ppm[0] = getVar(ppm1) * getVar(pengalippm1);
	modelKontrol->ppm[1] = getVar(ppm2) * getVar(pengalippm2);
	modelKontrol->ppm[2] = getVar(ppm3) * getVar(pengalippm3);
	modelKontrol->ppm[3] = getVar(ppm4) * getVar(pengalippm4);

	for(int i=0; i<4; i++){
		cabinet->setRack(i+1, modelKontrol->aerator[i].waktuOn, modelKontrol->aerator[i].waktuOff, modelKontrol->lamp[i].waktuOn, modelKontrol->lamp[i].waktuOff, modelKontrol->ppm[i], modelKontrol->waktuMistOn, modelKontrol->waktuMistOff);
	}

}

void seeKontrol(ModelKontrol *modelKontrol){

	/**
	 * FUNGSI INI AKAN MENAMPILKAN DATA SETTING YANG TERSIMPAN
	 * DI EEPROM DAN MENAMPILKANNYA DI SERIAL MONITOR PLATFORM IO/
	 * ARDUINO IDE
	 * */

	loadKontrol(modelKontrol, &cabinet);
	Serial.println("================================");
	for(int i=0; i<4; i++){
		Serial.print(i+1);
		Serial.print(".Aerator, ON = ");
		Serial.print(modelKontrol->aerator[i].waktuOn);
		Serial.print(", OFF = ");
		Serial.println(modelKontrol->aerator[i].waktuOff);
	}
	Serial.println("================================");
	for(int i=0; i<4; i++){
		Serial.print(i+1);
		Serial.print(".Lamp, ON = ");
		Serial.print(modelKontrol->lamp[i].waktuOn);
		Serial.print(", OFF = ");
		Serial.println(modelKontrol->lamp[i].waktuOff);
	}
	Serial.println("================================");
	for(int i=0; i<4; i++){
		Serial.print(i+1);
		Serial.print(".ppm = ");
		Serial.println(modelKontrol->ppm[i]);
	}
	Serial.print("Mist waktu ON = ");
	Serial.println(modelKontrol->waktuMistOn );
	Serial.print("Mist waktu OFF = ");
	Serial.println(modelKontrol->waktuMistOff);
}


void sendToNode(ModelKontrol *modelKontrol, ModelMonitor *modelMonitor){

	/**
	 * DATA AKAN DIKIRIMKAN KE NODE MCU DALAM FORMAT TABLE HTML
	 * MELALUI KOMUNIKASI SERIAL, DATA DAPAT DIAKSES MELALUI
	 * SEMUA DEVICE YANG TERHUBUNG KE WIFI KANTOR.
	 * IP ADDRESS DAPAT DILIHAT PADA SERIAL MONITOR ARDUINO (IP BERUBAH-UBAH)
	 *
	 * INFO LEBIH LANJUT TENTANG HTML LIHAT:  n7
	 *
	 * */
	String data = "";
//	String tableHeader[7] = {	"RAK",
//								"PPM",
//							 	"SET PPM",
//								"JARAK",
//								"LUX",
//								"LAMP(ON/OFF)",
//								"AERATOR(ON/OFF)"};
	data += "<head><style>\
			table, th, td {\
			  border: 1px solid;\
			  text-align: center;\
			}\
			</style></head>";
	data += "<table>";
//	// ==|RAK|PPM|Set PPM|jarak|Cahaya|LAMPU(on/off)|Aerator(on/off)|==
//
//	// ==HEADER==
	data += "<tr>";


	data += "<th>RAK</th>";
	data += "<th>PPM</th>";
	data += "<th>SET PPM</th>";
	data += "<th>JARAK</th>";
	data += "<th>LUX</th>";
	data += "<th>LAMPU(ON/OFF)</th>";
	data += "<th>AERATOR(ON/OFF)</th>";

	data += "</tr>";

	// ==DATA MASING MASING RAK==

	for(int i=0; i<4; i++){
		data += "<tr style=\"border: 1px solid #990000; \">";

		// NOMOR RAK
		data += "<td> RAK ";
		data += (i+1);
		data += "</td>";

		// PPM
		data += "<td>";
		data += cabinet.getInfoNumPerKomponen(i, 3);
		data += "</td>";

		// SETTING PPM
		data += "<td>";
		data += modelKontrol->ppm[i];
		data += "</td>";

		// JARAK
		data += "<td>";
		data += cabinet.getInfoNumPerKomponen(i, 1);
		data += "</td>";

		// LUX
		data += "<td>";
		data += cabinet.getInfoNumPerKomponen(i, 2);
		data += "</td>";

		// LAMPU (ON/OFF)
		data += "<td>";
		data += modelKontrol->lamp[i].waktuOn;
		data += "/";
		data += modelKontrol->lamp[i].waktuOff;
		data += "</td>";

		// AERATOR (ON/OFF)
		data += "<td>";
		data += modelKontrol->aerator[i].waktuOn;
		data += "/";
		data += modelKontrol->aerator[i].waktuOff;
		data += "</td>";

		data += "</tr>";
	}

	data += "</table>";

	data += "<p>Mist waktu ON = ";
	data += modelKontrol->waktuMistOn;
	data += "</p>";
	data += "<p>Mist waktu OFF = ";
	data += modelKontrol->waktuMistOff;
	data +="</p>";

	// tds mixer
	data += "<p>Nutrisi MIXER: ";
	data += cabinet.getPpm();
	data += "</p>";

	// srf mixer
	data += "<p>Jarak Mixer: ";
	data += cabinet.getRangeMixer();
	data += "</p>";

	// srf pupuk a
	data += "<p>Jarak Pupuk A: ";
	data += cabinet.getRangePpkA();
	data += "</p>";

	// srf pupuk b
	data += "<p>Jarak Pupuk B: ";
	data += cabinet.getRangePpkB();
	data += "</p>";

	data += "<script>setTimeout(()=>{window.location.reload(1);},5000);</script>";

	Serial2.print(data);
	Serial.println("Send TO NODE");


//	data +="<h2>Kontrol</h2>";
//	for(int i=0; i<4; i++){
//		data += "<p>";
//		data += (i+1);
//		data += ".Aerator, ON = ";
//		data += modelKontrol->aerator[i].waktuOn;
//		data += ", OFF = ";
//		data += modelKontrol->aerator[i].waktuOff;
//		data += "</p>";
//	}
//
//
//
//	for(int i=0; i<4; i++){
//		data += "<p>";
//		data += (i+1);
//		data += ".Lamp, ON = ";
//		data += modelKontrol->lamp[i].waktuOn;
//		data += ", OFF = ";
//		data += modelKontrol->lamp[i].waktuOff;
//		data += "</p>";
//	}
//
//	for(int i=0; i<4; i++){
//		data += "<p>";
//		data += (i+1);
//		data += ".ppm = ";
//		data += modelKontrol->ppm[i];
//		data += "</p>";
//	}
//	data += "<p>";
//	data += "Mist waktu ON = ";
//	data += modelKontrol->waktuMistOn;
//	data += "</p>";
//
//	data += "<p>";
//	data += "Mist waktu OFF = ";
//	data += modelKontrol->waktuMistOff;
//	data += "</p>";
//
//	//==============MONITOR====================
//	data += "<h2>Monitor</h2>";
//
//	//1 ping
//	//2 lux
//	//3 tds
//	for(int i=0; i<4; i++){
//		// jarak
//		data += "<p>";
//		data += "Jarak";
//		data += (i+1);
//		data += ": ";
//		data += cabinet.getInfoNumPerKomponen(i, 1);
//		data += "</p>";
//
//		//cahaya
//		data += "<p>";
//		data += "Cahaya";
//		data += (i+1);
//		data += ": ";
//		data += cabinet.getInfoNumPerKomponen(i, 2);
//		data += "</p>";
//
//		//nutrisi
//		data += "<p>";
//		data += "Nutrisi";
//		data += (i+1);
//		data += ": ";
//		data += cabinet.getInfoNumPerKomponen(i, 3);
//		data += "</p>";
//
//	}
//	data += "=======================";
//
//	// tds mixer
//	data += "<p>";
//	data += "Nutrisi MIXER";
//	data += ": ";
//	data += cabinet.getPpm();
//	data += "</p>";
//
//	// srf mixer
//	data += "<p>";
//	data += "Jarak Mixer";
//	data += ": ";
//	data += cabinet.getRangeMixer();
//	data += "</p>";
//
//	// srf pupuk a
//	data += "<p>";
//	data += "Jarak Pupuk A";
//	data += ": ";
//	data += cabinet.getRangePpkA();
//	data += "</p>";
//
//	// srf pupuk b
//	data += "<p>";
//	data += "Jarak Pupuk B";
//	data += ": ";
//	data += cabinet.getRangePpkB();
//	data += "</p>";
//	data += "<script>setTimeout(()=>{window.location.reload(1);},5000);</script>";
//
//	Serial2.print(data);
//	Serial.println("Send TO NODE");


}


//void readKontrol(ModelKontrol* modelKontrol){
//    if (!Serial2.available()) return;
//
//    if (Serial2.read() == '@')
//    {
//
//        String serialString = Serial2.readStringUntil('\n');
//        Serial.println(serialString);
//        int *data = (int*)calloc(13, sizeof(int));
//
//        for (int i = 0; i < 13; i++)
//        {
//            data[i] = serialString.substring(0, serialString.indexOf(',')).toInt();
//            serialString = serialString.substring(serialString.indexOf(',') + 1);
//        }
//
//        if
//        (
//            !(
//                data[0] == modelKontrol->aerator[0] &&
//                data[1] == modelKontrol->aerator[1] &&
//                data[2] == modelKontrol->aerator[2] &&
//                data[3] == modelKontrol->aerator[3] &&
//                data[4] == modelKontrol->light[0] &&
//                data[5] == modelKontrol->light[1] &&
//                data[6] == modelKontrol->light[2] &&
//                data[7] == modelKontrol->light[3] &&
//                data[8] == modelKontrol->ppm[0] &&
//                data[9] == modelKontrol->ppm[1] &&
//                data[10] == modelKontrol->ppm[2] &&
//                data[11] == modelKontrol->ppm[3] &&
//                data[12] == modelKontrol->starth
//            )
//        )
//        {
//            for (int i = 0; i < 4; i++)
//            {
//                modelKontrol->aerator[i] = data[i];
//                modelKontrol->light[i] = data[4+i];
//                modelKontrol->ppm[i] = data[8+i];
//            }
//            modelKontrol->starth = data[12];
//        }
//        free(data);
//
//        saveKontrol(modelKontrol);
//    }
//    else if(Serial2.available()){
//        while (Serial2.available())
//        {
//            Serial.print(Serial2.read());
//        }
//    }
//}

void sendMonitor(ModelMonitor* modelMonitor){
	//1 ping
	//2 lux
	//3 tds
	for(int i=0; i<4; i++){
		modelMonitor->volume[i] = cabinet.getInfoNumPerKomponen(i, 1);
		modelMonitor->cahaya[i] = cabinet.getInfoNumPerKomponen(i, 2);
		modelMonitor->nutrisi[i] = cabinet.getInfoNumPerKomponen(i, 3);
	}

	modelMonitor->suhu = cabinet.getSuhu();
	modelMonitor->kelembapan = cabinet.getHumidity();


    if 
    (
        !(
            modelMonitor->nutrisi[0] == 0 &&
            modelMonitor->nutrisi[1] == 0 &&
            modelMonitor->nutrisi[2] == 0 &&
            modelMonitor->nutrisi[3] == 0 &&
            modelMonitor->cahaya[0] == 0 &&
            modelMonitor->cahaya[1] == 0 &&
            modelMonitor->cahaya[2] == 0 &&
            modelMonitor->cahaya[3] == 0 &&
            modelMonitor->volume[0] == 0 &&
            modelMonitor->volume[1] == 0 &&
            modelMonitor->volume[2] == 0 &&
            modelMonitor->volume[3] == 0 &&
            modelMonitor->volCampuran == 0 &&
            modelMonitor->volTampung == 0 &&
            modelMonitor->volNutrisiA == 0 &&
            modelMonitor->volNutrisiB == 0 &&
            modelMonitor->volMisting == 0 &&
            modelMonitor->suhu == 0 &&
            modelMonitor->kelembapan == 0
        )
    )
    {
        String dummy =  "{\"airTampung\":"+String(modelMonitor->volTampung)+
                        ",\"airNutrisiA\":"+String(modelMonitor->volNutrisiA)+
                        ",\"airNutrisiB\":"+String(modelMonitor->volNutrisiB)+
                        ",\"airMisting\":"+String(modelMonitor->volMisting)+
                        ",\"nutrisi1\":"+String(modelMonitor->nutrisi[0])+
                        ",\"nutrisi2\":"+String(modelMonitor->nutrisi[1])+
                        ",\"nutrisi3\":"+String(modelMonitor->nutrisi[2])+
                        ",\"nutrisi4\":"+String(modelMonitor->nutrisi[3])+
                        ",\"cahaya1\":"+String(modelMonitor->cahaya[0])+
                        ",\"cahaya2\":"+String(modelMonitor->cahaya[1])+
                        ",\"cahaya3\":"+String(modelMonitor->cahaya[2])+
                        ",\"cahaya4\":"+String(modelMonitor->cahaya[3])+
                        ",\"suhu\":"+String(modelMonitor->suhu)+
                        ",\"kelembapan\":"+String(modelMonitor->kelembapan)+
                        ",\"volume1\":"+String(modelMonitor->volume[0])+
                        ",\"volume2\":"+String(modelMonitor->volume[1])+
                        ",\"volume3\":"+String(modelMonitor->volume[2])+
                        ",\"volume4\":"+String(modelMonitor->volume[3])+
                        ",\"volumeCampuran\":"+String(modelMonitor->volCampuran)+"}"+"\n";

        Serial2.print(dummy);
        Serial.println("sent monitor data");
    }
    
}


#endif



