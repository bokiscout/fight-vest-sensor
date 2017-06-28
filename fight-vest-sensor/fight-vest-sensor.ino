// declare global variables
int INPUT_PIN_MEDIUM;
int INPUT_PIN_STRONG;
int LED_PIN;
boolean ledState;

boolean waitPunch;
int punchStrength;

char inbyte;

void setup() {
  // put your setup code here, to run once:

  // initialize global variables
  INPUT_PIN_MEDIUM = A0;
  INPUT_PIN_STRONG = A1;
  
  LED_PIN = 13;
  ledState = false;
  waitPunch = true;
  punchStrength = 0;

  // initial state of pins
  pinMode(LED_PIN, OUTPUT);       // led pin to behave as output
  digitalWrite(LED_PIN, LOW);     // turn off led
  Serial.begin(9600);             // serial communication is done at 96000 bouds
  analogWrite(INPUT_PIN_MEDIUM, 0);      // force 0v,
  analogWrite(INPUT_PIN_STRONG, 0);      // force 0v,
  // otherwise readings can fluctuate due to noise,
  // but this way readings have smaller values than usual!

  inbyte = 0;
}

void loop() {
  // read data from the sensor
  int value_medium = analogRead(INPUT_PIN_MEDIUM);
  int value_strong = analogRead(INPUT_PIN_STRONG);

  if (value_strong >= 450) {
    // strong punch
    waitPunch = false;      // notify that punch is detected and not waiting for
    punchStrength = 2;

    // blink led at 5V
    ledState = true;
    digitalWrite(LED_PIN, HIGH);
    delay(70);
    
    ledState = false;
    digitalWrite(LED_PIN, LOW);
    delay(70);
  }
  else if (value_medium >= 450 && waitPunch) {
    // medium punch
    waitPunch = false;      // notify that punch is detected and not waiting for

    // if coming back from strong punch let it be strong
    // othervise let it be medium punch
    if (punchStrength < 2) {
      punchStrength = 1;
    }

    // turn on led at 3V
    ledState = true;
    digitalWrite(LED_PIN, HIGH);
  }
  else if(value_medium <= 450 && value_strong <= 450) {
    // no punch // 0v

    // if first time 0v notify for punch strength
    // notify for punch strenght
    // then reset value to 0 and turn off led
    if (! waitPunch) {
      if (punchStrength == 2) {
        // strong punch was detected
        //Serial.println("# punch = STRONG $");
        Serial.println("[1_2]");
      }
      else if (punchStrength == 1) {
        // medium punch
        //Serial.println("# punch = MEDIUM $");
        Serial.println("[1_1]");
      }
      else {
        // unknown punch value
        //Serial.println("# Unknown punch strength $");
        Serial.println("[x_y]");
      }

      // reset punch
      punchStrength = 0;

      // reset led
      ledState = false;                 
      digitalWrite(LED_PIN, LOW);
    }

    // notify that we are waiting for next punch
    waitPunch = true;
  }

  // read data send from the Android device
  while(Serial.available() > 0)
  {
    inbyte = Serial.read();
    if(inbyte == 'x'){
      // android device is sending 'x' to check for active conection
      // at multiple points of the activities life cycle
      // just ignore them
 // Serial.println("x from android to be ignorred");  
    }
    else{
      // Serial.println(inbyte);  
    }
    
  }
  
  delay(10);
}
