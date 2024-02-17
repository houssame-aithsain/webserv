/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hait-hsa <hait-hsa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/27 18:16:38 by hait-hsa          #+#    #+#             */
/*   Updated: 2024/02/17 18:12:23 by hait-hsa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#pragma once

#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include "../parsing/parser.hpp"
#include "../request_response/Response.hpp"
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
#include "serverSocket/ServerSocket.hpp"


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
        std::vector<ServerSocket> virtualServer;
        std::vector<int> serverPort;
        std::vector<pollfd> tmpEvents;
        // std::map<int, Client> clientsSocket;
    public:
        // virtual ~Server( void );
        // virtual ~Server();
        Server( void );
        Server( int sockPort );
        // server( const server & other );
        // server operator=(const server & other);
        void runServer( void );
        void initializeSocket(  std::vector<server_data> serverData );
        void handleHttpRequest(int clientSocket);
        int receiveData(int index);
        bool acceptNewConnection( ServerSocket server );
        bool readFromClientSocketFd(int index);
        std::vector<Client> creatClientOBJ(void);
        static void handelSignal(int signum);
        void GET( int index );
        void POST(int index, std::map<std::string, std::string>::iterator contentLength);
        int getServerPort( void );
        int getServerSocketFd( void );
        // void handleConnection(int clientSocket);
        std::vector<pollfd> getAllClientsFd(void);
        class MyExceptio : public std::exception {};
};
