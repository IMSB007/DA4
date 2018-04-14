#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
volatile uint8_t ADCvalue;
ISR(ADC_vect)
{
	ADCvalue = ADCH; // only need to read the high value for 8 bit
}
int main(void)
{
	DDRB = 0xFF;
	TCCR1A |= (1<<WGM11)|(1<<COM1A1);
	TCCR1B |= (1<<WGM12)|(1<<WGM13)|(1<<CS11);
	ICR1 = 4165;
	ADMUX = 0; // use ADC0
	ADMUX |= (1 << REFS0); // use AVcc as the reference
	ADMUX |= (1 << ADLAR); // Right adjust for 8 bit resolution
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
	ADCSRA |= (1 << ADATE); // Set ADC Auto Trigger Enable
	ADCSRB = 0; // 0 for free running mode
	ADCSRA |= (1 << ADEN); // Enable the ADC
	ADCSRA |= (1 << ADIE); // Enable Interrupts
	ADCSRA |= (1 << ADSC); // Start the ADC conversion
	DDRB = 0xFF;
	sei();
	while (1)
	{
		OCR1A = ADCvalue;
		_delay_ms(10);
	}
}
