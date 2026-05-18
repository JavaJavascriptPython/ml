/*
 * sftp_get_put.c
 *
 * This program demonstrates how to:
 *   1. DOWNLOAD (GET) a file from a remote SSH/SFTP server to your local machine
 *   2. UPLOAD   (PUT) a file from your local machine to a remote SSH/SFTP server
 *
 * Libraries used:
 *   - WinSock2  : Windows networking (TCP/IP sockets)
 *   - libssh2   : SSH protocol implementation (handles encryption, auth, SFTP)
 *
 * Build in Visual Studio 2010:
 *   Linker -> Input -> Additional Dependencies:
 *       libssh2.lib
 *       ws2_32.lib
 */

/* ------------------------------------------------------------------ */
/*  INCLUDES                                                            */
/* ------------------------------------------------------------------ */

#include <stdio.h>      /* printf, fprintf, fopen, fclose, fread, fwrite */
#include <string.h>     /* memset, memcpy, strlen                        */
#include <winsock2.h>   /* Windows socket API (SOCKET, connect, etc.)   */
#include <ws2tcpip.h>   /* getaddrinfo, addrinfo (DNS resolution)        */
#include "libssh2.h"    /* Core SSH session, handshake, auth             */
#include "libssh2_sftp.h" /* SFTP subsystem (open, read, write, close)  */

/*
 * Tell the linker to automatically include these Windows libraries.
 * This is a Visual Studio shortcut so you don't have to manually
 * add them in Project Properties every time.
 */
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "libssh2.lib")


/* ------------------------------------------------------------------ */
/*  SERVER / AUTH CONFIGURATION                                         */
/*  Change these values to match your SFTP server details.             */
/* ------------------------------------------------------------------ */

#define SERVER_HOST  "192.168.1.100"   /* IP address or hostname of the SFTP server    */
#define SERVER_PORT  22                /* SSH runs on port 22 by default                */
#define SSH_USER     "your_username"   /* Your username on the remote server            */
#define SSH_PASS     "your_password"   /* Your password on the remote server            */

/*
 * Paths for the GET (download) operation:
 *   REMOTE_GET_PATH  = full path of the file ON THE SERVER you want to download
 *   LOCAL_SAVE_PATH  = full path where the downloaded file will be saved LOCALLY
 */
#define REMOTE_GET_PATH  "/home/user/remote_file.txt"
#define LOCAL_SAVE_PATH  "C:\\downloaded_file.txt"

/*
 * Paths for the PUT (upload) operation:
 *   LOCAL_PUT_PATH   = full path of the file ON YOUR MACHINE you want to upload
 *   REMOTE_SAVE_PATH = full path where the uploaded file will be saved ON THE SERVER
 */
#define LOCAL_PUT_PATH   "C:\\local_file.txt"
#define REMOTE_SAVE_PATH "/home/user/uploaded_file.txt"

/*
 * Buffer size used when reading/writing files in chunks.
 * 4096 bytes (4 KB) is a common efficient chunk size.
 * We read this many bytes at a time instead of the whole file at once,
 * which is important when files are large (e.g., hundreds of MB).
 */
#define BUFFER_SIZE 4096


