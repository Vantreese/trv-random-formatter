/*
 * 21-Jan-2004
 * 13-Sep-2006
 * 02-Apr-2013
 * 03-Apr-2013
 * 2013-04-09
 * 2013-07-06
 */

#include "rndlib.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <fcntl.h>

#define VERSION_DATE "2013-07-06 15:10"

int main(int argc, char *argv[]) {
    char *encodetype = "raw";
    int rndsize = 8;
    unsigned char *rndbuf;
    int addline = 1;

    if (argc >= 2) {
        rndsize = atoi(argv[1]);
        if (rndsize <= 0) {
            fprintf(stderr, "%s\nrandom [<n> [<type>]]; n > 0\n\tTypes:\n"
                    "\t%s\n"    // alpha
                    "\t%s\n"    // b64
                    "\t%s\n"    // b32
                    "\t%s\n"    // B32
                    "\t%s\n"    // b16
                    "\t%s\n"    // decimal
                    "\t%s\n"    // binary
                    "\t%s\n"    // biometric
                    "\t%s\n"    // dice
                    "\t%s\n"    // pbm
                    "\t%s\n"    // pgm
                    "\t%s\n",   // ppm
                    VERSION_DATE,
                    "alpha, ALPHA: Alphabet in -lower- or -UPPER- case",
                    "b64, b64m: Base64, Base64-web",
                    "b32, b32hex, zb32, b32msdn: Base32 formats in lower-case",
                    "B32, B32HEX, ZB32, B32MSDN: Base32 formats in upper-case",
                    "b16, hex: Hexadecimal",
                    "dec, b10: Decimal",
                    "bin*: Binary (0's and 1's)",
                    "bio*: Biometric words",
                    "dice: Dice",
                    "pbm: Portable Bit Map image",
                    "pgm: Portable Gray Map image",
                    "ppm: Portable Pixel Map image"
                    );
            fprintf(stderr, "\n\tDefaults: n = 8, type = (raw)\n");
            return 1;
        }
    }
    if (argc >= 3) {
        encodetype = argv[2];
    }
    if (0) {
        // place holder
    } else if (!strcasecmp(encodetype, "alpha")) {
        rndbuf = alphadevice(rndsize, encodetype);
    } else if (!strcmp(encodetype, "b64") || !strcmp(encodetype, "b64m")) {
        rndbuf = base64device(rndsize, encodetype);
    } else if (!strncasecmp(encodetype, "b32", 3) || !strncasecmp(encodetype, "zb32", 4)) {
        rndbuf = base32device(rndsize, encodetype);
    } else if (!strcmp(encodetype, "b16") || !strcmp(encodetype, "hex")) {
        rndbuf = hexadecimaldevice(rndsize);
    } else if (!strcmp(encodetype, "dec") || !strcmp(encodetype, "b10")) {
        rndbuf = decimal10device(rndsize);
    } else if (!strncmp(encodetype, "bin", 3)) {
        rndbuf = binarydevice(rndsize);
    } else if (!strncmp(encodetype, "bio", 3)) {
        rndbuf = biometricdevice(&rndsize); // Necessary to change size of rndsize in biometricdevice()
    } else if (!strncmp(encodetype, "dice", 4)) {
        rndbuf = dice(rndsize);
    } else if (!strcmp(encodetype, "pbm")) {
        addline = 0;
        setmode(fileno(stdout), O_BINARY);
        rndbuf = PBMdevice(&rndsize);
    } else if (!strcmp(encodetype, "pgm")) {
        addline = 0;
        setmode(fileno(stdout), O_BINARY);
        rndbuf = PGMdevice(&rndsize);
    } else if (!strcmp(encodetype, "ppm")) {
        addline = 0;
        setmode(fileno(stdout), O_BINARY);
        rndbuf = PPMdevice(&rndsize);
    } else {
        addline = 0;
        setmode(fileno(stdout), O_BINARY);
        rndbuf = randomdevice(rndsize, NULL);
    }

    fwrite(rndbuf, 1, rndsize, stdout);
    if (addline) fputs("\n", stdout);
    fflush(stdout);
    free(rndbuf);
    return 0;
}
