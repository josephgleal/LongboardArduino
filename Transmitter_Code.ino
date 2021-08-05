/* Transmitter test code
 *  
 */
#include <Servo.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";
int throttle = 1500;
int prevThrottle = 1500;
int delta = 25;
void throttle_control(int last, int &curr){
    if(curr - last > delta){
      curr = last + delta;
    }
}
void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}

//limit acceleration


//read input, verify acceleration does not exceed delta, write, repeat.
void loop() {
  throttle = analogRead(A0);
  throttle = map(throttle, 125, 925, 1000, 2000);
  throttle_control(prevThrottle, throttle);
  radio.write(&throttle, sizeof(throttle));
  Serial.println(throttle);
  prevThrottle = throttle;
  
}
