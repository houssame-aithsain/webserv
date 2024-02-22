/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hait-hsa <hait-hsa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 20:18:01 by hait-hsa          #+#    #+#             */
/*   Updated: 2024/02/22 23:55:35 by hait-hsa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
    public:
        ~Client( void );
        Client( void );
        Client( pollfd ev);
        void clearBuffer( void );
        pollfd& getPollFd( void );
        void setPollfd(const pollfd pfd);
        int getFD( void );
        std::string getBuffer( void );
        void appendStr(char *chunk);
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
};
