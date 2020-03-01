#include "DFRobotDFPlayerMini.h"
#include "Arduino.h"
#include "SoftwareSerial.h"

SoftwareSerial mp3Serial(11, 12); // RX, TX
DFRobotDFPlayerMini myDFPlayer;

int PIR_SENSOR_PIN = 3;
int state = LOW;

void setup() {

  mp3Serial.begin(9600);
    
  pinMode(PIR_SENSOR_PIN, INPUT);    // initialize sensor as an input
  pinMode(2, OUTPUT);
  digitalWrite(2,HIGH);
  Serial.begin(115200);        // initialize serial

  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));
  
  if (!myDFPlayer.begin(mp3Serial)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true);
  }
  Serial.println(F("DFPlayer Mini online."));
  
  myDFPlayer.volume(30);  //Set volume value. From 0 to 30

  }

void loop() {


  int val = digitalRead(PIR_SENSOR_PIN);   // read sensor value
  if (val == HIGH) {           // check if the sensor is HIGH
    delay(200);                // delay 100 milliseconds 
    if (state == LOW) {
      Serial.println("Motion detected!"); 
      state = HIGH;       // update variable state to HIGH
      mp3Serial.begin(9600);
      myDFPlayer.begin(mp3Serial);
      myDFPlayer.next();
      
      digitalWrite(12,LOW);
     }
  } 
  else {
      delay(200);             // delay 200 milliseconds       
      if (state == HIGH){
        Serial.println("Motion stopped!");
        state = LOW;       // update variable state to LOW
    }
  }
  Serial.print("PIR State = " + String(val) + "\n"); 

  if (myDFPlayer.available()) {
    int code = myDFPlayer.read();
    printDetail(myDFPlayer.readType(), code);
  }
}



void printDetail(uint8_t type, int value){
  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out!"));
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online!"));
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Number:"));
      Serial.print(value);
      Serial.println(F(" Play Finished!"));
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          Serial.println(F("Card not found"));
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match"));
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File"));
          break;
        case Advertise:
          Serial.println(F("In Advertise"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
}
