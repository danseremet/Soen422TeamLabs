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

/*
 * Hardware setup:
 * Arduino Nano
 * PD6 L-EN
 * PD4 L-B
 * PD2 L-F
 * PD5 R-EN
 * PD3 R-F
 * PC1 R-B
 * 
 * !!!Note: The 5V barely handles 1 motor, sometimes there isn't enough power for 2 motors without a little initial push on the motor.
 * If the motor doesn't start give it a higher value, or try turning it.
*/
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
     * RIGHT MOTOR
     * PD5 - OC0B out     -- 3,4 EN
     * PD3 - digital out  -- 4A CW    -- Forward
     * PC1 - digital out  -- 3A CCKW  -- Backwards
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
  
    /*
     * Using Timer2 for periodic 1 second PWM display without interfering with OCR0x pins.
     * TIFR2 - Interrupt flags register output compare A and B
     * TIMSK2 - Interrupt mask register enable output compare match A & B
    */

    TCCR1A |= (1 << COM1A0);
    TCCR1B |= (1 << WGM12) | (1 << CS12) | (1 << CS10);    
    OCR1A = 15650;
    TIFR1 |= (1 << OCF1A);
    TIMSK1 |= (1 << OCIE1A);

    sei();
}

// TODO fix output format and fix some ascii issues where it starts printing bad stuff or the serial monitor stops reading input values
ISR(TIMER1_COMPA_vect) {
//        usart_print("Left_motor PWM value: ");
//        char pwm_value[3];
//        itoa(OCR0A, pwm_value, 10);
//        usart_print(pwm_value);
//        usart_print(", ");
//        usart_print("Right_motor PWM value: ");
//        itoa(OCR0A, pwm_value, 10);
//        usart_println(pwm_value);
//        free(pwm_value);
}

void left_forward(int speed) {
    PORTD &= ~(1 << PD4);
    OCR0A = speed;
    PORTD |= (1 << PD2);
}

void left_backwards(int speed) {
    PORTD &= ~(1 << PD2);
    OCR0A = speed;
    PORTD |= (1 << PD4);
}

void right_forward(int speed) {
    PORTC &= ~(1 << PC1);
    OCR0B = speed;
    PORTD |= (1 << PD3);
}

void right_backwards(int speed) {
    PORTD &= ~(1 << PD3);
    OCR0B = speed;
    PORTC |= (1 << PC1);
}

void motor_move(void (*motor_action)(int)) {
    usart_print("Set speed to (0-255): ");
    char* speedChoice = usart_read();
    usart_println(speedChoice);
    int speed = atoi(speedChoice);
    
    if(speed >= 0 && speed <= 255) {
        (motor_action)(speed);
    } else {
        usart_println("Invalid speed value!");
    }
    free(speedChoice);
}

void left_motor_menu() {
    usart_println("Enter [F]orward or [B]ackward to change motor direction: ");
    char* directionChoice = usart_read();
    usart_println(directionChoice);
            
    if (strcmp(directionChoice, "F") == 0) {
        motor_move(left_forward);        
    } else if (strcmp(directionChoice, "B") == 0) {
        motor_move(left_backwards);
    } else {    
        usart_println("Invalid direction!");
    }
    free(directionChoice);
}

void right_motor_menu() {
    usart_print("Enter [F]orward or [B]ackward to change motor direction: ");
    char* directionChoice = usart_read();
    usart_println(directionChoice);
    
    if (strcmp(directionChoice, "F") == 0) {
        motor_move(right_forward);
    } else if (strcmp(directionChoice, "B") == 0) {
        motor_move(right_backwards);
    } else {
        usart_println("Invalid direction!");
    }
    free(directionChoice);
}

void print_menu() {
    usart_print("Enter [L]eft or [R]ight motor configuration: ");
    char* motorChoice = usart_read();
    usart_println(motorChoice);
    
    if (strcmp(motorChoice, "L") == 0) {
        left_motor_menu();
    } else if (strcmp(motorChoice, "R") == 0) {
        right_motor_menu();
    } else {
        usart_println("Invalid motor choice!");
    }
    free(motorChoice);
}

void loop(void) {
    while (1) {
        print_menu();
        _delay_ms(500);
    }
}

int main(void)
{
    usart_init(BAUD_PRESCALER);
    setup();
    loop();
}
