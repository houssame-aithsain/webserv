/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerSocket.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hait-hsa <hait-hsa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 11:40:00 by hait-hsa          #+#    #+#             */
/*   Updated: 2024/02/18 12:24:04 by hait-hsa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <netinet/in.h>
#include <vector>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include "../clientSocket/Client.hpp"

#define ZERO 0
#define ONE 1
#define FAILED -1

#define blueColor "\033[34m"
#define resetColor "\033[0m"

#define INIT_SOCKET(socketAddress, PORT)     \
    socketAddress.sin_family = AF_INET;       \
    socketAddress.sin_addr.s_addr = INADDR_ANY;\
    socketAddress.sin_port = htons(PORT);       \

class ServerSocket {
    private:
        int sockPort;
        sockaddr_in socketAddress;
        int serverSocketFd;
    public:
        ServerSocket( void );
        ServerSocket( int port );
        void sockCreat( void );
        void reuseSocket( void );
        void nonBlockingMode( void );
        void sockBind( void );
        void sockListen( void );
        int getServerSocketFd( void );
        sockaddr_in& getSocketAddress( void );
};
// hello