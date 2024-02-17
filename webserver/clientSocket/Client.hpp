/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hait-hsa <hait-hsa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 20:18:01 by hait-hsa          #+#    #+#             */
/*   Updated: 2024/02/16 23:06:38 by hait-hsa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <sys/poll.h>

class Client {
    private:
        pollfd event;
        std::string requestBuffer;
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
};
