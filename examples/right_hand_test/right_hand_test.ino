#include "config.h"
#include "Mcp23017Matrix.h"

Mcp23017Matrix matrix;

static matrix_row_t matrix_prev[MATRIX_ROWS];

void setup()
{
    matrix.begin();
    if (debug_enable) {
        Serial.begin(9600);
        while (!Serial)
            ;
    }
    Serial.println("right hand serial open\n");
    for (int i = 0; i < MATRIX_ROWS; i++) {
        matrix_prev[i] = 0;
    }
}

void loop()
{
    matrix_row_t matrix_row = 0;
    matrix_row_t matrix_change = 0;

    matrix.scan();
    for (uint8_t r = 0; r < MATRIX_ROWS; r++) {
        matrix_row = matrix.getRow(r);
        matrix_change = matrix_row ^ matrix_prev[r];
        if (matrix_change) {
            matrix_prev[r] = matrix_row;
            Serial.println( matrix_row, HEX);
        }
    }
}
