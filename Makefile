# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gothmane <gothmane@student.1337.ma>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/01/29 20:25:53 by hait-hsa          #+#    #+#              #
#    Updated: 2024/02/11 18:53:22 by gothmane         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


CC			=	c++
CXXFLAGS	=	-Wall -Wextra -Werror -fsanitize=address -g

SRC			=	main.cpp server/Server.cpp parsing/parser.cpp request_response/Response.cpp
HEADERS		=	server/Server.hpp parsing/parser.hpp  request_response/Response.hpp

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