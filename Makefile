SRCS = main.cpp Server.cpp Handler.cpp Request.cpp Response.cpp

CC = c++

FLAGS = -Wall -Wextra -Werror -std=c++98

INCLUDES = Server.hpp Handler.hpp Request.hpp Response.hpp

OBJS = $(SRCS:.cpp=.o) 

NAME = webserver

$(NAME): $(OBJS)
	$(CC) $(OBJS) -o $(NAME)

%.o : %.cpp $(INCLUDES) Makefile
	$(CC) ${FLAGS} -o $@ -c $<

all : $(NAME)

clean :
	rm -rf $(OBJS)

fclean : clean
	rm -rf $(NAME)

re : fclean all

x : $(NAME)
	rm -rf $(OBJS)
	./webserver

.PHONY: all clean fclean re