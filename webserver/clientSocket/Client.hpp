/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hait-hsa <hait-hsa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 20:18:01 by hait-hsa          #+#    #+#             */
/*   Updated: 2024/02/18 21:59:21 by hait-hsa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <sys/poll.h>

class Client {
    private:
        pollfd event;
        std::string requestBuffer;
        int bytesRead;
        size_t remainingBytes;
        size_t totalBytesSend;
        std::string HTTPrequest;
    public:
        ~Client( void );
        Client( void );
        Client( pollfd ev);
        void clearBuffer( void );
        pollfd getPollFd( void );
        int getFD( void );
        std::string getBuffer( void );
        void appendStr(char *chunk);
        void changePollFd(pollfd me);
        // read bytes
        int getBytesRead( void );
        void appendBytesRead( int bytes);
        void resetBytesRead( void );
        // write bytes
        size_t getBytesWrite( void );
        void appendBytesWrite( size_t bytes);
        void resetBytesWrite( void );
        // remaining Bytes
        size_t getRemainingBytes( void );
        void appendRemainingBytes( size_t bytes);
        void unappendRemainingBytes( size_t bytes);
        void resetRemainingBytes( void );
            // bytes to sent 
        std::string getBytesToSend( void );
        void setBytesToSend( std::string bytes );
        // bytes to sent
        // size_t getBytesSend( void );
        // void setBytesToSend( size_t bytes );
        void clearBytesToSend( void );
};
