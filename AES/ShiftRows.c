#include "ShiftRows.h"
#include <stdio.h>

void ShiftRows(uint8_t state[4][4]){
    // The i-th line is shift i times to the left 

    uint8_t temp1 = state[1][0];
    uint8_t temp2 = state[2][1];

    // Shift of the second line
    state[1][0] = state[1][1];
    state[1][1] = state[1][2];
    state[1][2] = state[1][3];
    state[1][3] = temp1;

    // Shift of the third line
    temp1 = state[2][0];
    state[2][0] = state[2][2];
    state[2][1] = state[2][3];
    state[2][2] = temp1;
    state[2][3] = temp2;

    // Shift of the fourth line
    temp2 = state[3][3];
    state[3][3] = state[3][2];
    state[3][2] = state[3][1];
    state[3][1] = state[3][0];
    state[3][0] = temp2;
}

void InvShiftRows(uint8_t state[4][4]){
    // The i-th line is shift i times to the right

    uint8_t temp1 = state[1][0]; 
    uint8_t temp2 = state[2][1];

    // Shift of the second line
    state[1][0] = state[1][3];
    state[1][3] = state[1][2];
    state[1][2] = state[1][1];
    state[1][1] = temp1;
    
    // Shift of the third line
    temp1 = state[2][0];
    state[2][0] = state[2][2];
    state[2][1] = state[2][3];
    state[2][2] = temp1;
    state[2][3] = temp2;

    // Shift of the fourth line
    temp2 = state[3][0];
    state[3][0] = state[3][1];
    state[3][1] = state[3][2];
    state[3][2] = state[3][3];
    state[3][3] = temp2;
}
