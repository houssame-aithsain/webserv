/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hait-hsa <hait-hsa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 01:27:52 by hait-hsa          #+#    #+#             */
/*   Updated: 2024/03/11 22:49:04 by hait-hsa         ###   ########.fr       */
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

void Server::callCGI( int index ) {
    CGI::extractClientContent(clientSocket[tmpEvents[index].fd]);
}

// void handelSignal(int signum) {
//     for (size_t i = ZERO; i < clientSocket.size(); i++)
//         close(clientSocket[i].getFD());
//     close(serverSocketFd);
    // std::cout << "Sockets have been closed!" << std::endl;
//     exit(signum);
// }


Server::Server( void ) {}

bool Server::Sent(int cSock, int index) {

    ssize_t bytesSent = send(cSock, clientSocket[cSock].getResponseBuffer().c_str() + clientSocket[cSock].getTotalBytesSent(), clientSocket[cSock].getRemainingBytes(), 0);
    if (bytesSent == FAILED || !bytesSent) { 
        if (bytesSent == FAILED) {
            perror("send");
            // throw "sent";
            // exit(11);
        }
        close(cSock);
        clientSocket.erase(cSock);
        tmpEvents.erase(tmpEvents.begin() + index);
        return false;
    }
    clientSocket[cSock].appendTotalBytesSent(bytesSent);
    clientSocket[cSock].unappendRemainingBytes(bytesSent);
    if (!clientSocket[cSock].getRemainingBytes()) {
        close(cSock);
        clientSocket[cSock].resetRemainingBytes();
        clientSocket[cSock].resetTotalBytesSent();
        clientSocket[cSock].clearResponseBuffer();
    }
    std::cout << greenColor << "sending..." << resetColor << std::endl;
    return true;
    // std::cout << greenColor << "server is sending data to client fd " << redColor << " <" << greenColor << ":" << redColor << "> " << yellowColor << cSock << resetColor << std::endl;
}

void Server::handleHttpRequest(int cSock) {
    // std::cout << greenColor << buffer << resetColor << std::endl;
    // std::cout << "######################################################\n";
    std::map<std::string , std::string>::iterator it;
    for (it = this->request_data.begin() ; it != this->request_data.end() ; it++)
        std::cout << it->first << " " << it->second << std::endl;
    
    if (request_data["Method"] == "GET") {
        std::string requestedResource = request_data["Asset"];
        if (requestedResource == "/")
            requestedResource = "/index.html";

        std::ifstream file("." + requestedResource, std::ios::binary);
        if (!file.is_open()) {
            std::string response = "HTTP/1.1 404 Not Found\r\n\r\n";
            // std::cout << "NOT FOUND!!!!" << std::endl;
            send(cSock, response.c_str(), response.length(), 0);
            close(cSock);
            clientSocket[cSock].resetRemainingBytes();
            clientSocket[cSock].resetTotalBytesSent();
            clientSocket[cSock].clearResponseBuffer();
            // clientSocket.erase(cSock);
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
            clientSocket[cSock].resetTotalBytesSent();
            clientSocket[cSock].fillResponseBuffer(httpResponse);
            clientSocket[cSock].setRemainingBytes(httpResponse.length());
        }
    }
    // std::cout << "######################################################\n\n";
    // close(cSock);
}

bool Server::acceptNewConnection( ServerSocket server ) {

    int cSock;
    pollfd newClient;
    int addrlen = sizeof(server.getSocketAddress());
    
    if ((cSock = accept(server.getServerSocketFd(), (struct sockaddr *)(&server.getSocketAddress()), (socklen_t *)&addrlen)) == FAILED) {
        // if (errno == EAGAIN || errno == EWOULDBLOCK) {
            return false;
        // }
        // perror("accept()");
    } else {
        int status = fcntl(cSock, F_SETFL, fcntl(cSock, F_GETFL, 0) | O_NONBLOCK);
        if (status == FAILED) {
            perror("calling fcntl");
            // throw "calling fcntl";
        }
        // std::cout << "connection has been done successfully" << std::endl;
        INIT_EVENT(newClient, cSock);
        std::cout << "client has been accepted!" << std::endl;
        tmpEvents.push_back(newClient);
        clientSocket.insert(std::make_pair(newClient.fd, Client(newClient)));
    }
    return true;
}

