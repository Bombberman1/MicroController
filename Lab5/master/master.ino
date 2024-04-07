unsigned char charArray[32], startArray = 0, endArray = 0, lastElement = 31;
unsigned char charComp[32], startComp = 0, endComp = 0, lastComp = 31;
unsigned char slave_a_input[32], slave_b_input[32];
unsigned char messagesRecieved = 0;
unsigned char slaveId;

unsigned char readChar() {
  unsigned char character = charArray[startArray++];
  startArray &= lastElement;
  return character;
}

void readString() {
  startArray = 0;
  endArray = 0;
  clearArr();
  clearSlave_a();
  clearSlave_b();
  _delay_ms(150);
  slaveId = readChar();
  int i = 0;
  if(slaveId == 24) {
    while(startArray != endArray) {
      slave_a_input[i++] = readChar();
    }
  } else if(slaveId == 45) {
    while(startArray != endArray) {
      slave_b_input[i++] = readChar();
    }
  }
}

void clearArr() {
  for(int i = 0; i < 32; i++) {
    charArray[i] = '\0';
  }
}

void clearSlave_a() {
  for(int i = 0; i < 32; i++) {
    slave_a_input[i] = '\0';
  }
}

void clearSlave_b() {
  for(int i = 0; i < 32; i++) {
    slave_b_input[i] = '\0';
  }
}

ISR(USART1_RX_vect) {
  charArray[endArray++] = UDR1;
  endArray &= lastElement;
}

void sendCharComp(unsigned char character) {
  charComp[endComp++] = character;
  endComp &= lastComp;
  UCSR0B |= (1 << UDRIE0);
}

void sendStringComp(unsigned char arr[], int size) {
  startComp = 0;
  endComp = 0;
  for(int i = 0; i < 32; i++) {
    charComp[i] = '\0';
  }
  _delay_ms(150);
  sendCharComp(slaveId);
  for(int i = 0; i < size; i++) {
    sendCharComp(arr[i]);
  }
}

int stringSize(unsigned char arr[], int maxSize) {
  int size = 0;
  for(int i = 0; i < maxSize; i++) {
    if(arr[i] != '\0') size++;
    else break;
  }
  return size;
}

ISR(USART0_UDRE_vect) {
  UDR0 = charComp[startComp++];
  startComp %= lastComp;
  if(startComp == endComp) {
    UCSR0B &= ~(1 << UDRIE0);
  }
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

void loop() {
  if(!messagesRecieved) {
    _delay_ms(150);
    readString();
    sendStringComp(slave_a_input, stringSize(slave_a_input, 32));
    readString();
    sendStringComp(slave_a_input, stringSize(slave_a_input, 32));
    readString();
    sendStringComp(slave_a_input, stringSize(slave_a_input, 32));
    readString();
    sendStringComp(slave_a_input, stringSize(slave_a_input, 32));
    readString();
    sendStringComp(slave_a_input, stringSize(slave_a_input, 32));

    readString();
    sendStringComp(slave_b_input, stringSize(slave_b_input, 32));
    readString();
    sendStringComp(slave_b_input, stringSize(slave_b_input, 32));
    readString();
    sendStringComp(slave_b_input, stringSize(slave_b_input, 32));
    readString();
    sendStringComp(slave_b_input, stringSize(slave_b_input, 32));
    readString();
    sendStringComp(slave_b_input, stringSize(slave_b_input, 32));

    messagesRecieved = 1;
  }
}
