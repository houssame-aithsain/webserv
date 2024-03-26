/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gothmane <gothmane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 18:36:05 by gothmane          #+#    #+#             */
/*   Updated: 2024/03/26 21:37:02 by gothmane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response() 
{
    initMapErrors();
    this->_errorCode = 0;
}

Response::~Response()
{
    
}

void Response::msg_code_arr()
{
    msg_code[100] = "Continue";
    msg_code[101] = "Switching Protocols";
    msg_code[200] = "OK";
    msg_code[201] = "Created";
    msg_code[202] = "Accepted";
    msg_code[203] = "Non-Authoritative Information";
    msg_code[204] = "No Content";
    msg_code[205] = "Reset Content";
    msg_code[206] = "Partial Content";
    msg_code[300] = "Multiple Choices";
    msg_code[301] = "Moved Permanently";
    msg_code[302] = "Found";
    msg_code[303] = "See Other";
    msg_code[304] = "Not Modified";
    msg_code[305] = "Use Proxy";
    msg_code[307] = "Temporary Redirect";
    msg_code[400] = "Bad Request";
    msg_code[401] = "Unauthorized";
    msg_code[402] = "Payment Required";
    msg_code[403] = "Forbidden";
    msg_code[404] = "Not Found";
    msg_code[405] = "Method Not Allowed";
    msg_code[406] = "Not Acceptable";
    msg_code[407] = "Proxy Authentication Required";
    msg_code[408] = "Request Timeout";
    msg_code[409] = "Conflict";
    msg_code[410] = "Gone";
    msg_code[411] = "Length Required";
    msg_code[412] = "Precondition Failed";
    msg_code[413] = "Request Entity Too Large";
    msg_code[414] = "Request-URI Too Long";
    msg_code[415] = "Unsupported Media Type";
    msg_code[416] = "Requested Range Not Satisfiable";
    msg_code[417] = "Expectation Failed";
    msg_code[500] = "Internal Server Error";
    msg_code[501] = "Not Implemented";
    msg_code[502] = "Bad Gateway";
    msg_code[503] = "Service Unavailable";
    msg_code[504] = "Gateway Timeout";
    msg_code[505] = "HTTP Version Not Supported";
}

std::string Response::get_msg(int code) 
{
    msg_code_arr();
    std::map<int, std::string>::iterator it = msg_code.find(code);
    if (it != msg_code.end()) {
        return it->second;
    } else {
        return "Unknown code";
    }
}


std::string ft_cut_file_from_req(std::string &rs)
{
    size_t find_dot = rs.rfind('.');
    if (find_dot == std::string::npos)
        rs += '/';
    size_t lastSlashPos = rs.rfind('/');
    std::string getp = rs.substr(0, lastSlashPos + 1);
    if (lastSlashPos != std::string::npos) 
        return (getp);
    else
        return ("");
}


std::string Response::ft_getPageToRender(std::vector<std::pair<std::string, std::vector<std::string> > > ParsedRequest)
{
    for (std::vector<std::pair<std::string, std::vector<std::string> > >::iterator it = ParsedRequest.begin(); it != ParsedRequest.end(); ++it)
    {
        std::vector<std::string> requestDataParsed = it->second;
        if (it->first == "URI")
        {
            std::cout << requestDataParsed.at(0) << std::endl;
            return (requestDataParsed.at(0));
        }
    }
    // std::cout << "Error: There's no data in here (URI data)\n";
    return ("");
}

void Response::initMapErrors()
{
    this->_errorCodes[400] = "HTTP/1.1 400 Bad Request\r\nContent-Length: 50\r\n\r\n<html><body><h1>400 Bad Request</h1></body></html>";
    this->_errorCodes[401] = "HTTP/1.1 401 Unauthorized\r\nContent-Length: 51\r\n\r\n<html><body><h1>401 Unauthorized</h1></body></html>";
    this->_errorCodes[403] = "HTTP/1.1 403 Forbidden\r\nContent-Length: 48\r\n\r\n<html><body><h1>403 Forbidden</h1></body></html>";
    this->_errorCodes[404] = "HTTP/1.1 404 Not Found\r\nContent-Length: 48\r\n\r\n<html><body><h1>404 Not Found</h1></body></html>";
    this->_errorCodes[405] = "HTTP/1.1 405 Method Not Allowed\r\nContent-Length: 57\r\n\r\n<html><body><h1>405 Method Not Allowed</h1></body></html>";
    this->_errorCodes[409] = "HTTP/1.1 409 Conflict\r\ncontent-type: text/html\r\nContent-Length: 47\r\n\r\n<html><body><h1>409 Conflict</h1></body></html>";
    this->_errorCodes[413] = "HTTP/1.1 413 Payload Too Large\r\nContent-Length: 56\r\n\r\n<html><body><h1>413 Payload Too Large</h1></body></html>";
    this->_errorCodes[500] = "HTTP/1.1 500 Internal Server Error\r\nContent-Length: 60\r\n\r\n<html><body><h1>500 Internal Server Error</h1></body></html>";
    this->_errorCodes[501] = "HTTP/1.1 501 Not Implemented\r\nContent-Length: 54\r\n\r\n<html><body><h1>501 Not Implemented</h1></body></html>";
    this->_errorCodes[505] = "HTTP/1.1 505 HTTP Version Not Supported\r\nContent-Length: 65\r\n\r\n<html><body><h1>505 HTTP Version Not Supported</h1></body></html>";
    this->_errorCodes[507] = "HTTP/1.1 507 Insufficient Storage\r\nContent-Length: 59\r\n\r\n<html><body><h1>507 Insufficient Storage</h1></body></html>";
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

int hexToDecimal(const std::string& hexString)
{
    if (hexString == "")
        return (0);
    return std::stoi(hexString, nullptr, 16);
}

// check for allowed characters in the string !!!!! I still dont know what are the allowed characters .
bool ForbiddenChars(const std::string& str)
{
    const std::string forbiddenChars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~:/?#[]@!$&'()*+,;=%";
    
    for (size_t i = 0; str[i]; i++)
        if (forbiddenChars.find(str[i]) == std::string::npos)
            return true;
    return false;
}

std::string Response::getPort()
{
    size_t pos = this->request_data["Host"].find_last_of(':');
    if (pos == std::string::npos)
        return (std::string(""));    
    std::string str = this->request_data["Host"].substr(pos + 1, 
            (this->request_data["Host"].size() - (pos + 2)));
    return (str);
}


bool Response::BodySizeChecker(Parser &p, std::string &port, std::string prefix)
{
    (void) prefix;
    // std::string loc = ft_cut_file_from_req(prefix);
    std::vector<std::string> vec =  p.get_data_from_conf(port, this->location , "_clientMaxBodySize", 1);

    if (std::atoi(vec[0].c_str()) <= std::atoi(this->request_data["Content-Length"].c_str()))
    {
        this->_errorCode = 413;
        this->response = this->_errorCodes[413];
        return (false);
    }
    return (true);
}

void    Response::requestHeadersCheck(Parser &p)
{
    (void) p;
    requestMap_it transfer_it = this->request_data.find("Transfer-Encoding");
    requestMap_it contentLength_it = this->request_data.find("Content-Length");
    if(transfer_it != this->request_data.end() && this->request_data["Transfer-Encoding"] != "" && 
        this->request_data["Transfer-Encoding"] != "chunked" && (this->_errorCode = 501))
    {
        // std::cout << "Transfer encoding >> " << this->request_data["Transfer-Encoding"] << "\n";
        this->response = this->_errorCodes[501];
        return;
    }
    else if (transfer_it == this->request_data.end() && contentLength_it == this->request_data.end()
            && this->request_data["Method"] == "POST" && (this->_errorCode = 400))
        {
            this->response = this->_errorCodes[400];
            return;
        }
    else if (ForbiddenChars(std::string(this->request_data["URI"])) && (this->_errorCode = 400))
    {
        this->response = this->_errorCodes[400];
        return;
    }
    else if (this->request_data["URI"].length() > 2048 && (this->_errorCode = 414))
    {
        this->response = this->_errorCodes[414];
        return;
    }
}



std::string ft_get_file(std::string &rs)
{
    if (rs.find(".") == std::string::npos)
        return ("");
    size_t lastSlashPos = rs.rfind('/');
    std::string getp = rs.substr(lastSlashPos + 1, rs.size());
    return (getp);
}



void    Response::locationsMatching(Parser &p, std::string &port, std::string &page_path)
{
    std::vector<std::string> locations_ = p.get_server_locations(port);
    int check = 0;
    int check_slash = 0;
    int check_cut_location = 1;
    size_t i = 0;

    if (page_path[page_path.size() - 1] != '/')
    {
        page_path.append("/");
        check_slash = 1;
    }
    while (check_cut_location || !page_path.empty())
    {
        i = 0;
        for(; i < locations_.size(); i++)
        {
            if (ft_trim(locations_[i], " \"\'\t") ==  page_path)
            {
                check = 1;
                check_cut_location = 0;
                this->location = page_path;
                return ;
            }
        }
        if (check_cut_location == 1)
        {
            int start = page_path.rfind('/');
            int end = page_path.size();
            if (start < end)
            {
             
                page_path.erase(start, end);
                // if (check_slash)
                // {
                //     page_path.append("/");
                //     check_slash = 1;
                // }
                // if (start > 0 && page_path[start - 1] != '/')
                //     page_path.append("/");

                std::cout << "PAGE PATH => " << page_path << "    START => " << start << " end => " << end << "\n";
                if (page_path.empty())
                {
                    i = 0;
                    for(; i < locations_.size(); i++)
                    {
                        if (ft_trim(locations_[i], " \"\'\t") ==  "/")
                        {
                            check = 1;
                            check_cut_location = 0;
                            this->location = "/";
                            page_path = "/";
                            return ;
                        }
                    }
                }
                else
                {
                    i = 0;
                    for(; i < locations_.size(); i++)
                    {
                        std::cout << "LOCATION >> " << locations_[i] << "\n";
                        std::cout << "page_path >> " << page_path << "\n";
                        // if (page_path[page_path.size() - 1]  != '/')
                        //     page_path.append("/");
                        if (locations_[i][locations_[i].size() - 1] == '/')
                        {
                            std::cout << "IN HERE\n";
                            locations_[i] = locations_[i].substr(0, locations_[i].size() - 1);
                        }
                        if (ft_trim(locations_[i], " \"\'\t") ==  page_path)
                        {
                            check = 1;
                            check_cut_location = 0;
                            this->location = page_path;
                            return ;
                        }
                    }
                }
            }
        }
    }
}


int    Response::requestErrorsHandling(Parser &p, std::string &port)
{
    (void) p;
    (void) port;

    if (this->request_data["URI"] == "")
        this->request_data["URI"] = "/";
    std::string page_path = ft_getPageName(this->request_data["URI"]);

    if (page_path == "")
        page_path = "/";
    else if (page_path.find(".") != std::string::npos)
    {
        page_path = ft_cut_file_from_req(page_path);
    }
    requestHeadersCheck(p);
    locationsMatching(p, port, page_path);
    if (locationRedirection(p, port) == 1 )
        return (1);
    if (!methodChecker(p, port, page_path))
    {
        return (1);  
    }
    return (0);
}


int Response::locationRedirection(Parser &p, std::string &port)
{
    std::string page_path = this->request_data["URI"];
    std::string httpResponse;
    
    if (page_path == "")
        page_path = "/";
    std::vector<std::string> data = p.get_data_from_conf(port, page_path, "redirect", 1);
    if (data.empty())
        return 0;
    httpResponse = "HTTP/1.1 301 Moved Permanently\r\n";
    httpResponse += "Content-Type: text/html\r\n";
    httpResponse += "Location: " + ft_trim(data[0], " \"\'\t") + "\r\n";
    this->response = httpResponse;
    return (1);
}

bool Response::methodChecker(Parser &p, std::string &port, std::string &prefix)
{
    std::vector<std::string> data = p.get_data_from_conf(port, prefix, "allowed_methods", 1);
    std::string method_ = this->request_data["Method"];

    int checker = 0;

    if (method_ != "GET" && method_ != "POST" && method_ != "DELETE" && method_ != "")
    {
        this->_errorCode = 405;
        this->response = this->_errorCodes[405];
        return (false);
    }
    if (!data.empty())
    {
        for (size_t i = 0; i < data.size(); i++)
        {
            if (ft_trim(data[i], " \"\'\t") == method_)
            {
                // std::cout << data[i] << " " << method_<<  " THERES POST\n";
                checker = 1;
                return (true);
            }
        }
        if (checker == 0)
        {
            if (method_ == "")
                return (true);
            this->_errorCode = 405;
            this->response = this->_errorCodes[405];
            return (false);
        }
    }
    else if (data.empty())
        return (true);
    return (false);
}

void Response::ft_parse_request(std::string request) 
{
    std::istringstream getrequestStream(request);
    std::string getrequestLine;
    std::string key;
    std::string value;
    int index = 0;
    int checker_body = 0;
    header.clear();
    size_header = 0;
    while (std::getline(getrequestStream, getrequestLine, '\n')) 
    {
        std::istringstream lines(getrequestLine);
        if (index++ == 0)
            lines >> this->request_data["Method"] >> this->request_data["URI"]
                 >> this->request_data["Type"];
        else if (getrequestLine != "\r" && getrequestLine != "")
        {
            // std::cout << "THE KEY => "<< key << "\n";
            lines >> key;
            if (getrequestLine.length() > key.length() && key.length() > 0)
            {
                key = key.substr(0, key.length() - 1);
                this->request_data[key] = ft_trim(getrequestLine.substr(key.length() + 2, getrequestLine.length()), " \t\n\""); // seg in firefox
            }
        }
        else if (!checker_body && (checker_body = 1))
            break ;
        header.push_back(getrequestLine);
        size_header += getrequestLine.size();
    }

    // POST PARSING REQUEST CHUNCKED
    if (checker_body == 1)
    {
        std::string content((std::istreambuf_iterator<char>(getrequestStream)), std::istreambuf_iterator<char>());
        if (this->request_data["Method"] == "POST")
        {
            // std::cout << "TRANSFER ENDCODING => " << this->request_data["Transfer-Encoding"] << "\n";
            std::stringstream file_name(content);
            std::string line;
            char buf[1024];
            if (this->request_data["Transfer-Encoding"].find("chunked") != std::string::npos)
            {
                // std::cout << "In this condition of chunked \n\n\n\n";
                getline(file_name, line);
                int nbr = hexToDecimal(line);
                // std::cout << "the line >> " << line << "\n";
                while (nbr != 0)
                {
                    bzero(buf,1024);
                    file_name.read(buf, nbr);
                    this->request_data["body"] += buf;
                    file_name.get();
                    file_name.get();
                    getline(file_name, line);
                    nbr = hexToDecimal(line);
                }
            }
           else
                this->request_data["body"] = content;
            

        }

    }
    // std::cout << "THE BODY >> " << this->request_data["body"] << "\n";

parse_request_body(this->request_data["body"]);
    // std::cout << "################## PARSED BODY #################\n" << parse_request_body(this->request_data["body"]) << "################## END PARSED BODY #################\n";
}

std::string Response::get_filename(std::string body)
{
    std::string key = "filename=\"";
    size_t start_idx = body.find(key);
    std::string filename = "";

    if (start_idx != std::string::npos)
    {
        start_idx += key.size();
        size_t end_idx = body.find("\"", start_idx);
        if (end_idx != std::string::npos)
        {
            filename = body.substr(start_idx, end_idx - start_idx);
            return (filename);
        }
    }
    return ("");
}

std::string Response::get_content_body(std::string body)
{
    std::string key = "Content-Disposition:";
    size_t start_idx = body.find(key);
    std::string content = "";
    
    if (start_idx != std::string::npos) 
    {
        start_idx = body.find("\r\n\r\n", start_idx); // Skip past headers
        if (start_idx != std::string::npos) 
        {
            start_idx += 4; // Skip past "\r\n\r\n"
            size_t end_idx = body.find("\r\n", start_idx); // Find the end of the file content
            if (end_idx != std::string::npos) 
            {
                content = body.substr(start_idx, (end_idx - start_idx));
                return (content);
            } 
        }
    }
    return ("");
}


std::string Response::parse_request_body(std::string body)
{
    std::string contentType = this->request_data["Content-Type"];
    std::string boundaryStr = "boundary=";
    std::string val;
    std::istringstream ss(contentType);
    std::string part;
    
    while (std::getline(ss, part, ';')) 
    {
        part.erase(0, part.find_first_not_of(" \t\r\n"));
        part.erase(part.find_last_not_of(" \t\r\n") + 1);
        size_t pos = part.find(boundaryStr);
    
        if (pos != std::string::npos) 
        {
            val.append("--");
            val += part.substr(pos + boundaryStr.length());
            break;
        }
    }
    
    // if (val.empty()) 
    //     std::cout << "Boundary not found" << std::endl;


    std::string filename = get_filename(body);
    std::string contentFile = get_content_body(body);

    this->request_data["filename"] = filename;

    std::istringstream st(body);
    std::string line;
    int checker = 0;
    int start_getting = 0;
    
    while (std::getline(st, line, '\n'))
    {
        line += "\n";
        if (line.find("filename") != std::string::npos)
            checker = 1;
        else if (checker)
        {
            if (line == "\r\n" || line == "\r")
                start_getting = 1;
            if (start_getting == 1 && (line.find(val) == std::string::npos))
                this->request_data["content_file"] += line;
            else if (start_getting == 1 && (line.find(val) != std::string::npos))
                break;
        }
    }
    // std::cout << "File content => " << this->request_data["content_file"] << "\n\n\n";
    return body ;
}

std::string Response::ft_getPageName(std::string pageName)
{
    size_t start = pageName.find("/", 0);
    size_t end = pageName.find("?", 0);
    std::string name ;
    if (end == std::string::npos )
        end = pageName.size();
    if (start == std::string::npos)
        return ("");
    name = pageName.substr(start, end);
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

std::string Response::get_root_for_location(Parser &p, std::string &port, std::string &prefix)
{
    std::string page_path = prefix;
    std::vector<std::string> roots;


    if (page_path == "")
        page_path = "/";
    else if (page_path.find(".") != std::string::npos)
        page_path = ft_cut_file_from_req(page_path);

    roots = p.get_data_from_conf(port, page_path, "root", 1);
    if (!roots.empty())
        return (roots[0]);
    return ("");
}

void Response::open_file(std::string new_req_resource, int check_301, std::string requestedResource)
{
    std::ifstream file(new_req_resource, std::ios::binary);
    if (!file.is_open())
    {
        this->_errorCode = 404;
        this->response = this->_errorCodes[404];
    }
    else 
    {
        std::string httpResponse;
        std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        std::string content_type = "text/html";
        std::string ext = requestedResource.substr(requestedResource.find_last_of(".") + 1);
        if (ext == "css")
            content_type = "text/css";
        else if (ext == "png")
            content_type = "image/png";
        else if (ext == "jpg")
            content_type = "image/jpeg";
        else if (ext == "mp4")
            content_type = "video/mp4";
        else if (ext == "webp")
            content_type = "image/webp";
        else if (ext == "js")
            content_type = "text/javascript";
        else if (ext == "avif")
            content_type = "image/avif";
        if (check_301)
        {
            this->_errorCode = 301;
            httpResponse = "HTTP/1.1 301 Moved Permanently\r\n";
            httpResponse += "Content-Type: "+content_type+"\r\n";
            httpResponse += "Location:" + requestedResource + "/" + "\r\n";
            httpResponse += "Content-Length: " + std::to_string(content.size()) + "\r\n"; // remove to_string
            httpResponse += "\r\n" + content;
        }
        else
        {
            httpResponse = "HTTP/1.1 200 OK\r\n";
            httpResponse += "Content-Type: "+content_type+"\r\n";
            httpResponse += "Content-Length: " + std::to_string(content.size()) + "\r\n"; // remove to_string
            httpResponse += "\r\n" + content;
        }
        
        file.close();
        if (httpResponse.empty())
        {
            httpResponse = "HTTP/1.1 200 OK\r\n";
            httpResponse += "Content-Type: text/html\r\n";
            httpResponse += "Content-Length: 4\r\n"; // remove to_string
            httpResponse += "\r\nabcd";
        }
        this->response = httpResponse;
    }
}

bool Response::access_checker(std::string path)
{
    const char* filename = path.c_str();
    
    if (access(filename, F_OK) == 0)
        return (true);
    else 
        return (false);
}

bool Response::look_for_index_files(std::string new_req_resource, std::string idx)
{
    if (access_checker(new_req_resource + "/" + idx))
        return (true);
    return (false);
}

bool Response::check_auto_index(Parser &p, std::string &port, std::string &prefix)
{
    std::vector<std::string> data = p.get_data_from_conf(port, prefix, "autoindex", 1);

    if (!data.empty())
    {
        if (data[0] == "on")
            return (true);
    }
    return (false);
}

void Response::list_dir(std::string new_req_resource)
{
    struct stat entry_stat;
    
    if (stat(new_req_resource.c_str(), &entry_stat) == 0 )
    {
        const char* directory_path = new_req_resource.c_str();
        DIR* dir = opendir(directory_path);

        if (dir != nullptr) 
        {
            dirent* entry;
            int counter = 0 ;
            std::string httpResponse;
            std::string response;
            std::string header = "<div><h1 style=\"text-align:center\">Listing Directory : </h1><hr/>";
            while ((entry = readdir(dir)) != nullptr) 
            {
                if (counter == 0)
                    response += header;
                if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) 
                {
                    std::string ere = entry->d_name;
                    response += "<p><a href="+ere+">"+ere+"</a></p></div>\r\n\r\n";
                }
                
                counter++;
            }
            httpResponse = "HTTP/1.1 200 OK\r\n";
            httpResponse += "Content-Type: text/html\r\n";
            httpResponse += "Content-Length: " + std::to_string(response.size()) + "\r\n"; // remove to_string
            httpResponse += "\r\n" + response;
            this->response = httpResponse;
            closedir(dir);
        } 
    }
}

bool doesFolderExist(const std::string& foldername) {
    struct stat buffer;

    if (stat(foldername.c_str(), &buffer))
        return (true);
    return (false);
}

void Response::get_resource(Parser &p, std::string &port)
{
    if (!auto_index)
    {
        // std::cout << "new_req_resource  => " << new_req_resource << "\n";
        // std::cout << "requestedResource => " << requestedResource << "\n";
        // std::cout << "check 301  => " << check_301 << "\n";
        open_file(new_req_resource, check_301, requestedResource);
    }
    check_error_page(p, port, requestedResource);
}


// POST
std::string Response::upload_checker(Parser &p, std::string &port, std::string &prefix)
{
    std::string page_path;

     if (page_path == "")
        page_path = "/";
    else if (page_path.find(".") != std::string::npos)
        page_path = ft_cut_file_from_req(prefix);
    std::vector<std::string> data = p.get_data_from_conf(port, prefix, "upload_path", 1);

    if (!data.empty())
        return (data[0]);
    return ("");
}

std::string Response::cgi_path_checker(Parser &p, std::string &port, std::string &prefix)
{
    std::vector<std::string> data = p.get_data_from_conf(port, prefix, "cgi", 1);

    if (!data.empty())
        return (data[0]);
    return ("");
}

std::string Response::get_index_conf(Parser &p, std::string &port, std::string &prefix)
{
    // std::cout << "THE PREFIX >> " << prefix << "\n";

    std::vector<std::string> data = p.get_data_from_conf(port, prefix, "index", 1);
    std::string cutted = prefix.substr(0, prefix.size() - 1);

    if (!data.empty())
    {
        return (data[0]);
    }
    else
    {
        data = p.get_data_from_conf(port, cutted, "index", 1);
    
        if (!data.empty())
            return (data[0]);
    }
    return ("");
}

void Response::fillAndSaveFile(const std::string& fileName, const std::string& content) {

    std::string filePath = fileName;
    std::ofstream file(filePath);

    if (!file) {
        // std::cout << "THE FILE PATH => " << filePath << " << " << fileName <<  "\n";
        // std::cerr << "Failed to open the file.\n";
        return;
    }

    file << content;
    file.close();
}

bool Response::post_req_handler(Parser &p, std::string &port, Client &client)
{
    if (!auto_index)
    {
        if (page_path == "")
            page_path = "/";
        else if (page_path.find(".") != std::string::npos)
            page_path = ft_cut_file_from_req(page_path);
        std::string cgi = cgi_path_checker(p, port, page_path);
        
        if (cgi != "")
        {
            //RUN THE CGI
            std::cout << "CGI => " << cgi << "\n";
            if (!cgi.compare(".py"))
            {
                CGI::extractClientContent(client, this->request_data);
                open_file(new_req_resource, check_301, requestedResource);
            }
            else
                open_file(new_req_resource, check_301, requestedResource);
        }
        else
        {
            if (upload_checker(p, port, this->location) != "")
            {
                std::string file_path = upload_checker(p, port, this->location) +"/"+ this->request_data["filename"];
                fillAndSaveFile(file_path, this->request_data["content_file"]);
                open_file(new_req_resource, check_301, requestedResource);
            }
            //403 FORBIDDEN
            // std::cout << "NOT RUNNING THE CGI\n";
            open_file(new_req_resource, check_301, requestedResource);
        }
    }
    return (false);
}


bool Response::check_error_page(Parser &p, std::string port, std::string prefix)
{
    (void) prefix;
    std::cout << "this->location >> " << this->location<< "\n";
    std::string root = get_root_for_location(p, port, this->location);
    std::vector<std::string> data = p.get_data_from_conf(port, this->location, "error_page", 1);

    if (!data.empty() && data.size() > 1 && this->_errorCode == std::atoi(data[0].c_str()))
    {
        root = root.append("/"+data[1]);
        std::cout << "in hereeee ERROR PAGE >> " << root << "\n";
        
        std::ifstream file(root, std::ios::binary);
        if (!file.is_open())
        {
            this->_errorCode = 404;
            this->response = this->_errorCodes[404];
        }
        else 
        {
            std::string httpResponse;
            std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
            std::string content_type = "text/html";
            std::string ext = data[1].substr(data[1].find_last_of(".") + 1);
            if (ext == "css")
                content_type = "text/css";
            else if (ext == "png")
                content_type = "image/png";
            else if (ext == "jpg")
                content_type = "image/jpeg";
            else if (ext == "mp4")
                content_type = "video/mp4";
            else if (ext == "webp")
                content_type = "image/webp";
            else if (ext == "js")
                content_type = "text/javascript";
            else if (ext == "avif")
                content_type = "image/avif";
            
            httpResponse = "HTTP/1.1 " + data[0] +" "+ get_msg(std::atoi(data[0].c_str())) +"\r\n";
            httpResponse += "Content-Type: "+content_type+"\r\n";
            httpResponse += "Content-Length: " + std::to_string(content.size()) + "\r\n"; // remove to_string
            httpResponse += "\r\n" + content;


            file.close();
            if (httpResponse.empty())
            {
                httpResponse = "HTTP/1.1 200 OK\r\n";
                httpResponse += "Content-Type: text/html\r\n";
                httpResponse += "Content-Length: 4\r\n"; // remove to_string
                httpResponse += "\r\nabcd";
            }
            this->response = httpResponse;
        }
    }
    return ("");
}

void Response::delete_handler(Parser &p, std::string &port)
{

    if (!auto_index)
    {
        open_file(new_req_resource, check_301, requestedResource);
    }
    check_error_page(p, port, requestedResource);
}

// 

std::string getResourceType(const std::string& path) {
    struct stat path_stat;
    stat(path.c_str(), &path_stat);
    if (S_ISREG(path_stat.st_mode)) {
        return "file";
    } else if (S_ISDIR(path_stat.st_mode)) {
        return "folder";
    } else {
        return "other";
    }
}


std::string Response::check_alias(Parser &p, std::string port, std::string prefix)
{
    std::string page_path = prefix;
    std::vector<std::string> roots;


    if (page_path == "")
        page_path = "/";
    else if (page_path.find(".") != std::string::npos)
        page_path = ft_cut_file_from_req(page_path);

    std::cout << "THE PAGE PATHHHH => " << page_path << "\n";
    roots = p.get_data_from_conf(port, page_path, "alias", 1);
    if (!roots.empty())
        return (roots[0]);
    return ("");
}

void Response::init_request_data(Parser &p)
{
    port = this->getPort(); // anrdoh dynamic
    requestedResource = ft_getPageName(request_data["URI"]);
    root_path = get_root_for_location(p, port, requestedResource);
    std::string alias = check_alias(p, port, this->location);
    std::cout << "ALIAS >>> " << alias << "\n";
    std::cout << "ALIAS >>> " << this->location << "\n";
    if (!alias.empty())
    {
        std::string rr = requestedResource;

        rr.append("/");
        size_t start_pos = rr.find(this->location);
        if(start_pos != std::string::npos)
            rr.replace(start_pos, this->location.size(), alias);
        new_req_resource = root_path + rr;
    }
    else
        new_req_resource = root_path + requestedResource;
    
    std::cout << "requestedRes => " << requestedResource << "\n";
    std::cout << "ROOT PATH => " << new_req_resource << "\n";
    std::cout << "LOCATION => " << location << "\n";

    type_ = getResourceType(new_req_resource);
    if (type_ == "file")
        fileName = ft_get_file(request_data["URI"]);
    check_301 = 0;
    auto_index = 0;
    this->p = p;
    requestedQuery = ft_getQuery(request_data["URI"]);
    this->request_data["QUERY_STRING"] = requestedQuery;
    if (type_ == "file" && fileName.find(".py"))
        this->request_data["CGI_PATH"] = new_req_resource;
}

bool Response::check_if_errors_nd_res(Parser &p)
{
    if (requestedResource[requestedResource.size() - 1] != '/')
    {
        new_req_resource = new_req_resource + "/";
        check_301 = 1;
    }
    else
    {
        if (look_for_index_files(new_req_resource, get_index_conf(p, port, this->location)))
        {
            new_req_resource = new_req_resource + "/" + get_index_conf(p, port, this->location); // we have to get index file from location
        }
        else
        {
            if (doesFolderExist(new_req_resource))
            {
                auto_index = -2; 
                this->_errorCode = 404;
                this->response = this->_errorCodes[404];
                return (false);

            }
            else if (check_auto_index(p, port, this->location))
            {
                auto_index = 1;
                list_dir(new_req_resource);
            }
            else
            {
                auto_index = -1; 
                this->_errorCode = 403;
                this->response = this->_errorCodes[403];
                return (false);
            }
        }
    }

    return (true);
}

std::string Response::createResponse(Parser &p , int type, Client &client)
{
    bool check = true;

    init_request_data(p);
    if (!BodySizeChecker(p, port, requestedResource))
        return ("");

    if (type_ == "folder")
        check = check_if_errors_nd_res(p);
    
    if (type == 1 && check) 
        get_resource(p, port);
    else if (type == 0 && check)
        post_req_handler(p, port, client);
    else if (type == 2 && check)
        delete_handler(p, port);
    return ("");
}
