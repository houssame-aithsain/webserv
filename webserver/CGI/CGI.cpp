/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gothmane <gothmane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/02 12:40:23 by hait-hsa          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2024/03/25 02:13:23 by gothmane         ###   ########.fr       */
=======
/*   Updated: 2024/03/25 19:30:47 by hait-hsa         ###   ########.fr       */
>>>>>>> ed030b7b4a37f89b9dcbf00fe7c22b92e2448571
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
<<<<<<< HEAD

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
=======
    ssize_t max;
    int status;
    char buffer[BUFFER_SIZE + 1];
    char cmd[] = "/usr/bin/python3";
    char *const args[] = {cmd, const_cast<char *>("/Users/hait-hsa/Desktop/second_v/webserver/CGI/CGIscript/runCGI.py"), nullptr};
    FILE *parentTmpFile = tmpfile();
    FILE *childTmpFile = tmpfile();

    if (!parentTmpFile || !childTmpFile) {
        perror("tmpfile");
        return;
    }

    int parentFD = fileno(parentTmpFile);
    int childFD = fileno(childTmpFile);

    if (parentFD == -1 || childFD == -1) {
        perror("fileno");
        if (parentTmpFile) fclose(parentTmpFile);
        if (childTmpFile) fclose(childTmpFile);
        return;
    }
    write(parentFD, client.getPostBuffer().c_str(), client.getPostBuffer().size());
    fflush(parentTmpFile);
    rewind(parentTmpFile);
    int pid = fork();
    if (pid == 0) {
        dup2(parentFD, STDIN_FILENO);
        dup2(childFD, STDOUT_FILENO);
        fclose(parentTmpFile);
        fclose(childTmpFile);
        execve(cmd, args, env.data());
        perror("execve");
    } else if (pid > 0) {
        fclose(parentTmpFile);
        waitpid(pid, &status, 0);
        rewind(childTmpFile);
        std::string output;
        while ((max = read(childFD, buffer, BUFFER_SIZE)) > 0) {
            buffer[max] = '\0';
            output += buffer;
        }
        if (max == -1) {
            perror("read");
        }
        fclose(childTmpFile);
>>>>>>> ed030b7b4a37f89b9dcbf00fe7c22b92e2448571
        std::cout << "CGI OUTPUT [" << output << "]" << std::endl;
        client.resetTotalBytesSent();
        client.fillResponseBuffer(output);
        client.setRemainingBytes(output.length());
<<<<<<< HEAD
=======
    } else {
        perror("fork");
        fclose(parentTmpFile);
        fclose(childTmpFile);
>>>>>>> ed030b7b4a37f89b9dcbf00fe7c22b92e2448571
    }
}

void CGI::GETenvInit(Client client, std::map<std::string, std::string> data) {
<<<<<<< HEAD
    (void)client;
    // std::cout << "-+-+-+-+-+-+-+-+-+-+-+-+-PRINT REQUEST DATA+-+-+-+-+-+-+-+-+-+-+-+-" << std::endl;
    // for (std::map<std::string, std::string>::iterator it = data.begin(); it != data.end(); it++)
    //     std::cout << it->first << " <:> " << it->second << std::endl;
    // std::cout << "-+-+-+-+-+-+-+-+-+-+-+-+-PRINT REQUEST DATA END+-+-+-+-+-+-+-+-+-+-+-+-" << std::endl;
=======

    (void)client;
>>>>>>> ed030b7b4a37f89b9dcbf00fe7c22b92e2448571
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
<<<<<<< HEAD
=======
    // std::cout << "-+-+-+-+-+-+-+-+-+-+-+-+-PRINT REQUEST DATA+-+-+-+-+-+-+-+-+-+-+-+-" << std::endl;
    // for (std::map<std::string, std::string>::iterator it = data.begin(); it != data.end(); it++)
    //     std::cout << it->first << " ::::::: " << it->second << std::endl;
    // std::cout << "-+-+-+-+-+-+-+-+-+-+-+-+-PRINT REQUEST DATA END+-+-+-+-+-+-+-+-+-+-+-+-" << std::endl;
>>>>>>> ed030b7b4a37f89b9dcbf00fe7c22b92e2448571
    if (data["Method"] == "GET") {
        GETenvInit(client, data);
    }
    else if (data["Method"] == "POST") {
        POSTenvInit(client, data);
    }
    executeCGIscript(client);
}
