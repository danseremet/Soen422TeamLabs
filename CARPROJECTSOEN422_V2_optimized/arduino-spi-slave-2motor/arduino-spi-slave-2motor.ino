#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#define UART_BAUDRATE 9600
#define BAUD_PRESCALER ((F_CPU / (UART_BAUDRATE * 16UL)) - 1)

#include <avr/io.h>
#include <string.h>
#include <stdlib.h>

/* Forward declaration of functions */
void spi_slave_init(void);
void usart_send_char(unsigned char data);
void usart_println(char * data);

void spi_slave_init(void) {
    DDRB &= ~((1 << 2) | (1 << 3) | (1 << 5));    // SS, MOSI and SCK as inputs
    DDRB |= (1 << 4);                             // MISO as output
    SPCR &= ~(1 << MSTR);                         // Set as slave 
    SPCR |= (1 << SPR0)|(1 << SPR1);              // divide clock by 128
    SPCR |= (1 << SPE);                           // Enable SPI
    SPCR |= (1 << SPIE);                          // enable interrupts
    SPSR |= (1 << SPIF);                          // interrupt flag
}

char command;

ISR(SPI_STC_vect) {

    command = SPDR;       // SPI read slave
    while(!SPIF){};  
    
    usart_send_char(command);

    // Motor control logic
    if (command == 'X' || command == 'O' || command == 'L') { // STOP
        stop();
    } else if (command == 'B') {                      // FORWARD
        forward(255);
    } else if (command == 'N') {                      // BACKWARD
        backward(255);
    } 
    
    else if (command == 'U' || command == 'J') {      // Forward turn
        forward(255);
    } else if (command == 'I' || command == 'K') {    // Backward turn
        backward(255);
    } 
}

void forward(int speed) {
    PORTD &= ~(1 << PD4);
    OCR0A = speed;
    PORTD |= (1 << PD2);

    PORTC &= ~(1 << PC1);
    OCR0B = speed;
    PORTD |= (1 << PD3);
}

void backward(int speed) {
    PORTD &= ~(1 << PD2);
    OCR0A = speed;
    PORTD |= (1 << PD4);

    PORTD &= ~(1 << PD3);
    OCR0B = speed;
    PORTC |= (1 << PC1);
}

void stop() {
    forward(0);
}

void init_pins(void) {
    // IMPORTANT: Left and right motor have inverse wiring between 1A and 2A, 3A and 4A.
    /*
     * TOP MOTOR configuration
     * PD6 - OC0A out     -- 1,2 EN
     * PD4 - digital out  -- 1A CW    -- Backwards  (left)
     * PD2 - digital out  -- 2A CCKW  -- Forward    (left)
    */
    DDRD |= (1 << PD6) | (1 << PD4) | (1 << PD2);

    /*
     * BOTTOM MOTOR
     * PD5 - OC0B out     -- 3,4 EN
     * PD3 - digital out  -- 3A CCKW  -- Backwards  (left)
     * PC1 - digital out  -- 4A CW    -- Forward    (left)
    */
    DDRD |= (1 << PD5) | (1 << PD3);
    DDRC |= (1 << PC1);
    
    /*
     * Timer 0 config with 2 registers A & B
     * TCCR0A - Set Clear OC0B on Compare Match & Phase Correct PWM Mode 5
     * TCCR0B - Set prescaler clkIO/64
     * TIFR0 - Interrupt flags register output compare A and B
     * TIMSK0 - Interrupt mask register enable output compare match A & B
    */
    TCCR0A |= (1 << COM0A1) | (1 << COM0B1) | (1 << WGM02) | (1 << WGM00);
    TCCR0B |= (1 << CS01) | (1 << CS00);

    
}

int main(void) {
    cli();
    init_pins();
    usart_init(BAUD_PRESCALER);
    spi_slave_init();
    _delay_ms(2000);
    sei();
    while (1) {
      //loop();
      _delay_ms(500);
    }
}
