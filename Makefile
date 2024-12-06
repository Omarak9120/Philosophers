NAME = philo

CC = gcc
CFLAGS = -Wall -Wextra -Werror -pthread

SRCS = main.c philosopher.c utils.c init.c cleanup.c monitor.c
OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re