/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gothmane <gothmane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 20:18:04 by hait-hsa          #+#    #+#             */
/*   Updated: 2024/03/24 22:13:36 by gothmane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "Client.hpp"
#include <unistd.h>
#include <stdio.h>

Client::~Client( void ) {}

Client::Client( void ) : remainingBytes(0), totalBytesSent(0), responseBodyCounter(0) {}

Client::Client( pollfd ev) : event(ev), remainingBytes(0), totalBytesSent(0), responseBodyCounter(0) {}

void Client::clearBuffer( void ) {requestBuffer.clear();}

pollfd& Client::getPollFd( void ) {return (event);}

int Client::getFD( void ) {return (event.fd);}

// requestbuffer

std::string Client::getBuffer( void ) {return (requestBuffer);}

void Client::appendStr(char *chunk, size_t bytesSize) {requestBuffer.append(chunk, bytesSize);}
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

// response counter

void Client::initResponseBodyCounter( size_t count ) {responseBodyCounter = count;}

void Client::resetResponseBodyCounter( void ) {responseBodyCounter = 0;}

size_t Client::getResponseBodyCounter( void ) {return (responseBodyCounter);}

void Client::clear( void ) {

    close(event.fd);
    requestBuffer.clear();
    responseBuffer.clear();
    Postbuffer.clear();
    remainingBytes = 0;
    totalBytesSent = 0;
    responseBodyCounter = 0;
}
