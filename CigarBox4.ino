
// Arduino AVR boards 1.8.8

#include <SD.h>                      // need to include the SD library
//#define SD_ChipSelectPin 53  //example uses hardware SS pin 53 on Mega2560
#include <TMRpcm.h>           //  V 1.3.7also need to include this library...
// #include <SPI.h>
#include <Wire.h>   //
// #include <Servo.h>

#define SD_ChipSelectPin 10  //using digital pin 4 on arduino nano 328, can use other pins
// #define doorSwitch 4
#define servoPin 5
#define dialContact A0
#define chainPull A1
#define pwrOn 4
#define DoorOpenPos 180
#define DoorClosePos 0
#define pwrDownTime 6
#define softStartTime 5

// TMRpcm tmrpcm;   // create an object for use in this sketch
// Servo myservo;  // create servo object to control a servo
TMRpcm tmrpcm;

String fileName = "french.wav"; 
unsigned long previousMillis = millis();
unsigned long previousSecond = millis(); 
int intervalMillis = 0;
bool intervalSecond = 0;
String inString = "";  // string to hold serial input
int ComponentSelect = 0;
//int CompIndex = 0;
int AttCount = 0;
int inNum = 0;
int dialActive = 0;
int PhoneDialInterval = 0;
int PhoneDialHis = 0;
int PhoneDialCount = 0;
int PhoneDialComplete = 1;
int PhoneRingInterval = 0;
int PhoneRingPeriod = 0;
bool PhoneRinging = 0;
int dialReport = 0;
int pos = DoorClosePos;    // variable to store the servo position
int pwrTimout = pwrDownTime;
int softStart = softStartTime;
bool playStatus = 0;


int PhoneDialMonitor() { //--if the phone is dialed the number is stored 
  dialReport = 0;
  // monitors the contacts of the Phone Dial 
  // and decodes the number.
  // sends the rrsult to the Serial port and saves it in attributes
  if(digitalRead(dialContact)){
    if(PhoneDialHis == 0){
      PhoneDialHis = 1;
      PhoneDialInterval = 0;  //begin a dial interval count
      dialActive = 0;
      PhoneDialComplete = 0;
    } else {
      PhoneDialInterval += intervalMillis;
    }
  }else{
    if(PhoneDialHis){
      PhoneDialHis = 0;
      if(PhoneDialInterval > 40){PhoneDialCount++;}
      //Serial.println(PhoneDialCount,DEC);
      PhoneDialInterval = 0;  //begin a dial interval count
    }else{
      if(PhoneDialComplete == 0){
        PhoneDialInterval += intervalMillis;
        //Serial.println(PhoneDialInterval,DEC); //save the results
        if(PhoneDialInterval > 200){
            PhoneDialComplete = 1;
            dialActive = 1;
            // dialCount = PhoneDialCount;
            if(PhoneDialCount > 10) PhoneDialCount = 10;
            dialReport = PhoneDialCount;
            Serial.println(PhoneDialCount,DEC); //save the results
            PhoneDialCount = 0;
        }
      }
    }
  }
  return dialReport;
}


void setup(){ 
  pinMode(servoPin, OUTPUT);
  digitalWrite(servoPin,LOW);
  pinMode(chainPull, INPUT_PULLUP);
  pinMode(pwrOn, OUTPUT);
  digitalWrite(pwrOn, HIGH);
  digitalWrite(servoPin,HIGH);
  pinMode(9, OUTPUT);  // audio output
  pinMode(dialContact, INPUT_PULLUP);
  delay(1000);
  tmrpcm.speakerPin = 9; //5,6,11 or 46 on Mega, 9 on Uno, Nano, etc

  Serial.begin(9600);
  if (!SD.begin(SD_ChipSelectPin)) {  // see if the card is present and can be initialized:
    Serial.println("SD fail");  
    return;   // don't do anything more if not
  }
  playStatus = false;
  tmrpcm.setVolume(5);
  tmrpcm.play("phone99.wav"); //the sound file "music" will play each time the arduino powers up, or is reset
}

