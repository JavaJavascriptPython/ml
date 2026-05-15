#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "libssh2.h"
#include "libssh2_sftp.h"

// ----- YOUR SERVER DETAILS (change these) -----
#define TEST_SERVER "192.168.1.100"
#define TEST_PORT   22
#define TEST_USER   "your_username"
#define TEST_PASS   "your_password"
#define REMOTE_FILE "/home/user/test.txt"
// ---------------------------------------------

// WARNING: Skipping hostkey verification (for testing only!)
static int skip_hostkey_check(const char *hostname,
                              size_t hostname_len,
                              const char *hostkey,
                              size_t hostkey_len,
                              const char *algo,
                              size_t algo_len,
                              void *abstract)
{
    (void)hostname; (void)hostname_len;
    (void)hostkey; (void)hostkey_len;
    (void)algo; (void)algo_len;
    (void)abstract;
    fprintf(stderr, "WARNING: Skipping hostkey verification (testing only!)\n");
    return 0;  // 0 = accept
}

int main()
{
    WSADATA wsaData;
    SOCKET sock = INVALID_SOCKET;
    LIBSSH2_SESSION *session = NULL;
    LIBSSH2_SFTP *sftp = NULL;
    LIBSSH2_SFTP_HANDLE *sftp_handle = NULL;
    char mem[4096];
    int rc;
    struct sockaddr_in sin;
    struct addrinfo hints, *res;

    // 1. Init WinSock
    rc = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (rc != 0) {
        fprintf(stderr, "WSAStartup failed: %d\n", rc);
        return 1;
    }

    // 2. Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        fprintf(stderr, "socket failed: %d\n", WSAGetLastError());
        goto cleanup;
    }

    // 3. Resolve host and connect
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    if (getaddrinfo(TEST_SERVER, "22", &hints, &res) != 0) {
        fprintf(stderr, "getaddrinfo failed\n");
        goto cleanup;
    }

    memset(&sin, 0, sizeof(sin));
    memcpy(&sin.sin_addr, &((struct sockaddr_in*)res->ai_addr)->sin_addr, sizeof(sin.sin_addr));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(TEST_PORT);
    freeaddrinfo(res);

    if (connect(sock, (struct sockaddr*)&sin, sizeof(sin)) != 0) {
        fprintf(stderr, "connect failed: %d\n", WSAGetLastError());
        goto cleanup;
    }

    // 4. Init libssh2 session
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

    libssh2_session_hostkey_callback_set(session, skip_hostkey_check, NULL);
    libssh2_session_set_timeout(session, 15000);

    rc = libssh2_session_handshake(session, sock);
    if (rc) {
        fprintf(stderr, "Handshake failed: %d\n", rc);
        goto cleanup;
    }

    // 5. Authenticate
    fprintf(stderr, "Authenticating as %s...\n", TEST_USER);
    if (libssh2_userauth_password(session, TEST_USER, TEST_PASS)) {
        fprintf(stderr, "Authentication failed\n");
        goto cleanup;
    }

    // 6. Start SFTP subsystem
    sftp = libssh2_sftp_init(session);
    if (!sftp) {
        fprintf(stderr, "SFTP init failed\n");
        goto cleanup;
    }

    // 7. Open remote file for reading
    fprintf(stderr, "Opening remote file: %s\n", REMOTE_FILE);
    sftp_handle = libssh2_sftp_open(sftp, REMOTE_FILE, LIBSSH2_FXF_READ, 0);
    if (!sftp_handle) {
        fprintf(stderr, "SFTP open failed\n");
        goto cleanup;
    }

    // 8. Read and print file
    fprintf(stderr, "Reading file:\n");
    while (1) {
        ssize_t nread = libssh2_sftp_read(sftp_handle, mem, sizeof(mem)-1);
        if (nread < 0) {
            fprintf(stderr, "SFTP read error\n");
            goto cleanup;
        }
        if (nread == 0) break;   // EOF
        mem[nread] = '\0';
        printf("%s", mem);
    }
    printf("\n--- End of file ---\n");

cleanup:
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