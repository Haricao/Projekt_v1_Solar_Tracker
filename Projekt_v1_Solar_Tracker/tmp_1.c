#include "tmp_1.h"

void TPM1_IRQHandler(void);
static uint8_t tpm0Enabled = 0;

/*
	TPM1 Odpowiedzialny za inicjalizacje PWM-a obslugujacego górny serwomechanizm
*/
void TPM1_Init_PWM(void) {
		
  SIM->SCGC6 |= SIM_SCGC6_TPM1_MASK;		 
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);  

	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK; // Port B podlaczony do zegara
	PORTB->PCR[13] = PORT_PCR_MUX(2);		// PTB13 Wyjscie PWM - Multiplekser na 2 

	TPM1->SC |= TPM_SC_PS(5);  					// Prescaler na 32 
	TPM1->SC |= TPM_SC_CMOD(1);					// 01 LPTPM counter increments on every LPTPM counter clock

	TPM1->MOD = 13334*2; 								// Ustawienie odpowiedniej wartosci modulo 
																			// ok 50Hz	
	
	TPM1->SC &= ~TPM_SC_CPWMS_MASK; 		
	TPM1->CONTROLS[1].CnSC |= (TPM_CnSC_MSB_MASK | TPM_CnSC_ELSB_MASK); //MSB = 1 ELSB = 1 Zbocze narastajace



	TPM1->CONTROLS[1].CnV = 1550;; 				// Wartosc poczatkowa po kalibracji

	tpm0Enabled = 1;  //Lokalna zmienna informujaca o zakonczeniu kalibracji
}


void TPM1_SetVal(uint32_t value) { 			
	if (tpm0Enabled) TPM1->CONTROLS[1].CnV = value;    // Wczytanie aktualnej wartosci PWM na kanal 1 tpm1
}

