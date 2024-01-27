/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gothmane <gothmane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/07 17:35:43 by gothmane          #+#    #+#             */
/*   Updated: 2024/01/07 17:38:30 by gothmane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.hpp"

int main()
{
    Parser p;

    p.ft_read_nd_parse("confg.toml");
    std::cout << "PORT IN HERE BRO: " << p.getPort() << "\n";
}