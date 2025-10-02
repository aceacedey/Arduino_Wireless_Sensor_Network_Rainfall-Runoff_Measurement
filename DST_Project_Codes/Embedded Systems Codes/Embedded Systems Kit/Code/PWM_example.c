/*
 PWM.c
 
 Controls three PWM pins and changes the duty cycle of all three at different rates.
 In this example, an RGB LED was wired to the PWM pins to create many different colors.
 */

#include <avr/io.h>

#define LED PB3

void delay_ms(uint16_t ms) {
    uint16_t delay_count = 1000000 / 17500;
    volatile uint16_t i;
    
    while (ms != 0) {
        for (i=0; i != delay_count; i++);
        ms--;
    }
}

int main(void) {
    int8_t i;
    int8_t change1;
    int8_t change2;
    int8_t change3;

    DDRD = _BV(6) | _BV(5);
    DDRB = _BV(3);

    TCCR0A = _BV(WGM01) | _BV(WGM00) | _BV(COM0A1) | _BV(COM0B1);
    TCCR0B = _BV(CS01);
    TCCR2A = _BV(WGM21) | _BV(WGM20) | _BV(COM2A1) | _BV(COM2B1);
    TCCR2B = _BV(CS21);

    OCR0A = 0;
    OCR0B = 0;
    OCR2A = 0;
    change1 = 1;
    change2 = 2;
    change3 = 3;
    
    for(;;) {
        delay_ms(3);
		OCR0A += change1;
		OCR0B += change2;
		OCR2A += change3;
		if(OCR0A>=255 | OCR0A<=0){change1 *= -1;}
		if(OCR0B>=255 | OCR0B<=0){change2 *= -1;}		
		if(OCR2A>=255 | OCR2A<=0){change3 *= -1;}	
    }
    return 0;
}