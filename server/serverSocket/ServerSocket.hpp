/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerSocket.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hait-hsa <hait-hsa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 11:40:00 by hait-hsa          #+#    #+#             */
/*   Updated: 2024/02/17 18:16:10 by hait-hsa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <netinet/in.h>
#include <vector>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include "../client/Client.hpp"

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
