#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

const int SERVER_PORT = 1336;

int main(int argc, char **argv) {
    // Create a socket
    while (true) {
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        perror("Error creating socket");
        return 1;
    }

    const char* SERVER_IP = argv[1];
    // Set up server address
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &(serverAddress.sin_addr));

    // Connect to the server
    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        perror("Error connecting to server");
        close(clientSocket);
        return 1;
    }

    // Send text to the server
   std::string message;
        getline(std::cin, message);
        ssize_t sentBytes = send(clientSocket, message.c_str(), message.length(), 0);
        if (sentBytes == -1)
            perror("Error sending data to server");
        send(clientSocket, message.c_str(), 0, 0);
        message.clear();
        close(clientSocket);
    }

    // Close the socket

    return 0;
}
