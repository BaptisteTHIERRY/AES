#include "AES.h"

void AddRoundKey(uint8_t state[4][4], uint32_t *w, int start){
    for(int col = 0; col < 4; col++){
        for(int line = 0; line < 4; line++){
            state[line][col] = state[line][col] ^ ( (w[start + col]&(0xff<<((3 - line)*8)))>>((3 - line)*8) );
        }
    }
}

void printState(uint8_t state[4][4]){
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < Nb; j++){
            printf("%x ",state[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void AES_cipher(uint8_t* in, uint8_t* out, uint8_t* key, int Nk){
    uint8_t state[4][Nb]; 
    int Nr = Nk + 6;
    uint32_t *w = malloc(sizeof(uint32_t) * Nb * (Nr + 1));

    KeyExpansion(key, w, Nk); // All round key are store in w

    // in to state
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < Nb; j++){
            state[i][j] = in[i + 4*j];
        }
    }

    // Encryption
    AddRoundKey(state, w, 0);
    for(int round = 1; round < Nr; round++){
        SubBytes(state);
        ShiftRows(state);
        MixColumns(state) ;
        AddRoundKey(state, w, round*Nb);
    }
    
    SubBytes(state);
    ShiftRows(state);
    AddRoundKey(state, w, Nr*Nb);

    // state to out
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < Nb; j++){
            out[i + 4*j] = state[i][j];
        }
    }
    free(w);
}