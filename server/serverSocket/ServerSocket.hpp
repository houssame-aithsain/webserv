/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerSocket.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hait-hsa <hait-hsa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 11:40:00 by hait-hsa          #+#    #+#             */
/*   Updated: 2024/02/17 17:37:19 by hait-hsa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <csignal>
#include <sys/event.h>
#include <limits.h>
#include <iterator>
#include <cstring>
#include <vector>
#include <sys/poll.h>
#include <exception>
#include <fcntl.h>
#include <fstream>
#include <unistd.h>
#include <map>
#include <iostream>
#include "../client/Client.hpp"

#define CHUNK_SIZE 1024
#define ZERO 0
#define ONE 1
#define FAILED -1

#define INIT_SOCKET(socketAddress, PORT)     \
    socketAddress.sin_family = AF_INET;       \
    socketAddress.sin_addr.s_addr = INADDR_ANY;\
    socketAddress.sin_port = htons(PORT);       \

class ServerSocket {
    private:
        int sockPort;
        sockaddr_in socketAddress;
        std::vector<pollfd> tmpEvents;
        int serverSocketFd;
    public:
        ServerSocket( void );
        ServerSocket( int port );
        void sockCreat( void );
        void socketError( void );
        void nonBlockingMode( void );
        void sockBind( void );
        void sockListen( void );
        int getServerSocketFd( void );
        sockaddr_in& getSocketAddress( void );
        std::vector<pollfd> getAllClientsFd(void);
};