/* ------------------------------------------------------------------ */
/*  FUNCTION: sftp_get                                                  */
/*  PURPOSE : Download a file FROM the remote server TO local disk     */
/*                                                                      */
/*  Parameters:                                                         */
/*    sftp        - the SFTP session object (already connected)         */
/*    remote_path - path of the file on the server                      */
/*    local_path  - path where to save the file on your machine         */
/*                                                                      */
/*  Returns: 0 on success, -1 on failure                               */
/* ------------------------------------------------------------------ */
int sftp_get(LIBSSH2_SFTP *sftp, const char *remote_path, const char *local_path)
{
    LIBSSH2_SFTP_HANDLE *remote_handle = NULL; /* Handle to the remote file (like a FILE* but over SSH) */
    FILE *local_file = NULL;                   /* Handle to the local file we will write into           */
    char buffer[BUFFER_SIZE];                  /* Temporary buffer to hold data during transfer         */
    long bytes_read;                           /* How many bytes libssh2 read in one chunk              */
    long total_bytes = 0;                      /* Running total of bytes transferred (for progress msg) */

    printf("\n[GET] Starting download...\n");
    printf("[GET] Remote: %s\n", remote_path);
    printf("[GET] Local : %s\n", local_path);

    /* -------------------------------------------------------------- */
    /* STEP 1: Open the remote file for READING                        */
    /*                                                                  */
    /* libssh2_sftp_open() works like fopen() but over the network.   */
    /* LIBSSH2_FXF_READ = open in read-only mode                       */
    /* The last argument (0) = file permissions, not needed for reads  */
    /* -------------------------------------------------------------- */
    remote_handle = libssh2_sftp_open(sftp, remote_path, LIBSSH2_FXF_READ, 0);
    if (!remote_handle) {
        fprintf(stderr, "[GET] ERROR: Could not open remote file: %s\n", remote_path);
        fprintf(stderr, "[GET]        Check that the path exists and you have read permission.\n");
        return -1;
    }
    printf("[GET] Remote file opened successfully.\n");

    /* -------------------------------------------------------------- */
    /* STEP 2: Open (or create) a local file for WRITING              */
    /*                                                                  */
    /* fopen() is standard C to open a file.                           */
    /* "wb" = write in binary mode (important! "w" on Windows can      */
    /*         corrupt binary files by converting \n to \r\n)          */
    /* -------------------------------------------------------------- */
    local_file = fopen(local_path, "wb");
    if (!local_file) {
        fprintf(stderr, "[GET] ERROR: Could not create local file: %s\n", local_path);
        fprintf(stderr, "[GET]        Check that the path is valid and you have write permission.\n");
        libssh2_sftp_close(remote_handle);
        return -1;
    }
    printf("[GET] Local file created successfully.\n");

    /* -------------------------------------------------------------- */
    /* STEP 3: Read from remote, write to local — chunk by chunk       */
    /*                                                                  */
    /* We loop until there's nothing left to read (EOF).               */
    /* Each iteration reads up to BUFFER_SIZE bytes from the server    */
    /* and immediately writes them to the local file.                  */
    /* This is called "streaming" and works for files of any size.     */
    /* -------------------------------------------------------------- */
    printf("[GET] Transferring data...\n");
    while (1) {

        /*
         * Read up to BUFFER_SIZE bytes from the remote file.
         * Returns:
         *   > 0  = number of bytes actually read (may be less than BUFFER_SIZE)
         *   = 0  = end of file (nothing more to read)
         *   < 0  = error occurred
         */
        bytes_read = libssh2_sftp_read(remote_handle, buffer, sizeof(buffer));

        if (bytes_read < 0) {
            fprintf(stderr, "[GET] ERROR: Failed while reading remote file.\n");
            fclose(local_file);
            libssh2_sftp_close(remote_handle);
            return -1;
        }

        if (bytes_read == 0) {
            /* End of file reached — we're done reading */
            break;
        }

        /*
         * Write the bytes we just read into the local file.
         * fwrite(data, size_of_each_item, number_of_items, file)
         * We write 'bytes_read' items of 1 byte each.
         */
        if (fwrite(buffer, 1, bytes_read, local_file) != (size_t)bytes_read) {
            fprintf(stderr, "[GET] ERROR: Failed to write to local file (disk full?).\n");
            fclose(local_file);
            libssh2_sftp_close(remote_handle);
            return -1;
        }

        /* Keep a running count so we can show progress */
        total_bytes += bytes_read;
        printf("[GET] Transferred %ld bytes so far...\r", total_bytes); /* \r rewinds line */
        fflush(stdout); /* Force the output to show immediately (stdout is buffered by default) */
    }

    printf("\n[GET] Download complete! Total: %ld bytes\n", total_bytes);

    /* -------------------------------------------------------------- */
    /* STEP 4: Close both file handles                                  */
    /* Always close files when done — like hanging up after a call.    */
    /* -------------------------------------------------------------- */
    fclose(local_file);
    libssh2_sftp_close(remote_handle);

    printf("[GET] File saved to: %s\n", local_path);
    return 0; /* 0 = success */
}


