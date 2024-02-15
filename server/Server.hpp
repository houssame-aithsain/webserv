/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hait-hsa <hait-hsa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/27 18:16:38 by hait-hsa          #+#    #+#             */
/*   Updated: 2024/02/15 15:14:37 by hait-hsa         ###   ########.fr       */
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

#define CHUNK_SIZE 1024
#define ZERO 0
#define ONE 1
#define FAILED -1

#define SOCKETFAILED "Error binding socket!"

class Server : public Parser , public Response  {
    public:
        // ~server( void );
        // server( void );
        // server( const server & other );
        // server operator=(const server & other);
        void initializeSocket(  std::vector <server_data> serverData );
        void handleHttpRequest(int clientSocket);
        int receiveData(int clientSocket);
        // void handleConnection(int clientSocket);
};
