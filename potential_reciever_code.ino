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
Servo VESC;
void setup() {
  Serial.begin(9600);
  VESC.attach(5,1000,2000);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_HIGH);
  radio.startListening();
  VESC.writeMicroseconds(1500);
}
void loop() {
  if (radio.available()) {    
    radio.read(&throttle, sizeof(throttle));
    VESC.writeMicroseconds(throttle);
    Serial.println(throttle);
  }
  else {
    VESC.writeMicroseconds(1500);
    Serial.println("not available");
    delay(500);
  }
  }
