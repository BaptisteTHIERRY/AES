#ifndef MAIN_H
#define MAIN_H

#define HELP_MESSAGE \
"Usage:\n\
\taes (-o FILE |--output FILE) (-i FILE |--input FILE) [-d | --decrypt] [-k KEY |--key KEY] [-m MODE |--mode MODE] [-I IV | --IV IV] [-A AD | --AD AD] \n\n\
Encrypt or decrypt a file with the AES algorithm.\n\
-o FILE, --output FILE : write output to FILE.\n\
-i FILE, --input FILE : read data from FILE.\n\
-k KEY, --key KEY : Use KEY as the secret key of AES. KEY must be in hexadecimal and of size 128, 192 or 256 bits. By default KEY = 0x00010203...0x0f.\n\
-m MODE, --mode MODE : Use MODE with AES to encrypt the input file. MODE = [ECB|CBC|CFB|GCM]. By default MODE = ECB.\n\
-I IV, --IV IV : Set IV for the modes CBC, CFB or GCM. IV must be in hexadecimal and of size 128 bits. By default IV = 0.\n\
-A AD, --AD AD : Use AD as additionnal data for the tag in the GCM mode. By default AD = 0.\n\
-h, --help : Display this help and exit.\n"

enum MODE {ECB, CBC, CFB, GCM};
enum CIPHER {ENCRYPT, DECRYPT}; 

#include "ECB.h"
#include "CBC.h"
#include "CFB.h"
#include "GCM.h"
#include <errno.h>
#include <getopt.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#endif