#include "SubBytes.h"

void SubBytes(uint8_t state[4][4]){
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            state[i][j] = sbox[state[i][j]];
        }
    }
}

void InvSubBytes(uint8_t state[4][4]){
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            state[i][j] = inv_sbox[state[i][j]];
        }
    }
}