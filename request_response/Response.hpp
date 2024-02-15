/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hait-hsa <hait-hsa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 18:36:03 by gothmane          #+#    #+#             */
/*   Updated: 2024/02/14 18:48:03 by hait-hsa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <vector>
#include <iterator>
#include <utility> // for std::pair
#include <map>
#include <sstream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include "../parsing/parser.hpp"
#include <csignal>
#include <sys/event.h>
#include <limits.h>
#include <cstring>

class Response
{
    public:
        std::map<std::string, std::string> request_data;
        Response();
        ~Response();
        std::string ft_getPageToRender(std::vector<std::pair<std::string, std::vector<std::string> > > ParsedRequest);
        void ft_parse_request(std::string request);
        void handleHttpRequest(int clientSocket, char* httpRequest); 
};