void loop(){  
  int playSelect = PhoneDialMonitor();
  // String fileName = "french.wav"; 

  // this is the clock. Each time the program loops the interval is recorded. It's usually 0,1, or 2 milliseconds.
  intervalMillis = int(millis() - previousMillis);
  previousMillis = millis();
  // if(intervalMillis == 3)Serial.println(intervalMillis,DEC);
  delay(1);
  if(millis() > (previousSecond + 1000)){  //set intervalSecond for one loop every second
    previousSecond = millis();
    intervalSecond = 1;
  }else{
    intervalSecond = 0;
  }

  if(!playStatus){
    if(!digitalRead(chainPull)){
      Serial.println("pull");
      tmrpcm.play("phone88.wav");
    }
  }

  if(playSelect){
    // Serial.println(dialStrings[playSelect]);
    Serial.println(playSelect*100,DEC);
    // moveServo(DoorOpenPos);
    pwrTimout = pwrDownTime;
    tmrpcm.setVolume(5);
    switch(playSelect){
      case 1: tmrpcm.play("phone01.wav"); break;
      case 2: tmrpcm.play("phone02.wav"); break;
      case 3: tmrpcm.play("phone03.wav"); break;
      case 4: tmrpcm.play("phone04.wav"); break;
      case 5: tmrpcm.play("phone05.wav"); break;
      case 6: tmrpcm.play("phone06.wav"); break;
      case 7: tmrpcm.play("phone07.wav"); break;
      case 8: tmrpcm.play("phone08.wav"); break;
      case 9: tmrpcm.play("phone09.wav"); break;
      case 10: tmrpcm.play("phone10.wav"); break;
      // case 4: tmrpcm.pause(); break;
      // case 5: if(tmrpcm.isPlaying()){ Serial.println("A wav file is being played");} break;
      // case 6: tmrpcm.stopPlayback(); break;
      // case 7: tmrpcm.volume(1); break;
      // case 8: tmrpcm.volume(0); break;
      // case '0': tmrpcm.quality(0); break;
      // case '1': tmrpcm.quality(1); break;
      default: break;
    }
  }

  if(Serial.available()){ 
    char s =  Serial.read();  
    if(s == 'p'){ //send the letter p over the serial monitor to start playback
      tmrpcm.play("phone01.wav");
    }
    if(s == 'o'){ //send the letter p over the serial monitor to start playback
      tmrpcm.play("phone02.wav");
    }
    if(s == 'd'){ //send the letter p over the serial monitor to start playback
      tmrpcm.play("phone03.wav");  //  reducedStaff
    }
    if(s == 'r'){ //send the letter p over the serial monitor to start playback
      tmrpcm.play("phone04.wav");  //  reducedStaff
    }
    if(s == 's'){ //send the letter 
      tmrpcm.play("ClockTick.wav");  //  stop
    }
  }

  if(intervalSecond){
    if(tmrpcm.isPlaying()){
      if(!playStatus){    // pwoer up sequence
        Serial.print("f");
        // digitalWrite(pwrOn, HIGH);
        // Serial.println("PowerOn");
        if(softStart){
          softStart--;
          Serial.println(softStart,DEC);
          if(softStart == (softStartTime - 1)){
            digitalWrite(pwrOn, HIGH);
            digitalWrite(servoPin,HIGH);
            Serial.println("PowerOn");
          }
          // if(softStart == 5)digitalWrite(servoPin,HIGH);
          // if(softStart == 4)digitalWrite(servoPin,LOW);
          if(softStart == 3){
            digitalWrite(servoPin,HIGH);
            Serial.println("OpenDoors");
          }
        }else{
          playStatus = true;
          pwrTimout = pwrDownTime;
          Serial.println("Play ");
        }
      }
    }else{
      if(playStatus){   // Power down sequence
        Serial.print("t");
        if(pwrTimout){
          pwrTimout--;
          Serial.println(pwrTimout,DEC);
          // if(pwrTimout == 18)digitalWrite(servoPin,HIGH);
          if(pwrTimout == (pwrDownTime - 2)){
            digitalWrite(servoPin,LOW);
            Serial.println("CloseDoors");
          }
          if(pwrTimout < 1){
            digitalWrite(pwrOn,LOW);
            Serial.println("PowerOff");
          }
        }else{
          playStatus = false;
          softStart = softStartTime;
          Serial.println("Quiet ");
        }
      }
    }
  }

}



