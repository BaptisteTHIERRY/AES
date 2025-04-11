#include "ShiftRows.h"
#include <stdio.h>

void ShiftRows(uint8_t state[4][4]){
    // The i-th line is shift i times to the left 

    // Shift of the second and fourth lines
    uint8_t temp1 = state[1][0]; // Temporary variable that will not be shift in the for loop
    uint8_t temp2 = state[3][0]; // Temporary variable that will not be shift in the for loop

    for(int k = 0; k < 3; k++){ 
        state[1][k] = state[1][(k+1)%4];
        state[3][(4-k)%4] = state[3][3-k]; 
    }
    
    state[1][3] = temp1; // Variable that has not been shifted
    state[3][1] = temp2; // Variable that has not been shifted

    // Shift of the third line
    temp1 = state[2][0];
    temp2 = state[2][1];
    state[2][0] = state[2][2];
    state[2][1] = state[2][3];
    state[2][2] = temp1;
    state[2][3] = temp2;
}

void InvShiftRows(uint8_t state[4][4]){
    // The i-th line is shift i times to the right

    // Shift of the second and fourth lines
    uint8_t temp1 = state[1][3]; // Temporary variable that will not be shift in the for loop
    uint8_t temp2 = state[3][0]; // Temporary variable that will not be shift in the for loop

    for(int k = 3; k > 0; k--){ 
        state[1][k] = state[1][(k-1)%4];
        state[3][3-k] = state[3][4-k]; 
    }
    
    state[1][0] = temp1; // Variable that has not been shifted
    state[3][3] = temp2; // Variable that has not been shifted

    // Shift of the third line
    temp1 = state[2][0];
    temp2 = state[2][1];
    state[2][0] = state[2][2];
    state[2][1] = state[2][3];
    state[2][2] = temp1;
    state[2][3] = temp2;
}

