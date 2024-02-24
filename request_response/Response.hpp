/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlaazouz <jlaazouz@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 18:36:03 by gothmane          #+#    #+#             */
/*   Updated: 2024/02/24 15:10:01 by jlaazouz         ###   ########.fr       */
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

#define LOG(x) std::cout << x << std::endl

class Response
{
    protected :
        std::string _method;
        std::string _path;
        std::string _htmlVersion;
        std::map<std::string, std::string> request_data;
    
    public:
        Response();
        ~Response();
        std::string ft_getPageToRender(std::vector<std::pair<std::string, std::vector<std::string> > > ParsedRequest);
        void ft_parse_request(std::string request);
        void handleHttpRequest(int clientSocket, char* httpRequest); 
};