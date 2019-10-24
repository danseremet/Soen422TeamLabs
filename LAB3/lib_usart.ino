/*
 * My USART function library
*/

/* Forward declaration of functions */
void usart_init(void);
unsigned char usart_read_char(void);
void usart_send_char(unsigned char data);
char* usart_read(void);
void usart_print(char* data);
void usart_println(char* data);

void usart_init(unsigned long baud) {
    UBRR0H = (baud >> 8); /* Set baud rate */
    UBRR0L = baud;
    UCSR0B |= (1 << RXEN0) | (1 << TXEN0);  /* Enable receiver and transmitter */
    UCSR0C = (1 << UCSZ00) | (1 << UCSZ01); /* Set frame format: 8-bit data, no parity bit, 1 stop bit */
}

/* Wait until a byte is ready to be read, then reads and returns it. */
unsigned char usart_read_char(void) {
    loop_until_bit_is_set(UCSR0A, RXC0);
    return UDR0;
}

/* Wait for empty transmit buffer, put data into buffer, sends the data. */
void usart_send_char(unsigned char data) {
    loop_until_bit_is_set(UCSR0A, UDRE0);    
    UDR0 = data;
}

/* Reads a line of characters with a max buffer size. */
char* usart_read(void) {
  int data_size = 40;
  // Allocate buffer memory
  char * data = malloc(data_size * sizeof(char));
  // Clean buffer memory before using it
  memset(data, '0', data_size * sizeof(char));
  
  int i = 0;
  while(1) {
    data[i] = usart_read_char();
    // A new line is the end of user input.
    if (data[i] == '\n') {
      break;
    }
    i++;
  }
  // adding null terminator for end of string
  data[i] = 0; // ascii code for '\0' 
  return data;
}

/* Print character array until null terminator */
void usart_print(char * data) {
  for (int i = 0; data[i] != '\0'; i++) {
    usart_send_char(data[i]);
  }
}

/* Same as usart_print but adds a newline */
void usart_println(char * data) {
  usart_print(data);
  usart_print("\n");
}
