# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/04 15:40:17 by okraus            #+#    #+#              #
#    Updated: 2024/04/03 19:24:25 by okraus           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		:=	42CV
NAMELOOP	:=	42CVloop
NAMEBONUS	:=	42CVbonus
HEADER		:=	cv42.h
CC 			:=	cc
CFLAGS		:=	-Wall -Wextra -Werror -g
GLFLAGS		:=	-lglut -lGL -lGLU
JPGFLAGS	:=	-ljpeg

# Source files

SRCS		:=	main.c \
				utils.c

SRCSLOOP	:=	main_loop.c
SRCSBONUS	:=	main_bonus.c

# Formating

NRM_FORMAT	:=	\033[0m

GREEN		:=	\033[1;32m
YELLOW		:=	\033[1;93m
RED			:=	\033[1;31m
REVERSE		:=	\033[7m

# Objects

OBJS		:=	$(SRCS:.c=.o)
OBJSLOOP	:=	$(SRCSLOOP:.c=.o)
OBJSBONUS	:=	$(SRCSBONUS:.c=.o)
# RULES

all:			$(NAME)

bonus:			$(NAMEBONUS)

loop:			$(NAMELOOP)

# COMPILATION

%.o:			%.c $(HEADER)
				$(CC) $(CFLAGS) -c $< -o $@

$(NAME): 		$(OBJS)
				$(CC) $(CFLAGS) $^ -o $@ $(GLFLAGS) $(JPGFLAGS)

$(NAMELOOP): 	$(OBJSLOOP)
				$(CC) $(CFLAGS) $< -o $@ $(GLFLAGS) $(JPGFLAGS)

$(NAMEBONUS): 	$(OBJSBONUS)
				$(CC) $(CFLAGS) $< -o $@ $(GLFLAGS) $(JPGFLAGS)

# $(GLFLAGS) $(JPGFLAGS)

# clean library

clean:
				rm -f $(OBJS)
				rm -f $(OBJSLOOP)
				rm -f $(OBJSBONUS)

# Remove the program after running clean

fclean:			clean
				rm -f $(NAME)
				rm -f $(NAMELOOP)
				rm -f $(NAMEBONUS)

# Remove stuff and make it all again

re: 			fclean all

# Phony stuff for rules

.PHONY: all announce clean fclean re bonus loop