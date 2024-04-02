CC := gcc
CFLAGS = -std=c99 -Wall -Wextra -Werror -Wpedantic

RM := rm -f

SUBPROJECTS := Libft minilibx-linux
SUBPROJINC := Libft/inc/ minilibx-linux/

INCDIR := .
INCDIR += $(SUBPROJINC)
INCLUDE := $(addprefix -I, $(INCDIR))

LIBSFLAGS := -L/usr/lib -lXext -lX11 -lm -lbsd -LLibft -lft -ljpeg

SRCDIR := .

SOURCES :=	main.c			\

SOURCES := $(addprefix $(SRCDIR)/, $(SOURCES))

OBJECTS := $(SOURCES:.c=.o)

NAME := digiteq

all : $(NAME)

debug : CFLAGS += -g
debug : $(NAME)

$(NAME) : $(OBJECTS) Libft/libft.a minilibx-linux/libmlx.a
	$(CC) $(CFLAGS) -o $@ $^ $(LIBSFLAGS)

Libft/libft.a : | Libft/Makefile
ifneq (,$(findstring debug, $(MAKECMDGOALS)))
	$(MAKE) -C Libft debug
else
	$(MAKE) -C Libft
endif

minilibx-linux/libmlx.a : | minilibx-linux/Makefile
	$(MAKE) -C minilibx-linux

%.o : %.c | Libft/Makefile
	$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDE)

%/Makefile :
	git submodule update --init $($@%/Makefile=%)

clean :
	$(RM) $(OBJECTS)

fclean :
	$(RM) $(OBJECTS)
	$(RM) $(NAME)
	$(foreach proj, $(SUBPROJECTS), $(MAKE) -C $(proj) fclean)

re : fclean
	$(MAKE) all
