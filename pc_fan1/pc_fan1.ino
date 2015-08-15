const int PWMPin = 3;

/* http://forum.arduino.cc/index.php?topic=160353.msg1201754#msg1201754
 *  
 * You connect the Blue fan CONTROL wire to Pin 3 and the 
 * Green SENSE wire to pin 11.  You set OCR2B to a value 
 * from 0 to 79 to set the PWM duty cycle.  The loop() 
 * will start the fan at full RPM and then gradually 
 * decrease it while constantly reporting the RPM.  
 * Fans run on 12V, not 5V and you may need a pull-up to +5V 
 * on the Green RPM sense wire.
 */
 
void setup() {
// generate 25kHz PWM pulse rate on Pin 3
  pinMode(PWMPin, OUTPUT);   // OCR2B sets duty cycle
// Set up Fast PWM on Pin 3
  TCCR2A = 0x23;     //0x23 COM2B1, WGM21, WGM20 
// Set prescaler  
  TCCR2B = 0x0C;   //0x0A WGM21, prescaler = /8
// Set TOP and initialize duty cycle to zero(0)
  OCR2A = 79;    // 79 TOP DO NOT CHANGE, SETS PWM PULSE RATE
  OCR2B = 79;    // duty cycle for Pin 3 (0-79) generates 1 500nS pulse even when 0 :(

  Serial.begin(57600);
}

void loop() {
  
  unsigned long x;
  unsigned int rpm;
  
  if((unsigned)OCR2B > 79){
    OCR2B = 79;
  }  
  delay(1000);
  Serial.print(OCR2B);
  Serial.print(" ");
  x = pulseIn(11, HIGH);
  Serial.print(x);
  x += pulseIn(11, LOW);
  Serial.print(" ");
  Serial.print(x);
  rpm = (1000000 * 60) / (x * 2); 
  Serial.print(" ");
  Serial.println(rpm);
  OCR2B--;
}
