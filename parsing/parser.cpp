/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gothmane <gothmane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/07 16:08:13 by gothmane          #+#    #+#             */
/*   Updated: 2024/01/08 19:37:24 by gothmane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.hpp"

Parser::Parser() {}

Parser::~Parser() {}

Parser::Parser(const Parser &p)
{
}

Parser &Parser::operator=(const Parser &pr)
{
    // if (this != &pr)
    //     this->server = pr.server;
    return (*this);
}

std::string ft_trim(const std::string& str, std::string trim)
{
    size_t first = str.find_first_not_of(trim);
    if (std::string::npos == first)
        return str;
    size_t last = str.find_last_not_of(trim);
    return str.substr(first, (last - first + 1));
}

int ft_count_special_char(std::string line, char car)
{
    size_t i = 0;
    int    counter = 0;

    for (; i < line.size(); i++)
    {
        if (line[i] == car)
            counter++;
    }
    return (counter);
}

bool ft_check_quotes_for_single_data(std::string value)
{
    int open_br = 0;
    int close_br = 0;
    
    if (value == "")
    {
        std::cout << "No value found !\n";
        return (false);
    }
    if (value.size() > 1)
    {
        if (ft_count_special_char(value, '\"') == 2 && ft_count_special_char(value, '\'') == 2)
        {
            if ((value[0] == '\"' && value[1] == '\'') || (value[0] == '\'' && value[1] == '\"'))
                open_br = 1;
            if (open_br == 1 && (value[value.size() - 1] == '\"' && value[value.size() - 1] == '\'') 
                || (value[value.size() - 1] == '\'' && value[value.size() - 1] == '\"'))
                close_br = 1;
            if (open_br == 1 && close_br == 1)
                return (true);
        }
        else 
        

    }
    std::cout << value << std::endl;
    std::cout << value[0] << " " << value[value.size()] << "\n";
    std::cout << "Check your quotes please !\n";
    return (false);
}

std::vector<std::string>  ft_split_the_multiple_data(std::string value)
{
    int open_brackets = 0;
    int close_brackets = 0;
    int open_dq = 0;
    int close_dq = 0;
    std::stringstream ss(value);
    // check begin and end format
    size_t i = 0;
    if (value[i] == '[' && value[i + 1] && value[i + 1] == '\"')
    {
        open_brackets = 1;
        open_dq = 1;
    }
    i = value.size();
    if (value[i] == '[' && value[i - 1] && value[i - 1] == '\"')
    {
        close_brackets = 1;
        close_dq = 1;
    }
    if ((open_brackets == 1 && open_dq != 1) || (close_brackets == 1 && close_dq != 1))
    {
        std::cout << "OB = " << open_brackets << " OD = " << open_dq << " CB = " << close_brackets << " close_dq = " << close_dq << "\n";
       std::cout << "Error in [\"\"]\n";
    }
    else if (open_brackets == 0 && close_brackets == 0)
    {
        std::vector<std::string> v ;
        
        if (ft_check_quotes_for_single_data(value))
            v.push_back(value);
        else
            exit(1); // throw exception of brackets
        return (v);
    }
    
    //remove brackets
    std::string no_brackets_value = value.substr(1, value.size() - 2); 
    std::string item;
    std::vector<std::string> result;
    while (std::getline(ss, item, ','))
    {
        item = item.substr(1, item.size() - 2);
        result.push_back(ft_trim(item, " \"\t"));
    }
    // for (size_t i = 0; i < result.size(); i++)
    // {
    //     std::cout << result[i] << std::endl;
    // }
    return (result);
}

void Parser::ft_read_nd_parse(std::string fileName)
{
    std::ifstream in(fileName);
    std::string line;
    int server_side = 0;
    int location_side = 0;
    int server_id = 0;
    int i = 0;
    int j = -1;

    if (in.is_open())
    {
        while (std::getline(in, line))
        {
            if (ft_trim(line, " \"\t") == "[[server]]")
            {
                if (server_side > 0)
                {
                    wrapper.push_back(data);
                    data.server.clear();
                    data.location.clear();                
                }
                server_side++;
                location_side = 0;
                j = -1;
                continue;
            }
            else if (ft_trim(line, " \"\t") == "[[server.location]]")
            {
                server_id = server_side;
                location_side++;
                j++;
                continue;
            }
            if (server_id != server_side && location_side == 0)
            {
                if (line.find("=") == std::string::npos)
                    continue;
                std::string key = ft_trim((line.substr(0, line.find("="))), " \"\t");
                if (key == "")
                    continue;
                std::vector<std::string> value;
                value = ft_split_the_multiple_data(ft_trim(line.substr(line.find("=") + 1, line.size()), " \t"));
                data.server.push_back(std::make_pair(key, value));
            }
            else if (location_side > 0)
            {
                std::string key = ft_trim(line.substr(0, line.find("=")), " \"\t");
                if (key == "")
                    continue;
                std::vector<std::string> value;
                value = ft_split_the_multiple_data(ft_trim(line.substr(line.find("=") + 1, line.size()), " \t"));
                if (j >= data.location.size()) {
                    data.location.resize(j+1);
                }
                data.location.at(j).push_back(std::make_pair(key, value));
            }
        }
        if (server_side > 0)
            wrapper.push_back(data);
    }

    // Printing
std::size_t r = 1;
for (std::vector<server_data>::iterator it = wrapper.begin(); it != wrapper.end(); ++it, ++r) {
    std::cout << "Server data " << i << ":\n";
    
    std::cout << "#Server:\n";
    std::size_t j = 1;
    for (std::vector<std::pair<std::string, std::vector<std::string> > >::iterator jt = it->server.begin(); jt != it->server.end(); ++jt, ++j) {
        std::string key = jt->first;
        std::vector<std::string> values = jt->second;

        std::cout << "Key: " << key << ", Values: ";
        for (std::size_t k = 0; k < values.size(); ++k) {
            std::cout << values[k] << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "#Location:\n";
    std::size_t k = 1;
    for (std::vector<std::vector<std::pair<std::string, std::vector<std::string> > > >::iterator kt = it->location.begin(); kt != it->location.end(); ++kt, ++k) {
        std::cout << "Location Number : " << k << std::endl;
        std::size_t l = 1;
        for (std::vector<std::pair<std::string, std::vector<std::string> > >::iterator lt = kt->begin(); lt != kt->end(); ++lt, ++l) {
            std::string key = lt->first;
            std::vector<std::string> values = lt->second;

            std::cout << "Key: " << key << ", Values: ";
            for (std::size_t m = 0; m < values.size(); ++m) {
                std::cout << values[m] << " ";
            }
            std::cout << std::endl;
        }
    }
}
}



// typedef std::vector <server_data> serv_data;
// typedef std::vector<std::pair<std::string, std::vector<std::string> > > s1;
// typedef std::vector<std::vector<std::pair<std::string, std::vector<std::string> > > > l1;

// void    Parser::ft_parse_data_step2()
// {
//     serv_data::iterator it;
//     s1::iterator it_s1;
//     l1::iterator it_l1;
    
//     for (it = wrapper.begin(); it != wrapper.end(); ++it)
//     {
//         for (it_s1 = it->server.begin(); it_s1 != it->server.end(); ++it_s1)
//         {
//             std::string key = ft_trim(it_s1->first);
//             std::vector<std::string> values = it_s1->second;
            
//         }
//     }
// }