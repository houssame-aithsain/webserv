# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hait-hsa <hait-hsa@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/01/29 20:25:53 by hait-hsa          #+#    #+#              #
#    Updated: 2024/01/29 22:22:29 by hait-hsa         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


CC			=	c++
CXXFLAGS	=	-Wall -Wextra -Werror -fsanitize=address -g

SRC			=	main.cpp server/Server.cpp parsing/parser.cpp
HEADERS		=	server/Server.hpp parsing/parser.hpp

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