bool Server::receiveData(int cSock, int index) {

    char chunk[CHUNK_SIZE];

    bzero(chunk, sizeof(chunk));
    // std::cout << redColor << cSock << resetColor << std::endl;
    int bytesRead = recv(cSock, chunk, sizeof(chunk) - ONE, ZERO);
    if (bytesRead == FAILED) {
        if (errno == EAGAIN || errno == EWOULDBLOCK)
            return false;
        else {
            // std::cout << redColor << cSock << resetColor << std::endl;
            perror("recv");
            // throw "file descriptor error";
            return false;
        }
    } else if (!bytesRead) {
        close(cSock);
        clientSocket.erase(cSock);
        tmpEvents.erase(tmpEvents.begin() + index);
        return false;
    }
    // std::cout << greenColor << bytesRead << resetColor << std::endl;
    chunk[bytesRead] = ZERO;
    clientSocket[cSock].appendStr(chunk);
    std::cout << redColor << "receving" <<resetColor << std::endl;
    return true;
}

void Server::GET( int index ) {

    std::cout << redColor << "GET" << resetColor << std::endl;
    // std::cout << redColor << tmpEvents[index].fd << resetColor << std::endl;
    handleHttpRequest(tmpEvents[index].fd);
}

void Server::POST(int index) {

    std::cout << greenColor << "POST" << resetColor << std::endl;
    clientSocket[tmpEvents[index].fd].appendPostBuffer(clientSocket[tmpEvents[index].fd].getBuffer());
    if (clientSocket[tmpEvents[index].fd].getResponseBodyCounter() <= (clientSocket[tmpEvents[index].fd].getPostBuffer().size() - 200)) {
        std::cout << "POST complited !" << std::endl;
        {
            std::ofstream file("POSTinput.csv");
            if (file.is_open()) {
                file <<  clientSocket[tmpEvents[index].fd].getPostBuffer();
                file.close();
            }
        }
        close(tmpEvents[index].fd);
        clientSocket[tmpEvents[index].fd].clearPostBuffer();
        clientSocket[tmpEvents[index].fd].resetResponseBodyCounter();
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
        tmpEvents.push_back(events);
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

    bool isPost;
    while (true) {
    // Use poll to wait for events
    poll(tmpEvents.data(), tmpEvents.size(), -1);

    for (std::vector<ServerSocket>::iterator it = virtualServer.begin(); it != virtualServer.end(); it++) { // loop over every server
        for (size_t i = 0; i < tmpEvents.size(); i++) { // loop every event
            //************************************************************************************
                int cS = tmpEvents[i].fd;
                sockaddr_in clientAddress;
                socklen_t addrLen = sizeof(clientAddress);
                int clientPort = 0x0000;
                if (getsockname(cS, reinterpret_cast<sockaddr*>(&clientAddress), &addrLen) == 0) {
                    clientPort = ntohs(clientAddress.sin_port);
                }
                if (clientPort != it->getServerPort() && i)
                    continue;
            //************************************************************************************
            isPost = false;
            if (tmpEvents[i].fd == it->getServerSocketFd()) {
                if (!this->acceptNewConnection(*it)) {
                    continue;
                }
            }
            if (tmpEvents[i].revents & POLLIN && !isServer(tmpEvents[i].fd)) { // hendl GET and POST request
                if (!receiveData(tmpEvents[i].fd, i)) {
                    break;
                }
                if (!clientSocket[tmpEvents[i].fd].getRemainingBytes() && !clientSocket[tmpEvents[i].fd].getResponseBodyCounter())
                {
                    ft_parse_request(clientSocket[tmpEvents[i].fd].getBuffer());
                    std::cout << yellowColor << "request has been parced!" << resetColor << std::endl;
                }
                std::map<std::string, std::string>::iterator it = request_data.find("Content-Length");
                //GET
                if (it == request_data.end() && !clientSocket[tmpEvents[i].fd].getResponseBuffer().size() && !clientSocket[tmpEvents[i].fd].getResponseBodyCounter())
                    GET(i);
                //POST
                else {
                    if (clientSocket[tmpEvents[i].fd].getPostBuffer().empty()) {
                        clientSocket[tmpEvents[i].fd].initResponseBodyCounter((size_t)atoi(it->second.c_str()));
                    }
                    POST(i);
                }
            }
            if (tmpEvents[i].revents & POLLOUT && !isServer(tmpEvents[i].fd)) {
                if (clientSocket[tmpEvents[i].fd].getRemainingBytes()) { // don't close connection if the client 
                    if (!Sent(tmpEvents[i].fd, i))
                        break;
                }
            }
            clientSocket[tmpEvents[i].fd].clearBuffer();
            request_data.clear();
        }
    }
}
}
