/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerSocket.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hait-hsa <hait-hsa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 11:40:05 by hait-hsa          #+#    #+#             */
/*   Updated: 2024/02/18 12:24:11 by hait-hsa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerSocket.hpp"

ServerSocket::ServerSocket( void ) {}

ServerSocket::ServerSocket( int port ) : sockPort(port) {

    sockCreat();
    INIT_SOCKET(socketAddress, sockPort);
    reuseSocket();
    nonBlockingMode();
    sockBind();
    sockListen();
}

void ServerSocket::sockCreat(void) {serverSocketFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);}

int ServerSocket::getServerSocketFd( void ) {return (serverSocketFd);}

void ServerSocket::reuseSocket( void ) {

    int reuse = ONE;
    if (setsockopt(serverSocketFd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
        throw "setsockopt(SO_REUSEADDR) failed";
    }
}

void ServerSocket::nonBlockingMode( void ) {

    if (fcntl(serverSocketFd, F_SETFL, fcntl(serverSocketFd, F_GETFL, 0) | O_NONBLOCK) == FAILED) {
        throw "calling fcntl";
    }
}

void ServerSocket::sockBind( void ) {

    if (bind(serverSocketFd, reinterpret_cast<struct sockaddr *>(&socketAddress), sizeof(socketAddress)) == FAILED) {
        close(serverSocketFd);
        throw "failed to bind server socket";
    }
}

void ServerSocket::sockListen( void ) {

    if (listen(serverSocketFd, 10) == FAILED) {
        close(serverSocketFd);
        throw "failed to make the socket at the listen mode!";
    }
    std::cout << blueColor << "server now is listening on port "<< resetColor << sockPort << std::endl;
}

sockaddr_in& ServerSocket::getSocketAddress( void ) {return (socketAddress);}
