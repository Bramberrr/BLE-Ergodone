#include <SoftwareSerial.h>

SoftwareSerial BT(10, 14);
char val;
void setup() {
  Serial.begin(38400);
  Serial.println("BT is ready!");
  // HC-05默认，38400
  BT.begin(38400);
}
void loop() {
  if (Serial.available()) {
    val = Serial.read();
    BT.print(val);
  }
  if (BT.available()) {
    val = BT.read();
    Serial.print(val);
  }
}