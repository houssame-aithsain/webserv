/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gothmane <gothmane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 18:36:03 by gothmane          #+#    #+#             */
/*   Updated: 2024/02/14 12:11:48 by gothmane         ###   ########.fr       */
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
        std::string handleHttpRequest(int clientSocket, char* httpRequest); 
};