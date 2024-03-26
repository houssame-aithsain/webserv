/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gothmane <gothmane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 01:27:52 by hait-hsa          #+#    #+#             */
/*   Updated: 2024/03/26 21:38:55 by gothmane         ###   ########.fr       */
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
    // std::cout << "Sockets have been closed!" << std::endl;
//     exit(signum);
// }


Server::Server( void ) {}

void Server::handleHttpRequest(int cSock, Parser &p, int flag, int type)
{
    if (flag == 1 && this->_errorCode == 0)
    {
        // std::cout << "IN FLAG\n";
        createResponse(p, type, clientSocket[cSock]);
    }
    clientSocket[cSock].resetRemainingBytes();
    clientSocket[cSock].resetTotalBytesSent();
    clientSocket[cSock].clearBuffer();
    clientSocket[cSock].clearResponseBuffer();
    clientSocket[cSock].fillResponseBuffer(this->response);
    clientSocket[cSock].setRemainingBytes(this->response.length());
    clientSocket[cSock].clearBuffer();
    // Send the HTTP response

}



int Server::getClientPort(int index) {
    
    int cS = tmpEvents[index].fd;
    sockaddr_in clientAddress;
    socklen_t addrLen = sizeof(clientAddress);
    int clientPort = 0x0000;

    if (getsockname(cS, reinterpret_cast<sockaddr*>(&clientAddress), &addrLen) == 0) {
        clientPort = ntohs(clientAddress.sin_port);
    }
    return clientPort;
}

bool Server::isServer( int cSock ) {

    for (std::vector<ServerSocket>::iterator it = virtualServer.begin(); it != virtualServer.end(); it++) {
        if (it->getServerSocketFd() == cSock)
            return true;
    }
    return false;
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
    // signal(SIGINT, Server::handelSignal); i should close all sockets fds
    signal(SIGPIPE, SIG_IGN);
}

