/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hait-hsa <hait-hsa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 01:27:52 by hait-hsa          #+#    #+#             */
/*   Updated: 2024/02/11 03:41:13 by hait-hsa         ###   ########.fr       */
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

void Server::handleHttpRequest(int clientSocket, char* httpRequest) {

    std::cout << "######################################################\n";
    for (std::vector<std::pair<std::string, std::vector<std::string> > >::iterator it = request_data.begin(); it != request_data.end(); ++it)
    {
        std::cout << "Key: " << it->first << ", Values: ";
        for (std::vector<std::string>::iterator vecIt = it->second.begin(); vecIt != it->second.end(); ++vecIt)
        {
            std::cout << *vecIt << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "######################################################\n\n";
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
                std::cout << buffer << std::endl;
                handleHttpRequest(clientSocket, buffer);
            }
        }
        close(clientSocket);
    }
    close(sockFD);
}

#include <utility> // for std::pair

void Server::ft_parse_request(std::string request) {
    std::istringstream getrequestStream(request);
    std::string getrequestLine;
    int index = 0;
    while (std::getline(getrequestStream, getrequestLine)) 
    {
        if (index == 0)
        {
            //get method
            std::vector<std::string>  new_v;
            int next = getrequestLine.find(" ");
            new_v.push_back(getrequestLine.substr(0, next));
            this->request_data.push_back(std::make_pair("Method", new_v));

            //get assets
            new_v.clear();
            int new_next = getrequestLine.find(" " , next + 1);
            new_v.push_back(getrequestLine.substr((next + 1), (new_next - (next + 1))));
            this->request_data.push_back(std::make_pair("Asset", new_v));

            //get type
            new_v.clear();
            new_v.push_back(getrequestLine.substr(new_next + 1, ((getrequestLine.size() - 2) - new_next)));
            this->request_data.push_back(std::make_pair("Type", new_v));
            index++;
            // request_data[0].first = "Method";
            // request_data[1].second[0] = getrequestLine.substr(0, getrequestLine.find(" "));
        }
        else
        {
            int i1 = getrequestLine.find(":");
            std::string key = getrequestLine.substr(0, i1);
            std::string value = getrequestLine.substr((i1 + 1), ((getrequestLine.size() - 2) - i1));

            if (key != "")
            {
                std::vector<std::string>  new_v;

                new_v.push_back(ft_trim(value, " \t\n\""));
                this->request_data.push_back(std::make_pair(key, new_v));
            }
            // std::cout << "The key => " << key << std::endl;
            // std::cout << "The value => " << value << std::endl;
        }
        // std::cout << getrequestLine << std::endl;
    }


}