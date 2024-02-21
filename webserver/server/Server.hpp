/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hait-hsa <hait-hsa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/27 18:16:38 by hait-hsa          #+#    #+#             */
/*   Updated: 2024/02/20 12:26:46 by hait-hsa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#pragma once

#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include "../../parsing/parser.hpp"
#include "../../request_response/Response.hpp"
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
#include "../serverSocket/ServerSocket.hpp"

#define greenColor "\033[32m"
#define redColor "\033[31m"
#define yellowColor "\033[33m"

#define CHUNK_SIZE 1024

#define INIT_EVENT(ev, clinetSocket) \
        ev.fd = clinetSocket;         \
        ev.events = POLLIN | POLLOUT;  \
        ev.revents = ZERO;              \

#define SOCKETFAILED "Error binding socket!"

class Server : public Parser , public Response, public ServerSocket, public Client {
    private:
        pollfd events;
        // int serverSocketFd;
        std::vector<int> serverPort;
        std::vector<pollfd> tmpEvents;
        // std::map<int, Client> clientsSocket;
    public:
        // virtual ~Server( void );
        // virtual ~Server();
        Server( void );
        // server( const server & other );
        // server operator=(const server & other);
        void runServer( void );
        void initializeSocket(  std::vector<server_data> serverData );
        void handleHttpRequest(int clientSocket);
        int receiveData(int index);
        bool acceptNewConnection( ServerSocket server );
        bool readFromClientSocketFd(int index);
        std::vector<Client> creatClientOBJ(void);
        // void handelSignal(int signum);
        void GET( int index );
        void POST(int index, std::map<std::string, std::string>::iterator contentLength);
        int getServerPort( void );
        int getServerSocketFd( void );
        // void handleConnection(int clientSocket);
        std::vector<pollfd> getAllClientsFd(void);
        bool isServer( int cSock );
        void Sent(int cSock);
        class MyExceptio : public std::exception {};
};
