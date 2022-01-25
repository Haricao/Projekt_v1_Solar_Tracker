#include "MKL05Z4.h"
//----------------------------------------------
// ADC0_CFG1
//----------------------------------------------
// Podzielnik zegara
#define ADIV_1            0x00
#define ADIV_4            0x02
// Rozdzielczosc (single ended).
#define MODE_12           0x01
//Zegar taktujacy
#define ADICLK_BUS_2      0x01

uint8_t ADC_Init(void);
