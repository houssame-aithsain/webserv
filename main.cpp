/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hait-hsa <hait-hsa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/07 17:35:43 by gothmane          #+#    #+#             */
/*   Updated: 2024/01/29 22:29:29 by hait-hsa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing/parser.hpp"
#include "server/Server.hpp"

int main()
{
    Parser p;
    Server s;

    p.ft_read_nd_parse("confg.toml");
    s.initializeSocket(p.getWrapper());
}
