NAME = philosopher

CC = gcc
CFLAGS = -Wall -Wextra -Werror -pthread -g

SRCS = main.c philosopher.c init.c utils.c utils_extra.c monitor.c
OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all
