#include "MixColumns.h"
#include <stdio.h> // A supprimer

uint8_t multX(uint8_t value){
    // value<<1 : Multiply by x
    // -(value>>7) = 0 if the most significant bit of value is 0
    //             = 1 if the most significant bit of value is 1
    // So (0x1b & -(value>>7) = 0    if the most significant bit of value is 0
    //                        = 0x1b if the most significant bit of value is 1
    return (value<<1) ^ (0x1b & -(value>>7));
}

uint8_t mult(uint8_t value1, uint8_t value2){
    uint8_t res = 0;
    for(int i = 0; i < 8; i++){
        res ^= (((value2&(1<<i))>>i) * value1); // i-th bit of value2 multiply by value1 * x^i
        value1 = multX(value1);
    }
    return res;
}

void MixColumns(uint8_t state[4][4]){
    uint8_t newstate[4][4];

    // uint8_t coef[4][4] = {
    //     {0x02,0x03,0x01,0x01},
    //     {0x01,0x02,0x03,0x01},
    //     {0x01,0x01,0x02,0x03},
    //     {0x03,0x01,0x01,0x02}
    //     };

    uint8_t coef[4] = {0x02,0x03,0x01,0x01};
    uint8_t current = 0;
    for(int col = 0; col < 4; col++){
        for(int line = 0; line < 4; line++){
            current = 0;
            for(int k = 0; k < 4; k ++){
                // current ^= mult(coef[line][k], state[k][col]);
                current ^= mult(coef[(k + 4 - line)%4], state[k][col]);
            }
            newstate[line][col] = current;
        }
    }
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            state[i][j] = newstate[i][j];
        }
    }
}

void InvMixColumns(uint8_t state[4][4]){
    uint8_t newstate[4][4];

    // uint8_t coef[4][4] = {
    //     {0x02,0x03,0x01,0x01},
    //     {0x01,0x02,0x03,0x01},
    //     {0x01,0x01,0x02,0x03},
    //     {0x03,0x01,0x01,0x02}
    //     };

    uint8_t coef[4] = {0x0e,0x0b,0x0d,0x09};
    uint8_t current = 0;
    for(int col = 0; col < 4; col++){
        for(int line = 0; line < 4; line++){
            current = 0;
            for(int k = 0; k < 4; k ++){
                // current ^= mult(coef[line][k], state[k][col]);
                current ^= mult(coef[(k + 4 - line)%4], state[k][col]);
            }
            newstate[line][col] = current;
        }
    }
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            state[i][j] = newstate[i][j];
        }
    }
}
