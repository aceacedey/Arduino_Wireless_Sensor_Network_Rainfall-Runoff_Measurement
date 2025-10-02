#include<avr/interrupt.h>

int status = 0; // variable to hold LED status
int count = 0;  // to count number of interrupts

ISR(TIMER0_OVF_vect)
{
    count++;                
    if( count == 3921 ) // see notes below
    {
        status = !status; // toggle status
        count = 0;          // reset count
    }

     PORTD = status; // write new status to PortD
}

int main()
{
    TCCR0 = 0x01;
    TCNT0 = 0x00;
    TIMSK = 0x01;

    DDRD = 0x01; // set bit0 of PortD as output
    sei(); // enable ALL interrupts
    for(;;); // loop forever
             //interrupts will handle the rest
    return 0;
}