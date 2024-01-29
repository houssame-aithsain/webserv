/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hait-hsa <hait-hsa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/27 18:16:38 by hait-hsa          #+#    #+#             */
/*   Updated: 2024/01/29 20:39:35 by hait-hsa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include "../parsing/parser.hpp"

#define ZERO 0
#define ONE 1

#define SOCKETFAILED "Error binding socket!"

class Server : public Parser {
    public:
        // ~server( void );
        // server( void );
        // server( const server & other );
        // server operator=(const server & other);
        void initializeSocket(  std::vector <server_data> serverData );
};
