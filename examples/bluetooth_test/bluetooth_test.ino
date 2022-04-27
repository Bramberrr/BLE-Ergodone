#include <SoftwareSerial.h>
#include "config.h"

SoftwareSerial EEBlue(10,14);//RX | TX

void setup()
{
Serial.begin(9600);
EEBlue.begin(BAUD_RATE);
}

void loop ()
{
    if(EEBlue.available())
        Serial.write(EEBlue.read());
    if(Serial.available())
        EEBlue.write(Serial.read ());
}
