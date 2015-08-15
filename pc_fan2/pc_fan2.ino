

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

const int SensePin = 11;  // Sense   Pin 3 from fan
const int PWMPin   =  3;  // Control Pin 4 from fan
const int TOP      = 79;
int fanspeed       = 100; 
 
void setup() {   

  // pull up internal resistor on sense pin
  pinMode(SensePin, INPUT);
  digitalWrite(SensePin, HIGH);
  
  // generate 25kHz PWM pulse rate on control pin
  pinMode(PWMPin, OUTPUT);

  // Set up Fast PWM on Pin 3
  TCCR2A = 0x23; //0x23 COM2B1, WGM21, WGM20 

  // Set prescaler to 8, dictating TOP OCR2A of 79
  // 16MHz / (prescaler * (TOP+1)) = 25KHz, solve for TOP
  TCCR2B = 0x0C; //0x0A WGM21, prescaler = /8

  // Set TOP and initialize duty cycle to zero(0)
  OCR2A = TOP; // 79 TOP DO NOT CHANGE, SETS PWM PULSE RATE
  OCR2B = TOP; // Set the duty cycle for Pin 3 (0-79) 
              // generates 1 500nS pulse even when 0 because it is a fan

  setFan(fanspeed);
  Serial.begin(57600);
}

void loop() {
  setFan(fanspeed);
  Serial.print(fanspeed);
  Serial.print(" ");
  Serial.print(OCR2B);      // duty cycle
  Serial.print(" ");
  delay(3000);              // let fan catch up before reading
  Serial.println(getRpm());

  fanspeed = fanspeed - 5;
  if (fanspeed < 30) {
    fanspeed = 100;
  }
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

  OCR2B = (TOP * speed) / 100;

  // Always configure the duty cycle to be between 0 and 79
  if ((unsigned)OCR2B > TOP){
    OCR2B = TOP;
  } 
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

