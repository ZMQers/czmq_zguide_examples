/*  =========================================================================
    msgqueue - simple message queuing broker
    Same as request-reply broker but using shared queue proxy

    Copyright (c) the Authors
    =========================================================================
*/

/*
@header
    msgqueue -
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
            puts ("msgqueue [options] ...");
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
        zsys_info ("msgqueue - Simple message queuing broker");

    void *context = zmq_ctx_new ();

    //  Socket facing clients
    void *frontend = zmq_socket (context, ZMQ_ROUTER);
    int rc = zmq_bind (frontend, "tcp://*:5559");
    assert (rc == 0);

    //  Socket facing services
    void *backend = zmq_socket (context, ZMQ_DEALER);
    rc = zmq_bind (backend, "tcp://*:5560");
    assert (rc == 0);

    //  Start the proxy
    zmq_proxy (frontend, backend, NULL);

    //  We never get here...
    zmq_close (frontend);
    zmq_close (backend);
    zmq_ctx_destroy (context);
    return 0;
}
