/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gothmane <gothmane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 20:18:01 by hait-hsa          #+#    #+#             */
/*   Updated: 2024/03/24 22:13:41 by gothmane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#pragma once


#include <iostream>
#include <sys/poll.h>

class Client {
    private:
        pollfd event;
        std::string requestBuffer;
        std::string responseBuffer;
        std::string Postbuffer;
        int remainingBytes;
        int totalBytesSent;
        size_t responseBodyCounter;
    public:
        ~Client( void );
        Client( void );
        Client( pollfd ev);
        void clearBuffer( void );
        pollfd& getPollFd( void );
        void setPollfd(const pollfd pfd);
        int getFD( void );
        std::string getBuffer( void );
        void appendStr(char *chunk, size_t bytesSize) ;
        void changePollFd(pollfd me);
        // response
        void fillResponseBuffer(std::string response);
        std::string getResponseBuffer( void );
        void clearResponseBuffer( void );
        // response counters
            // remainingBytes
        void setRemainingBytes( size_t bytes );
        void unappendRemainingBytes( size_t bytes );
        void resetRemainingBytes( void );
        size_t getRemainingBytes( void );
            // totalBytesSent
        void appendTotalBytesSent( size_t bytes );
        void resetTotalBytesSent( void );
        size_t getTotalBytesSent( void );
        //
        std::string getPostBuffer( void );
        void appendPostBuffer( std::string );
        void clearPostBuffer( void );
        Client & operator=(const Client & other);
        // responseBody
        void initResponseBodyCounter( size_t count);
        void resetResponseBodyCounter( void );
        size_t getResponseBodyCounter( void );

        //clear all
        void clear( void );
        void erase( void );
};
