/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gothmane <gothmane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/07 16:08:13 by gothmane          #+#    #+#             */
/*   Updated: 2024/03/26 21:37:57 by gothmane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.hpp"

Parser::Parser() {}

Parser::~Parser() {}

Parser::Parser(const Parser &p)
{
    (void) p;
}

Parser &Parser::operator=(const Parser &pr)
{
    (void) pr;
    // if (this != &pr)
    //     this->server = pr.server;
    return (*this);
}

std::string Parser::ft_trim(const std::string& str, std::string trim)
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
    if (value.size() >= 1)
    {
        if (ft_count_special_char(value, '\"') == 2 && ft_count_special_char(value, '\'') == 2)
        {
            if ((value[0] == '\"' && value[1] == '\'') || (value[0] == '\'' && value[1] == '\"'))
                open_br = 1;
            if (open_br == 1 && ((value[value.size() - 1] == '\"' && value[value.size() - 2] == '\'') 
                || (value[value.size() - 1] == '\'' && value[value.size() - 2] == '\"')))
                close_br = 1;
            if (open_br == 1 && close_br == 1)
                return (true);
        }
        else if (ft_count_special_char(value, '\"') == 2 && ft_count_special_char(value, '\'') >= 0)
        {
            if (value[0] == '\"')
                open_br = 1;
            if (open_br == 1 && value[value.size() - 1] == '\"')
                close_br = 1;
            if (open_br == 1 && close_br == 1)
                return (true);
        }
        else if (ft_count_special_char(value, '\"') >= 0 && ft_count_special_char(value, '\'') == 2)
        {
            if (value[0] == '\'')
                open_br = 1;
            if (open_br == 1 && value[value.size() - 1] == '\'')
                close_br = 1;
            if (open_br == 1 && close_br == 1)
                return (true);
        }
        else if (ft_count_special_char(value, '\"') == 0 && ft_count_special_char(value, '\'') == 0)
            return (true);
        else
            std::cout << "Error in your quotes, respect the format\n";
    }
    std::cout << value << std::endl;
    std::cout << value[0] << " " << value[value.size()] << "\n";
    std::cout << "Check your quotes please !\n";
    return (false);
}

std::vector<std::string>  Parser::ft_split_the_multiple_data(std::string value)
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
        
        if (value != "")
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
    return (result);
}

void Parser::ft_print_data()
{
    // Printing
    std::size_t r = 1;
    for (std::vector<server_data>::iterator it = wrapper.begin(); it != wrapper.end(); ++it, ++r)
    {
        std::cout << "Server data " << r << ":\n";

        std::cout << "\n\n\n#Server:\n";
        std::size_t j = 1;
        for (std::vector<std::pair<std::string, std::vector<std::string> > >::iterator jt = it->server.begin(); jt != it->server.end(); ++jt, ++j)
        {
            std::string key = jt->first;
            std::vector<std::string> values = jt->second;

            if (key == "port")
            {
                this->port = std::atoi(ft_trim(values[0], "\'\"").c_str());
                std::cout << "Port: " << ft_trim(values[0], "\'\"") << std::endl;
            }
            // std::cout << "Key: " << key << ", Values: ";
            // for (std::size_t k = 0; k < values.size(); ++k) {
            //     std::cout << values[k] << " ";
            // }
        }

        std::cout << "\n\n\n#Location:\n";
        std::size_t k = 1;
        for (std::vector<std::vector<std::pair<std::string, std::vector<std::string> > > >::iterator kt = it->location.begin(); kt != it->location.end(); ++kt, ++k)
        {
            std::cout << "Location Number : " << k << std::endl;
            std::size_t l = 1;
            for (std::vector<std::pair<std::string, std::vector<std::string> > >::iterator lt = kt->begin(); lt != kt->end(); ++lt, ++l)
            {
                std::string key = lt->first;
                std::vector<std::string> values = lt->second;

                std::cout << "Key: " << key << ", Values: ";
                for (std::size_t m = 0; m < values.size(); ++m)
                {
                    std::cout << values[m] << " ";
                }
                std::cout << std::endl;
            }
        }
    }
}


void Parser::ft_read_nd_parse(std::string fileName)
{
    std::ifstream in(fileName);
    std::string line;
    int server_side = 0;
    int location_side = 0;
    int server_id = 0;
    // int i = 0;
    size_t j = -1;

    if (in.is_open())
    {
        while (std::getline(in, line))
        {
            if (ft_trim(line, " \"\t") == "[[server]]")
            {
                // std::cout << "SERRRRRVER \n";
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
                if (!ft_check_quotes_for_single_data(value.back()))
                    exit(1);
                data.server.push_back(std::make_pair(key, value));
            }
            else if (location_side > 0)
            {
                std::string key = ft_trim(line.substr(0, line.find("=")), " \"\t");
                if (key == "")
                    continue;
                std::vector<std::string> value;
                value = ft_split_the_multiple_data(ft_trim(line.substr(line.find("=") + 1, line.size()), " \t"));
                if (!ft_check_quotes_for_single_data(value.back()))
                    exit(1);
                if (j >= data.location.size()) {
                    data.location.resize(j+1);
                }
                if (key == "prefix" && value[0][value[0].size() - 1] != '/' )
                {
                    value[0].append("/");
                    // std::cout << "APPENDED >> " <<  value[0] << "\n";
                }
                data.location.at(j).push_back(std::make_pair(key, value));
            }
        }
        if (server_side > 0)
            wrapper.push_back(data);
    }
    else
        std::cout << "Unable to open file\n";

    check_for_default_location();
}

std::vector<server_data> Parser::getWrapper( void ) {return (wrapper);}

void    Parser::check_for_default_location(void)
{
    size_t i = 0;
    int checker = 0;
    std::vector<std::pair<std::string, std::vector<std::string> > >  location;

    for (; i < this->wrapper.size(); i++)
    {
        for (size_t l = 0; l < wrapper[i].location.size(); l++)
        {
            size_t s = 0;
            for (; s < wrapper[i].location[l].size(); s++)
            {
                if (wrapper[i].location[l][s].first == "prefix" 
                    && ft_trim(wrapper[i].location[l][s].second[0], " \"\'\t") == "/")
                {
                    
                    checker = 1;
                    break;
                }
            }
        }
        if (checker == 0)
        {
            std::vector<std::string> v;

            v.push_back("/");
            location.push_back(std::make_pair("prefix", v));
            std::cout << "PUSHED default\n";
            wrapper[i].location.push_back(location);
        }
    }
}

std::vector<std::string> Parser::get_data_from_conf(std::string &port, std::string prefix, std::string key, int bv)
{
    size_t i = 0;
    size_t k = 0; 
    size_t checker = 0;
    std::vector<std::string> default_value;
    std::vector<std::string> empty_for_empty;
    

    if (prefix[prefix.size() - 1] != '/')
        prefix.append("/");

    // std::cout << "in parsing config data\n";
    // std::cout << "the port >> " << port << "\n";
    // std::cout << "the prefix >> " << prefix << "\n";
    // std::cout << "the key >> " << key << "\n";
    for (; i < wrapper.size(); i++)
    {
        for (size_t j = 0; j < wrapper[i].server.size(); j++)
        {
            checker = 0;
            if (ft_trim(wrapper[i].server[j].first, " \"\'\t") == "port" 
                && ft_trim(wrapper[i].server[j].second[0], " \"\'\t") == port)
            {
                k = j;
                checker = 1;
                for (size_t e = 0; e < wrapper[i].server.size(); e++)
                    if (ft_trim(wrapper[i].server[e].first, " \"\'\t") == ft_trim(key, " \"\'\t"))
                        default_value = wrapper[i].server[e].second;
                break;
            }
        }
        if (!bv && !default_value.empty())
        {
            return (default_value);
        }
        if (checker == 1 && bv)
        {

            for (size_t l = 0; l < wrapper[i].location.size(); l++)
            {
                size_t s = 0;
                for (; s < wrapper[i].location[l].size(); s++)
                {
                
                    if (wrapper[i].location[l][s].first == "prefix" 
                        && ft_trim(wrapper[i].location[l][s].second[0], " \"\'\t") == prefix)
                    {
                        
                        for (size_t k = 0; k < wrapper[i].location[l].size(); k++)
                        {
                        
                           
                            if (wrapper[i].location[l][k].first == key)
                            {
                                // std::cout << "THE KEY >> => " << ft_trim(wrapper[i].location[l][k].second[0], " \"\'\t") << std::endl;
                                // std::cout << "THE PREFIX >> => " << prefix << std::endl;
                                // std::cout << "THE KEY LOOKING FOR >> => " << key << std::endl;
                                // std::cout << "THE INDEX FOR LOCATION >> => " << l << std::endl;
                                // std::cout << "THE key FOR LOCATION >> => " << wrapper[i].location[l][k].first << std::endl;
                                return (wrapper[i].location[l][k].second);
                            }
                        }
                        break;
                    }

                }

            }
                           
                if (!default_value.empty())
                {
                    // std::cout << ft_trim(default_value[0], " \"\'\t") << std::endl;
                    return (default_value);
                }
        }

    }
    return (empty_for_empty);
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


std::vector<std::string> Parser::get_server_locations(std::string &port)
{
    size_t i = 0;
    size_t k = 0;
    int checker = 0;
    std::vector<std::string> default_value;
    std::vector<std::string> prefixes;
    
    for (; i < wrapper.size(); i++)
    {
        for (size_t j = 0; j < wrapper[i].server.size(); j++)
        {
            checker = 0;
            if (wrapper[i].server[j].first == "port" && ft_trim(wrapper[i].server[j].second[0], " \"\'\t") == port)
            {
                k = j;
                checker = 1;
                break;
            }
        }
        if (checker == 1)
        {
            for (size_t l = 0; l < wrapper[i].location.size(); l++)
            {
                size_t s = 0;
                for (; s < wrapper[i].location[l].size(); s++)
                {
                    if (wrapper[i].location[l][s].first == "prefix" )
                    {
                        prefixes.push_back(wrapper[i].location[l][s].second[0]);
                        break;
                    }
                }
            }
        }
    }
    return (prefixes);
}


// void Parser::replace_prefix_with_alias(void)
// {
//     size_t i = 0;
//     std::vector<std::string> default_value;
    
//     for (; i < wrapper.size(); i++)
//     {
//         for (size_t j = 0; j < wrapper[i].server.size(); j++)
//         {
//             for (size_t l = 0; l < wrapper[i].location.size(); l++)
//             {
//                 size_t s = 0;
//                 for (; s < wrapper[i].location[l].size(); s++)
//                 {
//                     if (wrapper[i].location[l][s].first == "alias" )
//                     {
//                         for (size_t k = 0; k < wrapper[i].location[l].size(); k++)
//                         {
//                             if (wrapper[i].location[l][k].first == "prefix")
//                             {
//                                 if (wrapper[i].location[l][s].second[0][wrapper[i].location[l][s].second[0].size() - 1] != '/')
//                                     wrapper[i].location[l][s].second[0].append("/");
//                                 wrapper[i].location[l][k].second =  wrapper[i].location[l][s].second;
//                             }
//                         }
//                         break;
//                     }
//                 }
//             }
//         }
//     }
// }
