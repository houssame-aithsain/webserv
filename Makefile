# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hait-hsa <hait-hsa@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/01/29 20:25:53 by hait-hsa          #+#    #+#              #
#    Updated: 2024/03/07 17:24:43 by hait-hsa         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


CC			=	c++
CXXFLAGS	=	-Wall -Wextra -Werror -std=c++98 -fsanitize=address -g

SRC			=	main.cpp webserver/server/Server.cpp parsing/parser.cpp \
				request_response/Response.cpp webserver/clientSocket/Client.cpp \
				webserver/serverSocket/ServerSocket.cpp webserver/CGI/CGI.cpp
HEADERS		=	webserver/server/Server.hpp parsing/parser.hpp request_response/Response.hpp \
				webserver/clientSocket/Client.hpp webserver/serverSocket/ServerSocket.hpp \
				webserver/CGI/CGI.hpp

NAME		=	webserv

RM			=	rm -rf

OBJS		=	$(SRC:.cpp=.o)

all			:	$(NAME)

$(NAME)		:	$(OBJS) $(HEADERS)
				$(CC) $(CXXFLAGS) $(OBJS) -o $(NAME)

%.o			:	%.cpp $(HEADERS)
				$(CC) $(CXXFLAGS) -c $< -o $@

clean		:
				$(RM) $(OBJS)

fclean		:	clean
				$(RM) $(NAME)

re			:	fclean all

.PHONY		:	all clean fclean re