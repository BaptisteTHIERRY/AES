#include "main.h"

uint8_t *keyExtraction(char *stringKey, uint8_t *key, int *Nk){
    int size = 0;
    int index = 0;
    char *currentHex = malloc(sizeof(char) * 3);  
    char *end = "\0";

    if(currentHex == NULL){
        fprintf(stderr,"Error while allocationg memory\n");
        exit(EXIT_FAILURE);
    }
    currentHex[0] = '0';
    currentHex[1] = '1';
    currentHex[2] = '\0';
    if(stringKey[0] == '\0' || stringKey[1] == '\0'){
        free(currentHex);
        fprintf(stderr,"The key must be of size 128, 192 or 256 bits\n");
        exit(EXIT_FAILURE);
    }
    if(stringKey[0] == '0' && stringKey[1] == 'x'){
        index = 2;
    }
    while(stringKey[index] != '\0' && stringKey[index+1] != '\0' && size < 32){
        if((size == 16) && (*Nk == 4)){
            *Nk = 6;
            key = (uint8_t *) realloc(key, 24*sizeof(uint8_t));
        }
        if((size == 24) && (*Nk == 6)){
            *Nk = 8;
            key = (uint8_t *) realloc(key, 32*sizeof(uint8_t));
        }
        currentHex[0] = stringKey[index];
        currentHex[1] = stringKey[index + 1];
        key[size] = (uint8_t) strtol(currentHex,&end,16);
        if(*end != '\0'){
            free(currentHex);
            fprintf(stderr,"The key must be write in hexadecimal.\n");
            exit(EXIT_FAILURE);
        }
        index += 2;
        size += 1;
    }
    if(size > 32 || (size != 32 && size != 24 && size != 16) || (stringKey[index] != '\0' && stringKey[index + 1] == '\0')){
        free(currentHex);
        fprintf(stderr,"The key must be of size 128, 192 or 256 bits\n");
        exit(EXIT_FAILURE);
    }
    free(currentHex);
    return key;
}

void handleError(char* str, FILE* fileIn, FILE* fileOut, uint8_t* key){
    if(fileIn != NULL){fclose(fileIn);}
    if(fileOut != NULL){fclose(fileOut);}
    if(key != NULL){free(key);}
    fprintf(stderr,str);
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv){
    int Nk = 4;
    enum MODE mode = ECB;
    enum CIPHER operation = ENCRYPT;
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

    // Options parser

    int opt = 0;
    int option_index = 0;
    struct option long_options[] = {{"help", no_argument, NULL, 'h'},
                                    {"input", required_argument, NULL, 'i'},
                                    {"output", required_argument, NULL, 'o'},
                                    {"key", required_argument, NULL, 'k'},
                                    {"mode", required_argument, NULL, 'm'},
                                    {"decrypt", no_argument, NULL, 'd'}};
    /*
    Long option : {"name", ifArgument, pointer, pointerValueIfOption}
    Here pointer is NULL so the function return the pointerValueIfOption,
    like that we have the same behavior for the long options and short options
    */
    opt = getopt_long(argc, argv, "+hdi:k:o:m:", long_options, &option_index);
    while (opt != -1) {
        switch (opt) {
        case 'h':
            free(key);
            if(fileIn != NULL){fclose(fileIn);}
            if(fileOut != NULL){fclose(fileOut);}
            printf(HELP_MESSAGE);
            exit(EXIT_SUCCESS);
        case 'o':
            fileOut = fopen(optarg, "wb");
            if (fileOut == NULL) {handleError("An error occured while opening the output file\n",fileIn,fileOut,key);}
            break;
        case 'i':
            fileIn = fopen(optarg, "rb");
            if (fileIn == NULL) {handleError("An error occured while opening the input file\n",fileIn,fileOut,key);}
            break;
        case 'k' :
            key = keyExtraction(optarg,key,&Nk);
            break;
        case 'm' :
            if(!strcmp("ECB",optarg)){
                mode = ECB;
                break;
            }
            if(!strcmp("CBC",optarg)){
                mode = CBC;
                break;
            }
            if(!strcmp("CFB",optarg)){
                mode = CFB;
                break;
            }
            if(!strcmp("GCM",optarg)){
                mode = GCM;
                break;
            }
            handleError("MODE must be ECB, CBC, CFB or GCM\n",fileIn,fileOut,key);
            break;
        case 'd' :
            operation = DECRYPT;
            break;
        default:
            handleError(HELP_MESSAGE,fileIn,fileOut,key);
        }
        opt = getopt_long(argc, argv, "+hdi:k:o:m:", long_options, &option_index);
    }
    if(fileIn == NULL){handleError("No input file, error.\n",fileIn,fileOut,key);}
    if(fileOut == NULL){handleError("No output file, error.\n",fileIn,fileOut,key);}
    switch (mode)
    {
        case ECB :
            if(operation == ENCRYPT){
                ECB_encrypt(fileIn,fileOut,key,Nk);
            } else{
                ECB_decrypt(fileIn,fileOut,key,Nk);
            }
            break;
        
        default:
            handleError("Mode not implemented yet.\n",fileIn,fileOut,key);
            break;
    }

    fclose(fileIn);
    fclose(fileOut);
    free(key);
    
    return EXIT_SUCCESS;
}