
//Pins
const int button_pin = 2;
const int led_pin = 7;


//----------------- LED -----------------//
#define ON_TIME 100
#define OFF_TIME 600

#define ON 0
#define OFF 1
int led_state = OFF;

void blinkLed() {
  static unsigned long last_led_switch_time = 0;
  unsigned long current_time = millis();

  if (led_state == ON && current_time > ON_TIME + last_led_switch_time) {
    turnLedOff();
    last_led_switch_time = current_time;
  }
  else if (led_state == OFF && current_time > OFF_TIME + last_led_switch_time) {
    turnLedOn();
    last_led_switch_time = current_time;
  }
}

void turnLedOn() {
  digitalWrite(led_pin, HIGH);
  led_state = ON;
}
void turnLedOff() {
  digitalWrite(led_pin, LOW);
  led_state = OFF;
}

//----------------- Connection --------------//
#define CONNECTED_BYTE 'C'
#define STOP_BYTE 'S'
#define CONNECTION_TIMEOUT 1000
unsigned long last_connection_time = 0;


//----------------- Button --------------//
void buttonPressedISR() {
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();

  if (interrupt_time - last_interrupt_time > 100) {
    Serial.write(STOP_BYTE);
  }
  last_interrupt_time = interrupt_time;
}


//----------------- Main functions -----------------//
void setup() {
  pinMode(button_pin, INPUT_PULLUP);
  pinMode(led_pin, OUTPUT);

  Serial.begin(9600);

  attachInterrupt(digitalPinToInterrupt(button_pin), buttonPressedISR, FALLING);
}

void loop() {
  //Check for connection message
  
  if (Serial.available() && Serial.read() == CONNECTED_BYTE) {
    last_connection_time = millis();
  }

  //Set state based on connection time
  if (millis() < last_connection_time + 1000) {
    turnLedOn();
  }
  else {
    blinkLed();
  }
  
}
