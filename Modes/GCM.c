#include "GCM.h"

static uint8_t R[16] = {0b11100001, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

void extractTag(FILE* file, uint8_t tag[16]){
    int size = 0;
    char *currentHex = malloc(sizeof(char) * 3);  
    char *end = "\0";
    char string[2] = "aa";
    if(currentHex == NULL){
        fprintf(stderr,"Error while allocationg memory\n");
        exit(EXIT_FAILURE);
    }
    currentHex[0] = '0';
    currentHex[1] = '1';
    currentHex[2] = '\0';
    while(string[0] != '\0' && string[1] != '\0' && size < 16){
        fread(string,1,2,file);
        currentHex[0] = string[0];
        currentHex[1] = string[1];
        tag[size] = (uint8_t) strtol(currentHex,&end,16);
        if(*end != '\0'){
            free(currentHex);
            fprintf(stderr,"The TAG must be write in hexadecimal.\n");
            exit(EXIT_FAILURE);
        }
        size += 1;
    }
    if(size != 16){
        free(currentHex);
        fprintf(stderr,"The TAG must be of size 128 bits.\n");
        exit(EXIT_FAILURE);
    }
    free(currentHex);
}

void RS(uint8_t l[16]){
    uint8_t last_bit_next = 0;
    uint8_t last_bit_before = 0;
    for(int i = 0; i < 16; i++){
        last_bit_next = l[i]&1;
        l[i] = l[i]>>1 ^ last_bit_before<<7;
        last_bit_before = last_bit_next;
    }
}

void product(uint8_t X[16], uint8_t Y[16], uint8_t out[16]){
    /*
    Compute X.Y and store the result in out.
    Algorithm :
    X = x_0,x_1,x_2,...,x_127;
    Z_0 = 0;
    V_0 = Y;
    R = 11100001 || 0^120 (define in the header)

    for i=0 to 127 :
        Z_i+1 = Z_i         if x_i = 0;
              = Z_i XOR V_i if x_i = 1;

        Vi+1 =  V_i >> 1        if LSB1(V_i) = 0;
             = (V_i >> 1) XOR R if LSB1(V_i) = 1;

    return Z_128
    */
    uint8_t Z[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    uint8_t V[16];
    memcpy(V,Y,16); // Copy Y in V
    uint8_t LSB1_V = 0;


    for(int i = 0; i < 16; i++){
        for(int j = 0; j < 8; j++){
            if(X[i]&(1<<(7-j))){ // x_i == 1
                blockXOR(Z,V);
            }

            LSB1_V = (uint8_t) V[15]&((uint8_t) 1);
            RS(V); // (V_i >> 1)
            if(LSB1_V){ // LSB1(V_i) == 1 
                blockXOR(V,R);
            }
        }
    }
    memcpy(out,Z,16);
}

void GHASH(uint8_t H[16], uint8_t* (X[16]), int lenX, uint8_t out[16]){
    uint8_t Y[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    for(int i = 0; i < lenX; i++){
        blockXOR(Y,X[i]);
        product(Y,H,Y);
    }
    memcpy(out,Y,16);
}

void inc32(uint8_t value[16]){
    uint32_t v = ((uint32_t) value[12])<<24 ^ ((uint32_t) value[13])<<16 ^ ((uint32_t) value[14])<<8 ^ ((uint32_t) value[15]);
    v += 1;
    value[12] = (uint8_t) (v&(0xff<<24))>>24;
    value[13] = (uint8_t) (v&(0xff<<16))>>16;
    value[14] = (uint8_t) (v&(0xff<<8))>>8;
    value[15] = (uint8_t)  v&0xff;
}

void GCTR_encrypt(FILE* fileIn, FILE* fileOut,  uint8_t* key,  int Nk, uint8_t ICB[16], uint8_t X[16],uint8_t H[16],int lenAD){
    int Nr = Nk + 6;
    uint8_t in[16]  = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    uint8_t out[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    uint8_t CB[16]  = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    long index = 0;

    // Get size of the input file
    fseek(fileIn,0,SEEK_END);
    long sizeFileIn = ftell(fileIn);
    fseek(fileIn,0,SEEK_SET);

    // Compute round key
    uint32_t *w = malloc(sizeof(uint32_t) * Nb * (Nr + 1));
    KeyExpansion(key, w, Nk); // All round keys are store in w

    memcpy(CB,ICB,16);
    while(index + 16 <= sizeFileIn){
        // Encryption 
        AES_encrypt(CB,out,w,Nk);
        fread(in,1,16,fileIn);
        blockXOR(out,in);
        fwrite(out,1,16,fileOut); 

        // Authentification Tag
        blockXOR(X,out);
        product(X,H,X);


        inc32(CB);
        index += 16;
    }

    int left = (16 + sizeFileIn - index)%16;
    if(left != 0){
        // Encryption
        AES_encrypt(CB,out,w,Nk);
        fread(in,1,left,fileIn);
        blockXOR(out,in);
        fwrite(out,1,left,fileOut);

        for(int i = left; i < 16; i++){
            out[i] = 0;
        }
        // Authentification Tag
        blockXOR(X,out);
        product(X,H,X);
    }

    uint64_t lenA = (uint64_t) lenAD*8;
    uint64_t lenC = (uint64_t) sizeFileIn*8;
    uint8_t concat[16];
    for(int i = 0; i < 8; i++){
        concat[7-i] = (uint8_t) ((lenA&(0xff<<(i*8)))>>(i*8));
        concat[15-i] = (uint8_t) ((lenC&(0xff<<(i*8)))>>(i*8));
    }

    blockXOR(X,concat);
    product(X,H,X);
    free(w);
}

void GCM_encrypt( FILE* fileIn,  FILE* fileOut,  uint8_t* key,  int Nk, uint8_t* IV, uint8_t* AD, int lenAD){
    int Nr = Nk + 6;

    // Compute round key
    uint32_t *w = malloc(sizeof(uint32_t) * Nb * (Nr + 1));
    KeyExpansion(key, w, Nk); // All round keys are store in w

    // Compute ICB
    uint8_t ICB[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    uint8_t J0[16]  = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    uint8_t H[16]   = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    uint8_t X[16]   = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    uint8_t AD_[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    int index = 0;

    AES_encrypt(H,H,w,Nk); // H = E_k(0)
    
    // lenIV must be equal at 12 bytes (96 bits), NIST recommandation
    memcpy(J0,IV,12);
    J0[15] = (uint8_t) 1;
    memcpy(ICB,J0,16);

    inc32(ICB);

    while(index + 16 <= lenAD){
        memcpy(AD_,AD,16);
        blockXOR(X,AD_);
        product(X,H,X);
        AD += 16;
        index += 16;
    }
    int left = (16 + lenAD - index)%16;
    if(left != 0){
        memcpy(AD_,AD,left);
        for(int i=left;i<16;i++){AD_[i] = 0;}
        blockXOR(X,AD_);
        product(X,H,X);
    }

    GCTR_encrypt(fileIn,fileOut,key,Nk,ICB,X,H,lenAD);

    AES_encrypt(J0,J0,w,Nk);

    blockXOR(X,J0);

    fprintf(fileOut,"\n\nTag : ");
    for(int i = 0; i < 16; i++){
        fprintf(fileOut,"%02hhx",X[i]);
    }

    free(w);
}

void GCTR_decrypt(FILE* fileIn, FILE* fileOut,  uint8_t* key,  int Nk, uint8_t ICB[16], uint8_t X[16],uint8_t H[16],int lenAD, int indexTag){
    int Nr = Nk + 6;
    uint8_t in[16]  = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    uint8_t out[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    uint8_t CB[16]  = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    long index = 0;

    // Compute round key
    uint32_t *w = malloc(sizeof(uint32_t) * Nb * (Nr + 1));
    KeyExpansion(key, w, Nk); // All round keys are store in w

    memcpy(CB,ICB,16);
    while(index + 16 <= indexTag){
        // Decryption 
        AES_encrypt(CB,out,w,Nk); // Counter_i
        fread(in,1,16,fileIn); // C_i
        blockXOR(out,in); // Counter_I XOR C_i
        fwrite(out,1,16,fileOut); 

        // Authentification Tag
        blockXOR(X,in);
        product(X,H,X);

        inc32(CB);
        index += 16;
    }

    int left = (16 + indexTag - index)%16;
    if(left != 0){
        // Decryption
        AES_encrypt(CB,out,w,Nk); // Counter_i
        fread(in,1,left,fileIn); // C_i
        blockXOR(out,in); // Counter_i XOR C_i
        fwrite(out,1,left,fileOut);

        for(int i = left; i < 16; i++){
            in[i] = 0;
        }
        
        // Authentification Tag
        blockXOR(X,in);
        product(X,H,X);
    }

    uint64_t lenA = (uint64_t) lenAD*8;
    uint64_t lenC = (uint64_t) indexTag*8;
    uint8_t concat[16];
    for(int i = 0; i < 8; i++){
        concat[7-i] = (uint8_t) ((lenA&(0xff<<(i*8)))>>(i*8));
        concat[15-i] = (uint8_t) ((lenC&(0xff<<(i*8)))>>(i*8));
    }

    blockXOR(X,concat);
    product(X,H,X);
    free(w);
}

void GCM_decrypt(FILE* fileIn, FILE* fileOut, uint8_t *key, int Nk, uint8_t* IV, uint8_t* AD, int lenAD){
    int Nr = Nk + 6;

    // Get size of the input file
    fseek(fileIn,0,SEEK_END);
    long sizeFileIn = ftell(fileIn);

    // Extract the tag
    uint8_t TAG[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    long index_tag = sizeFileIn-3;
    char *read_tag = malloc(sizeof(char) * 4);
    read_tag[0] = 'a'; read_tag[1] = 'a'; read_tag[2] = 'a'; read_tag[3] = '\0';
    fseek(fileIn,-3,SEEK_CUR);
    while(index_tag >= 3 && strcmp(read_tag,"Tag") != 0){ // We start from the end of the file and stop when we find "Tag"
        fseek(fileIn, -1, SEEK_CUR); 
        fread(read_tag,1,3,fileIn);
        fseek(fileIn, -3,SEEK_CUR);
        index_tag -= 1;
    }
    free(read_tag);
    if(index_tag < 3){
        fprintf(stderr,"Tag not find\n");
        fclose(fileIn);
        fclose(fileOut);
        free(key);
        free(AD);
    } else {
        fseek(fileIn ,3*2,SEEK_CUR);
        extractTag(fileIn,TAG);
        index_tag -= 2;
    }
    fseek(fileIn,0,SEEK_SET);

    // Compute round key
    uint32_t *w = malloc(sizeof(uint32_t) * Nb * (Nr + 1));
    KeyExpansion(key, w, Nk); // All round keys are store in w

    // Compute ICB
    uint8_t ICB[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    uint8_t J0[16]  = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    uint8_t H[16]   = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    uint8_t X[16]   = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    uint8_t AD_[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    int index = 0;

    AES_encrypt(H,H,w,Nk); // H = E_k(0)
    
    // lenIV must be equal at 12 bytes (96 bits), NIST recommandation
    memcpy(J0,IV,12);
    J0[15] = (uint8_t) 1;
    memcpy(ICB,J0,16);

    inc32(ICB);

    while(index + 16 <= lenAD){
        memcpy(AD_,AD,16);
        blockXOR(X,AD_);
        product(X,H,X);
        AD += 16;
        index += 16;
    }
    int left = (16 + lenAD - index)%16;
    if(left != 0){
        memcpy(AD_,AD,left);
        for(int i=left;i<16;i++){AD_[i] = 0;}
        blockXOR(X,AD_);
        product(X,H,X);
    }

    GCTR_decrypt(fileIn,fileOut,key,Nk,ICB,X,H,lenAD,index_tag);

    AES_encrypt(J0,J0,w,Nk);

    blockXOR(X,J0);
    if(memcmp(X,TAG,16)){
        fprintf(stderr,"The tags don't match\n");
        exit(EXIT_FAILURE);
    }  
    free(w);
}

