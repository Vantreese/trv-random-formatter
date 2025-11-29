#include "biometric.h"
#include "rndlib.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <fcntl.h>

typedef union {
    unsigned long long
    i;

    struct _b32_ {
        unsigned long long
        b32_0 : 5, b32_1 : 5, b32_2 : 5, b32_3 : 5, b32_4 : 5, b32_5 : 5, b32_6 : 5, b32_7 : 5;
    } b32;

    struct _c5_ {
        unsigned char
        c_0, c_1, c_2, c_3, c_4;
    } c;
} base32;

typedef union {
    unsigned int
    i;

    struct _b64_ {
        unsigned int
        b64_0 : 6, b64_1 : 6, b64_2 : 6, b64_3 : 6;
    } b64;

    struct _c3_ {
        unsigned char
        c_0, c_1, c_2;
    } c;
} base64;

typedef union {
    unsigned char
    c;

    struct _h_ {
        unsigned char
        h_0 : 4, h_1 : 4;
    } h;
} hex;

typedef union {
    unsigned char
    c;

    struct _b_ {
        unsigned char
        b_0 : 1, b_1 : 1, b_2 : 1, b_3 : 1, b_4 : 1, b_5 : 1, b_6 : 1, b_7 : 1;
    } b;
} bin;

/****** THE RANDOM DEVICE ******/

unsigned char *randomdevice(int rndsize, unsigned char *rndbuf) {
    FILE *rnddev;
    char devicecmd[80];
    int add1 = 0;

    if (rndsize <= 0) {
        fprintf(stderr, "Random size must be larger than 0.\n");
        return NULL;
    }
    if (!rndbuf)
        rndbuf = malloc(rndsize);
    if (!rndbuf) {
        fprintf(stderr, "Can't allocate memory for random data.\n");
        return NULL;
    }
    //sprintf(devicecmd, "/dev/random");
    //rnddev = fopen("/dev/random", "rb");	// Does not work well in Windows.
    add1 = (rndsize % 4) == 0 ? 0 : 1;
    sprintf(devicecmd, "randomdevice %d", (rndsize / 4) + add1);
    rnddev = popen(devicecmd, "rb");
    if (!rnddev) {
        fprintf(stderr, "Can't open %s\n", devicecmd);
        return NULL;
    }
    fread(rndbuf, 1, rndsize, rnddev);
    fclose(rnddev);
    return rndbuf;
}

/*******************************/



unsigned int number(unsigned int array[], size_t arraylength)
{
    // array contains all the random numbers.
    // arraylength is the length of the array and implies max range of random numbers.
    // Higest number in array determines the position and random number.
    int flag = 0;   // 1 = found 1, > 1 = found more than 1.
    int iloop;
    unsigned int hipos;     // position of highest number
    unsigned int hi = 0;    // highest number

    for (iloop = 0; iloop < arraylength; ++iloop)
    {
        if (array[iloop] == hi)
        {
            ++flag;
        }
        if (array[iloop] > hi)
        {
            flag = 1;
            hi = array[iloop];
            hipos = iloop;
        }
    }

    if (flag > 1)
    {
        // Odds are 4 billion to 1 that flag is not 1.
        for (iloop = 0; iloop < arraylength; ++iloop)
        {
            if (array[iloop] == hi)
            {
                // Draw a new random number.
                randomdevice(4, (unsigned char*) &(array[iloop]));
            }
            if ((hi > 0) && (array[iloop] < hi))
            {
                array[iloop] = 0;
            }
        }
        hipos = number(array, arraylength);
    }

    return hipos;
}




