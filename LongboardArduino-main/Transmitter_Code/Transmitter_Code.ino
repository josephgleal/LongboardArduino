/* Transmitter test code
 *  
 */
#include <Servo.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define LED 4
#define STICK 2
#define UP 3
#define RED 9
#define GREEN 5
#define BLUE 6
RF24 radio(7, 8); // CE, CSN
const byte address[6] = "01001";
int throttle = 1500;
int prevThrottle = 1500;
int delta = 25;

int maxSpeed = 2000;
bool speedLimit = false;
unsigned long lastDebounceTimeUp = 0;
unsigned long lastDebounceTimeDown = 0;
unsigned long debounceDelay = 50;
int lastUpStatus = LOW;
int upStatus = LOW;
int lastDownStatus = LOW;
int downStatus = LOW;
int gear = 1;
int connectionLostCount = 0;

void throttle_control(int last, int &curr){
    if(curr - last > delta){
      curr = last + delta;
    }
    if (curr > maxSpeed){
      curr = maxSpeed;  
    }
}

void setColor(int red, int green, int blue){
  analogWrite(RED, red);
  analogWrite(GREEN, green);
  analogWrite(BLUE, blue);
}

void flashLED(int led){
  int stat = digitalRead(led);
  digitalWrite(led, !stat);
}

//change so that there is only one number
//in throttle_control change second if to check for speedLimit boolean
//if true then curr = limitedSpeed
void checkGear(){
  int reading = digitalRead(STICK);
  Serial.print("reading ");
  Serial.println(reading);
  if (reading != lastDownStatus) {
    // reset the debouncing timer
    lastDebounceTimeDown = millis();
  }
  if ((millis() - lastDebounceTimeDown) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != downStatus) {
      downStatus = reading;

      // only change gear if the new button state is HIGH
      if (downStatus == HIGH) {
        //do not let gear go below 1
//        if(gear > 1){
        gear = gear + 1;
//        }
      }
    }
  }
  // save the reading. Next time through the loop, it'll be the lastButtonState:
  lastDownStatus = reading;

//  reading = digitalRead(UP);
//  if (reading != lastUpStatus) {
//    lastDebounceTimeUp = millis();
//  }
//  if ((millis() - lastDebounceTimeUp) > debounceDelay) {
//
//    if (reading != upStatus) {
//      upStatus = reading;
//
//      if (upStatus == HIGH) {
//        //do not let gear go above 3
//        if(gear < 3 ){
//          gear = gear + 1;
//        }
//      }
//    }
//  }
//  lastUpStatus = reading; 
  
  
  if(gear == 1){
    setColor(0,100,255);
    maxSpeed = 1650;
  }
  else if(gear == 2){
    setColor(0,255,0);
    
    maxSpeed = 1750;
  }
  else if(gear == 3){
    setColor(0,255,255);
    maxSpeed = 2000;  
  }
  else{gear = 1;}
  Serial.print("Speed Limit: ");
  Serial.println(speedLimit);
  Serial.print("Gear: ");
  Serial.println(gear);
    
  }


void setup() {
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
  pinMode(STICK, INPUT_PULLUP);
  pinMode(UP, INPUT_PULLUP);
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
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
  checkGear();
  throttle_control(prevThrottle, throttle);
  if(radio.write(&throttle, sizeof(throttle))){
    Serial.print("Throttle: ");
    Serial.println(throttle);
    prevThrottle = throttle;
    connectionLostCount = 0;
    if(digitalRead(LED) == HIGH){
      flashLED(LED);
    }
  }
  else{Serial.println("Connection Failed");}
  connectionLostCount += 1;
  if(connectionLostCount >= 10){
    flashLED(LED);
  }
  
  
  
}
