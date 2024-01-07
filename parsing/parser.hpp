/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gothmane <gothmane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/07 16:08:15 by gothmane          #+#    #+#             */
/*   Updated: 2024/01/07 17:39:02 by gothmane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <fstream>
#include <iostream>
#include <vector>
#include <map>

class Parser
{
    private:
        std::vector<std::vector <std::pair<std::string, std::vector<std::string> > > > wrapper;
        std::vector<std::pair<std::string, std::vector<std::string> > > server;
        std::vector<std::pair<std::string, std::vector<std::string> > > location;
   public:
        Parser();
        Parser(const Parser& p);
        Parser& operator=(const Parser& p);
        ~Parser();
        std::vector<std::pair<std::string, std::vector<std::string> > > getServer();
        void setServer(const std::vector<std::pair<std::string, std::vector<std::string> > > &server);

        void ft_read_nd_parse(std::string fileName);

};