/* ------------------------------------------------------------------ */
/*  FUNCTION: sftp_put                                                  */
/*  PURPOSE : Upload a file FROM local disk TO the remote server       */
/*                                                                      */
/*  Parameters:                                                         */
/*    sftp        - the SFTP session object (already connected)         */
/*    local_path  - path of the file on your local machine              */
/*    remote_path - path where the file should be saved on the server   */
/*                                                                      */
/*  Returns: 0 on success, -1 on failure                               */
/* ------------------------------------------------------------------ */
int sftp_put(LIBSSH2_SFTP *sftp, const char *local_path, const char *remote_path)
{
    LIBSSH2_SFTP_HANDLE *remote_handle = NULL; /* Handle to the remote file we will write into */
    FILE *local_file = NULL;                   /* Handle to the local file we will read from   */
    char buffer[BUFFER_SIZE];                  /* Temporary buffer to hold data during transfer */
    size_t bytes_read;                         /* How many bytes fread() read from local file   */
    long bytes_written;                        /* How many bytes libssh2 wrote to remote file   */
    long total_bytes = 0;                      /* Running total of bytes transferred            */

    /*
     * File permissions for the uploaded file on the server.
     * 0644 is a Unix/Linux permission value:
     *   Owner  -> read + write  (6)
     *   Group  -> read only     (4)
     *   Others -> read only     (4)
     * This is the standard permission for a regular file.
     * The "0" prefix means it's an octal (base-8) number.
     */
    long file_permissions = 0644;

    printf("\n[PUT] Starting upload...\n");
    printf("[PUT] Local : %s\n", local_path);
    printf("[PUT] Remote: %s\n", remote_path);

    /* -------------------------------------------------------------- */
    /* STEP 1: Open the local file for READING                         */
    /*                                                                  */
    /* "rb" = read in binary mode (same reason as above — avoid        */
    /*         Windows line-ending conversions corrupting binary files) */
    /* -------------------------------------------------------------- */
    local_file = fopen(local_path, "rb");
    if (!local_file) {
        fprintf(stderr, "[PUT] ERROR: Could not open local file: %s\n", local_path);
        fprintf(stderr, "[PUT]        Check that the file exists and you have read permission.\n");
        return -1;
    }
    printf("[PUT] Local file opened successfully.\n");

    /* -------------------------------------------------------------- */
    /* STEP 2: Open (or create) the remote file for WRITING            */
    /*                                                                  */
    /* Flags used (combined with | which is bitwise OR):               */
    /*   LIBSSH2_FXF_WRITE  = open for writing                         */
    /*   LIBSSH2_FXF_CREAT  = create the file if it doesn't exist      */
    /*   LIBSSH2_FXF_TRUNC  = if file exists, erase its content first  */
    /*                         (so we don't get leftover old data)      */
    /* -------------------------------------------------------------- */
    remote_handle = libssh2_sftp_open(
        sftp,
        remote_path,
        LIBSSH2_FXF_WRITE | LIBSSH2_FXF_CREAT | LIBSSH2_FXF_TRUNC,
        LIBSSH2_SFTP_S_IRUSR | LIBSSH2_SFTP_S_IWUSR |   /* owner: read + write */
        LIBSSH2_SFTP_S_IRGRP | LIBSSH2_SFTP_S_IROTH     /* group/others: read  */
    );
    if (!remote_handle) {
        fprintf(stderr, "[PUT] ERROR: Could not create remote file: %s\n", remote_path);
        fprintf(stderr, "[PUT]        Check that the directory exists and you have write permission.\n");
        fclose(local_file);
        return -1;
    }
    printf("[PUT] Remote file created successfully.\n");

    /* -------------------------------------------------------------- */
    /* STEP 3: Read from local, write to remote — chunk by chunk       */
    /*                                                                  */
    /* Same idea as GET but reversed direction:                         */
    /* local disk → buffer → SSH network → remote server disk          */
    /* -------------------------------------------------------------- */
    printf("[PUT] Transferring data...\n");
    while (1) {

        /*
         * Read up to BUFFER_SIZE bytes from the local file.
         * fread() returns the number of items successfully read.
         * Since each item is 1 byte, it returns the number of bytes read.
         * Returns 0 at end of file.
         */
        bytes_read = fread(buffer, 1, sizeof(buffer), local_file);

        if (bytes_read == 0) {
            /*
             * Check if we stopped because of an error or because of EOF.
             * feof() returns non-zero if we've reached end of file (normal).
             * ferror() returns non-zero if an actual read error occurred.
             */
            if (ferror(local_file)) {
                fprintf(stderr, "[PUT] ERROR: Failed while reading local file.\n");
                fclose(local_file);
                libssh2_sftp_close(remote_handle);
                return -1;
            }
            break; /* EOF — done reading */
        }

        /*
         * Write the bytes we just read to the remote file.
         * libssh2_sftp_write() sends data over the encrypted SSH connection.
         * Returns the number of bytes written, or < 0 on error.
         */
        bytes_written = libssh2_sftp_write(remote_handle, buffer, bytes_read);
        if (bytes_written < 0) {
            fprintf(stderr, "[PUT] ERROR: Failed while writing to remote file.\n");
            fclose(local_file);
            libssh2_sftp_close(remote_handle);
            return -1;
        }

        /*
         * Sanity check: did we write all the bytes we intended to?
         * A partial write can happen under some error conditions.
         */
        if ((size_t)bytes_written != bytes_read) {
            fprintf(stderr, "[PUT] ERROR: Partial write! Read %zu bytes but wrote only %ld bytes.\n",
                    bytes_read, bytes_written);
            fclose(local_file);
            libssh2_sftp_close(remote_handle);
            return -1;
        }

        total_bytes += bytes_written;
        printf("[PUT] Transferred %ld bytes so far...\r", total_bytes);
        fflush(stdout);
    }

    printf("\n[PUT] Upload complete! Total: %ld bytes\n", total_bytes);

    /* -------------------------------------------------------------- */
    /* STEP 4: Close both file handles                                  */
    /* -------------------------------------------------------------- */
    fclose(local_file);
    libssh2_sftp_close(remote_handle);

    printf("[PUT] File saved on server at: %s\n", remote_path);
    return 0;
}


