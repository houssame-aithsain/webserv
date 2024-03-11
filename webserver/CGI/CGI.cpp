/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hait-hsa <hait-hsa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/02 12:40:23 by hait-hsa          #+#    #+#             */
/*   Updated: 2024/03/09 16:56:58 by hait-hsa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CGI.hpp"
#include <fcntl.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

void CGI::executeCGIscript( Client & client ) {

    (void)client;
    int fd[2];

    if (pipe(fd) == -1) {
        perror("pipe");
        return;
    }

    const char *CGIpath = "/usr/bin/python3";
    char *const CGIargs[] = {const_cast<char *>("python3"), const_cast<char *>("/Users/hait-hsa/Desktop/0x0002/webserver/CGI/CGIscript/runCGI.py"), nullptr};

    pid_t psId = fork();

    if (psId == -1) {
        perror("fork");
        return;
    }
    if (psId == 0) {
        close(fd[0]);
        dup2(fd[1], STDOUT_FILENO);
        if (execve(CGIpath, CGIargs, env) == -1) {
            perror("execve");
            exit(0);
        }
    } else {
        close(fd[1]);
        waitpid(psId, nullptr, 0);
        char buffer[BUFFER_SIZE];
        std::string CGIoutput;
        ssize_t bytesRead;

        while ((bytesRead = read(fd[0], buffer, sizeof(buffer))) > 0) {
            CGIoutput += std::string(buffer, bytesRead);
        }
        if (bytesRead == -1) {
            perror("read");
            return;
        }
        close(fd[0]);
        // std::cout << "CGI: " << CGIoutput << std::endl;
        // client.resetRemainingBytes();
        // client.resetResponseBodyCounter();
        // client.resetTotalBytesSent();
        // client.clearBuffer();
        // client.clearResponseBuffer();
        // client.clearPostBuffer();
        // client.fillResponseBuffer(CGIoutput);
        // client.setRemainingBytes(CGIoutput.length());
    }
}

void CGI::EnvInit( void ) {

    static char *env[] = {
        const_cast<char *>("REQUEST_METHOD=GET"),
        const_cast<char *>("QUERY_STRING=param1=value1&param2=value2"),
        nullptr
    };
    this->env = env;
}

void CGI::extractClientContent(Client & client) {

    (void)client;
    EnvInit();
    executeCGIscript(client);
}