void encodeatom_base32(char b32[8], unsigned char c[5], size_t csize, char *option) {
    static char
    encodingTable[33];
    static int
    first = 1;
    base32
    n;

    if (first) {
        first = 0;
        if (!strcasecmp(option, "b32hex")) {
            switch (option[0]) {
                case 'b':
                    strcpy(encodingTable, "0123456789abcdefghijklmnopqrstuv");
                    break;
                case 'B':
                    strcpy(encodingTable, "0123456789ABCDEFGHIJKLMNOPQRSTUV");
                    break;
            }
        } else if (!strcasecmp(option, "zb32")) {
            switch (option[0]) {
                case 'z':
                    strcpy(encodingTable, "ybndrfg8ejkmcpqxot1uwisza345h769");
                    break;
                case 'Z':
                    strcpy(encodingTable, "YBNDRFG8EJKMCPQXOT1UWISZA345H769");
                    break;
            }
        } else if (!strcasecmp(option, "b32msdn")) {
            switch (option[0]) {
                case 'b':
                    strcpy(encodingTable, "0123456789abcdefghjklmnpqrstuvwx");
                    break;
                case 'B':
                    strcpy(encodingTable, "0123456789ABCDEFGHJKLMNPQRSTUVWX");
                    break;
            }
        } else {
            switch (option[0]) {
                case 'b':
                    strcpy(encodingTable, "abcdefghijklmnopqrstuvwxyz234567");
                    break;
                case 'B':
                    strcpy(encodingTable, "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567");
                    break;
            }
        }
    }
    n.i = 0L;
    switch (csize) {
        case 1:
            n.c.c_0 = c[0];
            b32[0] = encodingTable[n.b32.b32_0];
            b32[1] = encodingTable[n.b32.b32_1];
            b32[2] = '=';
            b32[3] = '=';
            b32[4] = '=';
            b32[5] = '=';
            b32[6] = '=';
            b32[7] = '=';
            break;
        case 2:
            n.c.c_0 = c[0];
            n.c.c_1 = c[1];
            b32[0] = encodingTable[n.b32.b32_0];
            b32[1] = encodingTable[n.b32.b32_1];
            b32[2] = encodingTable[n.b32.b32_2];
            b32[3] = encodingTable[n.b32.b32_3];
            b32[4] = '=';
            b32[5] = '=';
            b32[6] = '=';
            b32[7] = '=';
            break;
        case 3:
            n.c.c_0 = c[0];
            n.c.c_1 = c[1];
            n.c.c_2 = c[2];
            b32[0] = encodingTable[n.b32.b32_0];
            b32[1] = encodingTable[n.b32.b32_1];
            b32[2] = encodingTable[n.b32.b32_2];
            b32[3] = encodingTable[n.b32.b32_3];
            b32[4] = encodingTable[n.b32.b32_4];
            b32[5] = '=';
            b32[6] = '=';
            b32[7] = '=';
            break;
        case 4:
            n.c.c_0 = c[0];
            n.c.c_1 = c[1];
            n.c.c_2 = c[2];
            n.c.c_3 = c[3];
            b32[0] = encodingTable[n.b32.b32_0];
            b32[1] = encodingTable[n.b32.b32_1];
            b32[2] = encodingTable[n.b32.b32_2];
            b32[3] = encodingTable[n.b32.b32_3];
            b32[4] = encodingTable[n.b32.b32_4];
            b32[5] = encodingTable[n.b32.b32_5];
            b32[6] = encodingTable[n.b32.b32_6];
            b32[7] = '=';
            break;
        case 5:
            n.c.c_0 = c[0];
            n.c.c_1 = c[1];
            n.c.c_2 = c[2];
            n.c.c_3 = c[3];
            n.c.c_4 = c[4];
            b32[0] = encodingTable[n.b32.b32_0];
            b32[1] = encodingTable[n.b32.b32_1];
            b32[2] = encodingTable[n.b32.b32_2];
            b32[3] = encodingTable[n.b32.b32_3];
            b32[4] = encodingTable[n.b32.b32_4];
            b32[5] = encodingTable[n.b32.b32_5];
            b32[6] = encodingTable[n.b32.b32_6];
            b32[7] = encodingTable[n.b32.b32_7];
            break;
        default: /* This line should never happen. */
            b32[0] = '=';
            b32[1] = '=';
            b32[2] = '=';
            b32[3] = '=';
            b32[4] = '=';
            b32[5] = '=';
            b32[6] = '=';
            b32[7] = '=';
            break;
    }
}

void encodeatom_base64(char b64[4], unsigned char c[3], size_t csize, char *option) {
    static char
    encodingTable[64] = {
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
        'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
        'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
        'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
        'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
        'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
        'w', 'x', 'y', 'z', '0', '1', '2', '3',
        '4', '5', '6', '7', '8', '9', '+', '/'
    };
    static int
    first = 1;
    base64
    n;

    if (first) {
        first = 0;
        if (!strcasecmp(option, "b64m")) {
            encodingTable[62] = '-';
            encodingTable[63] = '_';
        }
    }
    n.i = 0;
    n.c.c_0 = c[0];
    b64[0] = encodingTable[n.b64.b64_0];
    if (csize == 1) {
        b64[1] = encodingTable[n.b64.b64_1];
        b64[2] = '=';
        b64[3] = '=';
    } else {
        n.c.c_1 = c[1];
        b64[1] = encodingTable[n.b64.b64_1];
    }
    if (csize == 2) {
        b64[2] = encodingTable[n.b64.b64_2];
        b64[3] = '=';
    } else if (csize != 1) {
        n.c.c_2 = c[2];
        b64[2] = encodingTable[n.b64.b64_2];
        b64[3] = encodingTable[n.b64.b64_3];
    }
}

