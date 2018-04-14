
#define F_CPU 8000000UL
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint8_t ADCvalue; // Global variable, set to volatile if used with ISR
ISR(INT0_vect)
{
		PORTC ^= 0xff;
	_delay_ms(500);	
}
ISR(ADC_vect)
{
	ADCvalue = ADCH; // only need to read the high value for 8 bit
}

int main(void)
{
    /* Write a code to interface DC motor with L293D.
	   Motor is connected to PC3 and enable is PC0. To
	   Rotate we want en H and m3 L  */
	    //ADC prescaler 64, enable ADC,Start conversion,enable auto trigger
	ADMUX = 0; // use ADC0
	ADMUX |= (1 << REFS0); // use AVcc as the reference
	ADMUX |= (1 << ADLAR); // Right adjust for 8 bit resolution
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // 128 prescale for 16Mhz
	ADCSRA |= (1 << ADATE); // Set ADC Auto Trigger Enable
	ADCSRB = 0; // 0 for free running mode
	ADCSRA |= (1 << ADEN); // Enable the ADC
	ADCSRA |= (1 << ADIE); // Enable Interrupts
	ADCSRA |= (1 << ADSC); // Start the ADC conversion
	DDRB = 0xFF;	//set PortB to Output
	DDRD = 0;	
	DDRC = 0xFF;	
	TCCR1A |= (1 << COM1A1);	// set non-inverting mode a
	TCCR1A |= (1 << WGM11);//|(1 << WGM13);
	TCCR1B |= (1 << WGM12)|(1 << WGM13);	// set Fast PWM mode using ICR1 as TOP
	TCCR1B |= (1<<CS10);
	ICR1 = 0x3ff;
	EIMSK |= (1<<INT0); //enable INT0
	//External Interrupt Control Register
	EICRA |= (0<<ISC00)|(1<<ISC01);  //rising-edge trigger
	
    sei();
	
	

	while (1) 
    { 
		OCR1A = ADCvalue;
    }
}
