/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gothmane <gothmane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/27 18:16:38 by hait-hsa          #+#    #+#             */
/*   Updated: 2024/02/11 19:26:19 by gothmane         ###   ########.fr       */
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


#define ZERO 0
#define ONE 1
#define FAILED -1

#define SOCKETFAILED "Error binding socket!"

class Server : public Parser , public Response  {
    public:
        std::map<std::string, std::string> request_data;
        // ~server( void );
        // server( void );
        // server( const server & other );
        // server operator=(const server & other);
        void initializeSocket(  std::vector <server_data> serverData );
        void handleHttpRequest(int clientSocket, char* httpRequest);
        void handleConnection(int clientSocket);
        void ft_parse_request(std::string buffer);
};
