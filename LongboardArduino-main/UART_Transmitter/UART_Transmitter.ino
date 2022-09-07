/* Transmitter test code
 *  
 */
#include <Servo.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define LED 4
#define BUT 2
RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";
int throttle = 1500;
int prevThrottle = 1500;
int delta = 25;
int maxSpeed = 2000;
bool speedLimit = false;

void throttle_control(int last, int &curr){
    if(curr - last > delta){
      curr = last + delta;
    }
    if (curr > maxSpeed){
      curr = maxSpeed;  
    }
}

//change so that there is only one number
//in throttle_control change second if to check for speedLimit boolean
//if true then curr = limitedSpeed
void checkSpeedLimit(){
  if (digitalRead(BUT) == true){
    speedLimit = !speedLimit;
    digitalWrite(LED, speedLimit);
    if(speedLimit){maxSpeed = 1750;}
    else{maxSpeed = 2000;}
    Serial.print("Speed Limit: ");
    Serial.println(speedLimit);
    while(digitalRead(BUT) == true){delay(10);}
    }
  }

void setup() {
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
  pinMode(BUT, INPUT_PULLUP);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MAX);
  radio.stopListening();
}

//limit acceleration


//read input, verify acceleration does not exceed delta, write, repeat.
void loop() {
  throttle = analogRead(A0);
  throttle = map(throttle, 125, 925, 1000, 2000);
  checkSpeedLimit();
  throttle_control(prevThrottle, throttle);
  if(radio.write(&throttle, sizeof(throttle))){
    Serial.print("Throttle: ");
    Serial.println(throttle);
    prevThrottle = throttle;
  }
  else{Serial.println("Connection Failed");}
  
  
}
