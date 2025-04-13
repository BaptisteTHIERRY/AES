#include "CFB.h"

void CFB_encrypt( FILE* fileIn,  FILE* fileOut,  uint8_t* key,  int Nk, uint8_t IV[16]){
    int Nr = Nk + 6;
    uint8_t in[16]  = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    uint8_t out[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    long index = 0;
    int left = 0;

    // Get size of the input file
    fseek(fileIn,0,SEEK_END);
    long sizeFileIn = ftell(fileIn);
    fseek(fileIn,0,SEEK_SET);

    // Compute round key
    uint32_t *w = malloc(sizeof(uint32_t) * Nb * (Nr + 1));
    KeyExpansion(key, w, Nk); // All round keys are store in w

    // Copy IV into out
    memcpy(out,IV,16);

    while(index + 16 <= sizeFileIn){
        fread(in, 1, 16, fileIn); // Read 16 bytes of fileIn and store it in "in" 
        AES_encrypt(out, out, w, Nk);
        blockXOR(out,in);
        fwrite(out, 1, 16, fileOut); // Write the 16 bytes of out in fileOut
        index += 16;
    }

    left = (16 + sizeFileIn - index)%16;
    fread(in, 1, left, fileIn);

    AES_encrypt(out, out, w, Nk);
    blockXOR(out,in);
    fwrite(out, 1, left, fileOut);

    free(w);
}

void CFB_decrypt(FILE* fileIn, FILE* fileOut, uint8_t *key, int Nk, uint8_t IV[16]){
    int Nr = Nk + 6;
    uint8_t in[16]  = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    uint8_t out[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    long index = 0;
    int left = 0;

    // Get size of the input file
    fseek(fileIn,0,SEEK_END);
    long sizeFileIn = ftell(fileIn);
    fseek(fileIn,0,SEEK_SET);

    // Compute round key
    uint32_t *w = malloc(sizeof(uint32_t) * Nb * (Nr + 1));
    KeyExpansion(key, w, Nk); // All round keys are store in w

    // Copy IV into in
    memcpy(in,IV,16);

    while(index + 16 <= sizeFileIn){
        AES_encrypt(in, out, w, Nk);
        fread(in, 1, 16, fileIn); // Read 16 bytes of fileIn and store it in "in" 
        blockXOR(out,in);
        fwrite(out, 1, 16, fileOut); // Write the 16 bytes of out in fileOut
        index += 16;
    }

    left = (16 + sizeFileIn - index)%16;
    AES_encrypt(in, out, w, Nk);
    fread(in, 1, left, fileIn); // Read 16 bytes of fileIn and store it in "in" 
    blockXOR(out,in);
    fwrite(out, 1, left, fileOut);

    free(w);
}