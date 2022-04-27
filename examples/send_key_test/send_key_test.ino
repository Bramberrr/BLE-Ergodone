#include "config.h"
#include "HID-Project.h"

const int caps_pin = LED_PIN;
const int pinRow = A3;
const int pinCol = 8;
static bool isCap = false;

void setup() {
  pinMode(caps_pin, OUTPUT);
  pinMode(pinRow,OUTPUT);
  digitalWrite(pinRow, LOW);
  pinMode(pinCol, INPUT_PULLUP);
  Keyboard.begin();
}


void loop() {
  if (!digitalRead(pinCol)) {
    Keyboard.write(KEY_CAPS_LOCK);
    isCap = !isCap;
  }
  if(isCap)
    digitalWrite(caps_pin,HIGH);
  else
    digitalWrite(caps_pin,LOW);
}
