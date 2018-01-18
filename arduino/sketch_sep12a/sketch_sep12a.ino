
#define encoder0PinA  2
#define encoder0PinB  3
#define encoder1PinA  4
#define encoder1PinB  5
#define encoder2PinA  6
#define encoder2PinB  7
#define encoder3PinA  8
#define encoder3PinB  9


volatile byte encoder0Pos = 0;    // a counter to see how many times the pin has changed
volatile byte encoder1Pos = 0;    // a counter to see how many times the pin has changed
volatile byte encoder2Pos = 0;    // a counter to see how many times the pin has changed
volatile byte encoder3Pos = 0;    // a counter to see how many times the pin has changed
volatile uint8_t encoder0PinA_old = 0xFF;     // default is high because the pull-up
volatile uint8_t encoder1PinA_old = 0xFF;     // default is high because the pull-up
volatile uint8_t encoder2PinA_old = 0xFF;     // default is high because the pull-up
volatile uint8_t encoder3PinA_old = 0xFF;     // default is high because the pull-up
byte cmd=0;     // a place to put our serial data
 
void setup() {
  Serial.begin(9600);
  Serial.print("Encoder Testing\n");
  
  pinMode(encoder0PinA, INPUT);     //set the pin to input
  digitalWrite(encoder0PinA, HIGH); //use the internal pullup resistor
  pinMode(encoder0PinB, INPUT);     //set the pin to input
  digitalWrite(encoder0PinB, HIGH); //use the internal pullup resistor
  *digitalPinToPCMSK(encoder0PinA) |= bit (digitalPinToPCMSKbit(encoder0PinA));  // enable pin
  
  pinMode(encoder1PinA, INPUT);     //set the pin to input
  digitalWrite(encoder1PinA, HIGH); //use the internal pullup resistor
  pinMode(encoder1PinB, INPUT);     //set the pin to input
  digitalWrite(encoder1PinB, HIGH); //use the internal pullup resistor
  *digitalPinToPCMSK(encoder1PinA) |= bit (digitalPinToPCMSKbit(encoder1PinA));  // enable pin
  
  pinMode(encoder2PinA, INPUT);     //set the pin to input
  digitalWrite(encoder2PinA, HIGH); //use the internal pullup resistor
  pinMode(encoder2PinB, INPUT);     //set the pin to input
  digitalWrite(encoder2PinB, HIGH); //use the internal pullup resistor
  *digitalPinToPCMSK(encoder2PinA) |= bit (digitalPinToPCMSKbit(encoder2PinA));  // enable pin
  
  pinMode(encoder3PinA, INPUT);     //set the pin to input
  digitalWrite(encoder3PinA, HIGH); //use the internal pullup resistor
  pinMode(encoder3PinB, INPUT);     //set the pin to input
  digitalWrite(encoder3PinB, HIGH); //use the internal pullup resistor
  *digitalPinToPCMSK(encoder3PinA) |= bit (digitalPinToPCMSKbit(encoder3PinA));  // enable pin

  PCIFR |= 5; //Enable pin change interrupts for pins D0 to D7  and B0 to B7
  PCICR |= 5; //Clear interrupts for this group
  }
 
void loop() {
  cmd=Serial.read();
  cmd = 'p';  
  if (cmd=='p')
  {
    Serial.print("encoder0:\t");
    Serial.println(encoder0Pos, DEC);
    Serial.print("encoder1:\t");
    Serial.println(encoder1Pos, DEC);
    Serial.print("encoder2:\t");
    Serial.println(encoder2Pos, DEC);
    Serial.print("encoder3:\t");
    Serial.println(encoder3Pos, DEC);
    Serial.println();
    
  }
  cmd=0;
  delay(1000);
  
}
 
ISR (PCINT2_vect) {// handle pin change interrupt for D0 to D7 here 
  if(encoder0PinA_old != digitalRead(encoder0PinA)) {
    if (digitalRead(encoder0PinA) == digitalRead(encoder0PinB)) {
      encoder0Pos++;
    } else {
      encoder0Pos--;
    }
    encoder0PinA_old = digitalRead(encoder0PinA);
  }
  
  if(encoder1PinA_old != digitalRead(encoder1PinA)) {
    if (digitalRead(encoder1PinA) == digitalRead(encoder1PinB)) {
      encoder1Pos++;
    } else {
      encoder1Pos--;
    }
    encoder1PinA_old = digitalRead(encoder1PinA);
  }
 
  if(encoder2PinA_old != digitalRead(encoder2PinA)) {
    if (digitalRead(encoder2PinA) == digitalRead(encoder2PinB)) {
      encoder2Pos++;
    } else {
      encoder2Pos--;
    }ISR (PCINT2_vect) {// handle pin change interrupt for D0 to D7 here 
      if(encoder0PinA_old != digitalRead(encoder0PinA)) {
        if (digitalRead(encoder0PinA) == digitalRead(encoder0PinB)) {
          encoder0Pos++;
        } else {
          encoder0Pos--;
        }
        encoder0PinA_old = digitalRead(encoder0PinA);
      }
      
      if(encoder1PinA_old != digitalRead(encoder1PinA)) {
        if (digitalRead(encoder1PinA) == digitalRead(encoder1PinB)) {
          encoder1Pos++;
        } else {
          encoder1Pos--;
        }
        encoder1PinA_old = digitalRead(encoder1PinA);
      }
     
      if(encoder2PinA_old != digitalRead(encoder2PinA)) {
        if (digitalRead(encoder2PinA) == digitalRead(encoder2PinB)) {
          encoder2Pos++;
        } else {
          encoder2Pos--;
        }
        encoder2PinA_old = digitalRead(encoder2PinA);
      }
    }
    
    ISR (PCINT0_vect) {// handle pin change interrupt for D0 to D7 here 
      if(encoder3PinA_old != digitalRead(encoder3PinA)) {
        if (digitalRead(encoder3PinA) == digitalRead(encoder3PinB)) {
          encoder3Pos++;
        } else {
          encoder3Pos--;
        }
        encoder3PinA_old = digitalRead(encoder3PinA);
      }
    }
    
    
    encoder2PinA_old = digitalRead(encoder2PinA);
  }
}

ISR (PCINT0_vect) {// handle pin change interrupt for D0 to D7 here 
  if(encoder3PinA_old != digitalRead(encoder3PinA)) {
    if (digitalRead(encoder3PinA) == digitalRead(encoder3PinB)) {
      encoder3Pos++;
    } else {
      encoder3Pos--;
    }
    encoder3PinA_old = digitalRead(encoder3PinA);
  }
}


