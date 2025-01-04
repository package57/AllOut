#ifndef MATRIX_H
#define MATRIX_H
// on board led matrix 12x8
// To use ArduinoGraphics APIs, please include BEFORE Arduino_LED_Matrix
#include "ArduinoGraphics.h"
#include "Arduino_LED_Matrix.h"

// onboard module
ArduinoLEDMatrix matrix;

// variables
/*const*/ char Mtext[30];

// functions
void Matrixsetup();
void Matrixloop();
#endif // MATRIX_H