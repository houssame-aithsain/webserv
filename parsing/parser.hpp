/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gothmane <gothmane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/07 16:08:15 by gothmane          #+#    #+#             */
/*   Updated: 2024/01/08 12:57:00 by gothmane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <fstream>
#include <iostream>
#include <vector>
#include <map>

typedef struct t_server_data
{
    std::vector<std::pair<std::string, std::vector<std::string> > > server;
    std::vector<std::vector<std::pair<std::string, std::vector<std::string> > > > location;
}           server_data;

class Parser
{
    private:
        std::vector <server_data> wrapper;
        server_data data;
   public:
        Parser();
        Parser(const Parser& p);
        Parser& operator=(const Parser& p);
        ~Parser();
        // std::vector<std::pair<std::string, std::vector<std::string> > > getServer();
        // void setServer(const std::vector<std::pair<std::string, std::vector<std::string> > > &server);

        void ft_read_nd_parse(std::string fileName);

};