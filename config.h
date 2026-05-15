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






#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "libssh2.h"
#include "libssh2_sftp.h"

#define TEST_SERVER "test.rebex.net"
#define TEST_PORT   22
#define TEST_USER   "demo"
#define TEST_PASS   "password"
#define REMOTE_FILE "/pub/example/readme.txt"

// For production, you should verify the host key. For testing, we skip it.
static int skip_hostkey_check(const char *hostname,
                              size_t hostname_len,
                              const char *hostkey,
                              size_t hostkey_len,
                              const char *algo,
                              size_t algo_len,
                              void *abstract) {
    (void)hostname; (void)hostname_len;
    (void)hostkey; (void)hostkey_len;
    (void)algo; (void)algo_len;
    (void)abstract;
    fprintf(stderr, "WARNING: Skipping hostkey verification (testing only!)\n");
    return 0;  // 0 means accept the key
}

int main() {
    WSADATA wsaData;
    SOCKET sock = INVALID_SOCKET;
    LIBSSH2_SESSION *session = NULL;
    LIBSSH2_SFTP *sftp = NULL;
    LIBSSH2_SFTP_HANDLE *sftp_handle = NULL;
    char mem[4096];
    int rc, i;
    struct sockaddr_in sin;

    // 1. Init WinSock
    rc = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (rc != 0) {
        fprintf(stderr, "WSAStartup failed: %d\n", rc);
        return 1;
    }

    // 2. Create socket and connect
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        fprintf(stderr, "socket failed: %d\n", WSAGetLastError());
        goto cleanup;
    }

    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(TEST_PORT);
    // test.rebex.net -> resolve IP
    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    if (getaddrinfo(TEST_SERVER, "22", &hints, &res) != 0) {
        fprintf(stderr, "getaddrinfo failed\n");
        goto cleanup;
    }
    memcpy(&sin.sin_addr, &((struct sockaddr_in*)res->ai_addr)->sin_addr, sizeof(sin.sin_addr));
    freeaddrinfo(res);

    if (connect(sock, (struct sockaddr*)&sin, sizeof(sin)) != 0) {
        fprintf(stderr, "connect failed: %d\n", WSAGetLastError());
        goto cleanup;
    }

    // 3. Init libssh2 session and handshake
    rc = libssh2_init(0);
    if (rc != 0) {
        fprintf(stderr, "libssh2_init failed: %d\n", rc);
        goto cleanup;
    }

    session = libssh2_session_init();
    if (!session) {
        fprintf(stderr, "libssh2_session_init failed\n");
        goto cleanup;
    }

    // Set hostkey callback to skip verification (only for testing!)
    libssh2_session_hostkey_callback_set(session, skip_hostkey_check, NULL);

    // Set banner timeout
    libssh2_session_set_timeout(session, 15000);

    rc = libssh2_session_handshake(session, sock);
    if (rc) {
        fprintf(stderr, "Handshake failed: %d\n", rc);
        goto cleanup;
    }

    // 4. Authenticate
    fprintf(stderr, "Authenticating as %s...\n", TEST_USER);
    if (libssh2_userauth_password(session, TEST_USER, TEST_PASS)) {
        fprintf(stderr, "Authentication failed\n");
        goto cleanup;
    }

    // 5. Request SFTP subsystem
    sftp = libssh2_sftp_init(session);
    if (!sftp) {
        fprintf(stderr, "SFTP init failed\n");
        goto cleanup;
    }

    // 6. Open remote file for reading
    fprintf(stderr, "Opening remote file: %s\n", REMOTE_FILE);
    sftp_handle = libssh2_sftp_open(sftp, REMOTE_FILE, LIBSSH2_FXF_READ, 0);
    if (!sftp_handle) {
        fprintf(stderr, "SFTP open failed\n");
        goto cleanup;
    }

    // 7. Read and print file contents
    fprintf(stderr, "Reading file:\n");
    while (1) {
        ssize_t nread = libssh2_sftp_read(sftp_handle, mem, sizeof(mem)-1);
        if (nread < 0) {
            fprintf(stderr, "SFTP read error\n");
            goto cleanup;
        }
        if (nread == 0) break;  // EOF
        mem[nread] = '\0';
        printf("%s", mem);
    }
    printf("\n--- End of file ---\n");

cleanup:
    // Close handles in reverse order
    if (sftp_handle) libssh2_sftp_close(sftp_handle);
    if (sftp)        libssh2_sftp_shutdown(sftp);
    if (session) {
        libssh2_session_disconnect(session, "Normal Shutdown");
        libssh2_session_free(session);
    }
    if (sock != INVALID_SOCKET) closesocket(sock);
    libssh2_exit();
    WSACleanup();

    return 0;
}