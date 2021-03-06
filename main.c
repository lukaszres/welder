#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define LED1 (1<<PB0)

#define LED1_ON PORTB |= LED1
#define LED1_OFF PORTB &= ~LED1
#define LED1_TOG PORTB ^= LED1

void blink(unsigned int ms);
void weld(unsigned int ms);
void wait(unsigned int ms);

volatile int a = 3;
volatile char flag = 0, flag1 = 0;

int main(void) {
	DDRC |= (1 << PC5); //ustawienie PC5 jako wyjscie
	DDRB |= (1 << PB0);  // ustawienie PB0 jako wyjscie
	PORTD |= (1 << PD2); //Podciagniecie Vcc do PD2
	PORTD |= (1 << PD3); //Podciagniecie Vcc do PD3
	MCUCR |= (1 << ISC01);
	MCUCR |= (1 << ISC11);
	MCUCR &= ~(1 << ISC10); //Narastaj�ce zbocze na INT1 generuje przerwanie
	GICR |= (1 << INT0); //włączenie przerwania INT0
	GICR |= (1 << INT1);
	sei();

	while (1) {
		blink(a);

		if (flag) {
			if (a < 5)
				a++;
			else
				a = 1;
			flag = 0;
		}
	}
}

void blink(unsigned int ms) {
	for (int i = 0; i < ms; i++) {
		LED1_ON;
		wait(250);
		LED1_OFF;
		wait(250);
	}
	wait(1000);
}
void weld(unsigned int ms) {
	PORTC |= (1 << PC5); //zaświecenie LEDów
	for (unsigned int i = 0; i < ms; i++)
		_delay_ms(10);
	PORTC &= ~(1 << PC5); //zgaszenie  LEDów
	_delay_ms(2000);
}
void wait(unsigned int ms) {
	for (unsigned int i = 0; i < ms; i++) {
		if (flag1) {
			weld(a);
			flag1 = 0;
		}
		_delay_ms(1);
	}
	flag1 = 0;
}
ISR(INT0_vect) {
	flag = 1;
}
ISR (INT1_vect) {
	flag1 = 1;
}
