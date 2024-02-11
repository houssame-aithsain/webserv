/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gothmane <gothmane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 18:36:03 by gothmane          #+#    #+#             */
/*   Updated: 2024/02/11 18:54:10 by gothmane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include<vector>
#include <iterator>

class Response
{
    public:
        Response();
        ~Response();
        std::string ft_getPageToRender(std::vector<std::pair<std::string, std::vector<std::string> > > ParsedRequest);
};