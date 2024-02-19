/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hait-hsa <hait-hsa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 01:27:52 by hait-hsa          #+#    #+#             */
/*   Updated: 2024/02/19 22:29:30 by hait-hsa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <sstream>
#include <map>

std::map<int, Client> clientSocket;

// atributes
std::string trim(const std::string& str, const std::string& charsToTrim) {
    size_t start = str.find_first_not_of(charsToTrim);
    size_t end = str.find_last_not_of(charsToTrim);

    if (start == std::string::npos || end == std::string::npos)
        return std::string();
    return str.substr(start, end - start + 1);
}

// void handelSignal(int signum) {
//     for (size_t i = ZERO; i < clientSocket.size(); i++)
//         close(clientSocket[i].getFD());
//     close(serverSocketFd);
//     std::cout << "Sockets have been closed!" << std::endl;
//     exit(signum);
// }

std::vector<pollfd> Server::getAllClientsFd(void) {

    std::vector<pollfd> totalClientsPollFd;

    for (std::map<int, Client>::iterator it = clientSocket.begin(); it != clientSocket.end(); it++)
        totalClientsPollFd.push_back(it->second.getPollFd());
    return totalClientsPollFd;
}

Server::Server( void ) {}

void Server::Sent(int cSock) {

    ssize_t bytesSent = send(cSock, clientSocket[cSock].getBytesToSend().c_str() + clientSocket[cSock].getBytesWrite(), clientSocket[cSock].getRemainingBytes(), 0);
    if (bytesSent == FAILED) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
        } else {
            perror("send");
            // exit(11);
        }
    }
    clientSocket[cSock].appendBytesWrite(bytesSent);
    clientSocket[cSock].unappendRemainingBytes(bytesSent);
    // if (clientSocket[cSock].getBytesToSend().length() == clientSocket[cSock].getBytesWrite())
    //     close(cSock);
    // clientSocket[cSock].resetBytesWrite();
    // clientSocket[cSock].resetRemainingBytes();
    std::cout << greenColor << "server is sending..." << resetColor << std::endl;
}

void Server::handleHttpRequest(int cSock) {
    // std::cout << greenColor << buffer << resetColor << std::endl;
    // std::cout << "######################################################\n";

    if (request_data["Method"] == "GET") {
        std::string requestedResource = request_data["Asset"];
        if (requestedResource == "/")
            requestedResource = "/index.html";

        std::ifstream file("." + requestedResource, std::ios::binary);
        if (!file.is_open()) {
            std::string response = "HTTP/1.1 404 Not Found\r\n\r\n";
            std::cout << "NOT FOUND!!!!" << std::endl;
            send(cSock, response.c_str(), response.length(), 0);
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
            // if ((clientSocket[cSock].getBytesWrite() == clientSocket[cSock].getBytesToSend().length() || clientSocket[cSock].getBytesToSend().empty())) {
            // std::cout << greenColor << "request sent buffer now is fulling..." << resetColor << std::endl;
            clientSocket[cSock].resetBytesWrite();
            clientSocket[cSock].resetRemainingBytes();
            clientSocket[cSock].clearBytesToSend();
            clientSocket[cSock].setBytesToSend(httpResponse);
            clientSocket[cSock].appendRemainingBytes(httpResponse.length());
            // }
            // Send the HTTP response
            // size_t totalBytesSent = 0;
            // size_t remainingBytes = httpResponse.length();
            
            //     totalBytesSent += static_cast<size_t>(bytesSent);
            //     remainingBytes -= static_cast<size_t>(bytesSent);
            // }
        }
    }
    // std::cout << "######################################################\n\n";
}

bool Server::acceptNewConnection( ServerSocket server ) {

    int cSock;
    pollfd newClient;
    int addrlen = sizeof(server.getSocketAddress());
    
    if ((cSock = accept(server.getServerSocketFd(), (struct sockaddr *)(&server.getSocketAddress()), (socklen_t *)&addrlen)) == FAILED) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            return false;
        }
        perror("accept()");
    } else {
        int status = fcntl(cSock, F_SETFL, fcntl(cSock, F_GETFL, 0) | O_NONBLOCK);
        if (status == FAILED) {
            perror("calling fcntl");
        }
        std::cout << "connection has been done successfully" << std::endl;
        INIT_EVENT(newClient, cSock);
        clientSocket.insert(std::make_pair(newClient.fd, Client(newClient)));
    }
    return true;
}

int Server::receiveData(int cSock) {

    char chunk[CHUNK_SIZE];

    bzero(chunk, sizeof(chunk));
    // std::cout << redColor << cSock << resetColor << std::endl;
    int bytesRead = recv(cSock, chunk, sizeof(chunk) - ONE, ZERO);
    if (bytesRead == FAILED) {
        if (errno == EAGAIN || errno == EWOULDBLOCK)
            return bytesRead;
        else {
            perror("recv");
            return FAILED;
        }
    }
    chunk[bytesRead] = ZERO;
    clientSocket[cSock].appendStr(chunk);
    return bytesRead;
}

bool Server::readFromClientSocketFd(int index) {

    int requestByteSize;

    requestByteSize = receiveData(tmpEvents[index].fd);
    if (requestByteSize == FAILED) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            return false;
        }
        perror("recv");
    } else if (requestByteSize == 0) {
        close(tmpEvents[index].fd);
        clientSocket[tmpEvents[index].fd].clearBuffer();
        clientSocket[tmpEvents[index].fd].resetBytesRead();
    } else {
        clientSocket[tmpEvents[index].fd].appendBytesRead(requestByteSize);
    }
    tmpEvents[index].revents = 0;
    std::cout << redColor << "client is reading...." << resetColor << std::endl;
    return true;
}

void Server::GET( int index ) {

    std::cout << redColor << "GET" << resetColor << std::endl;
    // std::cout << redColor << tmpEvents[index].fd << resetColor << std::endl;
    handleHttpRequest(tmpEvents[index].fd);
    clientSocket[tmpEvents[index].fd].clearBuffer();
}

void Server::POST(int index, std::map<std::string, std::string>::iterator contentLength) {

    std::cout << greenColor << "POST" << resetColor << std::endl;
    if ((int)atoi(contentLength->second.c_str()) <= clientSocket[tmpEvents[index].fd].getBytesRead()) {
        std::cout << greenColor << clientSocket[tmpEvents[index].fd].getBuffer() << resetColor << std::endl;
        // handleHttpRequest(tmpEvents[index].fd);
        clientSocket[tmpEvents[index].fd].clearBuffer();
        close(tmpEvents[index].fd);
    }
}

void Server::initializeSocket(std::vector<server_data> serverData) {
    
    for (std::vector<server_data>::iterator it = serverData.begin(); it != serverData.end(); it++) {
        serverPort.push_back(std::atoi(trim(it->server[ZERO].second[ZERO], "\"").c_str()));
    }
    for (std::vector<int>::iterator it = serverPort.begin(); it != serverPort.end(); it++) {
        virtualServer.push_back(ServerSocket(*it));
        INIT_EVENT(events, virtualServer.back().getServerSocketFd());
        std::cout << redColor << "server port" << greenColor << " <" << redColor << ":" << greenColor << "> " << yellowColor << *it << resetColor << std::endl;
        std::cout << greenColor << "server   fd" << redColor << " <" << greenColor << ":" << redColor << "> " << yellowColor << virtualServer.back().getServerSocketFd() << resetColor << std::endl;
        std::cout << "-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-" << std::endl;
        clientSocket.insert(std::make_pair(virtualServer.back().getServerSocketFd(), Client(events)));
    }
    // signal(SIGINT, Server::handelSignal);
    signal(SIGPIPE, SIG_IGN);
}

bool Server::isServer( int cSock ) {

    for (std::vector<ServerSocket>::iterator it = virtualServer.begin(); it != virtualServer.end(); it++) {
        if (it->getServerSocketFd() == cSock)
            return true;
    }
    return false;
}

void Server::runServer( void ) {

    while (true) {
        tmpEvents = this->getAllClientsFd();
        poll(tmpEvents.data(), tmpEvents.size(), FAILED);
        for (std::vector<ServerSocket>::iterator it = virtualServer.begin(); it != virtualServer.end(); it++) {
            for (size_t i = ZERO; i < tmpEvents.size(); i++) {
                if (tmpEvents[i].fd == it->getServerSocketFd()) {
                    if (!this->acceptNewConnection(*it))
                        continue;
                } else {
                    // ****************
                    int cS = tmpEvents[i].fd;
                    sockaddr_in clientAddress;
                    socklen_t addrLen = sizeof(clientAddress);
                    int clientPort;
                    if (getsockname(cS, reinterpret_cast<sockaddr*>(&clientAddress), &addrLen) == 0) {
                        clientPort = ntohs(clientAddress.sin_port);
                    }
                    //**********************
                    if (!isServer(tmpEvents[i].fd) && tmpEvents[i].revents & POLLIN && clientPort == it->getSockPort() && !clientSocket[tmpEvents[i].fd].getRemainingBytes() && !this->readFromClientSocketFd(i))
                            continue;
                    if (!isServer(tmpEvents[i].fd) && tmpEvents[i].revents & POLLOUT && clientPort == it->getSockPort()) {
                        ft_parse_request(clientSocket[tmpEvents[i].fd].getBuffer());
                        size_t requestLenght = clientSocket[tmpEvents[i].fd].getBuffer().find("\r\n\r\n"); // parcer
                        std::map<std::string, std::string>::iterator contentLength = request_data.find("Content-Length");
                        if (requestLenght != std::string::npos && contentLength == request_data.end() && !clientSocket[tmpEvents[i].fd].getRemainingBytes())
                            this->GET(i);
                        else if (clientSocket[tmpEvents[i].fd].getRemainingBytes() && (tmpEvents[i].revents & POLLOUT))
                            Sent(tmpEvents[i].fd);
                        if (contentLength != request_data.end() && (int)atoi(contentLength->second.c_str()) > ZERO)
                            this->POST(i, contentLength);
                        request_data.clear();
                    }
                }
            }
        }
    }
}
