#include <iostream>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

int main() {
    struct sockaddr_in serverAddr, clientAddr;
    int serverSocket, clientSocket, bytesRead;
    socklen_t clientAddrLen;
    char recvBuffer[100], studentID[20], email[100];

    // Clear and initialize server address
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(1234);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    // Create server socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        perror("Socket creation failed");
        return 1;
    }

    // Bind socket to local address
    if (bind(serverSocket, (struct sockaddr*) &serverAddr, sizeof(serverAddr)) < 0) {
        perror("Bind failed");
        return 1;
    }

    // Listen with a backlog of 10 connections
    listen(serverSocket, 10);
    clientAddrLen = sizeof(clientAddr);

    std::cout << "Waiting for requirement...\n";

    while (true) {
        // Accept an incoming client connection
        clientSocket = accept(serverSocket, (struct sockaddr*) &clientAddr, &clientAddrLen);

        // Send requirement prompt to client
        const char* promptMsg = "What's your requirement? 1.DNS 2.QUERY 3.QUIT : ";
        write(clientSocket, promptMsg, strlen(promptMsg) + 1);

        // Read client input
        bytesRead = read(clientSocket, recvBuffer, sizeof(recvBuffer));
        std::cout << "Client's requirement: " << recvBuffer << std::endl;

        // Handle DNS lookup
        if (strcmp(recvBuffer, "DNS") == 0) {
            const char* askUrlMsg = "Input URL address: ";
            write(clientSocket, askUrlMsg, strlen(askUrlMsg) + 1);

            read(clientSocket, recvBuffer, sizeof(recvBuffer));
            struct hostent* host = gethostbyname(recvBuffer);
            if (host != nullptr) {
                strcpy(recvBuffer, inet_ntoa(*(struct in_addr*)host->h_addr_list[0]));
                write(clientSocket, recvBuffer, sizeof(recvBuffer));
            } else {
                write(clientSocket, "URL Not Found", strlen("URL Not Found") + 1);
            }
        }

        // Handle QUERY from file
        else if (strcmp(recvBuffer, "QUERY") == 0) {
            bool found = false;
            FILE* queryFile = fopen("query.txt", "r");

            const char* askIDMsg = "Input student ID: ";
            write(clientSocket, askIDMsg, strlen(askIDMsg) + 1);

            read(clientSocket, recvBuffer, sizeof(recvBuffer));
            while (fscanf(queryFile, "%s %s", studentID, email) != EOF) {
                if (strcmp(recvBuffer, studentID) == 0) {
                    write(clientSocket, email, sizeof(email));
                    found = true;
                    break;
                }
            }
            fclose(queryFile);

            if (!found) {
                write(clientSocket, "ID Not Found", strlen("ID Not Found") + 1);
            }
        }

        // Handle QUIT
        else if (strcmp(recvBuffer, "QUIT") == 0) {
            const char* goodbyeMsg = "...Disconnected";
            write(clientSocket, goodbyeMsg, strlen(goodbyeMsg) + 1);
            std::cout << "Waiting for requirement...\n";
        }

        // Handle unknown input
        else if (strcmp(recvBuffer, "client error") == 0) {
            write(clientSocket, "Invalid Command", strlen("Invalid Command") + 1);
        }

        // Close connection to client
        close(clientSocket);
    }

    return 0;
}
