/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gothmane <gothmane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 18:36:05 by gothmane          #+#    #+#             */
/*   Updated: 2024/02/11 18:57:56 by gothmane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response()
{

}

Response::~Response()
{
    
}

std::string Response::ft_getPageToRender(std::vector<std::pair<std::string, std::vector<std::string> > > ParsedRequest)
{
    for (std::vector<std::pair<std::string, std::vector<std::string> > >::iterator it = ParsedRequest.begin(); it != ParsedRequest.end(); ++it)
    {
        std::vector<std::string> requestDataParsed = it->second;
        if (it->first == "Asset")
        {
            std::cout << requestDataParsed.at(0) << std::endl;
            return (requestDataParsed.at(0));
        }
    }
    std::cout << "Error: There's no data in here (Asset data)\n";
    return ("");
}