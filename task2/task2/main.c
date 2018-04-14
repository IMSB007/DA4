
#define  F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

volatile uint8_t ADCvalue; // Global variable, set to volatile if used with ISR


ISR(ADC_vect)
{
	ADCvalue = ADCH; // only need to read the high value for 8 bit
}
void T0Delay()
{
	// Load initial count value
	TCNT0 = 0;
	// Load compare match value
	OCR0A = ADCvalue;
	// Init timer mode and prescaler
	TCCR0A = (1 << WGM01);
	TCCR0B = (1 << CS02) | ( 1 << CS00);
	// Wait until timer 0 compare match
	while ((TIFR0 & (1 << OCF0A)) == 0);
	// Stop timer 0
	TCCR0B = 0;
	// Clear compare math flag
	TIFR0 = (1 << OCF0A);
}
int main(void)
{
	ADMUX = 0; // use ADC0
	ADMUX |= (1 << REFS0); // use AVcc as the reference
	ADMUX |= (1 << ADLAR); // Right adjust for 8 bit resolution
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // 128 prescale for 16Mhz
	ADCSRA |= (1 << ADATE); // Set ADC Auto Trigger Enable
	ADCSRB = 0; // 0 for free running mode
	ADCSRA |= (1 << ADEN); // Enable the ADC
	ADCSRA |= (1 << ADIE); // Enable Interrupts
	ADCSRA |= (1 << ADSC); // Start the ADC conversion
	DDRB = 0xFF;
	sei();
	while (1)
	{
		PORTB = (1<<PB0);
		T0Delay();
		PORTB = (1<<PB1);
		T0Delay();
		PORTB = (1<<PB2);
		T0Delay();
		PORTB = (1<<PB3);
		T0Delay();
	}
}

