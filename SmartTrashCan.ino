////////////////////////////////////////////////////////////////
// Copyright Naveen Sharma, Robonix Corportion.               //
// The sketch contains code to control a motor rotation from  //
// a ultrasonic motion sensor.                                //
////////////////////////////////////////////////////////////////
#include <Servo.h>

////////////////////////////////////////////////////////////////
// PINS related configuration.                            /////
///////////////////////////////////////////////////////////////
// Pin to trigger the ultrasonic wave.
#define TRIG_PIN (8)
// Pin to listen for the ultrasonic echo.
#define ECHO_PIN (9)
// Buzzer pin, to indicate the nearness of
// obstacle, Sound would increase as object comes near.
#define BEEP_PIN (11)
// The pin where motor is attached.
#define MOTOR_PIN (10)

// Motor related configuration
#define MAX_DEG 179
#define MAX_VAL 1023
#define DELAY_TIME 15
#define TRIGGER_DISTANCE (50)
// Map distance to time for beep delay.
#define SCALING_FACTOR (10)
// Delay for main loop.
#define LOOP_DELAY (500)

///////////////////////////////////////////////////////////
// The servo motor, one global Instance.                 //
Servo motor;                                             //
///////////////////////////////////////////////////////////
// put your setup code here, to run once:
void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BEEP_PIN, OUTPUT);
  // Attach the motor to motor pin.
  motor.attach(MOTOR_PIN);
  // Set Motor to Midpoint
  motor.write(90);
}
//////////////////////////
// Utility Functions.   //
//////////////////////////

// inch_distance ():
// @brief: Calculate the distance of the object (in inches) from the time duration
// it takes to return the echo of ultrasonic pulse.
// @param(duration:long): The delay duration in micro-seconds. The caller
// must ensure that units are correct.

long inch_distance (long duration) {
  // According to Parallax's datasheet for the PING))), there are 73.746
  // microseconds per inch (i.e. sound travels at 1130 feet per second).
  // This gives the distance travelled by the ping, outbound and return,
  // so we divide by 2 to get the distance of the obstacle.
  // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
  return duration / 74 / 2;
}

// @brief: Calculate the distance of the object (in centimeters) from the time duration
// it takes to return the echo of ultrasonic pulse.
// @param(duration:long): The delay duration in micro-seconds. The caller
// must ensure that units are correct.
long centimeter_distance(long duration)
{
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the object we
  // take half of the distance travelled.
  return duration / 29 / 2;
}

void send_pulse (int setup_delay, int echo_delay)
{
  digitalWrite (TRIG_PIN, LOW);
  delayMicroseconds (setup_delay);
  digitalWrite (TRIG_PIN, HIGH);
  delayMicroseconds (echo_delay);
  digitalWrite (TRIG_PIN, LOW);
}

// Beep for a given duration on the pin given by
// beep_pin.
void beep(int beep_pin, int duration)
{
  digitalWrite(beep_pin, HIGH);
  delay(duration);
  digitalWrite(BEEP_PIN, LOW);
}

// Rotate a servo motor by given number of degrees
// @param (angle:int): The degrees by which motor is to be rotated in
// degrees.
//
void rotate (Servo *pmotor, int angle)
{
  angle = (abs(angle) >= MAX_DEG) 
               ? (angle % (MAX_DEG + 1)) : angle;
  angle = (angle < 0) ? (angle + MAX_DEG + 1) : angle;
  
  pmotor->write (angle);
  
  delay (DELAY_TIME);
}
//////////////////////////////////////////////////////////////
//                      THE MAIN LOOP.                      //
//////////////////////////////////////////////////////////////
void loop()
{
  long duration, cm;
  send_pulse (2 /* Setup delay.(ms) */, 10 /* Echo Delay.(ms) */);
  duration = pulseIn(ECHO_PIN, HIGH);
  cm = centimeter_distance(duration);
  if(cm < TRIGGER_DISTANCE )
  {
    rotate (&motor, 90);
    beep (11, 100);
  }
  else {
    rotate (&motor, 0);
  }
  delay (LOOP_DELAY);
}
