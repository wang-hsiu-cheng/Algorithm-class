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

// function to clear the input buffer
void clear_input_buffer() {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}


int main() {
    struct sockaddr_in addr;
    int sock, s;
    char m[2];
    socklen_t addr_size;
    char b[100];

    addr.sin_family = PF_INET;
    addr.sin_port = htons(1234);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    while (1) {
        sock = socket(PF_INET, SOCK_STREAM, 0);
        connect(sock, (struct sockaddr*) &addr, sizeof(struct sockaddr_in));

        // Read the initial message from the server
        s = read(sock, b, 100);
        printf("%s", b);
		
        scanf("%s", m);

        if (m[0] == '1' && m[1]=='\0') {
            s = write(sock, "DNS", strlen("DNS") + 1);
            // Read the message asking for URL address
            s = read(sock, b, 100);
            printf("%s", b);
            scanf("%s", b);
            s = write(sock, b, sizeof(b) + 1);
            s = read(sock, b, 100);
            printf("Domain IP address: %s\n", b);
        } else if (m[0] == '2' && m[1]=='\0') {
            s = write(sock, "QUERY", strlen("QUERY") + 1);
            // Read the message asking for student ID
            s = read(sock, b, 100);
            printf("%s", b);
            scanf("%s", b);
            s = write(sock, b, sizeof(b) + 1);
            s = read(sock, b, 100);
            printf("Email of the student ID: %s\n", b);
        } else if (m[0] == '3' && m[1]=='\0') {
            s = write(sock, "QUIT", strlen("QUIT") + 1);
			s = read(sock, b, 100);
			printf("%s\n", b);
            break;
        } else {
			s = write(sock, "client error", strlen("client error") + 1);
			s = read(sock, b, 100);
            printf("%s\n", b);
			clear_input_buffer(); // clear input buffer for avoiding unexpected input to server
        }

        printf("\n");
        close(sock);
    }
    return 0;
}
