#include "blockXOR.h"

void blockXOR(uint8_t l1[16], uint8_t l2[16]){
    // XOR every element of l1 and l2 and store the result in l1
    for(int i = 0; i < 16; i++){
        l1[i] ^= l2[i];
    }
}