/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gothmane <gothmane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 01:27:52 by hait-hsa          #+#    #+#             */
/*   Updated: 2024/02/11 20:52:13 by gothmane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <unistd.h>
#include <fstream>
#include <sstream>

int sockFD;
int clientSocket;
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

// std::vector<std::pair<std::string, std::vector<std::string> > > request_data;

void Server::handleHttpRequest(int clientSocket, char* httpRequest) 
{
    (void) httpRequest;
    std::cout << "######################################################\n";
    // std::vector<std::string> requestDataParsed = it->second;
    if (request_data["Method"] == "GET") 
    {
    // std::cout << ft_getPageToRender(request_data) << std::endl;
    std::cout << "Test || "<< request_data["Asset"];
    std::string requestedResource = request_data["Asset"];
    if (requestedResource == "/")
        requestedResource = "/index.html";
    std::ifstream file("."+requestedResource, std::ios::binary);
    if (!file.is_open()) {
        // File not found, respond with a 404 error
        std::string response = "HTTP/1.1 404 Not Found\r\n\r\n";
        send(clientSocket, response.c_str(), response.length(), 0);
    } else {
        // Read the entire content of the file into a string        
        std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
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
        httpResponse += "Content-Type: "+content_type+"\r\n";
        httpResponse += "Content-Length: " + std::to_string(content.size()) + "\r\n"; // remove to_string
        httpResponse += "\r\n" + content;
        // Send the HTTP response
        send(clientSocket, httpResponse.c_str(), httpResponse.length(), 0);
        // Clean up
        file.close();
    }
        std::cout << std::endl;
    }
    std::cout << "######################################################\n\n";

}

void Server::initializeSocket(std::vector<server_data> serverData) {

    int kq;
    int eventNumb;
    int requestByteSize;
    struct kevent event;
    struct kevent Revent;
    sockaddr_in socketAddress;
    std::string sockPort = trim(serverData[ZERO].server[ZERO].second[ZERO], "\"");
    int sockFD = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    socketAddress.sin_family = AF_INET;
    socketAddress.sin_addr.s_addr = INADDR_ANY;
    socketAddress.sin_port = htons(std::atoi(sockPort.c_str()));
    if (bind(sockFD, reinterpret_cast<struct sockaddr *>(&socketAddress), sizeof(socketAddress)) == FAILED) {
        std::cout << "failed to bind server socket" << std::endl;
        close(sockFD);
        exit(10);
    }
    if ((kq = kqueue()) == FAILED) {
        std::cout << "failed to create a kernel event queue" << std::endl;
        close(sockFD);
        close(kq);
        exit(14);
    }
    EV_SET(&event, sockFD, EVFILT_READ, EV_ADD , ZERO, ZERO, nullptr);
    if (kevent(kq, &event, ONE, nullptr, ZERO, nullptr)) {
        std::cout << "failed to associat event with kernel event queue!" << std::endl;
        close(sockFD);
        close(kq);
        exit(18);
    }
    if (listen(sockFD, 10) == FAILED) {
        std::cout << "failed to make the socket at the listen mode!" << std::endl;
        close(sockFD);
        exit(11);
    }
    std::cout << "server now is listening in port " << sockPort << std::endl;
    signal(SIGINT, handelSignal);
    while (true) {
        eventNumb = kevent(kq, nullptr, ZERO, &Revent, ONE, nullptr);
        for (int i = ZERO; i < eventNumb; i++) {
            if (Revent.ident == (uintptr_t)sockFD) {
                if ((clientSocket = accept(sockFD, nullptr, nullptr)) == FAILED) {
                    std::cout << "{=====> error: connection failed <=====}" << std::endl;
                    continue;
                } else
                    std::cout << "connection has been done successfully" << std::endl;
                requestByteSize = recv(clientSocket, buffer, sizeof(buffer), ZERO);
                if (requestByteSize <= ZERO)
                    break;
                buffer[requestByteSize] = ZERO;
                // std::cout << buffer << std::endl;
                // Hadi ra dyali la bghiti hyedha
                ft_parse_request(buffer);
                handleHttpRequest(clientSocket, buffer);
            }
        }
        close(clientSocket);
    }
    close(sockFD);
}
