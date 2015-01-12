#include <avr/io.h>
#include <avr/interrupt.h>

uint8_t pwmarray[4] = {1, 10, 50, 200}; //definicja tablicy
uint8_t count = 0; //licznik kolejnych przerwań od timera

void initialisation(void)
{
    DDRC = 0x0F; //ustawienie PC 0, 1,2,3 jako wyjscia, zapis 000011111 szesnastkowo
    TCCR2 |= _BV(CS20); // Ustawienie 8 bitowego timera2 z preskalerem przez 1
    TCCR2 |= _BV(WGM21); // Ustawia timer w tryb CTC
    OCR2 = 50; // Ustawia wartość pożądaną na CTC
    TIMSK |= _BV(OCIE2); // Zezwolenie na przerwania dla CTC
    sei(); // Zezwolenie globalne na przerwania

}
int main (void)
{
    initialisation();
    while(1)
    {

    }

}

ISR(TIMER2_COMP_vect) //przerwanie od timera2
{
    uint8_t i;
    for (i = 0; i < 4; ++i) //wykonanie dla kazdego elemntu tablicy i przy okazji kazdego portu po kolei
    {
        if (count >= pwmarray[i]) 
        {
            if (count < 255) //jesli licznik mniejszy niz 255 i wiekszy od wartosci w tablicy to porty są na stan niski
            {                
                PORTC &= ~(_BV(i)); //porty startuja od PC0
            }
        }    
    }
    if (count == 255) //zerowanie licznika
    {
        count = 0;
        PORTC |= 0x0F; //jeśli równy 255 to porty sa na stan wysoki wlaczane
    }
    count++;    
}
