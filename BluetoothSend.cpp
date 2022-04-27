#include "BluetoothSend.h"

BluetoothSend::BluetoothSend()
{
}

void BluetoothSend::begin()
{
    bluetoothHost = new SoftwareSerial(RX_PIN, TX_PIN);
    bluetoothHost->begin(BAUD_RATE);
}

void BluetoothSend::write(KeyboardKeycode k)
{
    
}

bool BluetoothSend::available()
{
    if(bluetoothHost->available())
        return true;
    else
        return false;
}