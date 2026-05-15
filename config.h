#ifndef LIBSSH2_CONFIG_H
#define LIBSSH2_CONFIG_H

/* Use Windows CNG crypto backend – no OpenSSL needed */
#define LIBSSH2_WINCNG

/* Tell libssh2 what system headers are available */
#define HAVE_WINSOCK2_H
#define HAVE_STDIO_H
#define HAVE_STRING_H
#define HAVE_STDLIB_H
#define HAVE_MEMORY_H
#define HAVE_INTTYPES_H
#define HAVE_STDINT_H

/* Windows lacks certain POSIX functions – map them */
#define snprintf  _snprintf
#define strcasecmp  _stricmp
#define strncasecmp _strnicmp

/* Disable optional features that pull in extra dependencies */
#define LIBSSH2_NO_AGENT        /* No SSH agent forwarding */
#define LIBSSH2_NO_HOSTKEY      /* No hostkey export functions */
#define LIBSSH2_NO_ED25519      /* No ED25519 key support */

#endif




bcrypt.lib
crypt32.lib
ws2_32.lib
advapi32.lib
user32.lib





#include <stdio.h>
#include "libssh2.h"

int main() {
    int rc = libssh2_init(0);
    if (rc == 0) {
        printf("libssh2 initialized successfully!\n");
        libssh2_exit();
    } else {
        printf("libssh2_init failed with error %d\n", rc);
    }
    return rc;
}