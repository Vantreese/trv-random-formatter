#ifndef _rndlib_h_
#define _rndlib_h_

#include <stdlib.h>

unsigned char *randomdevice(int rndsize, unsigned char *rndbuf);

void encodeatom_base32(char b32[8], unsigned char c[5], size_t csize, char *option);
void encodeatom_base64(char b64[4], unsigned char c[3], size_t csize, char *option);

char *binarydevice(int binsize);
char *biometricdevice(int *biosize);
char *decimal10device(int decsize);
char *alphadevice(int alphasize, char *option);
char *hexadecimaldevice(int hexsize);
char *base32device(int b32size, char *option);
char *base64device(int b64size, char *option);
char *PBMdevice(int *imagesize);
char *PGMdevice(int *imagesize);
char *PPMdevice(int *imagesize);

#endif
