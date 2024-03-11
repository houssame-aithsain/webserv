/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hait-hsa <hait-hsa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/02 12:40:13 by hait-hsa          #+#    #+#             */
/*   Updated: 2024/03/08 20:27:42 by hait-hsa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../clientSocket/Client.hpp"
#include <vector>
#include <cstring>

class CGI {
    private:
        std::string input;
        std::string output;
        char **env;
    public:
        void extractClientContent(Client & client);
        void executeCGIscript( Client & client );
        void EnvInit( void );
};