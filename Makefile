##
## EPITECH PROJECT, 2019
## CppPool rush3
## File description:
## Makefile
##

CC=			gcc

NAME= 			libmy_malloc.so

SRC=			src/malloc.c

OBJ= 			$(SRC:.c=.o)

CFLAGS=			 -Wall -Wextra -Werror -I./include -g3

LDFLAGS=		-fPIC -shared

SUCCESS         =       \033[32;1m[Compilation succeeded]\033[0m

OBJRM           =       \033[33;1m[Sources Objects Removed]\033[0m

NAMERM          =       \033[33;1m[Sources Names Removed]\033[0m

FAIL            =       \033[31;1m[Compilation failed]\033[0m

all: 			$(NAME)

$(NAME): 		$(OBJ)
				$(CC) $(CFLAGS) $(OBJ) -o $@ $(LDFLAGS)
clean:
			rm -f $(OBJ)
			rm -f *~
			rm -f \#*\#
			rm -f *.gch
			@echo -e "$(OBJRM)"

fclean: 		clean
			rm -f $(NAME)
			@echo -e "$(NAMERM)"

re: 		fclean all
