#include "test.h"

void handleError(char* str, FILE* fileIn, FILE* fileOut, uint8_t* key){
    if(fileIn != NULL){fclose(fileIn);}
    if(fileOut != NULL){fclose(fileOut);}
    if(key != NULL){free(key);}
    fprintf(stderr,str);
    exit(EXIT_FAILURE);
}

void ECB_modify(uint8_t* dataIn, uint8_t* dataOut, long sizeData, uint8_t in[16], uint8_t out[16], uint32_t *w){
    long index = 0;
    int i = 0;

    while(index + 16 <= sizeData){
        for(i = 0; i < 16; i++){in[i] = dataIn[index+i];} // Read 16 bytes of fileIn and store it in "in" 
        AES_encrypt(in, out, w, 4);
        for(i = 0; i < 16; i++){dataOut[index + i] = in[i];} // Write the 16 bytes of out in fileOut
        index += 16;
    }

    // left = (16 + sizeData - index)%16
    // pad = 16 - left

    for(i = 0; i < (16 + sizeData - index)%16; i++){in[i] = dataIn[index+i];} // Last data
    for(i = 0; i < 16 - (16 + sizeData - index)%16; i++){in[15-i] = (uint8_t) (16 - (16 + sizeData - index)%16);} // Padding

    AES_encrypt(in, out, w, 4);
    for(i = 0; i < 16; i++){dataOut[index + i] = out[i];}

}

int main(){
    uint8_t *key = malloc(sizeof(uint8_t) * 16);
    FILE* fileIn = NULL;
    FILE* fileOut = NULL;

    if(key == NULL){
        fprintf(stderr,"Error while allocationg memory\n");
        exit(EXIT_FAILURE);
    }

    // Default key
    for(int i = 0; i < 16; i++){
        key[i] = (uint8_t) i;
    }

    fileIn = fopen("./Tests/alice.sage","rb");
    fileOut = fopen("./Tests/alice.crypt", "wb");
    if(fileIn == NULL){handleError("Error : can't open alice.sage\n",fileIn,fileOut,key);}
    if(fileOut == NULL){handleError("Error : can't open the output file\n",fileIn,fileOut,key);}
    
    struct timeval stop, start;
    
    // time_t time_start = time(NULL);
    gettimeofday(&start, NULL);

    for(int i = 0; i < 100; i++){
        fseek(fileIn,0,SEEK_SET);
        fseek(fileOut,0,SEEK_SET);
        ECB_encrypt(fileIn,fileOut,key,4);
    }

    //time_t time_end = time(NULL);
    gettimeofday(&stop, NULL);
    double duration = ((stop.tv_sec - start.tv_sec)*1000000 + (stop.tv_usec - start.tv_usec))/((double) 1000000);
    printf("Time to encrypt 100 times the files alice.sage : %lf sec\n",duration);
    
    fclose(fileIn);
    fclose(fileOut);
    free(key);

    return EXIT_SUCCESS;

}