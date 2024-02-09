/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hait-hsa <hait-hsa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 01:27:52 by hait-hsa          #+#    #+#             */
/*   Updated: 2024/02/09 08:11:38 by hait-hsa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <unistd.h>
#include <fstream>
#include <sstream>

int clientSocket;
int sockFD;
char buffer[1024];

void handelSignal(int signum) {
    // Properly handle signal, close sockets, and exit
    close(clientSocket);
    close(sockFD);
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

    int kq;
    int eventNumb;
    struct kevent event;
    struct kevent events[64];
    sockaddr_in socketAddress;
    std::string sockPort = trim(serverData[0].server[0].second[0], "\"");
    int sockFD = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    socketAddress.sin_family = AF_INET;
    socketAddress.sin_addr.s_addr = INADDR_ANY;
    socketAddress.sin_port = htons(std::atoi(sockPort.c_str()));
    if (bind(sockFD, reinterpret_cast<struct sockaddr *>(&socketAddress), sizeof(socketAddress)) == FAILED) {
        std::cout << "failed to bind server socket" << std::endl;
        close(sockFD);
        exit(10);
    }
    if (listen(sockFD, 10) == FAILED) {
        std::cout << "failed to make the socket at the listen mode!" << std::endl;
        close(sockFD);
        exit(11);
    }
    if ((kq = kqueue()) == FAILED) {
        std::cout << "failed to create a kernel event queue" << std::endl;
        close(sockFD);
        close(kq);
        exit(14);
    }
    EV_SET(&event, sockFD,EVFILT_READ, EV_ADD , 0, 0, nullptr);
    if (kevent(kq, &event, 1, nullptr, 0, nullptr)) {
        std::cout << "failed to associat event with kernel event queue!" << std::endl;
        close(sockFD);
        close(kq);
        exit(18);
    }
    std::cout << "server now is listening in port " << sockPort << std::endl;
    signal(SIGINT, handelSignal);
    while (true) {
        eventNumb =  kevent(kq, nullptr, 0, events, 64, nullptr);
        for (int i = ZERO; i < eventNumb; i++) {
            if (events[i].ident == (uintptr_t)sockFD) {
                if ((clientSocket = accept(sockFD, nullptr, nullptr)) == FAILED) {
                    std::cout << "error: connection failed" << std::endl;
                    continue;
                } else
                    std::cout << "connection has been done successfully" << std::endl;
                recv(clientSocket, buffer, sizeof(buffer), 0);
                handleHttpRequest(clientSocket, buffer);
            }
        }
        close(clientSocket);
    }
    close(sockFD);
}
    
