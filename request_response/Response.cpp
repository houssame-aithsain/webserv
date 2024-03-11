/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hait-hsa <hait-hsa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 18:36:05 by gothmane          #+#    #+#             */
/*   Updated: 2024/03/09 17:39:02 by hait-hsa         ###   ########.fr       */
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

int ft_check_if_hexa_line(std::string line)
{
    size_t pos = line.find("\r");

    if (pos == std::string::npos)
        return (-1);
    else
        return (1);
}

int hexToDecimal(const std::string& hexString) {
    return std::stoi(hexString, nullptr, 16);
}

void Response::ft_parse_request(std::string request) 
{
    std::istringstream getrequestStream(request);
    std::string getrequestLine;
    int index = 0;
    int checker_body = 0;
    std::string key;
    std::string value;
    
    while (std::getline(getrequestStream, getrequestLine, '\n')) 
    { 
        std::istringstream lines(getrequestLine);
        if (index++ == 0)
            lines >> this->request_data["Method"] >> this->request_data["Asset"] >> this->request_data["Type"];
        else if (getrequestLine != "\r")
        {
            lines >> key;
            if (key.length() > 0)
            {
                key = key.substr(0, key.length() - 1);
                this->request_data[key] = ft_trim(getrequestLine.substr(key.length() + 2, getrequestLine.length()), " \t\n\"");
            }
        }
        else
        {
            checker_body = 1;
            break ;
        }
    }
    // POST PARSING REQUEST CHUNCKED
    // if (checker_body == 1)
    // {
    //     std::string content((std::istreambuf_iterator<char>(getrequestStream)), std::istreambuf_iterator<char>());
    //     std::cout << "######## Start of the Body ####### \n";
    //     if (this->request_data["Method"] == "POST")
    //     {
    //         std::stringstream file_name(content);
    //         std::string line;
    //         char buf[1024];
    //         if (this->request_data["Transfer-encoding"] == "chunked")
    //         {
    //             getline(file_name, line);
    //             int nbr = hexToDecimal(line);

    //             while (nbr != 0)
    //             {
    //                 bzero(buf,1024);
    //                 file_name.read(buf, nbr);
    //                 file_name.get();
    //                 file_name.get();
    //                 getline(file_name, line);
    //                 nbr = hexToDecimal(line);
    //             }
    //         }
    //         this->request_data["body"] = content;
    //     }
    //     else
    //         this->request_data["body"] = content;
    //     std::cout << this->request_data["body"] << std::endl;
    //     std::cout << "\n######## End of the Body ####### \n";
    // }

}

std::string ft_getPageName(std::string pageName)
{
    size_t start = pageName.find("/", 0);
    size_t end = pageName.find("?", 0);
    if (end == std::string::npos)
        end = pageName.size();
    std::string name = pageName.substr(start, (end));
    return (name);
}

std::string ft_getQuery(std::string pageName)
{
    size_t start = pageName.find("?", 0);
    if (start == std::string::npos)
        return ("");
    std::string name = pageName.substr(start + 1, (pageName.size() - start));
    return (name);
}

void Response::handleHttpRequest(int clientSocket, char* httpRequest) 
{
    (void) httpRequest;
    (void) clientSocket;
    std::cout << "######################################################\n";
    // std::vector<std::string> requestDataParsed = it->second;
    if (request_data["Method"] == "GET") 
    {
    std::string requestedResource = ft_getPageName(request_data["Asset"]);
    std::string requestedQuery = ft_getQuery(request_data["Asset"]);

    // DATA OF THE PAGES
    std::cout << "############### [DATA] ############\n";
    std::cout << "Page Name = " << requestedResource << std::endl;
    std::cout << "Query = " << requestedQuery << std::endl;
    std::cout << "###################################\n";
    // end data

  
    

    if (requestedResource == "/")
        requestedResource = "/index.html";
    std::ifstream file("."+requestedResource, std::ios::binary);
    if (!file.is_open()) {
        // File not found, respond with a 404 error

        std::string response = "<div><h1 style=\"text-align:center\">404 Not Found</h1><hr/><p style=\"text-align:center\">nginx/1.25.3</p></div>\r\n\r\n";
        std::string httpResponse = "HTTP/1.1 404 Not Found\r\n";
        httpResponse += "Content-Type: text/html\r\n";
        httpResponse += "Content-Length: " + std::to_string(response.size()) + "\r\n"; // remove to_string
        httpResponse += "\r\n" + response;
        // return t houssam
        send(clientSocket, httpResponse.c_str(), httpResponse.length(), 0);
    } else {
        // Read the entire content of the file into a string        
        std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        std::string content_type = "text/html";
        std::string ext = requestedResource.substr(requestedResource.find_last_of(".") + 1);
        if (ext == "css")
            content_type = "text/css";
        else if (ext == "png")
            content_type = "image/png";
        else if (ext == "jpg")
            content_type = "image/jpeg";
        // Build the HTTP response
        std::string httpResponse = "HTTP/1.1 200 OK\r\n";
        httpResponse += "Content-Type: "+content_type+"\r\n";
        httpResponse += "Content-Length: " + std::to_string(content.size()) + "\r\n"; // remove to_string
        httpResponse += "\r\n" + content;
        // hadi l houssam
        // Send the HTTP response
        size_t sizeto; 
        sizeto = send(clientSocket, httpResponse.c_str(), httpResponse.length(), 0);
        std::cout << "=>>>>>>>>>>> sizeto = " << sizeto << "\n";
        // Clean up
        file.close();
    }
        // std::cout << std::endl;
    }
    // return ("");
    // std::cout << "######################################################\n\n";

}