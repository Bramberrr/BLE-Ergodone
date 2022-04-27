//
// read the keys on the right hand
//

#include "Mcp23017Matrix.h"
#include <SoftwareSerial.h>
static uint8_t mcp23017 = 0x20;
static uint8_t row_codes[] = { 0xFD, 0xFB, 0xF7, 0xEF, 0xDF, 0xBF}; //Row5~Row0

Mcp23017Matrix::Mcp23017Matrix(){
}

void Mcp23017Matrix::begin()
{
    Wire.begin();
    Wire.setClock(400000UL); // set I2C to 400khz

    // set row pins on B
    Wire.beginTransmission(mcp23017);
    // select register IODIRB
    Wire.write(0x01);
    // set 0b00000000 means all pins are set as output
    Wire.write(0x00);
    Wire.endTransmission();

    // set col pins on A
    Wire.beginTransmission(mcp23017);
    // select register IODIRA
    Wire.write(0x00);
    // set 0b11111111 means all pins are set as input
    Wire.write(0xFF);
    Wire.endTransmission();

    // wire pull up resisters to all col pins
    Wire.beginTransmission(mcp23017);
    // select register GPPUA
    Wire.write(0x0C);
    // set 0b11011111 means all col pins are pull-up enabled
    Wire.write(0xFF);
    Wire.endTransmission();
    // Serial1.begin(BAUD_RATE);
    debouncing = DEBOUNCE;
}

void Mcp23017Matrix::select_row(uint8_t row)
{
    Wire.beginTransmission(mcp23017);
    // select register GPIOB 
    Wire.write(0x13);
    Wire.write(row_codes[row]);
    Wire.endTransmission();
}

void Mcp23017Matrix::unselect_rows()
{
    Wire.beginTransmission(mcp23017);
    Wire.write(0x13);
    Wire.write(0xFF);
    Wire.endTransmission();
}

bool Mcp23017Matrix::isGhostInRow(uint8_t row)
{
    return false;
}

matrix_row_t Mcp23017Matrix::read_cols()
{
    Wire.beginTransmission(mcp23017);
    // select register GPIOA
    Wire.write(0x12);
    Wire.endTransmission();
    Wire.requestFrom(mcp23017, (uint8_t)1);
    byte col_data = Wire.read();
    col_data = ~col_data;
    col_data &= 0xDF;
    return col_data;
}

bool Mcp23017Matrix::isOn(uint8_t row, uint8_t col)
{
    return (_matrix[row] & (1 << col));
}

matrix_row_t Mcp23017Matrix::getRow(uint8_t row)
{
    return _matrix[row];
}

uint8_t Mcp23017Matrix::scan()
{
    for (uint8_t i = 0; i < SINGLE_HAND_MATRIX_ROWS; i++) {
        select_row(i);
        matrix_row_t cols = read_cols();
        if (matrix_debouncing[i] != cols) {
            matrix_debouncing[i] = cols;
            if (debouncing) {
            }
            debouncing = DEBOUNCE;
        }
        unselect_rows();
    }

    if (debouncing) {
        if (--debouncing) {
            _delay_ms(1);
        } else {
            // dprintf("debouncing update\n");
            for (uint8_t i = 0; i < SINGLE_HAND_MATRIX_ROWS; i++) {
                _matrix[i] = matrix_debouncing[i];
            }
        }
    }
    return 1;
}

uint8_t Mcp23017Matrix::getRows()
{
    return MATRIX_ROWS;
}

uint8_t Mcp23017Matrix::getCols()
{
    return MATRIX_COLS;
}
