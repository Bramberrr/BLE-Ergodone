#include "ArduinoProMicroMatrix.h"
#include "Mcp23017Matrix.h"
#include "KeyboardMap.h"

ArduinoProMicroMatrix l_matrix;
Mcp23017Matrix r_matrix;
static matrix_row_t matrix_prev[MATRIX_ROWS];
matrix_row_t matrix_deficit[MATRIX_ROWS];
uint8_t last_r = 0,last_c_row = 0, last_c_change = 0;
const int caps_pin = LED_PIN;
static bool isCap = false;
uint8_t layer = 0;
#if defined(BLE_KEYBOARD)
BluetoothKeyboard_ BLEKeyboard(RX_PIN, TX_PIN, 9600);
#endif

void setup()
{
    pinMode(caps_pin, OUTPUT);
    #if defined(USB_KEYBOARD)
        Keyboard.begin();
    #endif

    l_matrix.begin();
    r_matrix.begin();
    if (debug_enable) {
        Serial.begin(115200);
        while (!Serial)
            ;
    }

    Serial.println("keyboard open\n");

    for (uint8_t r = 0; r < MATRIX_ROWS; r++)
    {
        matrix_deficit[r] = 0;
    }
    keyboardScan();
    for (uint8_t r = 0; r < MATRIX_ROWS; r++)
    {
        matrix_deficit[r] = matrix_prev[r];
        matrix_prev[r] = 0;
    }
}

void loop()
{
    keyboardScan();
}


void keyboardScan()
{
    matrix_row_t l_matrix_row = 0;
    matrix_row_t r_matrix_row = 0;
    matrix_row_t matrix_row = 0;
    matrix_row_t matrix_change = 0;
    
    l_matrix.scan();
    r_matrix.scan();

    for (uint8_t r = 0; r < MATRIX_ROWS; r++) {
        l_matrix_row = l_matrix.getRow(r);
        r_matrix_row = r_matrix.getRow(r);
        matrix_row = (l_matrix_row << 8) + r_matrix_row - matrix_deficit[r];
        matrix_change = matrix_row ^ matrix_prev[r];
        if (matrix_change) {
            matrix_prev[r] = matrix_row;
            Serial.println(matrix_change);
            uint8_t c_row = getCol(matrix_row);
            uint8_t c_change = getCol(matrix_change);
            
            if(c_change == c_row)
                Press(r,c_change);
            else
            {
                if(c_row == 14)
                    Release(r,c_change);
                else
                
                {
                    if(c_row == 15)
                        Press(r,c_change);
                    else
                    {
                        if(last_r == r && last_c_change == c_change)
                            Release(r,c_change);
                    }
                }
            }

            if(r == 1 && c_row == 0)
                CapLED();
            
            last_r = r;
            last_c_row = c_row;
            last_c_change = c_change; 
        }
    }
}

void CapLED()
{
    isCap = !isCap;
    if(isCap)
        digitalWrite(caps_pin,HIGH);
    else
        digitalWrite(caps_pin,LOW);
}

uint8_t getCol(matrix_row_t matrix)
{
    switch(matrix)
    {
        case 0x100: return 0;
        case 0x200: return 1;
        case 0x400: return 2;
        case 0x800: return 3;
        case 0x1000: return 4;
        case 0x2000: return 5;
        case 0x4000: return 6;
        case 0x80: return 7;
        case 0x40: return 8;
        case 0x10: return 9;
        case 0x08: return 10;
        case 0x04: return 11;
        case 0x02: return 12;
        case 0x01: return 13;
        case 0x00: return 14;
        default: return 15;
    }
}

void Press(uint8_t row, uint8_t col)
{
    #if defined(USB_KEYBOARD)
        Keyboard.press(KeyMap[layer][row][col]);
    #elif defined(BLE_KEYBOARD)
        BLEKeyboard.press(KeyMap[layer][row][col]);
    #else
        #error Keyboard not yet supported.
    #endif
}

void Release(uint8_t row, uint8_t col)
{
    #if defined(USB_KEYBOARD)
        Keyboard.release(KeyMap[layer][row][col]);
    #elif defined(BLE_KEYBOARD)
        BLEKeyboard.release(KeyMap[layer][row][col]);
    #else
        #error Keyboard not yet supported.
    #endif
}