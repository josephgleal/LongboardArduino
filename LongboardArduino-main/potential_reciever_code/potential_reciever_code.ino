/* Transmitter test code
 *  
 */
#include <Servo.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CE, CSN
const byte address[6] = "01001";
int throttle = 1500;
int connectionLoss = 0;
Servo VESC;

void checkConnection(int connectionLost){
  if (connectionLoss >= 1){
    if (connectionLoss >= 35){
      VESC.writeMicroseconds(1350);
      Serial.println("Terminal Loss of Connection: BRAKING (1200)");
    }
    else if (connectionLoss >= 20){
      VESC.writeMicroseconds(1450);
      Serial.println("WARNING: Weak or No Connection. SLOWING (1450)");
    }
  }  
}

void setup() {
  Serial.begin(9600);
  VESC.attach(5,1000,2000);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MAX);
  radio.startListening();
  VESC.writeMicroseconds(1500);
}
void loop() {
  if (radio.available()) {    
    radio.read(&throttle, sizeof(throttle));
    VESC.writeMicroseconds(throttle);
    Serial.println("Throttle" + String(throttle));
    connectionLoss = 0;
  }
  else {
    //VESC.writeMicroseconds(1500);
    Serial.println("not available (1500)");
    connectionLoss += 1;
    delay(50);
  }
  // If connection to the controller is lost, then brake the board
  checkConnection(connectionLoss);
 }
