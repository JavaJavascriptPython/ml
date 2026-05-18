validates and returns it.      */
    /* ================================================================ */
    printf("[INIT] Resolving host: %s...\n", SERVER_HOST);
    memset(&hints, 0, sizeof(hints));
    hints.ai_family   = AF_INET;       /* We want an IPv4 address */
    hints.ai_socktype = SOCK_STREAM;   /* We'll use TCP           */

    if (getaddrinfo(SERVER_HOST, "22", &hints, &res) != 0) {
        fprintf(stderr, "[INIT] ERROR: Could not resolve hostname: %s\n", SERVER_HOST);
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    /*
     * Copy the resolved IP address into our sockaddr_in struct.
     * sin_family = address family (IPv4)
     * sin_addr   = the IP address
     * sin_port   = the port number (htons converts from host byte order to network byte order)
     *              Different CPU architectures store numbers differently (big-endian vs little-endian).
     *              htons() ensures the port is in the format the network expects.
     */
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port   = htons(SERVER_PORT);
    memcpy(&sin.sin_addr,
           &((struct sockaddr_in*)res->ai_addr)->sin_addr,
           sizeof(sin.sin_addr));
    freeaddrinfo(res); /* Free the memory getaddrinfo() allocated */

    /* ================================================================ */
    /* PHASE 4: CONNECT THE SOCKET TO THE SERVER                         */
    /*                                                                    */
    /* connect() does the TCP three-way handshake:                        */
    /*   Your PC → SYN → Server                                          */
    /*   Your PC ← SYN-ACK ← Server                                     */
    /*   Your PC → ACK → Server                                          */
    /* After this, you have a live TCP connection.                        */
    /* ================================================================ */
    printf("[INIT] Connecting to %s:%d...\n", SERVER_HOST, SERVER_PORT);
    if (connect(sock, (struct sockaddr*)&sin, sizeof(sin)) != 0) {
        fprintf(stderr, "[INIT] ERROR: connect() failed: %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return 1;
    }
    printf("[INIT] TCP connection established.\n");

    /* ================================================================ */
    /* PHASE 5: INITIALISE LIBSSH2                                        */
    /*                                                                    */
    /* libssh2_init() must be called once before anything else in        */
    /* libssh2. It initialises the underlying crypto library (OpenSSL    */
    /* or similar). Argument 0 = use default flags.                      */
    /* ================================================================ */
    rc = libssh2_init(0);
    if (rc != 0) {
        fprintf(stderr, "[SSH] ERROR: libssh2_init() failed: %d\n", rc);
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    /* ================================================================ */
    /* PHASE 6: CREATE SSH SESSION                                        */
    /*                                                                    */
    /* A session is the main SSH object. All SSH operations go through   */
    /* it. It manages the encryption keys, the SSH protocol state, etc.  */
    /* ================================================================ */
    printf("[SSH] Creating SSH session...\n");
    session = libssh2_session_init();
    if (!session) {
        fprintf(stderr, "[SSH] ERROR: libssh2_session_init() failed.\n");
        closesocket(sock);
        libssh2_exit();
        WSACleanup();
        return 1;
    }

    /*
     * Set a timeout for SSH operations.
     * If the server doesn't respond within 15 seconds, operations will
     * fail instead of waiting forever. Good practice to prevent hangs.
     * Value is in milliseconds: 15000 ms = 15 seconds.
     */
    libssh2_session_set_timeout(session, 15000);

    /* ================================================================ */
    /* PHASE 7: SSH HANDSHAKE                                             */
    /*                                                                    */
    /* This is where the SSH protocol magic happens:                      */
    /*   - Client and server agree on which crypto algorithms to use      */
    /*   - They exchange keys and establish an encrypted channel          */
    /*   - Server sends its host key (which we skip verifying here)       */
    /* After this, all data is encrypted.                                 */
    /* ================================================================ */
    printf("[SSH] Performing SSH handshake...\n");
    rc = libssh2_session_handshake(session, sock);
    if (rc) {
        fprintf(stderr, "[SSH] ERROR: SSH handshake failed: %d\n", rc);
        goto cleanup;
    }
    printf("[SSH] Handshake successful.\n");

    /* ================================================================ */
    /* HOST KEY CHECK (skipped for testing)                               */
    /*                                                                    */
    /* In production you should verify the server's host key to prevent  */
    /* "man-in-the-middle" attacks (someone pretending to be your server).*/
    /* Here we just retrieve it and print a warning that we're skipping  */
    /* the check. DO NOT skip this in a real application.                 */
    /* ================================================================ */
    {
        size_t hostkey_len;
        int    hostkey_type;
        const char *hostkey = libssh2_session_hostkey(session, &hostkey_len, &hostkey_type);
        if (hostkey) {
            printf("[SSH] WARNING: Skipping host key verification (testing only!).\n");
        }
    }

    /* ================================================================ */
    /* PHASE 8: AUTHENTICATE WITH USERNAME + PASSWORD                     */
    /*                                                                    */
    /* This proves to the server that you are who you say you are.        */
    /* The password is sent encrypted (because the SSH channel is        */
    /* already established and encrypted from the handshake above).       */
    /* libssh2 also supports key-based auth (more secure than passwords). */
    /* ================================================================ */
    printf("[SSH] Authenticating as user: %s...\n", SSH_USER);
    rc = libssh2_userauth_password(session, SSH_USER, SSH_PASS);
    if (rc) {
        fprintf(stderr, "[SSH] ERROR: Authentication failed. Check username/password.\n");
        goto cleanup;
    }
    printf("[SSH] Authentication successful.\n");

    /* ================================================================ */
    /* PHASE 9: START THE SFTP SUBSYSTEM                                  */
    /*                                                                    */
    /* SSH supports multiple "channels" running over the same connection. */
    /* SFTP is a sub-protocol that runs inside SSH.                       */
    /* libssh2_sftp_init() asks the server to start the SFTP service and  */
    /* returns an SFTP session object we use for all file operations.     */
    /* ================================================================ */
    printf("[SFTP] Initialising SFTP subsystem...\n");
    sftp = libssh2_sftp_init(session);
    if (!sftp) {
        fprintf(stderr, "[SFTP] ERROR: Could not initialise SFTP.\n");
        goto cleanup;
    }
    printf("[SFTP] SFTP ready.\n");

    /* ================================================================ */
    /* PHASE 10: PERFORM GET (DOWNLOAD)                                   */
    /* ================================================================ */
    rc = sftp_get(sftp, REMOTE_GET_PATH, LOCAL_SAVE_PATH);
    if (rc == 0) {
        printf("[GET] SUCCESS\n");
    } else {
        printf("[GET] FAILED\n");
    }

    /* ================================================================ */
    /* PHASE 11: PERFORM PUT (UPLOAD)                                     */
    /* ================================================================ */
    rc = sftp_put(sftp, LOCAL_PUT_PATH, REMOTE_SAVE_PATH);
    if (rc == 0) {
        printf("[PUT] SUCCESS\n");
    } else {
        printf("[PUT] FAILED\n");
    }

    /* ================================================================ */
    /* CLEANUP                                                            */
    /*                                                                    */
    /* Always release resources in reverse order of acquisition.         */
    /* This is like closing tabs and apps before shutting down your PC.  */
    /*                                                                    */
    /* "goto cleanup" is used above when errors occur — it jumps here    */
    /* to ensure we never skip the cleanup, avoiding resource leaks.     */
    /* ================================================================ */
cleanup:
    printf("\n[CLEANUP] Releasing resources...\n");

    if (sftp) {
        libssh2_sftp_shutdown(sftp);   /* Close the SFTP subsystem              */
        printf("[CLEANUP] SFTP shut down.\n");
    }

    if (session) {
        /* Gracefully tell the server we're disconnecting */
        libssh2_session_disconnect(session, "Normal Shutdown, Thank You!");
        libssh2_session_free(session); /* Free the session memory               */
        printf("[CLEANUP] SSH session freed.\n");
    }

    if (sock != INVALID_SOCKET) {
        closesocket(sock);             /* Close the TCP connection               */
        printf("[CLEANUP] Socket closed.\n");
    }

    libssh2_exit();   /* Shut down the libssh2 library globally                 */
    WSACleanup();     /* Shut down WinSock                                      */

    printf("[CLEANUP] Done. Exiting.\n");
    return 0;
}
