unsigned char slaveId = 45;
unsigned char charArray[32], startArray = 0, endArray = 0, lastElement = 31;
unsigned char studentBirth[] = "25.07.2005";
unsigned char studentBirthT3[] = "25.07.2005";
unsigned char studentBirthT4[] = "25.07.2005";
unsigned char messagesSended = 0;
unsigned char CRCchar[3];

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
  setCharCRC(CRC(arr, size - 1));
  _delay_ms(160);
  sendChar(slaveId);
  for(int i = 0; i < size - 1; i++) {
    sendChar(arr[i]);
  }
  sendChar(CRCchar[0]);
  sendChar(CRCchar[1]);
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

unsigned short CRC(unsigned char arr[], unsigned char arrSize) {
  unsigned short divider = 0x1021;
  unsigned short initialReg = 0xFFFF;
  unsigned short xorOut = 0x0000;
  unsigned short crc = initialReg;
  unsigned char charBit = 0;
  
  for(unsigned char i = 0; i < (arrSize * 8); i++) {
    if(i % 8 == 0 || i == 0) {
      crc ^= (arr[charBit++] << 8);
    }

    unsigned short lostBit = (crc >> 15);
    crc <<= 1;

    if(lostBit) crc ^= divider;
  }

  return (crc ^ xorOut);
}

void setCharCRC(unsigned short crc) {
  CRCchar[0] = (crc >> 8) & 0xFF;
  CRCchar[1] = crc & 0xFF;
}

void loop() {
  if(!messagesSended) {
    _delay_ms(1600);
    sendString(studentBirth, 11);
    _delay_ms(160);
    sendString(studentBirth, 11);
    _delay_ms(160);
    studentBirthT3[0] ^= (1 << 6);
    sendString(studentBirthT3, 11);
    _delay_ms(160);
    studentBirthT4[1] ^= (1 << 7) | (1 << 6);
    sendString(studentBirthT4, 11);
    _delay_ms(160);
    sendString(studentBirth, 11);

    messagesSended = 1;
  }
}
