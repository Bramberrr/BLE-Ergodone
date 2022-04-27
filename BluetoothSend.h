#ifndef BLUETOOTHSEND_H
#define BLUETOOTHSEND_H 1

#include "Arduino.h"
#include <SoftwareSerial.h>
#include "config.h"
#include "HID-Project.h"

class BluetoothSend{
public:
    BluetoothSend();
    void begin();
    void write(KeyboardKeycode k);
    bool available();

private:
    SoftwareSerial* bluetoothHost;
};
#endif