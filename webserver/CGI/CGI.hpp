/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gothmane <gothmane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/02 12:40:13 by hait-hsa          #+#    #+#             */
/*   Updated: 2024/03/24 22:20:26 by gothmane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../clientSocket/Client.hpp"
#include <vector>
#include <cstring>
#include <map>


class CGI {
    private:
        std::string input;
        std::string output;
        std::string request;
        std::vector<char *> env;
        std::string REQUEST_METHOD;
        std::string CONTENT_LENGTH;
        std::string CONTENT_TYPE;
        std::string PATH_INFO;
        std::string QUERY_STRING;
    public:
        void extractClientContent(Client & client, std::map<std::string, std::string> data);
        void executeCGIscript(Client & client );
        void EnvInit(Client & client, std::map<std::string, std::string> data);
        std::string getContentType(std::map<std::string, std::string> data);
        void GETenvInit(Client client, std::map<std::string, std::string> data);
        void POSTenvInit(Client client, std::map<std::string, std::string> data);
};
