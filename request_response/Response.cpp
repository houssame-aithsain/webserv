/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gothmane <gothmane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 18:36:05 by gothmane          #+#    #+#             */
/*   Updated: 2024/02/11 20:56:32 by gothmane         ###   ########.fr       */
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

std::string ft_trim(const std::string& str, std::string trim)
{
    size_t first = str.find_first_not_of(trim);
    if (std::string::npos == first)
        return str;
    size_t last = str.find_last_not_of(trim);
    return str.substr(first, (last - first + 1));
}


void Response::ft_parse_request(std::string request) 
{
    std::istringstream getrequestStream(request);
    std::string getrequestLine;
    int index = 0;
    while (std::getline(getrequestStream, getrequestLine)) 
    {
        if (index == 0)
        {
            //get method
            std::vector<std::string>  new_v;
            int next = getrequestLine.find(" ");
            new_v.push_back(getrequestLine.substr(0, next));
            this->request_data["Method"] = new_v[0];

            //get assets
            new_v.clear();
            int new_next = getrequestLine.find(" " , next + 1);
            new_v.push_back(getrequestLine.substr((next + 1), (new_next - (next + 1))));
            this->request_data["Asset"] = new_v[0];
            std::cout << this->request_data["Asset"] << std::endl;
            //get type
            new_v.clear();
            new_v.push_back(getrequestLine.substr(new_next + 1, ((getrequestLine.size() - 2) - new_next)));
            this->request_data["Type"] = new_v[0];
            index++;
            // request_data[0].first = "Method";
            // request_data[1].second[0] = getrequestLine.substr(0, getrequestLine.find(" "));
        }
        else
        {
            int i1 = getrequestLine.find(":");
            std::string key = getrequestLine.substr(0, i1);
            std::string value = getrequestLine.substr((i1 + 1), ((getrequestLine.size() - 2) - i1));

            if (key != "")
            {
                std::vector<std::string>  new_v;

                new_v.push_back(ft_trim(value, " \t\n\""));
                this->request_data[key] = new_v[0];
            }
            // std::cout << "The key => " << key << std::endl;
            // std::cout << "The value => " << value << std::endl;
        }
        // std::cout << getrequestLine << std::endl;
    }


}