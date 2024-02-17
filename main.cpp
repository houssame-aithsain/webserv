/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hait-hsa <hait-hsa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/07 17:35:43 by gothmane          #+#    #+#             */
/*   Updated: 2024/02/17 17:09:56 by hait-hsa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing/parser.hpp"
#include "server/Server.hpp"

int main()
{
    Parser p;
    Server s;

    try {
        p.ft_read_nd_parse("confg.toml");
        s.initializeSocket(p.wrapper);
        s.runServer();
    } catch (const char * error) {
        std::cout << error << std::endl;
    } catch (...) {
        std::cout << "WEBSERV ERROR" << std::endl;
    }
}
