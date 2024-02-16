/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hait-hsa <hait-hsa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/27 18:16:38 by hait-hsa          #+#    #+#             */
/*   Updated: 2024/02/16 22:12:56 by hait-hsa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#pragma once

#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include "../parsing/parser.hpp"
#include "../request_response/Response.hpp"
#include "client/Client.hpp"
#include <csignal>
#include <sys/event.h>
#include <limits.h>
#include <iterator>
#include <cstring>
#include <vector>
#include <sys/poll.h>
#include <exception>


#define CHUNK_SIZE 1024
#define ZERO 0
#define ONE 1
#define FAILED -1

#define INIT_SOCKET(socketAddress, PORT)     \
    socketAddress.sin_family = AF_INET;       \
    socketAddress.sin_addr.s_addr = INADDR_ANY;\
    socketAddress.sin_port = htons(PORT);       \

#define INIT_EVENT(ev, clinetSocket) \
        ev.fd = clinetSocket;         \
        ev.events = POLLIN | POLLOUT;    \
        ev.revents = ZERO;                \

#define SOCKETFAILED "Error binding socket!"

class Server : public Parser , public Response , public Client {
    private:
        pollfd events;
        int serverSocketFd;
        sockaddr_in socketAddress;
        std::vector<pollfd> tmpEvents;
    public:
        // virtual ~Server( void );
        // virtual ~Server();
        // server( void );
        // server( const server & other );
        // server operator=(const server & other);
        void initializeSocket(  std::vector <server_data> serverData );
        void handleHttpRequest(int clientSocket);
        int receiveData(int index);
        void sockCreat( void );
        void socketError( void );
        void nonBlockingMode( void );
        void sockBind( void );
        void sockListen( void );
        bool acceptNewConnection( void );
        bool readFromClientSocketFd(int index);
        std::vector<pollfd> getAllClientsFd(void);
        std::vector<Client> creatClientOBJ(void);
        // void handleConnection(int clientSocket);
        class MyExceptio : public std::exception {};
};
