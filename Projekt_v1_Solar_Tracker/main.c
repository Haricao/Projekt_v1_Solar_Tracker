#include "MKL05Z4.h"
#include "ADC.h"
#include "frdm_bsp.h"
#include "lcd.h"
#include "tmp_1.h"
#include "tpm_0.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

float volt_coeff = ((float)(((float)2.91) / 4095) );			// Wspólczynnik korekcji wyniku, w stosunku do napiecia referencyjnego przetwornika
uint8_t zmienne [4] = {0, 2, 3 ,7};
uint8_t n = 0;
uint16_t temp;
float wynik [4];
float czujnik_1 = 0;
float czujnik_2 = 0;
float czujnik_3 = 0;
float czujnik_4 = 0;
int Y_AXIS = 0;
static uint8_t msTicks = 0;
long int  fluent_tpm0 = 1000;
long int  fluent_tpm1 = 1550;



void SysTick_Handler(void);
void lcd_static(void);
void lcd_update(void);


void ADC0_IRQHandler()
{	
	temp = ADC0->R[0];		// Odczyt danej i skasowanie flagi COCO
	wynik[n] = temp;				// Wyslij wynik do petli glównej
		
	if (n<3)
		{
			n = n + 1;
		}
	else { n = 0; }
	
	ADC0->SC1[0] = ADC_SC1_AIEN_MASK | ADC_SC1_ADCH(zmienne[n]);
	NVIC_ClearPendingIRQ(ADC0_IRQn);
}

int main (void)
{
	
	char display[]={0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20};
	LCD1602_Init();		 // Inicjalizacja wyswietlacza LCD
	
	LCD1602_Backlight(TRUE);
	LCD1602_SetCursor(0,1);
	
	SysTick_Config(1000000); 				
	/* initialize system timer */
	TPM1_Init_PWM();
	TPM0_Init_PWM();
	
	
	ADC_Init();		// Inicjalizacja przetwornika A/C

	
	
	ADC0->SC1[0] = ADC_SC1_AIEN_MASK | ADC_SC1_ADCH(0); 		// Pierwsze wyzwolenie przetwornika ADC0 i odblokowanie przerwania
	
	while(1)
	{			
			//Przypisanie wyników pomiaru pokorekcji wyniku
			czujnik_1 = wynik[3]*volt_coeff;
			czujnik_2 = wynik[0]*volt_coeff;
			czujnik_3 = wynik[2]*volt_coeff;
			czujnik_4 = wynik[1]*volt_coeff;
			/*
			Ulozenie: 
			2				4
		
			
			1				3
			*/
			// Wyswietlanie danech z czujników 
			sprintf(display,"%.3fV",czujnik_1);
			LCD1602_SetCursor(0,0);
			LCD1602_Print(display);
			sprintf(display,"%.3fV",czujnik_2);
			LCD1602_SetCursor(10,0);
			LCD1602_Print(display);
			sprintf(display,"%.3fV",czujnik_3);
			LCD1602_SetCursor(0,1);
			LCD1602_Print(display);
			sprintf(display,"%.3fV",czujnik_4);
			LCD1602_SetCursor(10,1);
			LCD1602_Print(display);
			
			if(czujnik_1>2.2&&czujnik_3>2.2)
			{
				
				for(fluent_tpm0 = fluent_tpm0; fluent_tpm0 < 2000; fluent_tpm0++)
				{
					DELAY(1);
					TPM0_SetVal(fluent_tpm0);
				}
				fluent_tpm1 = czujnik_4 * 1000;
				while (fluent_tpm1 > 1100 && fluent_tpm1 < 1500){
						DELAY(1);
						fluent_tpm1 = wynik[1]*volt_coeff * 1000;
						TPM1_SetVal( fluent_tpm1);		
					}
			}
			else if(czujnik_2>2.2&&czujnik_4>2.2)
			{
				for(fluent_tpm0 = fluent_tpm0; fluent_tpm0 < 2000; fluent_tpm0++)
				{
					DELAY(1);
					TPM0_SetVal(fluent_tpm0);
				}
				fluent_tpm1 = czujnik_3 * 1000;
				while (fluent_tpm1 > 1100 && fluent_tpm1 < 1500){
						DELAY(1);
						fluent_tpm1 = wynik[2]*volt_coeff * 1000;
						TPM1_SetVal( fluent_tpm1 + 400 );		
					}
			}
			/*else if(czujnik_4>2.2&&czujnik_3>2.2)
			{
				for(fluent_tpm0 = fluent_tpm0; fluent_tpm0 > 999; fluent_tpm0--)
				{
					DELAY(1);
					TPM0_SetVal(fluent_tpm0);
				}
				if(czujnik_1<1||czujnik_2<1)
					{
						for(fluent_tpm1 = fluent_tpm1; fluent_tpm1 < 1800; fluent_tpm1++)
						{
							DELAY(1);
							TPM1_SetVal(fluent_tpm1);
						}
					}
			}
			else if(czujnik_1>2.2&&czujnik_2>2.2)
			{
				for(fluent_tpm0 = fluent_tpm0; fluent_tpm0 > 999; fluent_tpm0--)
				{
					DELAY(1);
					TPM0_SetVal(fluent_tpm0);
				}
				if(czujnik_4<1||czujnik_3<1)
					{
						for(fluent_tpm1 = fluent_tpm1; fluent_tpm1 > 1300; fluent_tpm1--)
						{
							DELAY(1);
							TPM1_SetVal(fluent_tpm1);
						}
					}
			}*/
			
			else
				{
					TPM0_SetVal(fluent_tpm0);
					TPM1_SetVal(fluent_tpm1);
				}
	}
}

void SysTick_Handler(void) {
	msTicks++;
}