bool Server::Sent(int cSock, int index) {

    ssize_t bytesSent = send(cSock, clientSocket[cSock].getResponseBuffer().c_str() + clientSocket[cSock].getTotalBytesSent(), clientSocket[cSock].getRemainingBytes(), 0);
    if (bytesSent == FAILED || !bytesSent) { 
        if (bytesSent == FAILED) {
            perror("send");
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
}

void Server::GET(int index,  Parser &p, int type) {

    std::cout << redColor << "GET" << resetColor << std::endl;
    this->_errorCode = 0;
    std::string pr = this->getPort();
    // std::cout << "THE METHOD => " << request_data["Method"] << "\n";
    int check_error = requestErrorsHandling(p, pr);
    // std::cout << "ERROR CHECK >> " << check_error << "\n";
    if (this->_errorCode || check_error == 1)
    {
        // std::cout << "In errorCode > " << this->_errorCode  << "\n";
        handleHttpRequest(tmpEvents[index].fd, p, 0, 0);
    }
    else if (this->request_data["Method"] == "GET" && check_error == 0)
    {
        std::cout << "IN GET\n";
        handleHttpRequest(tmpEvents[index].fd, p, 1, type);
    }
    // std::cout << redColor << tmpEvents[index].fd << resetColor << std::endl;
}

void Server::POST(int index, Parser &p, int type) {

    // (void) type;
    // (void) p;
    // std::cout << greenColor << "POST" << resetColor << std::endl;

    clientSocket[tmpEvents[index].fd].appendPostBuffer(clientSocket[tmpEvents[index].fd].getBuffer());
    // std::cout << "content lent ==> " << (int)atoi(contentLength->second.c_str()) << std::endl;
    // std::cout << "lient buffer lent ==> " << (int)clientSocket[tmpEvents[index].fd].getPostBuffer().size() << std::endl;
    if (clientSocket[tmpEvents[index].fd].getResponseBodyCounter() <= clientSocket[tmpEvents[index].fd].getPostBuffer().size()) {
        std::cout << redColor << clientSocket[tmpEvents[index].fd].getPostBuffer() << resetColor << std::endl;
        //POST
        ft_parse_request(clientSocket[tmpEvents[index].fd].getPostBuffer());
        // std::ofstream file("output1.csv");
        // if (file.is_open()) {
        //     file << clientSocket[tmpEvents[index].fd].getPostBuffer();
        // }
        // TO GO TO THE REQUESTED RESPONSE
        this->_errorCode = 0;
        std::string pr = this->getPort();
        // std::cout << "THE METHOD asdasdasd >> " << this->request_data["Method"] << "\n";
        int check = requestErrorsHandling(p, pr);

        if (check == 0)
        {
            // std::cout << "ERROR CODE => " <<  this->_errorCode << "\n";
            handleHttpRequest(tmpEvents[index].fd, p, 1, type);

        }

        clientSocket[tmpEvents[index].fd].fillResponseBuffer(this->response);
        clientSocket[tmpEvents[index].fd].setRemainingBytes(this->response.length());
        Sent(tmpEvents[index].fd, index);
        close(tmpEvents[index].fd);
        clientSocket[tmpEvents[index].fd].clearPostBuffer();
        clientSocket[tmpEvents[index].fd].resetResponseBodyCounter();
    }
    clientSocket[tmpEvents[index].fd].clearBuffer();
}


void Server::DELETE(int index,  Parser &p, int type) {

    std::cout << redColor << "GET" << resetColor << std::endl;
    this->_errorCode = 0;
    std::string pr = this->getPort();
    // std::cout << "THE METHOD => " << request_data["Method"] << "\n";
    int check_error = requestErrorsHandling(p, pr);
    // std::cout << "ERROR CHECK >> " << check_error << "\n";
    if (this->_errorCode || check_error == 1)
    {
        // std::cout << "In errorCode > " << this->_errorCode  << "\n";
        handleHttpRequest(tmpEvents[index].fd, p, 0, 0);
    }
    else if (this->request_data["Method"] == "GET" && check_error == 0)
    {
        std::cout << "IN GET\n";
        handleHttpRequest(tmpEvents[index].fd, p, 1, type);
    }
    // std::cout << redColor << tmpEvents[index].fd << resetColor << std::endl;
}

bool Server::receiveData(int cSock, int index) {

    char chunk[CHUNK_SIZE + 1];

    bzero(chunk, CHUNK_SIZE + 1);
    int bytesRead = recv(cSock, chunk, CHUNK_SIZE, ZERO);
    if (bytesRead == FAILED) {
        if (errno == EAGAIN || errno == EWOULDBLOCK)
            return false;
        else {
            perror("recv");
            return false;
        }
    } else if (!bytesRead) {
        close(cSock);
        clientSocket.erase(cSock);
        tmpEvents.erase(tmpEvents.begin() + index);
        return false;
    }
    chunk[bytesRead] = ZERO;
    clientSocket[cSock].appendStr(chunk, bytesRead);
    // std::cout << redColor << "receving" <<resetColor << std::endl;
    return true;
}

bool Server::acceptNewConnection( ServerSocket server ) {

    int cSock;
    pollfd newClient;
    int addrlen = sizeof(server.getSocketAddress());
    
    if ((cSock = accept(server.getServerSocketFd(), (struct sockaddr *)(&server.getSocketAddress()), (socklen_t *)&addrlen)) == FAILED) {
        return false;
    } else {
        int status = fcntl(cSock, F_SETFL, fcntl(cSock, F_GETFL, 0) | O_NONBLOCK);
        if (status == FAILED) {
            perror("calling fcntl");
        }
        INIT_EVENT(newClient, cSock);
        std::cout << "client has been accepted!" << std::endl;
        tmpEvents.push_back(newClient);
        clientSocket.insert(std::make_pair(newClient.fd, Client(newClient)));
    }
    return true;
}


void Server::runServer(Parser &p) {

(void) p;
    bool isPost;
    while (true) {
    poll(tmpEvents.data(), tmpEvents.size(), -1);

    for (std::vector<ServerSocket>::iterator it = virtualServer.begin(); it != virtualServer.end(); it++) { // loop over every server
        for (size_t i = 0; i < tmpEvents.size(); i++) { // loop every event
            if (getClientPort(i) != it->getServerPort() && i) // check if the client is in the same server port
                continue;
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
                    // std::cout << "THE PARSING\n";
                    // std::cout <<  "THE REQUEST >> " << clientSocket[tmpEvents[i].fd].getBuffer() << "\n";
                    // std::cout << "content length >> " << request_data["Content-Length"] << "\n";

                if (it == request_data.end() && !clientSocket[tmpEvents[i].fd].getResponseBuffer().size() && !clientSocket[tmpEvents[i].fd].getResponseBodyCounter())
                {
                    GET(i, p, 1);
                }
                //POST
                else {
                    if (clientSocket[tmpEvents[i].fd].getPostBuffer().empty()) {
                        // std::cout << "init body conter : " << (size_t) atoi(it->second.c_str()) << std::endl;
                        clientSocket[tmpEvents[i].fd].initResponseBodyCounter((size_t) atoi(it->second.c_str()) + size_header);
                    }
                    // std::cout << "IN POST\n";
                    POST(i, p, 0);
                        // std::cout << "THE RESPONSE [POST] >> " << this->response << "\n";
                }
                // else if (this->request_data["Method"] == "DELETE")
                // {
                //     std::cout << yellowColor << "DELETE\n";
                //     DELETE(i, p, 2);
                // }
            }
            if (tmpEvents[i].revents & POLLOUT && !isServer(tmpEvents[i].fd)) {
                if (clientSocket[tmpEvents[i].fd].getRemainingBytes()) { 
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
