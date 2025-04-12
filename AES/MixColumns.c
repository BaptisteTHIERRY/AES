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
        res ^= ( (-((value2 & (1<<i))>>i)) & value1);
        value1 = multX(value1);
    }
    return res;
}

void MixColumns(uint8_t state[4][4]){
    // uint8_t coef[4][4] = {
    //     {0x02,0x03,0x01,0x01},
    //     {0x01,0x02,0x03,0x01},
    //     {0x01,0x01,0x02,0x03},
    //     {0x03,0x01,0x01,0x02}
    //     };

    uint8_t l0,l1,l2,l3;
    for(int i = 0; i < 4; i++){
        l0 = state[0][i];
        l1 = state[1][i];
        l2 = state[2][i];
        l3 = state[3][i];

        state[0][i] = mult(l0,0x02) ^ mult(l1,0x03) ^ mult(l2, 0x01) ^ mult(l3,0x01);
        state[1][i] = mult(l0,0x01) ^ mult(l1,0x02) ^ mult(l2, 0x03) ^ mult(l3,0x01);
        state[2][i] = mult(l0,0x01) ^ mult(l1,0x01) ^ mult(l2, 0x02) ^ mult(l3,0x03);
        state[3][i] = mult(l0,0x03) ^ mult(l1,0x01) ^ mult(l2, 0x01) ^ mult(l3,0x02);
    }
}

void InvMixColumns(uint8_t state[4][4]){
    // uint8_t coef[4][4] = {
    //     {0x0e,0x0b,0x0d,0x09},
    //     {0x09,0x0e,0x0b,0x0d},
    //     {0x0d,0x09,0x0e,0x0b},
    //     {0x0b,0x0d,0x09,0x0e}
    //     };

    uint8_t l0,l1,l2,l3;
    for(int i = 0; i < 4; i++){
        l0 = state[0][i];
        l1 = state[1][i];
        l2 = state[2][i];
        l3 = state[3][i];

        state[0][i] = mult(l0,0x0e) ^ mult(l1,0x0b) ^ mult(l2, 0x0d) ^ mult(l3,0x09);
        state[1][i] = mult(l0,0x09) ^ mult(l1,0x0e) ^ mult(l2, 0x0b) ^ mult(l3,0x0d);
        state[2][i] = mult(l0,0x0d) ^ mult(l1,0x09) ^ mult(l2, 0x0e) ^ mult(l3,0x0b);
        state[3][i] = mult(l0,0x0b) ^ mult(l1,0x0d) ^ mult(l2, 0x09) ^ mult(l3,0x0e);
    }
}
