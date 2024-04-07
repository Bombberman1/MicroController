unsigned char slaveId = 24;
unsigned char charArray[32], startArray = 0, endArray = 0, lastElement = 31;
unsigned char studentData[] = "Pavlyuk Oleg Igorovych";
unsigned char studentDataT2[] = "Pavlyuk Oleg Igorovych";
unsigned char studentDataT5[] = "Pavlyuk Oleg Igorovych";
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

void setup() {
  DDRD = (1 << PD2);

  UBRR0L = 63;
  UBRR0H = (63 >> 8);
  UCSR0C = (1 << UCSZ01) | (1 << UCSZ00) | (1 << USBS0);
  UCSR0B = (1 << RXEN0) | (1 << TXEN0);
}

void loop() {
  if(!messagesSended) {
    sendString(studentData, 23);
    _delay_ms(160);
    studentDataT2[21] ^= (1 << 6);
    sendString(studentDataT2, 23);
    _delay_ms(160);
    sendString(studentData, 23);
    _delay_ms(160);
    sendString(studentData, 23);
    _delay_ms(160);
    studentDataT5[7] ^= (1 << 6) | (1 << 2)| (1 << 0);
    sendString(studentDataT5, 23);

    messagesSended = 1;
  }
}
