

/*
 *  Adapted from code here:
 *  http://forum.arduino.cc/index.php?topic=160353.msg1201754#msg1201754
 *
 *  The fan we're using goes between ~600 and ~1800 RPM for reference.
 *  Below about 30% duty cycle you don't see much difference in the lower
 *  speed.
 *
 *  The 4 pin PC fan has these connections:
 *  
 *     Pin Name    Color1  Color2
 *      1  GND     black   black
 *      2  +12VDC  red     yellow
 *      3  Sense   yellow  green
 *      4  Control blue    blue
 * 
 *  To connect you will need the GND wire connected to both the arduino
 *  GND and to the +12VDC power GND.  Then connect the +12VDC power wire
 *  to the power source, which should turn on the fan (if your source is
 *  plugged in).
 * 
 *  For sensing plug the Sense wire into digital pin 11 on the arduino.
 * 
 *  For control plug the Control wire into digital pin 3 on the arduino.
 * 
 *  For more information: http://www.allpinouts.org/index.php/Motherboard_%28CPU%29_4_Pin_Fan
 */

// Using timer1
// 16MHz / (prescaler * (TOP+1)) = 25KHz, solve for TOP

const int SensePin =  9;  // Sense   Pin 3 from fan
const int PWMPin   = 10;  // Control Pin 4 from fan
const int TOP      = 639; // 639 for prescale 1, 79 for prescale 8
int fanspeed       = 100; 
 
void setup() {   

  // pull up internal resistor on sense pin
  pinMode(SensePin, INPUT);
  digitalWrite(SensePin, HIGH);
  
  // generate 25kHz PWM pulse rate on control pin
  pinMode(PWMPin, OUTPUT);

  cli(); // stop interrupts

  TCCR1A = 0; // set entire TCCR1A register to 0
  TCCR1B = 0; // same for TCCR1B
  TCNT1  = 0; //initialize counter value to 0
  
  // Set up Fast PWM on Control Pin
  TCCR1A |= (1 << COM1B1) | (1 << WGM11) | (1 << WGM10);
  
  // Set CS10 bit for 1 prescaler
  TCCR1B |= (1 << WGM13) | (1 << WGM12) | (1 << CS10);

  // Set TOP and initialize duty cycle to zero(0)
  OCR1A = TOP; // SETS PWM PULSE RATE
  OCR1B = TOP; // Set the duty cycle for Control Pin 
               // generates 1 500nS pulse even when 0 because it is a fan
  
  sei(); //allow interrupts
  
  setFan(fanspeed);
  Serial.begin(57600);
}

void loop() {
  setFan(100);
  delay(5000);
  setFan(75);
  delay(5000);
}

/*
 * Set the fan speed to value between 0 and 100
 */
void setFan(int speed) {
  if (speed < 30) {
    speed = 30;
  } else if (speed > 100) {
    speed = 100;
  }

  OCR1B = (TOP * speed) / 100;

  // Always configure the duty cycle to be between 0 and TOP
  if ((unsigned)OCR1B > TOP){
    OCR1B = TOP;
  } 
  
  Serial.print(speed);
  Serial.print(" ");
  Serial.print(OCR1B);      // duty cycle
  delay(1000);
  Serial.print(" ");
  Serial.println(getRpm());
}

/*
 *  Get the Revolutions per Minute of the Fan
 */
int getRpm() {

  unsigned long duration;
  unsigned int rpm;
  
  // First get the time for the full revolution in microseconds
  duration = pulseIn(SensePin, HIGH);
  duration += pulseIn(SensePin, LOW);

  rpm = (1000000 * 60) / (duration * 2); 
  return rpm;
}

