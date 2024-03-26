/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gothmane <gothmane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/02 12:40:23 by hait-hsa          #+#    #+#             */
/*   Updated: 2024/03/25 02:13:23 by gothmane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CGI.hpp"
#include <fcntl.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <sstream>

#define BUFFER_SIZE 1024

void CGI::executeCGIscript(Client &client) {

    ssize_t max;
    int status;
    char buffer[BUFFER_SIZE + 1];
    char cmd[] = "/usr/bin/python3";
    char *const args[] = {const_cast<char *>("python3"), const_cast<char *>("/Users/gothmane/Desktop/second_v/webserver/CGI/CGIscript/runCGI.py"), nullptr};
    FILE *parentTmpFile = tmpfile();
    FILE *chiledTmpFile = tmpfile();
    int parentFD = fileno(parentTmpFile);
    int chiledFD = fileno(chiledTmpFile);
    write(parentFD, client.getPostBuffer().c_str(), client.getPostBuffer().size());
    fflush(parentTmpFile);
    rewind(parentTmpFile);
    int psid = fork();
    if (!psid) {
        dup2(parentFD, STDIN_FILENO);
        dup2(chiledFD, STDOUT_FILENO);
        execve(cmd, args, env.data());
    } else {
        waitpid(psid, &status, 0);
        bzero(buffer, BUFFER_SIZE);
        fflush(chiledTmpFile);
        rewind(chiledTmpFile);
        while ((max = read(chiledFD, buffer, BUFFER_SIZE))) {
            if (max == -1) {
                perror("read()");
                break;
            }
            buffer[max] = '\0';
            output += buffer;
            bzero(buffer, BUFFER_SIZE);
        }
        close(chiledFD);
        std::cout << "CGI OUTPUT [" << output << "]" << std::endl;
        client.resetTotalBytesSent();
        client.fillResponseBuffer(output);
        client.setRemainingBytes(output.length());
    }
}

void CGI::GETenvInit(Client client, std::map<std::string, std::string> data) {
    (void)client;
    // std::cout << "-+-+-+-+-+-+-+-+-+-+-+-+-PRINT REQUEST DATA+-+-+-+-+-+-+-+-+-+-+-+-" << std::endl;
    // for (std::map<std::string, std::string>::iterator it = data.begin(); it != data.end(); it++)
    //     std::cout << it->first << " <:> " << it->second << std::endl;
    // std::cout << "-+-+-+-+-+-+-+-+-+-+-+-+-PRINT REQUEST DATA END+-+-+-+-+-+-+-+-+-+-+-+-" << std::endl;
    REQUEST_METHOD = "REQUEST_METHOD=GET";
    CONTENT_TYPE = "CONTENT_TYPE=" + data["Content-Type"];
    QUERY_STRING = "QUERY_STRING=" + data["QUERY_STRING"];
    std::cout << QUERY_STRING << std::endl;
    CONTENT_TYPE.erase(CONTENT_TYPE.end() - 1);
    env.push_back((char *)REQUEST_METHOD.c_str());
    env.push_back((char *)QUERY_STRING.c_str());
    env.push_back((char *)CONTENT_TYPE.c_str());
    env.push_back(nullptr);
}

void CGI::POSTenvInit(Client client, std::map<std::string, std::string> data) {

    std::string lent;
    std::stringstream ss;

    ss << client.getResponseBodyCounter();
    lent = ss.str();
    CONTENT_LENGTH = "CONTENT_LENGTH=" + lent;
    REQUEST_METHOD = "REQUEST_METHOD=POST";
    CONTENT_TYPE = "CONTENT_TYPE=" + data["Content-Type"];
    CONTENT_TYPE.erase(CONTENT_TYPE.end() - 1);
    env.push_back((char *)REQUEST_METHOD.c_str());
    env.push_back((char *)CONTENT_LENGTH.c_str());
    env.push_back((char *)CONTENT_TYPE.c_str());
    env.push_back(nullptr);
}

void CGI::extractClientContent(Client & client, std::map<std::string, std::string> data) 
{
    if (data["Method"] == "GET") {
        GETenvInit(client, data);
    }
    else if (data["Method"] == "POST") {
        POSTenvInit(client, data);
    }
    executeCGIscript(client);
}
