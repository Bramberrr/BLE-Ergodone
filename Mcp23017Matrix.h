#ifndef MCP23017MATRI_H
#define MCP23017MATRI_H 1

#include "KeyboardMatrix.h"
#include <Wire.h>
#include "config.h"

class Mcp23017Matrix : public KeyboardMatrix {
public:
    Mcp23017Matrix();
    virtual void begin();
    virtual uint8_t scan();
    virtual uint8_t getRows();
    virtual uint8_t getCols();
    virtual bool isOn(uint8_t row, uint8_t col);
    virtual matrix_row_t getRow(uint8_t row);
    virtual bool isGhostInRow(uint8_t row);
    void select_row(uint8_t row);
    matrix_row_t read_cols();
    void unselect_rows();

protected:
    uint8_t _matrix[SINGLE_HAND_MATRIX_ROWS];
    matrix_row_t matrix_debouncing[SINGLE_HAND_MATRIX_ROWS];
    uint8_t debouncing;
};

#endif
