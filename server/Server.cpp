/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gothmane <gothmane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/01 01:27:52 by hait-hsa          #+#    #+#             */
/*   Updated: 2024/02/13 16:11:08 by gothmane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <unistd.h>
#include <fstream>
#include <sstream>

int sockFD;
int clientSocket;
char buffer[INT_MAX];

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
    int reuse = 1;
    if (setsockopt(sockFD, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
        perror("setsockopt(SO_REUSEADDR) failed");
        exit(EXIT_FAILURE);
    }
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
                //
                memset(&buffer, 0, INT_MAX);
                bzero(&buffer, 1);
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
