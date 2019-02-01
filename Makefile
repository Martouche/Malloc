##
## EPITECH PROJECT, 2019
## CppPool rush3
## File description:
## Makefile
##

CC=     gcc

NAME=   libmy_malloc.so

SRC=        src/malloc.c

OBJ=            $(SRC:.c=.o)

CFLAGS=         -fPIC -Wall -Wextra -Werror -I./include -g3

LDFLAGS=        -shared

all:            $(NAME)

$(NAME):        $(OBJ)
                $(CC) $(CFLAGS) $(OBJ) -o $@ $(LDFLAGS)
clean:
            rm -f $(OBJ)
            rm -f *~
            rm -f \#*\#
            rm -f *.gch
            @echo -e "$(OBJRM)"

fclean:         clean
            rm -f $(NAME)
            @echo -e "$(NAMERM)"

re:         fclean all
