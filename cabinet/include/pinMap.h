/**
 * pinMap.h BERISI ALTERNATIF NAMA KONEKSI PIN ARDUINO
 * UNTUK INFO LEBIH LANJUT LIHAT SKEMA DAN BOARD (.pdf)
 * */

#ifndef PINMAP
#define PINMAP

// set ppm mixer
#define SET_PPM_MIXER 	600


// modul relay 16 channel
#define P_PPKA 			38
#define P_AIRBAKU 		39
#define FANMIST 		40
#define P_PPKB 			41

#define FILLERSL1 		43
#define FILLERSL2 		42
#define FILLERSL3 		45
#define FILLERSL4 		44

#define AERATORSL1		47
#define AERATORSL2 		46
#define AERATORSL3 		49
#define AERATORSL4 		48

#define DRAINERSL1		51
#define DRAINERSL2		50
#define DRAINERSL3		53
#define DRAINERSL4		52

// modul ssr 8 channel (hanya digunakan 5 channel)

#define SSRLAMP1 		24
#define SSRLAMP2		23
#define SSRLAMP3		25
#define SSRLAMP4		27
#define SSRMIST			22

// SENSOR SECTION
// sensor yang ada di rak 0
#define RAK0_TMIST 		29
#define RAK0_EMIST		28
#define RAK0_TPPKA		33	
#define RAK0_EPPKA		32
#define RAK0_TPPKB		31
#define RAK0_EPPKB		30
#define RAK0_TMIXER		37
#define RAK0_EMIXER		36
#define RAK0_TBAKU		35 // note: bak air BAKU di skema disebut TANKI
#define RAK0_EBAKU		34 // note: bak air BAKU di skema disebut TANKI
#define RAK0_TDSMIXER 	A8
#define RAK0_TDSMIXERP	A9
#define RAK0_FLOWBAKU	2
#define RAK0_FLOWPPKA	18
#define RAK0_FLOWPPKB	19

// sensor yang ada di rak 1
#define RAK1_TRIG		6
#define RAK1_ECHO		5
#define RAK1_TDS		A0
#define RAK1_TDSP		A1
#define RAK1_LUX 		A10


// sensor yang ada di rak 2
#define RAK2_TRIG		8
#define RAK2_ECHO		7
#define RAK2_TDS		A2
#define RAK2_TDSP		A3
#define RAK2_LUX 		A11

// sensor yang ada di rak 3
#define RAK3_TRIG		10
#define RAK3_ECHO		9
#define RAK3_TDS		A4
#define RAK3_TDSP		A5
#define RAK3_LUX 		A12

// sensor yang ada di rak 4
#define RAK4_TRIG		12
#define RAK4_ECHO		11
#define RAK4_TDS		A6
#define RAK4_TDSP		A7
#define RAK4_LUX 		A13

#define PIN_DHT22		3

#endif
