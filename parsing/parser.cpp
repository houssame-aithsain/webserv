/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gothmane <gothmane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/07 16:08:13 by gothmane          #+#    #+#             */
/*   Updated: 2024/01/07 17:42:08 by gothmane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.hpp"

Parser::Parser(){}

Parser::~Parser(){}

Parser::Parser(const Parser& p) 
{
    this->server = p.server;
}

Parser& Parser::operator=(const Parser& pr)
{
    if (this != &pr)
        this->server = pr.server;
    return (*this);
}


std::vector<std::pair<std::string, std::vector<std::string> > > Parser::getServer() {
    return this->server;
}

void Parser::setServer(const std::vector<std::pair<std::string, std::vector<std::string> > > &server) 
{
    this->server = server;
}



void Parser::ft_read_nd_parse(std::string fileName)
{
    std::ifstream in(fileName);
    std::string line;
    int server_side = 0;
    int location_side = 0;
    
    if (in.is_open())
    {
        while (std::getline(in, line))
        {
            if (line == "[[server]]")
                server_side = 1;
            else if (line == "	[[server.location]]")
            {
                server_side = 0;
                location_side = 1;
            }
            if (server_side == 1)
            {
                std::string key = line.substr(0, line.find("="));
                std::cout << key << std::endl;
            }
            else if (location_side == 1)
            {
                std::string key = line.substr(0, line.find("="));
                std::cout << key << std::endl;
            }
        }
    }
}

