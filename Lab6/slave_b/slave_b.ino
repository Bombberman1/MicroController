unsigned char slaveId = 45;
unsigned char CRCchar[3];
unsigned char temperature = 0;
unsigned char lastTemperature = 0;
unsigned char dataArr[5];
unsigned char arrCounter = 0;


void sendTemperature() {
  temperature = readTC74();
  if(lastTemperature != temperature) {
    dataArr[0] = slaveId;
    dataArr[1] = temperature;
    setCharCRC(CRC(temperature));
    dataArr[2] = CRCchar[0];
    dataArr[3] = CRCchar[1];
    UCSR0B |= (1 << UDRIE0);
    lastTemperature = temperature;
  }
}

ISR(USART_UDRE_vect) {
  PORTD |= (1 << PD2);
  UDR0 = dataArr[arrCounter++];
  if(arrCounter == 4) {
    UCSR0B &= ~(1 << UDRIE0);
    arrCounter = 0;
  }
  _delay_ms(1);
  PORTD &= ~(1 << PD2);
}

unsigned short CRC(unsigned char temp) {
  unsigned short divider = 0x1021;
  unsigned short initialReg = 0xFFFF;
  unsigned short xorOut = 0x0000;
  unsigned short crc = initialReg;
  
  for(unsigned char i = 0; i < 8; i++) {
    if(i == 0) {
      crc ^= (temp << 8);
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

void I2Cstart() {
  TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
  while (!(TWCR & (1 << TWINT)));
}

void I2Cstop() {
  TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
}

void I2Csend(unsigned char data) {
  TWDR = data;
  TWCR = (1 << TWINT) | (1 << TWEN);
  while (!(TWCR & (1 << TWINT)));
}

unsigned char I2CgetNack() {
  TWCR = (1 << TWINT) | (1 << TWEN);
  while (!(TWCR & (1 << TWINT)));

  return TWDR;
}

unsigned char readTC74() {
  I2Cstart();
  I2Csend(0b10011010); // Write
  I2Csend(0x00);
  I2Cstop();

  I2Cstart();
  I2Csend(0b10011011); // Read
  unsigned char temp = I2CgetNack();
  I2Cstop();

  return temp;
}

void setup() {
  DDRD = (1 << PD2);

  UBRR0L = 63;
  UBRR0H = (63 >> 8);
  UCSR0C = (1 << UCSZ01) | (1 << UCSZ00) | (1 << USBS0);
  UCSR0B = (1 << RXEN0) | (1 << TXEN0);

  TWBR = 504; // ((7372800 / 7200) - 16) / 2
  TWSR = 0;
  DDRC &= ~((1 << PC4) | (1 << PC5));
  PORTC |= (1 << PC4) | (1 << PC5);
  TWAR = 0;
}

void loop() {
  _delay_ms(100);
  sendTemperature();
}
