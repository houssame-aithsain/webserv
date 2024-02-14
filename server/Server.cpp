/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hait-hsa <hait-hsa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 01:27:52 by hait-hsa          #+#    #+#             */
/*   Updated: 2024/02/14 09:54:24 by hait-hsa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <unistd.h>
#include <fstream>
#include <sstream>
#include <fcntl.h>
#include <sys/poll.h>

const char* greenColor = "\033[32m";
const char* redColor = "\033[31m";
const char* yellowColor = "\033[33m";
const char* resetColor = "\033[0m";

std::string buffer;
int serverSocketFd;
std::vector<pollfd> clientsSocket;

int receiveData(int clientSocket) {
    int dataSize = 0;
    char chunk[CHUNK_SIZE];
    
    buffer.clear();
    while (true) {
        memset(chunk, ZERO, sizeof(chunk));
        bzero(chunk, sizeof(chunk));
        int bytesRead = recv(clientSocket, chunk, sizeof(chunk), ZERO);
        if (bytesRead <= ZERO)
            break;
        chunk[bytesRead - 1] = ZERO;
        std::cout << yellowColor << chunk << resetColor << std::endl;
        buffer += chunk;
        dataSize += bytesRead;
    }
    std::cout << redColor << buffer << resetColor << std::endl;
    // exit(0);
    return (dataSize);
}

void handelSignal(int signum) {
    // Properly handle signal, close sockets, and exit
    for (size_t i = ZERO; i < clientsSocket.size(); i++)
        close(clientsSocket[i].fd);
    close(serverSocketFd);
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

// std::vector<std::pair<std::string, std::vector<std::string> > > request_data;

void Server::handleHttpRequest(int clientSocket, const char* httpRequest) {
    std::cout << greenColor << buffer << resetColor << std::endl;
    (void)httpRequest;
    std::cout << "######################################################\n";

    if (request_data["Method"] == "GET") {
        std::string requestedResource = request_data["Asset"];
        if (requestedResource == "/")
            requestedResource = "/index.html";

        std::ifstream file("." + requestedResource, std::ios::binary);
        if (!file.is_open()) {
            std::string response = "HTTP/1.1 404 Not Found\r\n\r\n";
            send(clientSocket, response.c_str(), response.length(), 0);
        } else {
            // Read the entire content of the file into a string        
            std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
            file.close();

            std::string content_type = "text/html";
            std::string ext = requestedResource.substr(requestedResource.find_last_of(".") + 1);
            if (ext == "css")
                content_type = "text/css";
            else if (ext == "png")
                content_type = "image/png";
            else if (ext == "jpg")
                content_type = "image/jpeg";

            // Build the HTTP response
            std::string httpResponse = "HTTP/1.1 200 OK\r\n";
            httpResponse += "Content-Type: " + content_type + "\r\n";
            httpResponse += "Content-Length: " + std::to_string(content.size()) + "\r\n";
            httpResponse += "\r\n" + content;

            // Send the HTTP response
            size_t totalBytesSent = 0;
            size_t remainingBytes = httpResponse.length();

            while (totalBytesSent < httpResponse.length()) {
                ssize_t bytesSent = send(clientSocket, httpResponse.c_str() + totalBytesSent, remainingBytes, 0);
                if (bytesSent == -1) {
                    if (errno == EAGAIN || errno == EWOULDBLOCK) {
                        // Output buffer is full, continue with other tasks
                        std::cout << "Output buffer is full. Continue with other tasks." << std::endl;
                        // You may want to add a delay or use a different strategy here
                        continue;
                    } else {
                        perror("send");
                        // Handle other error conditions
                        // Close or handle the socket accordingly
                        break;
                    }
                }
                totalBytesSent += static_cast<size_t>(bytesSent);
                remainingBytes -= static_cast<size_t>(bytesSent);
            }
        }
    }
    std::cout << "######################################################\n\n";
    close(clientSocket);
}

void Server::initializeSocket(std::vector<server_data> serverData) {

    int eventNumb;
    int clientSocket;
    int requestByteSize;
    sockaddr_in socketAddress;
    std::string sockPort = trim(serverData[ZERO].server[ZERO].second[ZERO], "\"");
    int serverSocketFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    socketAddress.sin_family = AF_INET;
    socketAddress.sin_addr.s_addr = INADDR_ANY;
    socketAddress.sin_port = htons(std::atoi(sockPort.c_str()));
    // need to be explained !
    int reuse = 1;
    if (setsockopt(serverSocketFd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
        perror("setsockopt(SO_REUSEADDR) failed");
        exit(EXIT_FAILURE);
    }
    // end explain
    int status = fcntl(serverSocketFd, F_SETFL, fcntl(serverSocketFd, F_GETFL, 0) | O_NONBLOCK);
    if (status == -1){
        perror("calling fcntl");
    }
    if (bind(serverSocketFd, reinterpret_cast<struct sockaddr *>(&socketAddress), sizeof(socketAddress)) == FAILED) {
        std::cout << "failed to bind server socket" << std::endl;
        close(serverSocketFd);
        exit(10);
    }
    // if ((kq = kqueue()) == FAILED) {
    //     std::cout << "failed to create a kernel event queue" << std::endl;
    //     close(serverSocketFd);
    //     close(kq);
    //     exit(14);
    // }
    // EV_SET(&event, serverSocketFd, EVFILT_READ, EV_ADD , ZERO, ZERO, nullptr);
    // if (kevent(kq, &event, ONE, nullptr, ZERO, nullptr)) {
    //     std::cout << "failed to associat event with kernel event queue!" << std::endl;
    //     close(serverSocketFd);
    //     close(kq);
    //     exit(18);
    // }
    if (listen(serverSocketFd, 10) == FAILED) {
        std::cout << "failed to make the socket at the listen mode!" << std::endl;
        close(serverSocketFd);
        exit(11);
    }
    std::cout << "server now is listening in port " << sockPort << std::endl;
    signal(SIGINT, handelSignal);
    pollfd server_fd;
    server_fd.fd = serverSocketFd;
    server_fd.events = POLLIN | POLL_OUT;
    server_fd.revents = 0;
    clientsSocket.push_back(server_fd);
    
    while (true) {
    // std::cout << "iter" << std::endl;

    // eventNumb = kevent(kq, nullptr, ZERO, Revent, 64, nullptr); // wait
    std::vector<pollfd> tmp = clientsSocket;
    eventNumb = poll(&tmp[0], tmp.size(), -1);
    // std::cout << "event number ===? " << eventNumb << std::endl;

    for (size_t i = ZERO; i < tmp.size(); i++) {
        if (tmp[i].fd == serverSocketFd) {
            int addrlen = sizeof(socketAddress);
            if ((clientSocket = accept(serverSocketFd, (struct sockaddr *)(&socketAddress), (socklen_t *)&addrlen)) == FAILED) {
                if (errno == EAGAIN || errno == EWOULDBLOCK) {
                    // std::cout << "NO DATA IS BEEN RECEIVED!" << std::endl;
                    continue;
                }
                perror("accept()");
            } else {
                std::cout << "connection has been done successfully" << std::endl;
                pollfd new_client;
                new_client.fd = clientSocket;
                new_client.events = POLLIN | POLLOUT;
                new_client.revents = 0;
                clientsSocket.push_back(new_client);
            }
        } else {
            if (tmp[i].revents & POLLIN) {
                requestByteSize = receiveData(tmp[i].fd);
                std::cout << "request size ==> " << requestByteSize << std::endl;
                if (requestByteSize == -1) {
                    if (errno == EAGAIN || errno == EWOULDBLOCK) {
                        // std::cout << "NO DATA IS BEEN RECEIVED!" << std::endl;
                        continue;
                    }
                    perror("recv");
                } else if (requestByteSize == 0) {
                    // Connection closed by the client
                    close(tmp[i].fd);
                    // std::cout << "CONNECTION CLOSED!" << std::endl;
                } else {
                    // Data received
                    buffer[requestByteSize] = '\0';
                    ft_parse_request(buffer.c_str());
                    handleHttpRequest(tmp[i].fd, buffer.c_str());
                }
            }
            // Handle POLLOUT for outgoing data if needed
            if (tmp[i].revents & POLLOUT) {
                // Handle outgoing data if necessary
            }
        }
    }
}
    close(serverSocketFd);
}
