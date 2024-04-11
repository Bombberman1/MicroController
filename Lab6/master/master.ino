
ISR(USART1_RX_vect) {
  UDR0 = UDR1;
}

void setup() {
  DDRD = (1 << PD1);

  UBRR1L = 63;
  UBRR1H = (63 >> 8);
  UCSR1C = (1 << UCSZ11) | (1 << UCSZ10) | (1 << USBS1);
  UCSR1A = 0;
  UCSR1B = (1 << RXEN1) | (1 << TXEN1) | (1 << RXCIE1);

  UBRR0L = 63;
  UBRR0H = (63 >> 8);
  UCSR0C = (1 << UCSZ01) | (1 << UCSZ00) | (1 << USBS0);
  UCSR0A = 0;
  UCSR0B = (1 << RXEN0) | (1 << TXEN0);
}

void loop() {}
