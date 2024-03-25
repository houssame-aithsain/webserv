/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gothmane <gothmane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/07 16:08:15 by gothmane          #+#    #+#             */
/*   Updated: 2024/03/24 19:54:20 by gothmane         ###   ########.fr       */
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
    protected :
        int         port;
        server_data data;

    public:
        std::vector <server_data> wrapper;
        Parser();
        Parser(const Parser& p);
        Parser& operator=(const Parser& p);
        ~Parser();
        
        int getPort() const {
            return port;
        }

        void setPort(int p) {
            port = p;
        }
        void    ft_read_nd_parse(std::string fileName);
        void    ft_parse_data_step2();
        void    ft_print_data();
        std::vector<server_data> getWrapper( void );
        std::string ft_trim(const std::string& str, std::string trim);
        std::vector<std::string>  ft_split_the_multiple_data(std::string value);
        std::vector<std::string>    get_data_from_conf(std::string &port, std::string prefix, std::string key, int bv);
        std::vector<std::string>      get_server_locations(std::string &port);
        void    check_for_default_location(void);
        void replace_prefix_with_alias(void);

};