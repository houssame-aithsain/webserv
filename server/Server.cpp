/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hait-hsa <hait-hsa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 01:27:52 by hait-hsa          #+#    #+#             */
/*   Updated: 2024/02/16 23:18:13 by hait-hsa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <unistd.h>
#include <fstream>
#include <sstream>
#include <fcntl.h>
#include <map>

const char* greenColor = "\033[32m";
const char* redColor = "\033[31m";
const char* yellowColor = "\033[33m";
const char* resetColor = "\033[0m";

int serverSocketFd;
std::map<int, Client> clientsSocket;
// atributes

void handelSignal(int signum) {
    for (size_t i = ZERO; i < clientsSocket.size(); i++)
        close(clientsSocket[i].getFD());
    close(serverSocketFd);
    std::cout << "Sockets have been closed!" << std::endl;
    exit(signum);
}

void Server::sockCreat(void) {serverSocketFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);}

void Server::socketError( void ) {

    int reuse = ONE;
    if (setsockopt(serverSocketFd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
        throw "setsockopt(SO_REUSEADDR) failed";
    }
}

void Server::nonBlockingMode( void ) {

    if (fcntl(serverSocketFd, F_SETFL, fcntl(serverSocketFd, F_GETFL, 0) | O_NONBLOCK) == FAILED) {
        throw "calling fcntl";
    }
}

void Server::sockBind( void ) {

    if (bind(serverSocketFd, reinterpret_cast<struct sockaddr *>(&socketAddress), sizeof(socketAddress)) == FAILED) {
        close(serverSocketFd);
        throw "failed to bind server socket";
    }
}

void Server::sockListen( void ) {

    if (listen(serverSocketFd, 10) == FAILED) {
        close(serverSocketFd);
        throw "failed to make the socket at the listen mode!";
    }
}

std::vector<pollfd> Server::getAllClientsFd(void) {

    std::vector<pollfd> totalClientsPollFd;

    for (std::map<int, Client>::iterator it = clientsSocket.begin(); it != clientsSocket.end(); it++)
        totalClientsPollFd.push_back(it->second.getPollFd());
    return totalClientsPollFd;
}

std::vector<Client> Server::creatClientOBJ(void) {
    
    std::vector<Client> clientsOBJ;
    for (std::vector<pollfd>::iterator it = tmpEvents.begin(); it != tmpEvents.end(); it++)
        clientsOBJ.push_back(Client(*it));
    return clientsOBJ;
}
// end atributes

std::string trim(const std::string& str, const std::string& charsToTrim) {
    // Trim function to remove leading and trailing whitespaces
    size_t start = str.find_first_not_of(charsToTrim);
    size_t end = str.find_last_not_of(charsToTrim);

    if (start == std::string::npos || end == std::string::npos)
        return std::string();
    return str.substr(start, end - start + 1);
}

void Server::handleHttpRequest(int clientSocket) {
    // std::cout << greenColor << buffer << resetColor << std::endl;
    std::cout << "######################################################\n";

    if (request_data["Method"] == "GET") {
        std::string requestedResource = request_data["Asset"];
        if (requestedResource == "/")
            requestedResource = "/index.html";

        std::ifstream file("." + requestedResource, std::ios::binary);
        if (!file.is_open()) {
            std::string response = "HTTP/1.1 404 Not Found\r\n\r\n";
            std::cout << "NOT FOUND!!!!" << std::endl;
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

            ssize_t bytesSent = send(clientSocket, httpResponse.c_str() + totalBytesSent, remainingBytes, 0);
            // while (totalBytesSent < httpResponse.length()) {
            //     ssize_t bytesSent = send(clientSocket, httpResponse.c_str() + totalBytesSent, remainingBytes, 0);
                if (bytesSent == FAILED) {
                    if (errno == EAGAIN || errno == EWOULDBLOCK) {
                        // Output buffer is full, continue with other tasks
                        // std::cout << "Output buffer is full. Continue with other tasks." << std::endl;
                        // You may want to add a delay or use a different strategy here
                        // continue;
                    } else {
                        perror("send");
                        // Handle other error conditions
                        // Close or handle the socket accordingly
                        // break;
                    }
                }
            //     totalBytesSent += static_cast<size_t>(bytesSent);
            //     remainingBytes -= static_cast<size_t>(bytesSent);
            // }
        }
    }
    std::cout << "######################################################\n\n";
    close(clientSocket);
}

bool Server::acceptNewConnection( void ) {

    int clientSocket;
    pollfd newClient;
    int addrlen = sizeof(socketAddress);
    
    if ((clientSocket = accept(serverSocketFd, (struct sockaddr *)(&socketAddress), (socklen_t *)&addrlen)) == FAILED) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            return false;
        }
        perror("accept()");
    } else {
        int status = fcntl(clientSocket, F_SETFL, fcntl(clientSocket, F_GETFL, 0) | O_NONBLOCK);
        if (status == FAILED) {
            perror("calling fcntl");
        }
        std::cout << "connection has been done successfully" << std::endl;
        INIT_EVENT(newClient, clientSocket);

        clientsSocket.insert(std::make_pair(newClient.fd, Client(newClient)));
    }
    return true;
}

int Server::receiveData(int clientSocket) {

    char chunk[CHUNK_SIZE];

    bzero(chunk, sizeof(chunk));
    int bytesRead = recv(clientSocket, chunk, sizeof(chunk) - ONE, ZERO);
    if (bytesRead == FAILED) {
        if (errno == EAGAIN || errno == EWOULDBLOCK)
            return bytesRead;
        else {
            perror("recv");
            return FAILED;
        }
    }
    std::cout << greenColor << bytesRead << resetColor << std::endl;
    chunk[bytesRead] = ZERO;
    clientsSocket[clientSocket].appendStr(chunk);
    // std::map<int, std::string>::iterator it = clientsBuffer.find(clientSocket);
    // if (it != clientsBuffer.end())
    //     clientsBuffer[clientSocket].append(chunk);
    // else
    //     clientsBuffer.insert(std::make_pair(clientSocket, chunk));
    // exit(0);
    return bytesRead;
}

bool Server::readFromClientSocketFd(int index) {

    int requestByteSize;

    if (tmpEvents[index].revents & POLLIN) {
        requestByteSize = receiveData(tmpEvents[index].fd);
        if (requestByteSize == FAILED) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                return false;
            }
            perror("recv");
        } else if (requestByteSize == 0) {
            close(tmpEvents[index].fd);
            clientsSocket[tmpEvents[index].fd].clearBuffer();
        }
    }
    return true;
}

void Server::initializeSocket(std::vector<server_data> serverData) {

    int sockPort = std::atoi(trim(serverData[ZERO].server[ZERO].second[ZERO], "\"").c_str());
    
    this->sockCreat();
    INIT_SOCKET(socketAddress, sockPort);
    this->socketError();
    this->nonBlockingMode();
    this->sockBind();
    this->sockListen();
    std::cout << "server now is listening on port " << sockPort << std::endl;
    signal(SIGINT, handelSignal);
    signal(SIGPIPE, SIG_IGN);
    INIT_EVENT(events, serverSocketFd);
    clientsSocket.insert(std::make_pair(serverSocketFd, events));
    while (true) {
    tmpEvents = this->getAllClientsFd();
    poll(tmpEvents.data(), tmpEvents.size(), FAILED);
    // clientsSocket = tmpEvents;
    for (size_t i = ZERO; i < tmpEvents.size(); i++) {
        if (tmpEvents[i].fd == serverSocketFd && !this->acceptNewConnection())
                continue;
        else {
            if (!this->readFromClientSocketFd(i))
                continue;
            // std::map<int, std::string>::iterator it = clientsBuffer.find(tmpEvents[i].fd);
            if ((tmpEvents[i].revents & POLLOUT)) {
                ft_parse_request(clientsSocket[tmpEvents[i].fd].getBuffer());
                size_t requestLenght = clientsSocket[tmpEvents[i].fd].getBuffer().find("\r\n\r\n");
                std::map<std::string, std::string>::iterator contentLength = request_data.find("Content-Length");
                if (requestLenght != std::string::npos && contentLength == request_data.end()) {
                    std::cout << redColor << "GET" << resetColor << std::endl;
                    std::cout << redColor << tmpEvents[i].fd << resetColor << std::endl;
                    handleHttpRequest(tmpEvents[i].fd);
                    clientsSocket[tmpEvents[i].fd].clearBuffer();
                }
                if (contentLength != request_data.end() && (int)atoi(contentLength->second.c_str()) > ZERO) {
                    std::cout << greenColor << "POST" << resetColor << std::endl;
                    if ((int)atoi(contentLength->second.c_str()) <= (int)clientsSocket[tmpEvents[i].fd].getBuffer().size()) {
                        std::cout << redColor << clientsSocket[tmpEvents[i].fd].getBuffer() << resetColor << std::endl;
                        handleHttpRequest(tmpEvents[i].fd);
                        clientsSocket[tmpEvents[i].fd].clearBuffer();
                    }
                request_data.clear();
                }
            }
        }
    }
}
    close(serverSocketFd);
}
