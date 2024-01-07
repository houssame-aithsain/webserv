/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gothmane <gothmane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/07 16:08:13 by gothmane          #+#    #+#             */
/*   Updated: 2024/01/07 16:20:31 by gothmane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <parser.hpp>

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


std::vector<std::pair<std::string, std::string>> Parser::getServer() {
    return this->server;
}

void Parser::setServer(const std::vector<std::pair<std::string, std::string>>& server) {
    this->server = server;
}

