/* RGB Analog Example, Teensyduino Tutorial #2
   http://www.pjrc.com/teensy/tutorial2.html

   This example code is in the public domain.
*/

const int redPin =  6;
const int greenPin =  9;
const int bluePin =  10;

void setup()   { 
  analogWriteResolution(16); /*default frequency of 488Hz */  
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
}

void loop()                     
{
  for(uint32_t i = 1; i < 65535; i+=1000) {
    analogWrite(redPin, i);
    analogWrite(greenPin, i);
    analogWrite(bluePin, i);
    delay(100);
  }
  /*
  for(uint16_t i = 0; i < ((2^16)-1); i++) {

  }
  for(uint16_t i = 0; i < ((2^16)-1); i++) {
    
  }*/


}

