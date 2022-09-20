#include <Arduino.h>
#ifndef Model
#define Model


typedef struct waktuRelay{
	int waktuOn, waktuOff;
};

struct ModelKontrol{
	/**
	 * rentang nilai PPM = 500 hingga 2000, dengan kenaikan 200PPM
	 * */
	waktuRelay 	aerator[4],
				lamp[4];
	int ppm[4],waktuMistOn, waktuMistOff;


//    int aerator[4],
//        light[4],
//        ppm[4],
//        starth;
};

struct ModelMonitor
{
    int nutrisi[4],
        cahaya[4],
        volume[4],
        volCampuran,
        volTampung,
        volNutrisiA,
        volNutrisiB,
        volMisting,
        suhu,
        kelembapan;
};

#endif
