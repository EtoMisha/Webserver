NAME = Webserver

SRC =	$(wildcard ./src/*.cpp)
INC = $(wildcard ./inc/*.hpp)
OBJ	= $(SRC:.cpp=.o)

CC = c++
FLAG = -Wall -Wextra -Werror -std=c++98

.PHONY: all bonus clean fclean re

all: $(NAME)

$(NAME): $(OBJ) $(INC) Makefile
	@$(CC) $(FLAG) $(OBJ) -o $(NAME)

%.o: ./src/%.cpp ./inc/%.hpp
	@$(CC) $(FLAG) -c $< -o $@

clean:
	@rm -f $(OBJ)

fclean: clean
	@rm -f $(NAME)

re: fclean all