/* ================================================================== */
/*  MAIN FUNCTION                                                       */
/*  This is where the program starts.                                   */
/*  It handles:                                                         */
/*    - Network setup (WinSock)                                         */
/*    - TCP socket connection to the SSH server                         */
/*    - SSH session handshake + authentication                          */
/*    - SFTP subsystem initialization                                   */
/*    - Calling sftp_get() and sftp_put()                               */
/*    - Cleanup of all resources                                        */
/* ================================================================== */
int main(void)
{
    /* ---- Variable declarations ---- */
    WSADATA wsaData;                      /* WinSock startup info struct            */
    SOCKET sock = INVALID_SOCKET;         /* The TCP socket (like a phone line)     */
    LIBSSH2_SESSION *session = NULL;      /* The SSH session (handles encryption)   */
    LIBSSH2_SFTP    *sftp    = NULL;      /* The SFTP subsystem (file transfers)    */
    struct addrinfo  hints, *res;         /* Used for DNS lookup (hostname → IP)    */
    struct sockaddr_in sin;               /* Stores the server's IP + port          */
    int rc;                               /* Return code — used to check for errors */

    printf("=== SFTP GET/PUT Demo ===\n\n");

    /* ================================================================ */
    /* PHASE 1: WINDOWS NETWORK INITIALIZATION                           */
    /*                                                                    */
    /* On Windows, you must call WSAStartup() before using ANY network   */
    /* functions. This initialises the Winsock DLL.                      */
    /* MAKEWORD(2,2) requests Winsock version 2.2.                       */
    /* ================================================================ */
    printf("[INIT] Starting WinSock...\n");
    rc = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (rc != 0) {
        fprintf(stderr, "[INIT] ERROR: WSAStartup failed with code %d\n", rc);
        return 1;
    }
    printf("[INIT] WinSock initialized.\n");

    /* ================================================================ */
    /* PHASE 2: CREATE A TCP SOCKET                                       */
    /*                                                                    */
    /* A socket is like a telephone receiver — it's the object you use   */
    /* to send and receive data over the network.                         */
    /* AF_INET     = use IPv4 addresses                                   */
    /* SOCK_STREAM = use TCP (reliable, ordered, connection-based)        */
    /* 0           = let the OS pick the right protocol for SOCK_STREAM  */
    /* ================================================================ */
    printf("[INIT] Creating socket...\n");
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        fprintf(stderr, "[INIT] ERROR: socket() failed: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }
    printf("[INIT] Socket created.\n");

    /* ================================================================ */
    /* PHASE 3: RESOLVE HOSTNAME TO IP ADDRESS                           */
    /*                                                                    */
    /* getaddrinfo() does a DNS lookup — it translates a hostname like   */
    /* "myserver.com" to an IP address like "192.168.1.100".             */
    /* For an IP address literal, it just validates and returns it.      */
    /* ========================================================