NAME    = philo
CC      = cc
CFLAGS  = -Wall -Wextra -Werror -g -fsanitize=thread
SRCS    = main.c init.c routine.c utils.c print_and_free.c
OBJS    = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(OBJS) -o $(NAME) $(CFLAGS) -pthread

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
