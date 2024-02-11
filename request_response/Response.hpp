/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gothmane <gothmane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 18:36:03 by gothmane          #+#    #+#             */
/*   Updated: 2024/02/11 20:55:57 by gothmane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <vector>
#include <iterator>
#include <utility> // for std::pair
#include <map>
#include <sstream>
#include <string>

class Response
{
    public:
        std::map<std::string, std::string> request_data;
        Response();
        ~Response();
        std::string ft_getPageToRender(std::vector<std::pair<std::string, std::vector<std::string> > > ParsedRequest);
        void ft_parse_request(std::string request);
};