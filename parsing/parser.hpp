/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hait-hsa <hait-hsa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/07 16:08:15 by gothmane          #+#    #+#             */
/*   Updated: 2024/01/29 22:08:32 by hait-hsa         ###   ########.fr       */
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
        int     port;
        server_data data;
   public:
        std::vector <server_data> wrapper;
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
        std::vector<server_data> getWrapper( void );
};