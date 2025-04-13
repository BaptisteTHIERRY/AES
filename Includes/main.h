#ifndef MAIN_H
#define MAIN_H

#define HELP_MESSAGE \
"Usage:\n\
\taes (-o FILE |--output FILE) (-i FILE |--input FILE) [-d | --decrypt] [-k KEY |--key KEY] [-m MODE |--mode MODE] \n\n\
Encrypt or decrypt a file with the AES algorithm\n\
-o FILE, --output FILE : write output to FILE\n\
-i FILE, --input FILE : read data from FILE\n\
-k KEY, --key KEY : Use KEY as the secret key of AES. KEY must be in hexadecimal and of size 128, 192 or 256 bits.\n\
-m MODE, --mode MODE : Use MODE with AES to encrypt the input file. MODE = [ECB|CBC|CFB|GCM].\n\
-I IV, --IV IV : Set IV for the modes CBC, CFB or GCM. IV must be in hexadecimal and of size 128 bits\n\
-h, --help display this help and exit\n"

enum MODE {ECB, CBC, CFB, GCM};
enum CIPHER {ENCRYPT, DECRYPT}; 

#include "ECB.h"
#include "CBC.h"
#include <errno.h>
#include <getopt.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#endif