/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hait-hsa <hait-hsa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 20:18:04 by hait-hsa          #+#    #+#             */
/*   Updated: 2024/02/16 22:21:58 by hait-hsa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::~Client( void ) {}

Client::Client( void ) {}

Client::Client( pollfd ev) : event(ev) {}

void Client::clearBuffer( void ) {requestBuffer.clear();}

pollfd Client::getPollFd( void ) {return (event);}

int Client::getFD( void ) {return (event.fd);}

std::string Client::getBuffer( void ) {return (requestBuffer);}

void Client::appendStr(char *chunk) {requestBuffer += chunk;}

void Client::changePollFd(pollfd me) {event = me;}
