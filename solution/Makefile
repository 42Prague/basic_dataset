CC := gcc
CFLAGS = -std=c99 -Wall -Wextra -Werror -Wpedantic

RM := rm -f

SUBPROJECTS := Libft

INCDIR := .
INCDIR += $(addsuffix /inc, $(SUBPROJECTS));
INCLUDE := $(addprefix -I, $(INCDIR))

LIBS := jpeg

LIBSFLAGS := $(addprefix -l, $(LIBS))

SRCDIR := .

SOURCES :=	main.c			\

SOURCES := $(addprefix $(SRCDIR)/, $(SOURCES))

OBJECTS := $(SOURCES:.c=.o)

NAME := digiteq

all : $(NAME)

debug : CFLAGS += -g
debug : $(NAME)

$(NAME) : $(OBJECTS) Libft/libft.a
	$(CC) $(CFLAGS) -o $@ $^ $(LIBSFLAGS)

Libft/libft.a : | Libft/Makefile
ifneq (,$(findstring debug, $(MAKECMDGOALS)))
	$(MAKE) -C Libft debug
else
	$(MAKE) -C Libft
endif

%.o : %.c | Libft/Makefile
	$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDE)

%/Makefile :
	git submodule update --init $($@%/Makefile=%)

clean :
	$(RM) $(OBJECTS)

fclean : clean
	$(RM) $(NAME)

re : fclean
	$(MAKE) all
