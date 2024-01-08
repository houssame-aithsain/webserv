/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gothmane <gothmane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/07 16:08:13 by gothmane          #+#    #+#             */
/*   Updated: 2024/01/08 13:10:40 by gothmane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.hpp"

Parser::Parser() {}

Parser::~Parser() {}

Parser::Parser(const Parser &p)
{
}

Parser &Parser::operator=(const Parser &pr)
{
    // if (this != &pr)
    //     this->server = pr.server;
    return (*this);
}



void Parser::ft_read_nd_parse(std::string fileName)
{
    std::ifstream in(fileName);
    std::string line;
    int server_side = 0;
    int location_side = 0;
    int server_id = 0;
    int i = 0;
    int j = -1;

    if (in.is_open())
    {
        while (std::getline(in, line))
        {
            if (line == "[[server]]")
            {
                if (server_side > 0)
                {
                    wrapper.push_back(data);
                    data.server.clear();
                    data.location.clear();                
                }
                server_side++;
                location_side = 0;
                j = -1;
                continue;
            }
            else if (line == "	[[server.location]]")
            {
                // server_side = 0;
                server_id = server_side;
                location_side++;
                j++;
                std::cout << "Server id = " << server_id << " location id = " << location_side << std::endl;
                continue;
            }
            if (server_id != server_side && location_side == 0)
            {
                std::string key = line.substr(0, line.find("="));
                std::vector<std::string> value;
                value.push_back(line.substr(line.find("=") + 1, line.size()));
                data.server.push_back(std::make_pair(key, value));
                // server.erase();
            }
            else if (location_side > 0)
            {
                // std::cout << "In location side ====> ";
                std::string key = line.substr(0, line.find("="));
                std::vector<std::string> value;
                value.push_back(line.substr(line.find("=") + 1, line.size()));
                std::cout << j << std::endl;
                if (j >= data.location.size()) {
                    data.location.resize(j+1);
                }
                data.location.at(j).push_back(std::make_pair(key, value));
                // std::cout << key << std::endl;
                // std::cout << std::endl;
            }
        }
        if (server_side > 0)
            wrapper.push_back(data);
    }
std::size_t r = 1;
for (std::vector<server_data>::iterator it = wrapper.begin(); it != wrapper.end(); ++it, ++r) {
    std::cout << "Server data " << i << ":\n";
    
    std::cout << "#Server:\n";
    std::size_t j = 1;
    for (std::vector<std::pair<std::string, std::vector<std::string> > >::iterator jt = it->server.begin(); jt != it->server.end(); ++jt, ++j) {
        std::string key = jt->first;
        std::vector<std::string> values = jt->second;

        std::cout << "Key: " << key << ", Values: ";
        for (std::size_t k = 0; k < values.size(); ++k) {
            std::cout << values[k] << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "#Location:\n";
    std::size_t k = 1;
    for (std::vector<std::vector<std::pair<std::string, std::vector<std::string> > > >::iterator kt = it->location.begin(); kt != it->location.end(); ++kt, ++k) {
        std::cout << "Location Number : " << k << std::endl;
        std::size_t l = 1;
        for (std::vector<std::pair<std::string, std::vector<std::string> > >::iterator lt = kt->begin(); lt != kt->end(); ++lt, ++l) {
            std::string key = lt->first;
            std::vector<std::string> values = lt->second;

            std::cout << "Key: " << key << ", Values: ";
            for (std::size_t m = 0; m < values.size(); ++m) {
                std::cout << values[m] << " ";
            }
            std::cout << std::endl;
        }
    }
}
}
