/*  =========================================================================
    printerrno - description

    Copyright (c) the Authors
    =========================================================================
*/

/*
@header
    printerrno -
@discuss
@end
*/

#include "czguide_classes.h"

int main (int argc, char *argv [])
{
    bool verbose = false;
    int argn;
    for (argn = 1; argn < argc; argn++) {
        if (streq (argv [argn], "--help")
        ||  streq (argv [argn], "-h")) {
            puts ("printerrno [options] ...");
            puts ("  --verbose / -v         verbose test output");
            puts ("  --help / -h            this information");
            return 0;
        }
        else
        if (streq (argv [argn], "--verbose")
        ||  streq (argv [argn], "-v"))
            verbose = true;
        else {
            printf ("Unknown option: %s\n", argv [argn]);
            return 1;
        }
    }
    //  Insert main code here
    if (verbose)
        zsys_info ("printerrno - Printing ErrorNo");

    printf("errno:%d\n", errno);

    printf("%d - EINVAL", EINVAL);
    printf("%d - EPROTONOSUPPORT\n", EPROTONOSUPPORT);
    printf("%d - ENOCOMPATPROTO\n", ENOCOMPATPROTO);
    printf("%d - EADDRINUSE\n", EADDRINUSE);
    printf("%d - EADDRNOTAVAIL\n", EADDRNOTAVAIL);
    printf("%d - ENODEV\n", ENODEV);
    printf("%d - ETERM\n", ETERM);
    printf("%d - ENOTSOCK:\n", ENOTSOCK);
    printf("%d - EMTHREAD:\n", EMTHREAD);

    return 0;
}
