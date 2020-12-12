#ifndef CHECK_H
#define CHECK_H

#include <limits.h>
#include <mpi.h>
#include <pthread.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#ifndef HOST_NAME_MAX
# if defined(_POSIX_HOST_NAME_MAX)
#  define HOST_NAME_MAX _POSIX_HOST_NAME_MAX
# elif defined(MAXHOSTNAMELEN)
#  define HOST_NAME_MAX MAXHOSTNAMELEN
# endif
#endif /* HOST_NAME_MAX */

#define MPI_ERR_CHECK(call)                                      \
    do { int err = call;                                         \
    if (err != MPI_SUCCESS) {                                    \
        char hostname[HOST_NAME_MAX] = "";                       \
        gethostname(hostname, HOST_NAME_MAX);                    \
        char errstr[MPI_MAX_ERROR_STRING];                       \
        int szerrstr;                                            \
        MPI_Error_string(err, errstr, &szerrstr);                \
        fprintf(stderr, "MPI error on %s at %s:%i : %s\n",       \
            hostname, __FILE__, __LINE__, errstr);               \
        if (!getenv("FREEZE_ON_ERROR")) {                        \
            fprintf(stderr, "You may want to set "               \
                "FREEZE_ON_ERROR environment "                   \
                "variable to debug the case\n");                 \
            MPI_Abort(MPI_COMM_WORLD, err);                      \
        }                                                        \
        else {                                                   \
            fprintf(stderr, "thread 0x%zx of pid %d @ %s "       \
               "is entering infinite loop\n",                    \
               (size_t)pthread_self(), (int)getpid(), hostname); \
            while (1) usleep(1000000); /* 1 sec */               \
        }                                                        \
    }} while (0)

#endif // CHECK_H

