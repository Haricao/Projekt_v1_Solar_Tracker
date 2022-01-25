#include "tpm_0.h"


void TPM0_IRQHandler(void);
static uint8_t tpm0Enabled = 0;
/*
	TPM0 Odpowiedzialny za inicjalizacje PWM-a obslugujacego dolny serwomechanizm
*/
void TPM0_Init_PWM(void) {
		
  SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK;		
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);  

	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK; // Port B podlaczony do zegara
	PORTB->PCR[11] = PORT_PCR_MUX(2);		// PTB11 Wyjscie PWM - Multiplekser na 2 
	

	TPM0->SC |= TPM_SC_PS(5);  					// Prescaler na 32 
	TPM0->SC |= TPM_SC_CMOD(1);					// 01 LPTPM counter increments on every LPTPM counter clock

	TPM0->MOD = 13334*2; 							// Ustawienie odpowiedniej wartosci modulo 
																		// ok 50Hz				
	

	TPM0->SC &= ~TPM_SC_CPWMS_MASK; 		
	TPM0->CONTROLS[0].CnSC |= (TPM_CnSC_MSB_MASK | TPM_CnSC_ELSB_MASK); //MSB = 1 ELSB = 1 Zbocze narastajace

	TPM0->CONTROLS[0].CnV = 1000; 				// Wartosc poczatkowa po kalibracji

	tpm0Enabled = 1;  //Lokalna zmienna informujaca o zakonczeniu kalibracji
}


void TPM0_SetVal(uint32_t value) {
	if (tpm0Enabled) TPM0->CONTROLS[0].CnV = value;    // Wczytanie aktualnej wartosci PWM na kanal 1 tpm1
}

