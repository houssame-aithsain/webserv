/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hait-hsa <hait-hsa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 20:18:04 by hait-hsa          #+#    #+#             */
/*   Updated: 2024/02/22 23:57:05 by hait-hsa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include <unistd.h>
#include <stdio.h>

Client::~Client( void ) {
    std::cout << "client killed!!" << std::endl;
    // usleep(100000);
    // exit(0);
}

Client::Client( void ) : remainingBytes(0), totalBytesSent(0) {}

Client::Client( pollfd ev) : event(ev), remainingBytes(0), totalBytesSent(0) {}

void Client::clearBuffer( void ) {requestBuffer.clear();}

pollfd& Client::getPollFd( void ) {return (event);}

int Client::getFD( void ) {return (event.fd);}

// requestbuffer

std::string Client::getBuffer( void ) {return (requestBuffer);}

void Client::appendStr(char *chunk) {requestBuffer += chunk;}

//

void Client::changePollFd(pollfd me) {event = me;}

// response

void Client::fillResponseBuffer(std::string response) {responseBuffer = response;}

std::string Client::getResponseBuffer( void ) {return (responseBuffer);}

void Client::clearResponseBuffer( void ) {responseBuffer.clear();}

// response counters

void Client::setRemainingBytes( size_t bytes ) {remainingBytes = bytes;}

void Client::unappendRemainingBytes( size_t bytes ) {remainingBytes -= bytes;}

void Client::resetRemainingBytes( void ) {remainingBytes = 0;}

size_t Client::getRemainingBytes( void ) {return (remainingBytes);}

// totalBytesSent

void Client::appendTotalBytesSent( size_t bytes ) {totalBytesSent += bytes;}

void Client::resetTotalBytesSent( void ) {totalBytesSent = 0;}

size_t Client::getTotalBytesSent( void ) {return (totalBytesSent);}

Client & Client::operator=(const Client & other) {
    
    event = other.event;
    requestBuffer = other.requestBuffer;
    responseBuffer = other.responseBuffer;
    remainingBytes = other.remainingBytes;
    totalBytesSent = other.totalBytesSent;
    return (*this);
}

void Client::setPollfd(const pollfd pfd) {event = pfd;}

std::string Client::getPostBuffer( void ) {return (Postbuffer);}

void Client::appendPostBuffer( std::string pb ) {Postbuffer += pb;}

void Client::clearPostBuffer( void ) {Postbuffer.clear();}
