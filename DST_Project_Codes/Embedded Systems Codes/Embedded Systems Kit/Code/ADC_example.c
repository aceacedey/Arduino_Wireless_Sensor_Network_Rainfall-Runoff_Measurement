/*
    ADC with Potentiometer example

*/

#include <avr/io.h>
#include <avr/interrupt.h>

ISR(ADC_vect){
    PORTD = ADCH;               //output ADCH to PORTD
    ADCSRA |= 1<<ADSC;          //Start a new conversion

}

int main(void){
    DDRD = 0xFF;
    DDRA = 0x00;

    ADCSRA = 0x8F;          //enable ADC and interrupt feature, prescaler to 128
    ADMUX = 0xE0;           //Vref = internal voltage, left justify and select ADC0 pin

    sei();                  //enable global interrupt

    ADCSRA |= 1<<ADSC;

    while(1);               //Sit in while forever
}