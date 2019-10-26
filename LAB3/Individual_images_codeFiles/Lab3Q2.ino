//Q2
//Q3
#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#define UART_BAUDRATE 9600
#define BAUD_PRESCALER ((F_CPU / (UART_BAUDRATE * 16UL)) - 1)

#include <avr/io.h>
#include <stdlib.h>
#include <string.h>

/* Forward declaration of functions from personal usart library. */
void usart_init(void);
unsigned char usart_read_char(void);
void usart_send_char(unsigned char data);
char* usart_read(void);
void usart_print(char* data);
void usart_println(char* data);

void setup(void) {
    cli();
    /*
     * LEFT MOTOR configuration
     * PD6 - OC0A out     -- 1,2 EN
     * PD4 - digital out  -- 1A CW    -- Backwards
     * PD2 - digital out  -- 2A CCKW  -- Forward
    */
    DDRD |= (1 << PD6) | (1 << PD4) | (1 << PD2);

    /*
     * Timer 0 config with 2 registers A & B
     * TCCR0A - Set Clear OC0B on Compare Match & Phase Correct PWM Mode 5
     * TCCR0B - Set prescaler clkIO/64
     * TIFR0 - Interrupt flags register output compare A and B
     * TIMSK0 - Interrupt mask register enable output compare match A & B
    */
    TCCR0A |= (1 << COM0A1) | (1 << COM0B1) | (1 << WGM02) | (1 << WGM00);
    TCCR0B |= (1 << CS01) | (1 << CS00);

    sei();


}

void loop(void) {
    while (1) {
    	bridge_menu();
    }
}

int main(void)
{
    usart_init(BAUD_PRESCALER);
    setup();
    loop();
}

void lighten_LED_One(int brightness) {
    PORTD &= ~(1 << PD4);
    OCR0A = brightness;
    PORTD |= (1 << PD2);
}

void lighten_LED_Two(int brightness) {
    PORTD &= ~(1 << PD2);
    OCR0A = brightness;
    PORTD |= (1 << PD4);
}

void motor_move(void (*motor_action)(int)) {
    usart_print("Set brightness to (0-255): ");
    char* brightnessChoice = usart_read();
    usart_println(brightnessChoice);
    int brightness = atoi(brightnessChoice);

    if(brightness >= 0 && brightness <= 255) {
        (motor_action)(brightness);
    } else {
        usart_println("Invalid brightness value!");
    }
    free(brightnessChoice);
}

void bridge_menu() {
    usart_println("Enter [F]orward or [B]ackward to illuminate a different LED: ");
    char* directionChoice = usart_read();
    usart_println(directionChoice);

    if (strcmp(directionChoice, "F") == 0) {
        motor_move(lighten_LED_One);
    } else if (strcmp(directionChoice, "B") == 0) {
        motor_move(lighten_LED_Two);
    } else {
        usart_println("Invalid direction!");
    }
    free(directionChoice);
}