char *binarydevice(int binsize) {
    char
    bintable[2] = {'0', '1'};
    char
    *binbuf, *rndbuf;
    bin
    n;
    int
    rndsize = (binsize + 7) / 8;
    int
    loop, pos = 0;

    if (rndsize <= 0) return NULL;
    binbuf = malloc(rndsize * 8);
    rndbuf = randomdevice(rndsize, NULL);
    for (loop = 0; loop < rndsize; ++loop) {
        n.c = rndbuf[loop];
        binbuf[pos++] = bintable[n.b.b_0];
        binbuf[pos++] = bintable[n.b.b_1];
        binbuf[pos++] = bintable[n.b.b_2];
        binbuf[pos++] = bintable[n.b.b_3];
        binbuf[pos++] = bintable[n.b.b_4];
        binbuf[pos++] = bintable[n.b.b_5];
        binbuf[pos++] = bintable[n.b.b_6];
        binbuf[pos++] = bintable[n.b.b_7];
    }
    free(rndbuf);
    return binbuf;
}

char *biometricdevice(int *biosize) {
    unsigned char
    *rndbuf;
    char
    *biobuf;
    int
    rndsize = *biosize * 2;
    int
    loop;

    rndbuf = randomdevice(rndsize, NULL);
    *biosize = 0;
    for (loop = 0; loop < rndsize; loop += 2) {
        *biosize += strlen(BioBE[rndbuf[loop]]) + strlen(BioLE[rndbuf[loop + 1]]) + 2;
    }
    biobuf = calloc(*biosize, sizeof (char));
    for (loop = 0; loop < rndsize; loop += 2) {
        strcat(biobuf, BioBE[rndbuf[loop]]);
        strcat(biobuf, "-");
        strcat(biobuf, BioLE[rndbuf[loop + 1]]);
        strcat(biobuf, " ");
    }
    biobuf[*biosize - 1] = '\0';
    free(rndbuf);
    return biobuf;
}

