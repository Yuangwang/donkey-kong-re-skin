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
	GPIO_PORTE_DIR_R &= ~0x04;	//make PE2 input
	GPIO_PORTE_AFSEL_R |= 0x04;	//enable alternate function
	GPIO_PORTE_DEN_R &= ~0x04;	//disable digital i/o
	GPIO_PORTE_AMSEL_R |= 0x04;	//enable analog function on PE4
	SYSCTL_RCGCADC_R |= 0x01;	//enable adc clock
	uint32_t delay = SYSCTL_RCGCADC_R;	//delay
	delay = SYSCTL_RCGCADC_R;	//delay
	delay = SYSCTL_RCGCADC_R;	//delay
	delay = SYSCTL_RCGCADC_R;	//delay
	ADC0_PC_R = 0x01;	// set 125khz conversion speed
	ADC0_SSPRI_R = 0x0123;	// set sequencer priority
	ADC0_EMUX_R &= ~0xF000;	// set software start trigger event
	ADC0_SSMUX3_R &= ~0x0007; // clear SS3 field
  ADC0_SSMUX3_R += 1;  // set channel 1
	ADC0_SSCTL3_R = 0x0006; //sample control bits
	ADC0_IM_R &= ~0x0008;	//Disable interrupts
	ADC0_ACTSS_R |= 0x0008;	//enable selected sequencer 3
}

//------------ADC_In------------
// Busy-wait Analog to digital conversion
// Input: none
// Output: 12-bit result of ADC conversion
uint32_t ADC_In(void){  
  uint32_t data;
	ADC0_PSSI_R = 0x0008;
	while ((ADC0_RIS_R&0x08) == 0){
	}
	data = ADC0_SSFIFO3_R&0xFFF;
	ADC0_ISC_R = 0x0008;
  return data;
}