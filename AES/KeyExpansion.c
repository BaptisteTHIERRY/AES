#include "KeyExpansion.h"

uint32_t SubWord(uint32_t word){
    uint32_t w = 0;
    for(int i = 0; i < 32; i += 8){
        w += sbox[(word & (0xff<<i))>>i]<<i;
    }
    return w;
}

uint32_t RotWord(uint32_t word){
    return (word<<8) + (word>>24);
}

void KeyExpansion(uint8_t *key, uint32_t *w, int Nk){
    int Nr = Nk + 6;
    int i = 0;
    uint32_t temp = 0;
    for(int i = 0; i < Nk; i++){
        w[i] = (key[Nb*i]<<24) + (key[Nb*i+1]<<16) + (key[Nb*i+2]<<8) + (key[Nb*i+3]); // w[i] = word(key[4*i], key[4*i+1], key[4*i+2], key[4*i+3])
    }
    for(i = Nk; i < Nb*(Nr + 1); i++){
        temp = w[i-1];
        if( !(i % Nk) ){
            temp = SubWord(RotWord(temp)) ^ rcon[i/Nk-1];
        }else {
            if(Nk > 6 && (i%Nk == 4)){
                temp = SubWord(temp);
            }
        }
        w[i] = w[i-Nk] ^ temp;
    }
}
