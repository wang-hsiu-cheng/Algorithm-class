#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// Utility function to flush input buffer
void clearInputBuffer() {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

int main() {
    int clientSocket;
    struct sockaddr_in serverAddr;
    char userChoice[2];
    char buffer[100];
    socklen_t addrLen;

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(1234);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    while (true) {
        clientSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (clientSocket < 0) {
            std::cerr << "Failed to create socket.\n";
            return 1;
        }

        if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
            std::cerr << "Connection to server failed.\n";
            return 1;
        }

        // Initial welcome/message from server
        read(clientSocket, buffer, sizeof(buffer));
        std::cout << buffer;

        std::cin >> userChoice;

        if (userChoice[0] == '1' && userChoice[1] == '\0') {
            write(clientSocket, "DNS", strlen("DNS") + 1);

            read(clientSocket, buffer, sizeof(buffer));
            std::cout << buffer;

            std::cin >> buffer;
            write(clientSocket, buffer, sizeof(buffer));

            read(clientSocket, buffer, sizeof(buffer));
            std::cout << "Domain IP address: " << buffer << std::endl;

        } else if (userChoice[0] == '2' && userChoice[1] == '\0') {
            write(clientSocket, "QUERY", strlen("QUERY") + 1);

            read(clientSocket, buffer, sizeof(buffer));
            std::cout << buffer;

            std::cin >> buffer;
            write(clientSocket, buffer, sizeof(buffer));

            read(clientSocket, buffer, sizeof(buffer));
            std::cout << "Email of the student ID: " << buffer << std::endl;

        } else if (userChoice[0] == '3' && userChoice[1] == '\0') {
            write(clientSocket, "QUIT", strlen("QUIT") + 1);
            read(clientSocket, buffer, sizeof(buffer));
            std::cout << buffer << std::endl;
            close(clientSocket);
            break;

        } else {
            write(clientSocket, "client error", strlen("client error") + 1);
            read(clientSocket, buffer, sizeof(buffer));
            std::cout << buffer << std::endl;
            clearInputBuffer();
        }

        std::cout << std::endl;
        close(clientSocket);
    }

    return 0;
}
