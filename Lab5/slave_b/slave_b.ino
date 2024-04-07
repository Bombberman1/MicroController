unsigned char slaveId = 45;
unsigned char charArray[32], startArray = 0, endArray = 0, lastElement = 31;
unsigned char studentBirth[] = "25.07.2005";
unsigned char studentBirthT3[] = "25.07.2005";
unsigned char studentBirthT4[] = "25.07.2005";
unsigned char messagesSended = 0;

void sendChar(unsigned char character) {
  charArray[endArray++] = character;
  endArray &= lastElement;
  UCSR0B |= (1 << UDRIE0);
}

void sendString(unsigned char arr[], int size) {
  startArray = 0;
  endArray = 0;
  for(int i = 0; i < 32; i++) {
    charArray[i] = '\0';
  }
  _delay_ms(150);
  sendChar(slaveId);
  for(int i = 0; i < size - 1; i++) {
    sendChar(arr[i]);
  }
}

ISR(USART_UDRE_vect) {
  PORTD |= (1 << PD2);
  UDR0 = charArray[startArray++];
  startArray %= lastElement;
  if(startArray == endArray) {
    UCSR0B &= ~(1 << UDRIE0);
  }
  _delay_ms(1);
  PORTD &= ~(1 << PD2);
}

void setup() {
  DDRD = (1 << PD2);

  UBRR0L = 63;
  UBRR0H = (63 >> 8);
  UCSR0C = (1 << UCSZ01) | (1 << UCSZ00) | (1 << USBS0);
  UCSR0B = (1 << RXEN0) | (1 << TXEN0);
}

void loop() {
  if(!messagesSended) {
    _delay_ms(1500);
    sendString(studentBirth, 11);
    _delay_ms(150);
    sendString(studentBirth, 11);
    _delay_ms(150);
    studentBirthT3[0] ^= (1 << 6);
    sendString(studentBirthT3, 11);
    _delay_ms(150);
    studentBirthT4[1] ^= (1 << 7) | (1 << 6);
    sendString(studentBirthT4, 11);
    _delay_ms(150);
    sendString(studentBirth, 11);

    messagesSended = 1;
  }
  _delay_ms(100);
}