char *decimal10device(int decsize) {
    char dectable[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
    char *decbuf;
    unsigned int rndbuf[6 * decsize];
    unsigned int n, pos;
    int m;
    int rndsize = (4 * 6 * decsize);
    int loop;

    if (decsize <= 0) return NULL;
    decbuf = malloc(decsize);
    randomdevice(rndsize, (unsigned char *) rndbuf);
    for (loop = 0; loop < decsize; ++loop) {
        pos = 6 * loop;
        n = number((unsigned int*)&(rndbuf[pos]), 5);
        m = (int) rndbuf[pos + 5];
        decbuf[loop] = (m < 0 ? dectable[n] : dectable[n + 5]);
    }
    return decbuf;
}

char *dice(int numofdice)
{
    char dicetable[6] = {'1', '2', '3', '4', '5', '6' };
    char *dicebuf;
    unsigned int rndbuf[4 * numofdice];
    unsigned int n, pos;
    int m;
    int rndsize = (4 * 4 * numofdice);
    int loop;

    if (numofdice <= 0) return NULL;
    dicebuf = malloc(numofdice);
    randomdevice(rndsize, (unsigned char *) rndbuf);
    for (loop = 0; loop < numofdice; ++loop)
    {
        pos = 4 * loop;
        n = number((unsigned int*)&(rndbuf[pos]), 3);
        m = (int) rndbuf[pos + 3];
        dicebuf[loop] = (m < 0 ? dicetable[n] : dicetable[n + 3]);
    }
    return dicebuf;
}

char *alphadevice(int alphasize, char *option) {
    static int
    first = 1;
    static char
    alphatable[26] = {
        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
        'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'
    };
    char
    *alphabuf;
    unsigned int
    rndbuf[14 * alphasize];
    unsigned int
    n, hi;
    int
    m;
    int
    //rndsize = (4 * 14);
    rndsize = (4 * 14 * alphasize);
    int
    loop, pos = 0;


    if (first) {
        first = 0;
        if (option[0] == 'A') {
            strncpy(alphatable, "ABCDEFGHIJKLMNOPQRSTUVWXYZ", 26);
        }
        /*		else
                {
                    strncpy(alphatable, "abcdefghijklmnopqrstuvwxyz", 26);
                } */
    }

    if (alphasize <= 0) return NULL;
    alphabuf = malloc(alphasize);
    randomdevice(rndsize, (unsigned char *) rndbuf);
    for (loop = 0; loop < alphasize; ++loop, hi = 0) {
        //randomdevice(rndsize, (unsigned char *) rndbuf);
        pos = 14 * loop;
        n = number((unsigned int*)&(rndbuf[pos]), 13);
        /*
        if (rndbuf[0] > hi) {
            n = 0;
            hi = rndbuf[n];
        }
        if (rndbuf[1] > hi) {
            n = 1;
            hi = rndbuf[n];
        }
        if (rndbuf[2] > hi) {
            n = 2;
            hi = rndbuf[n];
        }
        if (rndbuf[3] > hi) {
            n = 3;
            hi = rndbuf[n];
        }
        if (rndbuf[4] > hi) {
            n = 4;
            hi = rndbuf[n];
        }
        if (rndbuf[5] > hi) {
            n = 5;
            hi = rndbuf[n];
        }
        if (rndbuf[6] > hi) {
            n = 6;
            hi = rndbuf[n];
        }
        if (rndbuf[7] > hi) {
            n = 7;
            hi = rndbuf[n];
        }
        if (rndbuf[8] > hi) {
            n = 8;
            hi = rndbuf[n];
        }
        if (rndbuf[9] > hi) {
            n = 9;
            hi = rndbuf[n];
        }
        if (rndbuf[10] > hi) {
            n = 10;
            hi = rndbuf[n];
        }
        if (rndbuf[11] > hi) {
            n = 11;
            hi = rndbuf[n];
        }
        if (rndbuf[12] > hi) {
            n = 12;
            hi = rndbuf[n];
        }
        */
        m = (int) rndbuf[pos + 13];
        alphabuf[loop] = (m < 0 ? alphatable[n] : alphatable[n + 13]);
    }
    return alphabuf;
}

char *hexadecimaldevice(int hexsize) {
    char
    hextable[16] = {'0', '1', '2', '3', '4', '5', '6', '7',
        '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
    char
    *hexbuf, *rndbuf;
    hex
    n;
    int
    rndsize = (hexsize + 1) / 2;
    int
    loop, pos = 0;

    if (rndsize <= 0) return NULL;
    hexbuf = malloc(rndsize * 2);
    rndbuf = randomdevice(rndsize, NULL);
    for (loop = 0; loop < rndsize; ++loop) {
        n.c = rndbuf[loop];
        hexbuf[pos++] = hextable[n.h.h_0];
        hexbuf[pos++] = hextable[n.h.h_1];
    }
    free(rndbuf);
    return hexbuf;
}

char *base32device(int b32size, char *option) {
    char
    *b32buf = NULL;
    int
    rndsize = 5 * ((b32size - 1) / 8 + 1);
    int
    loop, pos = 0;
    unsigned char
    *rndbuf;

    if (rndsize <= 0) return NULL;
    b32buf = malloc(8 * ((b32size - 1) / 8 + 1));
    rndbuf = randomdevice(rndsize, NULL);
    for (loop = 0; loop < rndsize; loop += 5) {
        encodeatom_base32(&(b32buf[pos]), &(rndbuf[loop]), 5, option);
        pos += 8;
    }
    free(rndbuf);
    return b32buf;
}

char *base64device(int b64size, char *option) {
    char
    *b64buf;
    int
    rndsize = 3 * ((b64size - 1) / 4 + 1);
    int
    loop, pos = 0;
    unsigned char
    *rndbuf;

    if (rndsize <= 0) return NULL;
    b64buf = malloc(4 * ((b64size - 1) / 4 + 1));
    rndbuf = randomdevice(rndsize, NULL);
    for (loop = 0; loop < rndsize; loop += 3) {
        encodeatom_base64(&(b64buf[pos]), &(rndbuf[loop]), 3, option);
        pos += 4;
    }
    free(rndbuf);
    return b64buf;
}

char *PBMdevice(int *imagesize) {
    char header[160];
    int headerlength;
    unsigned char *image;
    int height = *imagesize;
    int modsize = *imagesize % 8;

    sprintf(header, "P4\n%d %d\n", *imagesize, *imagesize);
    headerlength = strlen(header);
    if (modsize != 0) {
        *imagesize += (8 - modsize);
    }
    *imagesize /= 8;
    *imagesize *= height;
    *imagesize += headerlength;
    image = malloc(*imagesize);
    strcpy(image, header);
    randomdevice((*imagesize - headerlength), &(image[headerlength]));
    return image;
}

char *PGMdevice(int *imagesize) {
    char header[160];
    int headerlength;
    unsigned char *image;

    sprintf(header, "P5\n%d %d\n255\n", *imagesize, *imagesize);
    headerlength = strlen(header);
    *imagesize *= *imagesize;
    *imagesize += headerlength;
    image = malloc(*imagesize);
    strcpy(image, header);
    randomdevice((*imagesize - headerlength), &(image[headerlength]));
    return image;
}

char *PPMdevice(int *imagesize) {
    char header[160];
    int headerlength;
    unsigned char *image;

    sprintf(header, "P6\n%d %d\n255\n", *imagesize, *imagesize);
    headerlength = strlen(header);
    *imagesize *= *imagesize;
    *imagesize *= 3;
    *imagesize += headerlength;
    image = malloc(*imagesize);
    strcpy(image, header);
    randomdevice((*imagesize - headerlength), &(image[headerlength]));
    return image;
}


