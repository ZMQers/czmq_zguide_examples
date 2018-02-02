/*  =========================================================================
    syncsub - Synchronized subscriber

    Copyright (c) the Authors
    =========================================================================
*/

/*
@header
    syncsub -
@discuss
@end
*/

#include "czguide_classes.h"

//  Receive 0MQ string from socket and convert into C string
//  Caller must free returned string. Returns NULL if the context
//  is being terminated.
static char *
s_recv (void *socket) {
    char buffer [256];
    int size = zmq_recv (socket, buffer, 255, 0);
    if (size == -1)
        return NULL;
    return strndup (buffer, sizeof(buffer) - 1);
    // remember that the strdup family of functions use malloc/alloc for space for the new string.  It must be manually
    // freed when you are done with it.  Failure to do so will allow a heap attack.
}

//  Convert C string to 0MQ string and send to socket
static int
s_send (void *socket, char *string) {
    int size = zmq_send (socket, string, strlen (string), 0);
    return size;
}

int main (int argc, char *argv [])
{
    bool verbose = false;
    int argn;
    for (argn = 1; argn < argc; argn++) {
        if (streq (argv [argn], "--help")
        ||  streq (argv [argn], "-h")) {
            puts ("syncsub [options] ...");
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
        zsys_info ("syncsub - Synchronized subscriber");

    void *context = zmq_ctx_new ();

    //  First, connect our subscriber socket
    void *subscriber = zmq_socket (context, ZMQ_SUB);
    zmq_connect (subscriber, "tcp://localhost:5561");
    zmq_setsockopt (subscriber, ZMQ_SUBSCRIBE, "", 0);

    //  0MQ is so fast, we need to wait a while...
    sleep (1);

    //  Second, synchronize with publisher
    void *syncclient = zmq_socket (context, ZMQ_REQ);
    zmq_connect (syncclient, "tcp://localhost:5562");

    //  - send a synchronization request
    s_send (syncclient, "");

    //  - wait for synchronization reply
    char *string = s_recv (syncclient);
    free (string);

    //  Third, get our updates and report how many we got
    int update_nbr = 0;
    while (1) {
        char *string = s_recv (subscriber);
        if (strcmp (string, "END") == 0) {
            free (string);
            break;
        }
        free (string);
        update_nbr++;
    }
    printf ("Received %d updates\n", update_nbr);

    zmq_close (subscriber);
    zmq_close (syncclient);
    zmq_ctx_destroy (context);
    return 0;
}
