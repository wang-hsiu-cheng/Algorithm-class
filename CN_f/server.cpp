#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <netdb.h>
#include <cstdio>

int main() {
    struct sockaddr_in myaddr; // Struct to hold the server address
    struct sockaddr_in addr;   // Struct to hold the client address
    int sock, stream, port, s;
    socklen_t addr_size;
    char b[100], n[20], m[100];

    // Initialize the server address struct to zero
    memset(&myaddr, 0, sizeof(myaddr));
    myaddr.sin_family = PF_INET; // Set the address family to IPv4
    myaddr.sin_port = htons(1234); // Set the port number, converting to network byte order
    myaddr.sin_addr.s_addr = htonl(INADDR_ANY); // Bind to any available network interface

    // Create a socket
    sock = socket(PF_INET, SOCK_STREAM, 0);
    // Bind the socket to the address and port number
    bind(sock, (struct sockaddr*) &myaddr, sizeof(struct sockaddr_in));
    // Listen for incoming connections, with a backlog of 10
    listen(sock, 10);
    addr_size = sizeof(addr);

    printf("Waiting for requirement... \n");

    while (1) {
        // Accept an incoming connection
        stream = accept(sock, (struct sockaddr*) &addr, &addr_size);

        // Send initial message to the client
        s = write(stream, "What's your requirement? 1.DNS 2.QUERY 3.QUIT : ", strlen("What's your requirement? 1.DNS 2.QUERY 3.QUIT : ") + 1);

        // Read the client's requirement
        s = read(stream, b, 100);
        printf("Client's requirement: %s\n", b);

        // If the client chose "DNS"
        if (strcmp(b, "DNS") == 0) {
            s = write(stream, "Input URL address: ", strlen("Input URL address: ") + 1); // Prompt the client for URL
            s = read(stream, b, 100); // Read the URL from the client
            struct hostent *host = gethostbyname(b); // Perform a DNS lookup
            if (host != NULL) {
                strcpy(b, inet_ntoa(*(struct in_addr*)host->h_addr_list[0])); // Convert the address to a string
                s = write(stream, b, sizeof(b) + 1); // Send the IP address back to the client
            } else {
                s = write(stream, "No such DNS", strlen("No such DNS") + 1); // Send an error message if the DNS lookup fails
            }
        }
        // If the client chose "QUERY"
        else if (strcmp(b, "QUERY") == 0) {
            int flag = 0;
            FILE *fp = fopen("query.txt", "r"); // Open the query file
            s = write(stream, "Input student ID: ", strlen("Input student ID: ") + 1); // Prompt the client for student ID
            s = read(stream, b, 100); // Read the student ID from the client
            while (fscanf(fp, "%s %s", n, m) != EOF) { // Read each entry from the file
                if (strcmp(b, n) == 0) { // If the ID matches
                    flag = 1;
                    s = write(stream, m, sizeof(m) + 1); // Send the corresponding value to the client
                    break;
                }
            }
            if (flag == 0) {
                s = write(stream, "NO such student ID", strlen("NO such student ID") + 1); // Send an error message if the ID is not found
            }
            fclose(fp); // Close the file
        }
        // If the client chose "QUIT"
        else if (strcmp(b, "QUIT") == 0) {
			s = write(stream, "...Disconneted", strlen("...Disconneted") + 1);
            printf("Waiting for requirement... \n"); // Print a message indicating the server is ready for new connections
        }
		else if (strcmp(b, "client error") == 0) {
			s = write(stream, "Invalid Command", strlen("Invalid Command") + 1); // Send an error message if the input command is invalid
		}

        // Close the connection to the client
        close(stream);
    }

    return 0;
}
