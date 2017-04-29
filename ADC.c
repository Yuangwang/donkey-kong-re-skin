// put implementations for functions, explain how it works
// Chris Reedy and Wei Wang

#include <stdint.h>
#include "tm4c123gh6pm.h"

// ADC initialization function 
// Input: none
// Output: none
void ADC_Init(void){
 SYSCTL_RCGCGPIO_R |= 0x10;
	while((SYSCTL_PRGPIO_R & 0x10) == 0){
	}
	GPIO_PORTE_DIR_R &= ~0x14;	//make PE2 input
	GPIO_PORTE_AFSEL_R |= 0x14;	//enable alternate function
	GPIO_PORTE_DEN_R &= ~0x14;	//disable digital i/o
	GPIO_PORTE_AMSEL_R |= 0x14;	//enable analog function on PE4
	SYSCTL_RCGCADC_R |= 0x01;	//enable adc clock
	uint32_t delay = SYSCTL_RCGCADC_R;	//delay
	delay = SYSCTL_RCGCADC_R;	//delay
	delay = SYSCTL_RCGCADC_R;	//delay
	delay = SYSCTL_RCGCADC_R;	//delay
	ADC0_PC_R &= ~0xF;              // 8) clear max sample rate field
	ADC0_PC_R = 0x01;	// set 125khz conversion speed
	ADC0_SSPRI_R = 0x3210;	// set sequencer priority	//was 0123
	ADC0_ACTSS_R &= ~0x000C;        // 10) disable sample sequencer 2 AND 3
	ADC0_EMUX_R &= ~0x0F000;	// set software start trigger event
	ADC0_SSMUX3_R &= ~0x0007; // clear SS3 field
  ADC0_SSMUX3_R += 1;  // set channel 1
	ADC0_SSMUX2_R = 0x0089;         // 12) set channels for SS2
  ADC0_SSCTL2_R = 0x0060;         // 13) no TS0 D0 IE0 END0 TS1 D1, yes IE1 END1
	ADC0_SSCTL3_R = 0x0006; //sample control bits
	ADC0_IM_R &= ~0x000C;	//Disable interrupts
	ADC0_ACTSS_R |= 0x000C;	//enable selected sequencer 2 and 3
	
//	init for second ADC sequencer 2 in channel 9 PE4
/*	ADC0_ACTSS_R &= ~0X04;	//Disables sequencer for the init
	ADC0_EMUX_R &= ~0x0F00;	//set software start for sequencer 2
	ADC0_SSMUX2_R &= ~0x0F;	
	ADC0_SSMUX2_R += 9;	//set channel 9
	ADC0_SSCTL2_R = 0x06;	//sample control bits
	ADC0_IM_R &= ~0x04;	//Disable interrupts
	ADC0_ACTSS_R |= 0x04;	//enable selected sequencer 2
	
	ADC0_SAC_R= 0x04;
*/
}

//------------ADC_In------------
// Busy-wait Analog to digital conversion
// Input: none
// Output: 12-bit result of ADC conversion
uint32_t x_ADC_In(void){  
  uint32_t data;
	ADC0_PSSI_R = 0x0008;
	while ((ADC0_RIS_R&0x08) == 0){
	}
	data = ADC0_SSFIFO3_R&0xFFF;
	ADC0_ISC_R = 0x0008;
  return data;
}

uint32_t y_ADC_In(void){  
  uint32_t data;
	ADC0_PSSI_R = 0x0004;
	while ((ADC0_RIS_R&0x04) == 0){
	}
	data = ADC0_SSFIFO2_R&0xFFF;
	ADC0_ISC_R = 0x0004;
  return data;
}
