/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gothmane <gothmane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/07 16:08:15 by gothmane          #+#    #+#             */
/*   Updated: 2024/01/08 18:54:06 by gothmane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <sstream>

typedef struct t_server_data
{
    std::vector<std::pair<std::string, std::vector<std::string> > > server;
    std::vector<std::vector<std::pair<std::string, std::vector<std::string> > > > location;
}           server_data;

class Parser
{
    private:
        std::vector <server_data> wrapper;
        int     port;
        server_data data;
   public:
        Parser();
        Parser(const Parser& p);
        Parser& operator=(const Parser& p);
        ~Parser();
        // std::vector<std::pair<std::string, std::vector<std::string> > > getServer();
        // void setServer(const std::vector<std::pair<std::string, std::vector<std::string> > > &server);
                // Getter pour port
        int getPort() const {
            return port;
        }

        // Setter pour port
        void setPort(int p) {
            port = p;
        }
        void    ft_read_nd_parse(std::string fileName);
        void    ft_parse_data_step2();
        void    ft_print_data();


};