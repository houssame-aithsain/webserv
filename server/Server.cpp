/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hait-hsa <hait-hsa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 01:27:52 by hait-hsa          #+#    #+#             */
/*   Updated: 2024/02/06 22:55:59 by hait-hsa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <unistd.h>
#include <fstream>
#include <sstream>

int clientSocket;
int socketFd;
char buffer[1024];

void handelSignal(int signum) {
    // Properly handle signal, close sockets, and exit
    close(clientSocket);
    close(socketFd);
    std::cout << "Sockets have been closed!" << std::endl;
    exit(signum);
}

std::string trim(const std::string& str, const std::string& charsToTrim) {
    // Trim function to remove leading and trailing whitespaces
    size_t start = str.find_first_not_of(charsToTrim);
    size_t end = str.find_last_not_of(charsToTrim);

    if (start == std::string::npos || end == std::string::npos)
        return std::string();
    return str.substr(start, end - start + 1);
}

void Server::handleHttpRequest(int clientSocket, char* httpRequest) {
    if (strncmp(httpRequest, "GET ", 4) == 0) {
        std::string requestedResource = "index.html";

        FILE* fileStream = fopen(requestedResource.c_str(), "r");
        if (!fileStream) {
            // File not found, respond with a 404 error
            std::string response = "HTTP/1.1 404 Not Found\r\n\r\n";
            send(clientSocket, response.c_str(), response.length(), 0);
        } else {
            // Read the file content
            fseek(fileStream, 0, SEEK_END);
            long fileSize = ftell(fileStream);
            fseek(fileStream, 0, SEEK_SET);

            // Allocate a buffer to hold the file content
            char* fileContent = new char[fileSize + 1];

            // Read the file content into the buffer
            fread(fileContent, 1, fileSize, fileStream);
            fclose(fileStream);

            // Null-terminate the buffer
            fileContent[fileSize] = '\0';

            // Build the HTTP response
            std::string httpResponse = "HTTP/1.1 200 OK\r\n";
            httpResponse += "Content-Type: text/html\r\n";
            httpResponse += "Content-Length: " + std::to_string(fileSize) + "\r\n";
            httpResponse += "\r\n" + std::string(fileContent);

            // Send the HTTP response
            send(clientSocket, httpResponse.c_str(), httpResponse.length(), 0);

            // Clean up
            delete[] fileContent;
        }
    }
}

void Server::initializeSocket(std::vector<server_data> serverData) {
    int byteSize;
    int kq = kqueue();
    struct kevent event;
    std::string portToConvert = trim(serverData[ZERO].server[ZERO].second[ZERO], "\"");
    sockaddr_in serverAddress;

    if (kq == -1) {
        perror("kqueue error!");
        close(socketFd);
        exit(ZERO);
    }
    socketFd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFd < ZERO) {
        std::cerr << SOCKETFAILED << std::endl;
        exit(ONE);
    }
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(std::atoi(portToConvert.c_str()));
    serverAddress.sin_addr.s_addr = INADDR_ANY; // i will be back
    // Bind the socket
    if (bind(socketFd, reinterpret_cast<struct sockaddr*>(&serverAddress), sizeof(serverAddress)) == -1) {
        std::cerr << "Error binding socket" << std::endl;
        close(socketFd); // Close the socket on error
        return;
    }

    // Listen for incoming connections
    if (listen(socketFd, 10) == -1) {
        std::cerr << "Error listening on socket" << std::endl;
        close(socketFd); // Close the socket on error
        return;
    }
    EV_SET(&event, socketFd, EVFILT_READ, EV_ADD, ZERO, ZERO, nullptr);
    if (kevent(kq, &event, 1, nullptr, 0, nullptr) == -1) {
        perror("Error registering server socket with kqueue");
        close(kq);
        close(socketFd);
        exit(ZERO);
    }
    std::cout << "Server is listening on port " << std::atoi(portToConvert.c_str()) << std::endl;

    while (true) {
        signal(SIGINT, handelSignal);
        struct kevent events[64];
        int numEvents = kevent(kq, nullptr, 0, events, 64, nullptr);
        if (numEvents == -1) {
            perror("Error in kevent");
            break;
        }
        // Accept a connection
        for (int i = ZERO; i < numEvents; ++i) {
            if (events[i].ident == (uintptr_t)socketFd) {
                clientSocket = accept(socketFd, nullptr, nullptr);
                if (clientSocket == -1) {
                    std::cerr << "Error accepting connection" << std::endl;
                    continue; // Continue to the next iteration of the loop on error
                } else
                    std::cout << "Connection has been done successfully!" << std::endl;
                listen(clientSocket, 10);
                if ((byteSize = recv(clientSocket, buffer, sizeof(buffer), ZERO)) < ZERO)
                    std::cerr << "An error has been detected while receiving data!" << std::endl;
                buffer[byteSize] = ZERO;
                std::cout << buffer << std::endl;
                handleHttpRequest(clientSocket, buffer);
            }
        }
        // Close the client socket when done
        close(clientSocket);
    }
    // Close the server socket (this part won't be reached in this example)
    close(socketFd);
}
