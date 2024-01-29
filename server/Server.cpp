/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hait-hsa <hait-hsa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/27 18:16:20 by hait-hsa          #+#    #+#             */
/*   Updated: 2024/01/29 22:30:46 by hait-hsa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <unistd.h>

std::string trim(const std::string& str, const std::string& charsToTrim) {
    
    size_t start = str.find_first_not_of(charsToTrim);
    size_t end = str.find_last_not_of(charsToTrim);

    if (start == std::string::npos || end == std::string::npos)
        return std::string();
    return str.substr(start, end - start + 1);
}

void Server::initializeSocket(std::vector <server_data> serverData)
{
    std::string portToConvert = trim(serverData[ZERO].server[ZERO].second[ZERO], "\"");
    int port = std::atoi(portToConvert.c_str());
    int socketFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    sockaddr_in serverAddress;
    if (socketFd < ZERO)
    {
        std::cerr << SOCKETFAILED << std::endl;
        exit(ONE);
    }
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = INADDR_ANY;
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

    std::cout << "Server is listening on port " << port << std::endl;
    sleep(100);
    // Now the server is set up to accept incoming connections.

    // Don't forget to close the socket when you're done.
    close(socketFd);
}