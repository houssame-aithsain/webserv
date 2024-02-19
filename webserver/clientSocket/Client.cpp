/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hait-hsa <hait-hsa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 20:18:04 by hait-hsa          #+#    #+#             */
/*   Updated: 2024/02/18 21:56:13 by hait-hsa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::~Client( void ) {}

Client::Client( void ) : bytesRead(0), remainingBytes(0) {}

Client::Client( pollfd ev) : event(ev), bytesRead(0), remainingBytes(0), totalBytesSend(0) {}

void Client::clearBuffer( void ) {requestBuffer.clear();}

pollfd Client::getPollFd( void ) {return (event);}

int Client::getFD( void ) {return (event.fd);}

std::string Client::getBuffer( void ) {return (requestBuffer);}

void Client::appendStr(char *chunk) {requestBuffer += chunk;}

void Client::changePollFd(pollfd me) {event = me;}

// bytes read

int Client::getBytesRead( void ) {return (bytesRead);}

void Client::appendBytesRead( int bytes) {bytesRead += bytes;}

void Client::resetBytesRead( void ) {bytesRead = 0;}

// bytes write

size_t Client::getBytesWrite( void ) {return (totalBytesSend);}

void Client::appendBytesWrite( size_t bytes) {totalBytesSend += bytes;}

void Client::resetBytesWrite( void ) {totalBytesSend = 0;}

// remaining bytes

size_t Client::getRemainingBytes( void ) {return (remainingBytes);}

void Client::appendRemainingBytes( size_t bytes) {remainingBytes = bytes;}

void Client::unappendRemainingBytes( size_t bytes) {remainingBytes -= bytes;}

void Client::resetRemainingBytes( void ) {remainingBytes = 0;}

// bytes to Send

std::string Client::getBytesToSend( void ) {return (HTTPrequest);}

void Client::setBytesToSend( std::string bytes ) {HTTPrequest = bytes;}

void Client::clearBytesToSend( void ) {HTTPrequest.clear();}