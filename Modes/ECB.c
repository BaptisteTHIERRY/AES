#include "ECB.h"

void ECB_encrypt(FILE* fileIn, FILE* fileOut, uint8_t *key, int Nk){
    int Nr = Nk + 6;
    uint8_t in[16] =  {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    uint8_t out[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    long index = 0;
    int pad = 0;
    int left = 0;

    // Get size of the input file
    fseek(fileIn,0,SEEK_END);
    long sizeFileIn = ftell(fileIn);
    fseek(fileIn,0,SEEK_SET);

    // Compute round key
    uint32_t *w = malloc(sizeof(uint32_t) * Nb * (Nr + 1));
    KeyExpansion(key, w, Nk); // All round keys are store in w

    while(index + 16 <= sizeFileIn){
        fread(in, 1, 16, fileIn); // Read 16 bytes of fileIn and store it in "in" 
        AES_encrypt(in, out, w, Nk);
        fwrite(out, 1, 16, fileOut); // Write the 16 bytes of out in fileOut
        index += 16;
    }

    left = (16 + sizeFileIn - index)%16;
    pad = 16 - left;
    if(left == 0){
        for(int i = 0; i < 16; i++){
            in[i] = (uint8_t) 0xf;
        }
    }
    else{
        fread(in, 1, left, fileIn);
        for(int i = 0; i < pad; i++){
            in[15-i] = (uint8_t) pad;
        }
    }

    AES_encrypt(in, out, w, Nk);
    fwrite(out, 1, 16, fileOut);

    free(w);
}

void ECB_decrypt(FILE* fileIn, FILE* fileOut, uint8_t *key, int Nk){
    int Nr = Nk + 6;
    uint8_t in[16] =  {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    uint8_t out[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    long index = 0;

    // Get size of the input file
    fseek(fileIn,0,SEEK_END);
    long sizeFileIn = ftell(fileIn);
    fseek(fileIn,0,SEEK_SET);

    if(sizeFileIn%16 != 0){
        fprintf(stderr,"THe size of the file must be a multiple of 16 to decrypt\n");
        exit(EXIT_FAILURE);
    }

    // Compute round key
    uint32_t *w = malloc(sizeof(uint32_t) * Nb * (Nr + 1));
    KeyExpansion(key, w, Nk); // All round keys are store in w

    while(index + 16 < sizeFileIn){
        fread(in, 1, 16, fileIn); // Read 16 bytes of fileIn and store it in "in" 
        AES_decrypt(in, out, w, Nk);
        fwrite(out, 1, 16, fileOut); // Write the 16 bytes of out in fileOut
        index += 16;
    }

    fread(in,1,16,fileIn);
    AES_decrypt(in, out, w, Nk);

    for(int i = 0; i < 16 - out[15]; i++){
        fwrite(&out[i], 1, 1, fileOut);
    }
    free(w);
}