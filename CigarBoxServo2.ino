// 1servo.ino: Example sketch for tinyServo85

#include "tinyServo85.h"
byte motor1 = 4; // motor1 will be connected to PB3 (using guide above)
bool hist = 1;
int pos=80; // keep track of current position. Start somewhere convenient for your setup (90 degrees in this case).

// Change these values to suit your application:
#define SVOMAXANGLE 170   // maximum angle for servo.
#define SVOMINANGLE 00   // maximum angle for servo.
// Note: The cycle of Timer1 has been adjusted to fit 249 divisions within 50-2000ms. Changing SVOMINPULSE 
// and SVOMAXPULSE outside the range 504-2485 will adversely impact the functionality of the timing routine.
#define SVOMINPULSE 504  // minimum pulse width in microseconds for servo signal (0 degrees). Default: 504
#define SVOMAXPULSE 2485  // maximum pulse width in microseconds for servo signal (for maximum angle). Default: 2485
#define SVOTIMEOUT 500    // timeout in ms to disable servos.

tinyServo85 myServos;     // declare object called myServos of class tinyServo85

  
void moveTo(byte servo_num, int loc, int wait) { // routine to move servo slower (more smoothly)
  if(loc != pos){ 
    if (loc > pos) pos++;
    if (loc < pos) pos--;
    myServos.setServo(servo_num, pos);
    delay(wait); // wait a bit (slows down movement)
  }
}

void setup() {
  myServos.setCTC();
  myServos.attachServo(motor1); // attach motor1
  // myServos.homeServos(); // home all attached servos
}

void loop() {
  // // Uncomment to rock motor1 using moveTo(), with a 10ms delay between steps:
  if(digitalRead(A3)){
      moveTo(motor1, SVOMAXANGLE, 5);
  }else{
      moveTo(motor1, 0, 5);
  }
}
  




