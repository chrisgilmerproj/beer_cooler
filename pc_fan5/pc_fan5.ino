/* Slightly modified from example found at:
 * https://github.com/PaulStoffregen/TimerOne/blob/master/examples/FanSpeed/FanSpeed.pde
 *
 * This example creates a PWM signal with 25 kHz carrier.
 *
 * Arduino's analogWrite() gives you PWM output, but no control over the
 * carrier frequency.  The default frequency is low, typically 490 or
 * 3920 Hz.  Sometimes you may need a faster carrier frequency.
 *
 * The specification for 4-wire PWM fans recommends a 25 kHz frequency
 * and allows 21 to 28 kHz.  The default from analogWrite() might work
 * with some fans, but to follow the specification we need 25 kHz.
 *
 * http://www.formfactors.org/developer/specs/REV1_2_Public.pdf
 *
 * Connect the PWM pin to the fan's control wire (usually blue).  The
 * board's ground must be connected to the fan's ground, and the fan
 * needs +12 volt power from the computer or a separate power supply.
 */

#include <TimerOne.h>

// For Timer1 you need to use pins 10 and 9
const int SensePin = 9;     // yellow
const int ControlPin = 10;  // blue

void setup(void)
{
  // pull up internal resistor on sense pin
  pinMode(SensePin, INPUT);
  digitalWrite(SensePin, HIGH); 
   
  // generate 25kHz PWM pulse rate on control pin
  pinMode(ControlPin, OUTPUT);
  
  Timer1.initialize(40);  // 40 us = 25 kHz
  Serial.begin(57600);
}

void loop(void)
{
  // slowly increase the PWM fan dutyCycle
  for (float dutyCycle = 30.0; dutyCycle <= 100.0; dutyCycle += 2) {
    Serial.print("PWM Fan, Duty Cycle = ");
    Serial.print(dutyCycle);
    setFan(dutyCycle);
    delay(1000); // wait for fan to stabilize
    if (dutyCycle == 100.0) {
      delay(5000);
    }
    Serial.print(", RPM = ");
    Serial.println(getRpm());
  }
}

/*
 * Set the fan duty cycle to value between 30.0 and 100.0
 */
void setFan(float dutyCycle) {
  if (dutyCycle < 30.0) {
    dutyCycle = 30.0;
  } else if (dutyCycle > 100.0) {
    dutyCycle = 100.0;
  }
  Timer1.pwm(ControlPin, (dutyCycle / 100) * 1023);
}

/*
 *  Get the Revolutions per Minute of the Fan
 */
int getRpm() {

  unsigned long duration;
  unsigned int rpm;
  
  // Get the time between sensor pulses in microseconds
  duration = pulseIn(SensePin, HIGH);
  duration += pulseIn(SensePin, LOW);
  Serial.print("duration: ");
  Serial.println(duration);
  // Return 0 if fan is off
  if (duration == 0) {
    return 0;
  } else {
    // convert one minute of seconds to microseconds
    // duration represents half of a revolution 
    rpm = (1000000 * 60) / (duration * 2); 
    return rpm;
  }
}
