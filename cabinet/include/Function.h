#include <Arduino.h>
#include "Model.h"
#include "Class.h"

#ifndef Function
#define Function

void interrupt1();
void interrupt2();
void interrupt3();

int stoi(String s);
void penstabilPPM(float jarak, float ppm, int rack);

/********** eeprom **********/
void setVar(int address, int data);
void setVar2(int address1, int address2, int data);
int getVar(int address);
int getVar2(int address1, int address2);
void saveKontrol(ModelKontrol* modelKontrol); // save model to eeprom
void loadKontrol(ModelKontrol* modelKontrol, Cabinet *cabinet); // load model from eeprom

/********** communication **********/
void readKontrol(ModelKontrol* modelKontrol);
void sendMonitor(ModelMonitor* modelMonitor);
void sendToNode(ModelKontrol* modelKontrol, ModelMonitor *modelMonitor);

void menuInfo();
void menuSettingRak(ModelKontrol *modelKontrol);
void menu(int command, ModelKontrol *modelKontrol);
void kontrolerRack();
void seeKontrol(ModelKontrol *modelKontrol);

#endif
