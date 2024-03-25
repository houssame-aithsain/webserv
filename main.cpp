/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gothmane <gothmane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/17 15:53:27 by gothmane          #+#    #+#             */
/*   Updated: 2024/03/18 15:54:25 by gothmane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing/parser.hpp"
#include "webserver/server/Server.hpp"

int main()
{
    Parser p;
    Server s;

    try {
        p.ft_read_nd_parse("confg.toml");
        s.initializeSocket(p.wrapper);
        s.runServer(p);
    } catch (const char * error) {
        std::cout << error << std::endl;
    } catch (std::exception &e) {
        std::cout << "WEBSERV ERROR >> " << e.what()  << std::endl;
    }
}
